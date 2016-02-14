{-# LANGUAGE DoAndIfThenElse #-}
{- |
  This module contains the implementation of the virtual machine which
  executes the instruction set from "TastierMachine.Instructions".

  The virtual processor has 4096 words of random-access data memory,
  and 4096 words of stack memory.

  The size of a machine word is 16 bits, and the storage format is
  big-endian (the first byte in the word is the most significant),
  or in other words, the two bytes in the word are 16 continuous
  bits in memory, going from most significant (bit 15) down to least
  significant (bit 0).

  The machine has three state registers which can be loaded onto the
  stack for manipulation, and stored back again.
  Our calling convention for procedures is as follows:

  stack frame layout and pointer locations:                 DMA
                                                            DMA
        *                         *                         DMA
  top ->*                         *                         DMA
        * local variables         *                         DMA
        ***************************                         DMA
        * dynamic link (dl)       *                         DMA
        * static link (sl)        *                         DMA
        * lexic level delta (lld) *                         DMA
  bp -> * return address          *                         DMA
        ***************************                         DMA
                                                            DMA
  dl  - rbp of calling procedure's frame for popping stack  DMA
  sl  - rbp of enclosing procedure for addressing variables DMA
  lld - ll difference (delta) between a called procedure    DMA
        and its calling procudure                           DMA

-}
module TastierMachine.Machine where
import qualified TastierMachine.Instructions as Instructions
import Data.Int (Int8, Int16, Int32)
import Data.Char (intToDigit, chr)
import Numeric (showIntAtBase) 
import Data.Bits (complement, (.|.), shiftR)
import Data.Array ((//), (!), Array, elems)
import Control.Monad.RWS.Lazy (RWS, put, get, ask, tell, local)
import System.IO.Unsafe (unsafePerformIO)
import System.IO (hFlush, stdout)
import Data.List (intersperse)

debug' m@(Machine rpc rtp rbp imem _ _) = do {
  putStrLn $
    concat $
      intersperse "\t| " $
        (zipWith (++)
          ["rpc: ", "rtp: ", "rbp: "]
          [show rpc, show rtp, show rbp])
        ++
        [(show $ imem ! rpc)];
  hFlush stdout;
  return m
}

debug = unsafePerformIO . debug'

data Machine = Machine { rpc :: Int16,  -- ^ next instruction to execute
                         rtp :: Int16,  -- ^ top of the stack
                         rbp :: Int16,  -- ^ base of the stack

                         imem :: (Array Int16 Instructions.InstructionWord),
                                                      -- ^ instruction memory
                         dmem :: (Array Int16 Int16), -- ^ data memory
                         smem :: (Array Int16 Int16)  -- ^ stack memory
                       }
                       deriving (Show)

{-
  This function implements the internal state machine executing the
  instructions.
-}

run :: RWS [Int16] [String] Machine ()
run = do
  machine'@(Machine rpc rtp rbp imem dmem smem) <- get
  let machine = machine'
  let instructionWord = imem ! rpc

  case instructionWord of
    Instructions.Nullary i ->
      case i of
        Instructions.Halt -> do
          return ()
          
        Instructions.MemErr -> do
          tell $ [show $ "Memory access out of bounds!"]
          return ()

        Instructions.Dup -> do
          put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                          smem = (smem // [(rtp, smem ! (rtp-1))]) }
          run

        Instructions.Nop -> do
          put $ machine { rpc = rpc + 1 }
          run

        Instructions.Add -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = b + a
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Sub    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = b - a
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Mul    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = b * a
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Div    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = b `div` a
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Equ    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b == a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Lss    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b < a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Gtr    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b > a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Neq    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b /= a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Lse    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b <= a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Gte    -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          let result = fromIntegral $ fromEnum (b >= a)
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(rtp-2, result)]) }
          run

        Instructions.Neg    -> do
          let a = smem ! (rtp-1)
          let result = complement a
          put $ machine { rpc = rpc + 1, smem = (smem // [(rtp-1, result)]) }
          run

        Instructions.Ret    -> do
          {-
            The return address is on top of stack, set the pc to that address
          -}
          put $ machine { rpc = (smem ! (rtp-1)), rtp = rtp - 1 }
          run

        Instructions.Read   -> do
          value <- ask
	  case value of
            (i:rest) -> do
              put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                              smem = (smem // [(rtp, i)]) }
              local tail run
            [] -> error $ "Read instruction issued but no data left to read"

        Instructions.Write  -> do
          tell $ [show $ smem ! (rtp-1)]
          put $ machine { rpc = rpc + 1, rtp = rtp - 1 }
          run
        
        {- StoArr takes the value to be stored and the array offset off
           the top of Smem. It then stores the value at the offset in dmem.
        -}
        Instructions.StoArr -> do
          let a = smem ! (rtp-1)
          let b = smem ! (rtp-2)
          put $ machine { rpc = rpc + 1, rtp = rtp - 2,
                          dmem = (dmem // [(b-3, (a))]) }
          run
          
        {- LoadArr takes pointer located at the top of smem and uses it
           to index the start address of the array out of dmem. It then
           loads this address on top of smem.
        -}
        Instructions.LoadArr -> do
          let a = smem ! (rtp-1)
          put $ machine { rpc = rpc + 1,
                          smem = (smem // [((rtp-1), (dmem ! (a-3)))]) }
          run
        
        {- StringPrint takes an address off the top of the stack and
           uses it to index the location of the desired string in dmem.
           The length of the string is stored at that position, this is 
           used to extract the string from memory. The ASCII representation
           of the string is converted to chars, and then output.
        -}
        Instructions.StringPrint  -> do
          let addr = (smem ! (rtp-1))

          let dlist = elems dmem                          -- convert dmem into a list
          let slength = dlist !! (fromIntegral(addr-3))
          let dlist2 = dropsome ((addr-3)-slength) dlist            -- drop all the elements of dmem up to the string
          let dlist3 = takesome (slength) dlist2  -- take length elements to extract the string
          
          
          let dlist4 = map fromIntegral dlist3         -- convert from 16 to 32 bit integers
          let dcharlist = map chr dlist4                  -- convert 32 bit integers to chars
          let revcharlist = reverse dcharlist
          tell $ [show $ revcharlist]
          
          put $ machine { rpc = rpc + 1, rtp = rtp - 1 }
          run

        Instructions.Leave  -> do
          {-
            When we're leaving a procedure, we have to reset rbp to the
            value it had in the calling context. Our calling convention is
            that we store the return address at the bottom of the stack
            frame (at rbp), and the old base pointer in the dynamic link
            field (at rbp+3).

            We reset rbp to whatever the value of the dynamic link field is.
            Since we created the stack frame for this procedure, the one
            from which we're now returning, at the *top* of the stack when
            it was called, we know that when we called the procedure, rtp
            was equal to whatever value rbp has *at present*. However, we
            want to leave the return address on the top of the stack for RET
            to jump to. We can simply set rtp to one past the current rbp
            (effectively popping off the local variables of this procedure,
            the dynamic link, static link, and lexical level delta fields).
          -}
          put $ machine { rpc = rpc + 1, rtp = rbp+1, rbp = (smem ! (rbp+3)) }
          run

    Instructions.Unary i a ->
      case i of
        Instructions.StoG   -> do
          -- memory mapped control and status registers implemented here
          case a of
            0 -> put $ machine { rpc = (smem ! (rtp-1)), rtp = rtp - 1 }
            1 -> put $ machine { rpc = rpc + 1, rtp = (smem ! (rtp-1)) }
            2 -> put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                                 rbp = (smem ! (rtp-1)) }
            _ -> put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                                 dmem = (dmem // [(a-3, (smem ! (rtp-1)))]) }
          run

        Instructions.LoadG  -> do
          -- memory mapped control and status registers implemented here
          case a of
            0 -> put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                                 smem = (smem // [(rtp, rpc)]) }
            1 -> put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                                 smem = (smem // [(rtp, rtp)]) }
            2 -> put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                                 smem = (smem // [(rtp, rbp)]) }
            _ -> put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                                 smem = (smem // [(rtp, (dmem ! (a-3)))]) }
          run

        Instructions.Const  -> do
          put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                          smem = (smem // [(rtp, a)]) }
          run

        Instructions.Enter  -> do
          {-
            ENTER has to set up both the static and dynamic link fields of
            the stack frame which is being entered. The dynamic link (the
            base pointer of the stack frame from which this procedure was
            called) needs to go on the top of stack, as specified by our
            calling convention.

            The static link (the base pointer of the stack frame belonging
            to the *enclosing* procedure (the one where this procedure was
            defined, and we should have access to the local variables)
            should be placed at rbp+2. If we are calling a procedure
            which is defined in the same scope, then the static link is just
            a copy of whatever the current static link is. However, if that
            isn't the case, then we need to call followChain to find out
            what the base pointer was in the stack frame where the procedure
            we're entering was defined.
          -}
          let lexicalLevelDelta = (smem ! (rtp-1))
          let staticLink = (followChain 0 lexicalLevelDelta rbp smem)
          put $ machine { rpc = rpc + 1, rtp = rtp+a+2, rbp = rtp-2,
                          smem = (smem // [(rtp, staticLink), (rtp+1, rbp)]) }
          run

        Instructions.Jmp  -> do
          put $ machine { rpc = a }
          run

        Instructions.FJmp -> do
          let jump = smem ! (rtp-1)
          if jump == 0 then do
            put $ machine { rtp = rtp - 1, rpc = a }
            run
          else do
            put $ machine { rtp = rtp - 1, rpc = rpc + 1 }
            run

    Instructions.Binary i a b ->
      case i of
        Instructions.Load   -> do
          {-
            Load gets a variable from a calling frame onto the top of the
            stack. We follow the chain of links to find the stack frame the
            variable is in, add b (the address of the variable in that
            frame) and add two, because each frame has the dynamic link and
            static link stored just before the start of the real stack
            variables, but it's the static link whose address we get from
            followChain.
          -}
          let loadAddr = (followChain 0 a rbp smem) + 4 + b
          put $ machine { rpc = rpc + 1, rtp = rtp + 1,
                          smem = (smem // [(rtp, (smem ! loadAddr))]) }
          run

        Instructions.Sto    -> do --Store updates a variable in a calling frame
          let storeAddr = (followChain 0 a rbp smem) + 4 + b
          put $ machine { rpc = rpc + 1, rtp = rtp - 1,
                          smem = (smem // [(storeAddr, (smem ! (rtp-1)))]) }
          run
          
        

        Instructions.Call   -> do
          {-
            CALL gets passed the lexical level delta in slot a, and the
            address of the procedure in slot b. CALL pushes the return
            address onto the stack, then the lexical level delta, so when
            the called procedure does ENTER, the stack contains the lexical
            level delta at (rtp - 1) and the return address at (rtp - 2).
          -}
          put $ machine { rpc = b, rtp = rtp + 2,
                          smem = (smem // [(rtp, (rpc+1)), (rtp+1, a)]) }
          run
          
{- Takes the first N elements from a list of 16bit integers
   and returns those elements as a list.
-}
takesome :: Int16 -> [a] -> [a]
takesome _ [] = []
takesome 0 _ = []
takesome n (x:xs) = x : takesome (n-1) xs

{- Drops the first N elements from a list of 16bit integers
   and returns the remainder of the list.
-}
dropsome :: Int16 -> [a] -> [a]
dropsome _ [] = []
dropsome 0 (x:xs) = (x:xs)
dropsome n (x:xs) = dropsome (n-1) xs

{-
  followChain follows the static link chain to find the absolute address in
  stack memory of the base of the stack frame (n-limit) levels down the call
  stack. Each time we unwind one call, we recurse with rbp set to the base
  pointer of the stack frame which we just unwound. When we've unwound the
  correct number of stack frames, as indicated by the argument *limit*, we
  return the base pointer of the stack frame we've unwound our way into.
-}

followChain :: Int16 -> Int16 -> Int16 -> (Array Int16 Int16) -> Int16
followChain limit n rbp smem =
  if n > limit then
    followChain limit (n-1) (smem ! (rbp+2)) smem
  else rbp
