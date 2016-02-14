import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNull;

import org.junit.Test;
import org.junit.Ignore;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

//-------------------------------------------------------------------------
/**
 *  Test class for Doubly Linked List
 *
 *  @author  Lorcan Reidy
 *  @version 2014.01.29
 */
@RunWith(JUnit4.class)
public class DoublyLinkedListTest
{
    //~ Constructor ........................................................
    @Test
    public void testConstructor()
    {
      new DoublyLinkedList<Integer>();
    }

    //~ Public Methods ........................................................

    // ----------------------------------------------------------
    /**
     * Check if isEmpty method works for empty and non-empty lists
     */
    @Test
    public void testIsEmpty()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        assertEquals( "Checking empty list", true, testDLL.isEmpty() );
        testDLL.insertFirst(1);
        assertEquals( "Checking non-empty list", false, testDLL.isEmpty() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if insert works at the beginning
     */
    @Test
    public void testInsertFirst()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertFirst(1);
        assertEquals( "Checking insertion at beginning", "1", testDLL.toString() );
        testDLL.insertFirst(2);
        assertEquals( "Checking insertion at beginning", "2,1", testDLL.toString() );
        testDLL.insertFirst(3);        
        assertEquals( "Checking insertion at beginning", "3,2,1", testDLL.toString() );
    }

    // ----------------------------------------------------------
    /**
     * Check if the insert works at the end
     */
    @Test
    public void testInsertLast()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        assertEquals( "Checking insertion at end", "1", testDLL.toString() );
        testDLL.insertLast(2);
        assertEquals( "Checking insertion at end", "1,2", testDLL.toString() );
        testDLL.insertLast(3);        
        assertEquals( "Checking insertion at end", "1,2,3", testDLL.toString() );
    }

    // ----------------------------------------------------------
    /**
     * Check if the insert works in the middle
     */
    @Test
    public void testInsertBefore()
    {
        // test non-empty list
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);

        testDLL.insertBefore(0,4);
        assertEquals( "Checking insertion at 0", "4,1,2,3", testDLL.toString() );
        testDLL.insertBefore(1,5);
        assertEquals( "Checking insertion at 1", "4,5,1,2,3", testDLL.toString() );
        testDLL.insertBefore(2,6);        
        assertEquals( "Checking insertion at 2", "4,5,6,1,2,3", testDLL.toString() );
        testDLL.insertBefore(-1,7);        
        assertEquals( "Checking insertion at 2", "7,4,5,6,1,2,3", testDLL.toString() );
        testDLL.insertBefore(7,8);        
        assertEquals( "Checking insertion at 2", "7,4,5,6,1,2,3,8", testDLL.toString() );
        testDLL.insertBefore(700,9);        
        assertEquals( "Checking insertion at 2", "7,4,5,6,1,2,3,8,9", testDLL.toString() );

        // test empty list
        testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertBefore(0,1);        
        assertEquals( "Checking insertion at 2", "1", testDLL.toString() );
        testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertBefore(10,1);        
        assertEquals( "Checking insertion at 2", "1", testDLL.toString() );
        testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertBefore(-10,1);        
        assertEquals( "Checking insertion at 2", "1", testDLL.toString() );
     }
    
    // ----------------------------------------------------------
    /**
     * Check if delete works at the beginning
     */
    @Test
    public void testDeleteFirst()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
    	// test non-empty list
        testDLL.deleteFirst();
        assertEquals( "Checking deletion of first element", "2,3", testDLL.toString() );
        testDLL.deleteFirst();
        assertEquals( "Checking deletion of second element", "3", testDLL.toString() );
        testDLL.deleteFirst();
        assertEquals( "Checking deletion of third element", "", testDLL.toString() );
        
    	// test empty list
        testDLL.deleteFirst();
        assertEquals( "Checking deletion on empty list", "", testDLL.toString() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if delete works at the end
     */
    @Test
    public void testDeleteLast()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        // test non-empty list
        testDLL.deleteLast();
        assertEquals( "Checking deletion of third element", "1,2", testDLL.toString() );
        testDLL.deleteLast();
        assertEquals( "Checking deletion of second element", "1", testDLL.toString() );
        testDLL.deleteLast();
        assertEquals( "Checking deletion of first element", "", testDLL.toString() );
        
    	// test empty list
        testDLL.deleteLast();
        assertEquals( "Checking deletion on empty list", "", testDLL.toString() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if delete works in the middle
     */
    @Test
    public void testDeleteAt()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        // test non-empty list
        testDLL.deleteAt((-1));
        assertEquals( "Checking deletion of negative out of bounds element", "1,2,3", testDLL.toString() );
        testDLL.deleteAt(10);
        assertEquals( "Checking deletion of positive out of bounds element", "1,2,3", testDLL.toString() );
        testDLL.deleteAt(1);
        assertEquals( "Checking deletion of middle element", "1,3", testDLL.toString() );
        testDLL.deleteAt(1);
        assertEquals( "Checking deletion of last element", "1", testDLL.toString() );
        testDLL.deleteAt(0);
        assertEquals( "Checking deletion of first element", "", testDLL.toString() );
        
    	// test empty list
        testDLL.deleteAt(0);
        assertEquals( "Checking deletion on empty list", "", testDLL.toString() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if getFirst works
     */
    @Test
    public void testGetFirst()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        // test non-empty list
        assertEquals( "Checking obtaining data in first node", (Integer)1, testDLL.getFirst() );
        
        testDLL = new DoublyLinkedList<Integer>();
    	// test empty list
        assertEquals( "Checking obtaining data from non-existent first node", null, testDLL.getFirst() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if getLast works
     */
    @Test
    public void testGetLast()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        // test non-empty list
        assertEquals( "Checking obtaining data in last node", (Integer)3, testDLL.getLast() );
        
        testDLL = new DoublyLinkedList<Integer>();
    	// test empty list
        assertEquals( "Checking obtaining data from non-existent last node", null, testDLL.getLast() );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if get at a particular position works
     */
    @Test
    public void testGet()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        // test non-empty list
        assertEquals( "Checking obtaining data in first node", (Integer)1, testDLL.get(0) );
        assertEquals( "Checking obtaining data in second node", (Integer)2, testDLL.get(1) );
        assertEquals( "Checking obtaining data in third node", (Integer)3, testDLL.get(2) );
        assertEquals( "Checking obtaining data in positive out of bounds node", null, testDLL.get(15) );
        assertEquals( "Checking obtaining data in negative out of bounds node", null, testDLL.get(-2) );
        
        testDLL = new DoublyLinkedList<Integer>();
    	// test empty list
        assertEquals( "Checking obtaining data from non-existent node", null, testDLL.get(0) );
    }
    
    // ----------------------------------------------------------
    /**
     * Check if the hasNext and next methods in the iterator class work
     */
    @Test
    public void testIteratorMethods()
    {
        DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        
        Iterator<Integer> iterator = testDLL.iterator();

        assertEquals("Checking if first element is there", true, iterator.hasNext());
        assertEquals("Obtaining first element", (Integer)1, iterator.next());
        assertEquals("Checking if second element is there", true, iterator.hasNext());
        assertEquals("Obtaining first element", (Integer)2, iterator.next());
        assertEquals("Checking if third element is there", true, iterator.hasNext());
        assertEquals("Obtaining first element", (Integer)3, iterator.next());
        assertEquals("Checking for out of bounds element", false, iterator.hasNext());
 
    }
    
    // ----------------------------------------------------------
    /**
     * ....
     */
    @Test
    public void testListIteratorMethods()
    {
    	DoublyLinkedList<Integer> testDLL = new DoublyLinkedList<Integer>();
        testDLL.insertLast(1);
        testDLL.insertLast(2);
        testDLL.insertLast(3);
        testDLL.insertLast(4);
    	
    	ListIterator<Integer> listIterator = testDLL.listIterator();
    	
    	
    	assertEquals("Testing hasNext before incremening starts", true, listIterator.hasNext());
    	assertEquals("Testing hasPrevious before incremening starts", true, listIterator.hasPrevious());
    	assertEquals("Checking decrement from fist element in list", (Integer)1, listIterator.previous());
    	assertEquals("Checking for element outside list boundary", false, listIterator.hasPrevious());
    	assertEquals("Checking for an element inside list boundary from outside boundary", true, listIterator.hasNext());
    	assertEquals("exceeding lower boundry of list", null, listIterator.previous());
    	assertEquals("Checking index outside list boundary", -1, listIterator.previousIndex());
    	assertEquals("Checking index inside boundary from outside boundary", 0, listIterator.nextIndex());
    	
    	assertEquals("Checking incrementing back through list having exceeded lower boundary", (Integer)1, listIterator.next());
    	assertEquals("Checking incrementing through list", (Integer)2, listIterator.next());
    	
    	assertEquals("Checking next index from the middle of the list", 2, listIterator.nextIndex());
    	assertEquals("Checking previous index from the middle of the list", 1, listIterator.previousIndex());
    	assertEquals("Checking incrementing through list", (Integer)2, listIterator.previous());
    	assertEquals("Checking incrementing through list", (Integer)2, listIterator.next());
    	
    	assertEquals("Checking incrementing through list", (Integer)3, listIterator.next());
    	assertEquals("Checking incrementing to tail of list", (Integer)4, listIterator.next());
    	
    	assertEquals("Checking if a next element exists past the tail of the list", false, listIterator.hasNext());
    	assertEquals("Checking for an element inside list boundary from outside boundary", true, listIterator.hasPrevious());
    	
    	assertEquals("exceeding upper boundary of list", null, listIterator.next());
    	assertEquals("Checking index outside list boundary", 4, listIterator.nextIndex());
    	assertEquals("Checking index inside boundary from outside boundary", 3, listIterator.previousIndex());
    	
    	assertEquals("Checking decrement back into the list", (Integer)4, listIterator.previous());
    }
}

