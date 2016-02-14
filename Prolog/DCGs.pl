s --> zeros(Count),[2],ones(Count).

zeros(0) --> [].
zeros(succ(Count)) --> [0], zeros(Count).
zeros(Count) --> [1], zeros(Count).



ones(0) --> [].
ones(succ(Count)) --> [1], ones(Count).
ones(Count) --> [0], ones(Count).

%%%%%%%%%%%%%%%%%%%%%%%

solution --> color(Col1),nat(Nat1),pet(Pet1),
             color(Col2),nat(Nat2),pet(Pet2), {Col2 \= Col1}, {Nat2 \= Nat1}, {Pet2 \= Pet1},
             color(Col3),nat(Nat3),pet(Pet3), {Col3 \= Col1}, {Col3 \= Col2}, {Nat3 \= Nat1}, {Nat3 \= Nat2}, {Pet3 \= Pet1}, {Pet3 \= Pet2}.

pet(snail) --> [snail].
pet(jaguar) --> [jaguar].
pet(zebra) --> [zebra].

nat(english) --> [english].
nat(japanese) --> [japanese].
nat(spanish) --> [spanish].

color(red) --> [red].
color(blue) --> [blue].
color(green) --> [green].

%%%%%%%%%%%%%%%%%%%%%%%

sumUp(Sum) --> {Sum > 0},
               {mkList(Sum, L)},
               search(L, 0, Sum, R),
               {R =:= Sum}.
               
search(L, Acc, Sum, R) --> {member(X,L)},
                           [X],
                           {Acc1 is Acc + X},
                           {Acc1 =< Sum},
                           search(L, Acc1, Sum, R).
search(_, Acc, _, Acc) --> [].
    
mkList(N, L):- 
  findall(Num, between(1, N, Num), L1),
  rev(L1,L).
    
revList([H|T],Acc,Res):- revList(T,[H|Acc],Res).
revList([],Acc,Acc).

rev(L,R):- revList(L,[],R).

