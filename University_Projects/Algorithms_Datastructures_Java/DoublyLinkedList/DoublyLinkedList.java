import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

// -------------------------------------------------------------------------
/**
 *  This class contains the methods of Doubly Linked List.
 *
 *  @author  Lorcan Reidy
 *  @version 12/10/14 11:16:54
 */

class DoublyLinkedList<T extends Comparable<T>> implements Iterable<T>
{

    /**
     * private class DLLNode: implements a *generic* Doubly Linked List node.
     */
    private class DLLNode
    {
        public T data;
        public DLLNode next;
        public DLLNode prev;
    
        /**
         * Constructor
         * @param theData : data of type T, to be stored in the node
         * @param prevNode : the previous Node in the Doubly Linked List
         * @param nextNode : the next Node in the Doubly Linked List
         * @return DLLNode
         */
        public DLLNode( T theData, DLLNode prevNode, DLLNode nextNode ) 
        {
          data = theData;
          prev = prevNode;
          next = nextNode;
        }
    }

    // the DLLNode class is defined at the end of this file.
    // Fields head and tail point to the first and last nodes of the list.
    //
    private DLLNode head, tail;
    private int size = 0; // stores the number of nodes in the doubly linked list

    /**
     * Constructor
     * @return DoublyLinkedList
     */
    public DoublyLinkedList() 
    {
      head = null; tail = null;
    }

    /**
     * Tests if the doubly linked list is empty
     * @return true if list is empty, and false otherwise
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the isEmpty method will always execute a constant number of lines
     * 				  as there is no iteration involved.
     *  
     */
    public boolean isEmpty()
    {
    	if(head == null)
    	{
    		return true;
    	}
      	return false;
    }

    /**
     * Inserts an element at the beginning of the doubly linked list
     * @param data : The new data of class T that needs to be added to the list
     * @return none
     *
     * Worst-case asymptotic runtime cost: theta(1)
     * 
     * Justification: the insertFirst method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *  
     */
    public void insertFirst( T data ) 
    {
    	if(this.isEmpty())
    	{
    		head = new DLLNode(data, null, null);
    		tail = head;
    	}
    	else
    	{
    		DLLNode newNode = new DLLNode(data, null, head);
    		head.prev = newNode;
    		head = newNode; // ???????????????????????????????????????????
    	}
    	size++;
    }

    /**
     * Inserts an element at the end of the doubly linked list
     * @param data : The new data of class T that needs to be added to the list
     * @return none
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the insertLast method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *  
     */
    public void insertLast( T data ) 
    {
    	if(this.isEmpty())
    	{
    		head = new DLLNode(data, null, null);
    		tail = head;
    	}
    	else
    	{
    		DLLNode newNode = new DLLNode(data, tail, null);
    		tail.next = newNode;
    		tail = newNode; // ???????????????????????????????????????????
    	}
    	size++;
    }

    /**
     * Inserts an element in the doubly linked list
     * @param pos : The integer location at which the new data should be
     *      inserted in the list. We assume that the first position in the list
     *      is 0 (zero). If pos is less than 0 then add to the head of the list.
     *      If pos is greater or equal to the size of the list then add the
     *      element at the end of the list.
     * @param data : The new data of class T that needs to be added to the list
     * @return none
     *
     * Worst-case asymptotic runtime cost: theta(n)
     *
     * Justification: considering a worst-case scenario, insertBefore will have to iterate
     * 				  through all the elements (n) in the link list. therefore it's
     * 				  asymptotic runtime is theta(n).
     *  
     */
    public void insertBefore( int pos, T data ) 
    {
    	if(pos <= 0)
    	{
    		insertFirst(data);
    	}
    	else if(pos >= size)
    	{
    		insertLast(data);
    	}
    	else
    	{
    		DLLNode tempNode = head;
        	for(int i = 0; i < pos; i++)
        	{
       			tempNode = tempNode.next;
        	}
        	DLLNode newNode = new DLLNode(data, tempNode.prev, tempNode);
        	(tempNode.prev).next = newNode;
        	tempNode.prev = newNode;
        	size++;
    	}
    }

    
    /**
     * @return the data at the beginning of the list, if the list is non-empty, and null otherwise.
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the getFirst method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *     
     */
    public T getFirst()
    {
    	if(!this.isEmpty())
    	{
    		return head.data;
    	}
    	return null;
    }

    /**
     * @return the data at the end of the list, if the list is non-empty, and null otherwise.
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the getLast method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *     
     */
    public T getLast()
    {
    	if(!this.isEmpty())
    	{
    		return tail.data;
    	}
    	return null;
    }

    /**
     * Returns the data stored at a particular position
     * @param pos : the position
     * @return the data at pos, if pos is within the bounds of the list, and null otherwise.
     *
     * Worst-case asymptotic runtime cost: theta(n)
     *
     * Justification: considering a worst-case scenario, get will have to iterate
     * 				  through all the elements (n) in the link list before it reaches
     * 				  the desired element. therefore it's asymptotic runtime is theta(n).
     *
     * Worst-case precise runtime cost: (2n + 2) time units
     *
     * Justification: considering a worst-case scenario, the get method will have to return the data
     * 				  from the very last node in the doubly linked list. In this case, the method will
     * 				  first determine that the position passed to it does not exceed the bounds of the list.
     * 				  Then create a new temporary node which will be used to increment through the list.
     * 				  This incrementing will be done in a for loop which will increment until the node 
     * 				  just before the desired node is reached. The method will then return the data contained
     * 				  in the temporary node. The cost of these operations can be seen in the comments below.
     *  
     */
    public T get( int pos ) 
    {
    	if(pos >= size || pos < 0)  	// +1 time unit
    	{
    		return null;
    	}
    	DLLNode tempNode = head;		// +1 time unit
       	for(int i = 0; i < pos; i++)	// +n time unit
       	{
       		tempNode = tempNode.next;	// +(n-1) time unit
       	}
       	return tempNode.data;			// +1 time unit
    }


    /**
     * Deletes an element at the beginning of the doubly linked list
     * @return true : on successful deletion, false : list has not been modified. 
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the deleteFirst method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *     
     */
    public boolean deleteFirst() 
    {
    	if(!this.isEmpty())
    	{
    		if(head.next != null)
    		{
    			head = head.next;
    			head.prev = null; // ????????
    			size--;
    			return true;
    		}
    		else
    		{
    			head = null;
    			tail = null;
    			size--;
    			return true;
    		}
    	}
    	return false;
    }

    /**
     * Deletes an element at the end of the doubly linked list
     * @param data : The new data of class T that needs to be added to the list
     * @return true : on successful deletion, false : list has not been modified. 
     *
     * Worst-case asymptotic runtime cost: theta(1)
     *
     * Justification: the deleteLast method will always execute a constant number of lines
     * 				  as there is no iteration involved. The isEmpty method also has a constant
     * 				  asymptotic runtime.
     *     
     */
    public boolean deleteLast() 
    {
    	if(!this.isEmpty())
    	{
    		if(tail.prev != null)
    		{
    			tail = tail.prev;
    			tail.next = null; // ????????
    			size--;
    			return true;
    		}
    		else
    		{
    			head = null;
    			tail = null;
    			size--;
    			return true;
    		}
    	}
    	return false;
    }

    /**
     * Deletes the element of the list at position pos.
     * First element in the list has position 0. If pos points outside the
     * elements of the list then no modification happens to the list.
     * @param pos : the position in the list to be deleted.
     * @return true : on successful deletion, false : list has not been modified. 
     *
     * Worst-case asymptotic runtime cost: theta(n)
     *
     * Justification: considering a worst-case scenario, deleteAt will have to iterate
     * 				  through all the elements (n) in the link list before it reaches
     * 				  the desired element. therefore it's asymptotic runtime is theta(n).
     *  
     */
    public boolean deleteAt( int pos ) 
    {
    	if(pos >= size || pos < 0)
    	{
    		return false;
    	}
    	DLLNode tempNode = head;
       	for(int i = 0; i < pos; i++)
       	{
       		tempNode = tempNode.next;
       	}
       	if(tempNode == head)
       	{
       		deleteFirst();
       	}
       	else if(tempNode == tail)
       	{
       		deleteLast();
       	}
       	else
       	{
       		(tempNode.prev).next = tempNode.next;
       		(tempNode.next).prev = tempNode.prev;
       		tempNode = null; //??????
       		size--;
       	}
       	return true;
    }


    /**
     * @return a string with the elements of the list as a comma-separated
     * list, from beginning to end
     *
     * Worst-case asymptotic runtime cost:   Theta(n)
     *
     * Justification:
     *  We know from the Java documentation that StringBuilder's append() method runs in Theta(1) asymptotic time.
     *  We assume all other method calls here (e.g., the iterator methods above, and the toString method) will execute in Theta(1) time.
     *  Thus, every one iteration of the for-loop will have cost Theta(1).
     *  Suppose the doubly-linked list has 'n' elements.
     *  The for-loop will always iterate over all n elements of the list, and therefore the total cost of this method will be n*Theta(1) = Theta(n).
     */
    public String toString() 
    {
      StringBuilder s = new StringBuilder();
      boolean isFirst = true; 

      // iterate over the list, starting from the head
      for (T i : this)
      {
        if (!isFirst)
        {
          s.append(",");
        } else {
          isFirst = false;
        }
        s.append(i.toString());
      }

      return s.toString();
    }



    /**
     * Returns an iterator over the DLL. The iterator supports the hasNext() and next() methods  but not the remove() method.
     * @return an object implementing the Iterator interface
     */

    public Iterator<T> iterator() { return new DLLIterator(); }

    private class DLLIterator implements Iterator<T>
    {
        private DLLNode current = head;

        public boolean hasNext() { return current != null; }
        public void remove() { throw new UnsupportedOperationException("remove() is not supported"); }
        public T next()
        {
            if (current == null)
                throw new NoSuchElementException("No next element in the list");
            T item = current.data;
            current = current.next;
            return item;
        }
    }



    /**
     * Returns a list iterator over the DLL. The iterator supports forwards and backwards iteration over the list but no modification operations.
     * @return an object implementing the ListIterator interface
     */
      public ListIterator<T> listIterator() { return new DLLListIterator(); }

      private class DLLListIterator implements ListIterator<T>
      {
    	  /**
    	   * Author note: Rather than throwing an exception when incrementing outside the boundaries of the list,
    	   * 		      a null value is returned. This is done so that you can increment back inside the boundaries of the list.
    	   * 			  (e.g. calling next() until the tail of the list is reached, then being able to call previous to decrement back into the list)  
    	   */
    	  
    	  
    	  private DLLNode current = head;
    	  private DLLNode previousNode;
    	  private int index = 0; 
    	  private boolean increasing = false;
    	  private boolean decreasing = false;
    	  
        /**
         * Returns true if this list iterator has more elements when
         * traversing the list in the forward direction. (In other words,
         * returns true if next() would return an element rather
         * than null)
         *
         * @return true if the list iterator has more elements when
         *         traversing the list in the forward direction
         */
          public boolean hasNext()
          {
        	  if(decreasing)
        	  {
        		  return true;
        	  }
        	  return current != null;
          }

        /**
         * Returns the next element in the list and advances the cursor position.
         * This method may be called repeatedly to iterate through the list,
         * or intermixed with calls to previous() to go back and forth.
         * (Note that alternating calls to next() and previous()
         * will return the same element repeatedly.)
         *
         * @return the next element in the list
         * @return null if the iteration has no next
         *         element
         */
          public T next()
          {
          		if (current == null)
          		{
          			//throw new NoSuchElementException("No next element in the list");
            		if(decreasing)
            		{
            			index = index + 2;
            			increasing = true;
            			decreasing = false;
            			current = previousNode;
            			T item = current.data;
            			current = current.next;
            			return item;
            		}
            		else
            		{
            			return null;
            		}
          		} 
              	increasing = true;
              	if(decreasing)
              	{
              		decreasing = false;
              		current = current.next;
              	}
              	index++;
              	T item = current.data;
              	previousNode = current;
            	current = current.next;
             	return item;
          }

        /**
         * Returns true if this list iterator has more elements when
         * traversing the list in the reverse direction.  (In other words,
         * returns true if previous() would return an element
         * rather than null.)
         *
         * @return true if the list iterator has more elements when
         *         traversing the list in the reverse direction
         */
          public boolean hasPrevious()
          {
        	  if(increasing)
        	  {
        		  return true;
        	  }
        	  return current != null;
          }

        /**
         * Returns the previous element in the list and moves the cursor
         * position backwards.  This method may be called repeatedly to
         * iterate through the list backwards, or intermixed with calls to
         * next() to go back and forth.  (Note that alternating calls
         * to next() and previous() will return the same
         * element repeatedly.)
         *
         * @return the previous element in the list
         * @return null if the iteration has no previous
         *         element
         */
          public T previous()
          {
            	if (current == null)
            	{
            		//throw new NoSuchElementException("No previous element in the list");
            		if(increasing)
            		{
            			index--;
            			increasing = false;
            			decreasing = true;
            			current = previousNode;
            			T item = current.data;
            			current = current.prev;
            			return item;
            		}
            		else
            		{
            			return null;
            		}
            	} 
            	decreasing = true;
            	if(increasing)
            	{
            		increasing = false;
            		current = current.prev;
            	}
            	index--;
            	T item = current.data;
            	previousNode = current;
            	current = current.prev;
            	return item;
          }

        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to next(). (Returns list size if the list
         * iterator is at the end of the list.)
         *
         * @return the index of the element that would be returned by a
         *         subsequent call to next(), or list size if the list
         *         iterator is at the end of the list
         */
          public int nextIndex()
          {
        	  if((current == null) && decreasing)
        	  {
        		  return 0;
        	  }
        	  if((index) >= size)
        	  {
        		  return size;
        	  }
        	  return index;
          }

        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to previous(). (Returns -1 if the list
         * iterator is at the beginning of the list.)
         *
         * @return the index of the element that would be returned by a
         *         subsequent call to previous(), or -1 if the list
         *         iterator is at the beginning of the list
         */
          public int previousIndex()
          {
        	  if((index) <= 0)
        	  {
        		  return -1;
        	  }
        	  return index-1;
          }

         public void remove() { throw new UnsupportedOperationException("remove() operation is not supported."); }
         public void set(T e) { throw new UnsupportedOperationException("set() operation is not supported."); }
         public void add(T e) { throw new UnsupportedOperationException("set() operation is not supported."); }

      }

}



