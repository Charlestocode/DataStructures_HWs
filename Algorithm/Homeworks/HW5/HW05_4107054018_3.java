public class HW05_4107054018_3 extends LLK {
    
    @Override
    public boolean checkLLK(int[][] array) {

        // If there are only two points. they must be collinear
        if (array.length <= 2) {
            return true;
        }

        // Create a slope-count map to record the counts of each slope.
        HashSet<Float> slopeCountSet = new HashSet<>();
        
        // Declare a variable slope to store each slope after getting a new slope.
        Float slope;

        // Outer loop index i is for pivot points. Inner loop index j is for any other points.
        for (int i = 0; i < array.length; i++) {
            for (int j = 0; j < array.length; j++) {
                if (i != j) {
                    slope = findSlope(array[i], array[j]);
                    
                    if (slopeCountSet.contains(slope)) {
                        return true;
                    }
                    else {
                        slopeCountSet.add(slope);
                    }
                }
            }
            // Before changing the pivot point, we have to clear the whole sets in `slopeCountSet`.
            slopeCountSet.clear();
        }

        return false;
    }

    private Float findSlope(int[] point1, int[] point2) {
        // If the variation of x coordinate is zero, the slope does not exist, so we return MAX_VALUE.
        if (point2[0] == point1[0]) {
            return Float.MAX_VALUE;
        }
        return Float.valueOf(((float) point2[1] - point1[1]) / ((float) point2[0] - point1[0]));
    }

    public static void main(String[] args) {

        HW05_4107054018_3 test = new HW05_4107054018_3();

        int[][] arr1 = new int[][]{{1, 2}, {1, 3}, {1, 4}};
        System.out.println(test.checkLLK(arr1));

        int[][] arr2 = new int[][]{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
        System.out.println(test.checkLLK(arr2));
    }
}


class HashSet<E> {

    private transient HashMap<E, Object> map;

    private static final Object PRESENT = new Object();

    public HashSet() {
        map = new HashMap<E, Object>();
    }

    public boolean add(E elem) {
        return map.put(elem, PRESENT) == null; 
    }

    public boolean remove(E elem) {
        return map.remove(elem) != null;
    }

    public boolean contains(E elem) {
        return map.containsKey(elem);
    }

    public void clear() {
        map.clear();
    }
}


class Entry<K, V> {

    int hash;
    K key;
    V value;
  
    public Entry(K key, V value) {
      this.key = key;
      this.value = value;
      this.hash = key.hashCode();
    }
  
    // We are not overriding the Object equals method
    // No casting is required with this method.
    public boolean equals(Entry<K, V> other) {
      if (hash != other.hash) return false;
      return key.equals(other.key);
    }
  
    @Override
    public String toString() {
      return key + " => " + value;
    }
}
  

@SuppressWarnings("unchecked")
class HashMap<K, V> {
  
    private static final int DEFAULT_CAPACITY = 3;
    private static final double DEFAULT_LOAD_FACTOR = 0.75;
  
    private double maxLoadFactor;
    private int capacity, threshold, size = 0;
    private LinkedList<Entry<K, V>>[] table;
  
    public HashMap() {
        this(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
    }
  
    public HashMap(int capacity) {
        this(capacity, DEFAULT_LOAD_FACTOR);
    }
  
    // Designated constructor
    public HashMap(int capacity, double maxLoadFactor) {
        if (capacity < 0) throw new IllegalArgumentException("Illegal capacity");
        if (maxLoadFactor <= 0 || Double.isNaN(maxLoadFactor) || Double.isInfinite(maxLoadFactor))
            throw new IllegalArgumentException("Illegal maxLoadFactor");
        this.maxLoadFactor = maxLoadFactor;
        this.capacity = Math.max(DEFAULT_CAPACITY, capacity);
        threshold = (int) (this.capacity * maxLoadFactor);
        table = new LinkedList[this.capacity];
    }
  
    // Returns the number of elements currently inside the hash-table
    public int size() {
        return size;
    }
  
    // Returns true/false depending on whether the hash-table is empty
    public boolean isEmpty() {
        return size == 0;
    }
  
    // Converts a hash value to an index. Essentially, this strips the
    // negative sign and places the hash value in the domain [0, capacity)
    private int normalizeIndex(int keyHash) {
        return (keyHash & 0x7FFFFFFF) % capacity;
    }
  
    // Clears all the contents of the hash-table
    public void clear() {
        for (int i = 0; i < table.length; i++) {
            table[i] = null;
        }
        size = 0;
    }
  
    public boolean containsKey(K key) {
        return hasKey(key);
    }
  
    // Returns true/false depending on whether a key is in the hash table
    public boolean hasKey(K key) {
        int bucketIndex = normalizeIndex(key.hashCode());
        return bucketSeekEntry(bucketIndex, key) != null;
    }
  
    // Insert, put and add all place a value in the hash-table
    public V put(K key, V value) {
        return insert(key, value);
    }
  
    public V add(K key, V value) {
        return insert(key, value);
    }
  
    public V insert(K key, V value) {
    
        if (key == null) throw new IllegalArgumentException("Null key");
        Entry<K, V> newEntry = new Entry<>(key, value);
        int bucketIndex = normalizeIndex(newEntry.hash);
        return bucketInsertEntry(bucketIndex, newEntry);
    }
  
    // Gets a key's values from the map and returns the value.
    // NOTE: returns null if the value is null AND also returns
    // null if the key does not exists, so watch out..
    public V get(K key) {
  
        if (key == null) return null;
        int bucketIndex = normalizeIndex(key.hashCode());
        Entry<K, V> entry = bucketSeekEntry(bucketIndex, key);
        if (entry != null) return entry.value;
        return null;
    }
  
    // Removes a key from the map and returns the value.
    // NOTE: returns null if the value is null AND also returns
    // null if the key does not exists.
    public V remove(K key) {
  
        if (key == null) return null;
        int bucketIndex = normalizeIndex(key.hashCode());
        return bucketRemoveEntry(bucketIndex, key);
    }
  
    // Removes an entry from a given bucket if it exists
    private V bucketRemoveEntry(int bucketIndex, K key) {
  
        Entry<K, V> entry = bucketSeekEntry(bucketIndex, key);
        if (entry != null) {
            LinkedList<Entry<K, V>> links = table[bucketIndex];
            links.remove(entry);
            --size;
            return entry.value;
        } else return null;
    }
  
    // Inserts an entry in a given bucket only if the entry does not already
    // exist in the given bucket, but if it does then update the entry value
    private V bucketInsertEntry(int bucketIndex, Entry<K, V> entry) {
  
      LinkedList<Entry<K, V>> bucket = table[bucketIndex];
      if (bucket == null) table[bucketIndex] = bucket = new LinkedList<>();
  
      Entry<K, V> existentEntry = bucketSeekEntry(bucketIndex, entry.key);
      if (existentEntry == null) {
        bucket.add(entry);
        if (++size > threshold) resizeTable();
        return null; // Use null to indicate that there was no previous entry
      } else {
        V oldVal = existentEntry.value;
        existentEntry.value = entry.value;
        return oldVal;
      }
    }
  
    // Finds and returns a particular entry in a given bucket if it exists, returns null otherwise
    private Entry<K, V> bucketSeekEntry(int bucketIndex, K key) {
  
      if (key == null) return null;
      LinkedList<Entry<K, V>> bucket = table[bucketIndex];
      if (bucket == null) return null;
      for (Entry<K, V> entry : bucket) if (entry.key.equals(key)) return entry;
      return null;
    }
  
    // Resizes the internal table holding buckets of entries
    private void resizeTable() {
  
      capacity *= 2;
      threshold = (int) (capacity * maxLoadFactor);
  
      LinkedList<Entry<K, V>>[] newTable = new LinkedList[capacity];
  
      for (int i = 0; i < table.length; i++) {
        if (table[i] != null) {
  
          for (Entry<K, V> entry : table[i]) {
            int bucketIndex = normalizeIndex(entry.hash);
            LinkedList<Entry<K, V>> bucket = newTable[bucketIndex];
            if (bucket == null) newTable[bucketIndex] = bucket = new LinkedList<>();
            bucket.add(entry);
          }
  
          // Avoid memory leak. Help the GC
          table[i].clear();
          table[i] = null;
        }
      }
  
      table = newTable;
    }


    // Return an iterator to iterate over all the keys in this map
    

    // Returns a string representation of this hash table
    @Override
    public String toString() {

        StringBuilder sb = new StringBuilder();
        sb.append("{");
        for (int i = 0; i < capacity; i++) {
            if (table[i] == null) continue;
            for (Entry<K, V> entry : table[i]) sb.append(entry + ", ");
        }
        sb.append("}");
        return sb.toString();
    }
}




class LinkedList<T> implements Iterable<T> {
    private int size = 0;
    private Node<T> head = null;
    private Node<T> tail = null;
  
    // Internal node class to represent data
    private static class Node<T> {
        private T data;
        private Node<T> prev, next;
    
        public Node(T data, Node<T> prev, Node<T> next) {
            this.data = data;
            this.prev = prev;
            this.next = next;
        }
    
        @Override
        public String toString() {
            return data.toString();
        }
    }
  
    // Empty this linked list, O(n)
    public void clear() {
        Node<T> trav = head;
        while (trav != null) {
            Node<T> next = trav.next;
            trav.prev = trav.next = null;
            trav.data = null;
            trav = next;
        }
        head = tail = trav = null;
        size = 0;
    }
  
    // Return the size of this linked list
    public int size() {
        return size;
    }
  
    // Is this linked list empty?
    public boolean isEmpty() {
        return size() == 0;
    }
  
    // Add an element to the tail of the linked list, O(1)
    public void add(T elem) {
        addLast(elem);
    }
  
    // Add a node to the tail of the linked list, O(1)
    public void addLast(T elem) {
        if (isEmpty()) {
            head = tail = new Node<T>(elem, null, null);
        } else {
            tail.next = new Node<T>(elem, tail, null);
            tail = tail.next;
        }
        size++;
    }
  
    // Add an element to the beginning of this linked list, O(1)
    public void addFirst(T elem) {
        if (isEmpty()) {
            head = tail = new Node<T>(elem, null, null);
        } else {
            head.prev = new Node<T>(elem, null, head);
            head = head.prev;
        }
        size++;
    }
  
    // Check the value of the first node if it exists, O(1)
    public T peekFirst() {
        if (isEmpty()) throw new RuntimeException("Empty list");
        return head.data;
    }
  
    // Check the value of the last node if it exists, O(1)
    public T peekLast() {
        if (isEmpty()) throw new RuntimeException("Empty list");
        return tail.data;
    }
  
    // Remove the first value at the head of the linked list, O(1)
    public T removeFirst() {
        // Can't remove data from an empty list
        if (isEmpty()) throw new RuntimeException("Empty list");
    
        // Extract the data at the head and move
        // the head pointer forwards one node
        T data = head.data;
        head = head.next;
        --size;
    
        // If the list is empty set the tail to null
        if (isEmpty()) tail = null;
    
        // Do a memory cleanup of the previous node
        else head.prev = null;
    
        // Return the data that was at the first node we just removed
        return data;
    }
  
    // Remove the last value at the tail of the linked list, O(1)
    public T removeLast() {
        // Can't remove data from an empty list
        if (isEmpty()) throw new RuntimeException("Empty list");
    
        // Extract the data at the tail and move
        // the tail pointer backwards one node
        T data = tail.data;
        tail = tail.prev;
        --size;
    
        // If the list is now empty set the head to null
        if (isEmpty()) head = null;
    
        // Do a memory clean of the node that was just removed
        else tail.next = null;
    
        // Return the data that was in the last node we just removed
        return data;
    }
  
    // Remove an arbitrary node from the linked list, O(1)
    private T remove(Node<T> node) {
        // If the node to remove is somewhere either at the
        // head or the tail handle those independently
        if (node.prev == null) return removeFirst();
        if (node.next == null) return removeLast();
    
        // Make the pointers of adjacent nodes skip over 'node'
        node.next.prev = node.prev;
        node.prev.next = node.next;
    
        // Temporarily store the data we want to return
        T data = node.data;
    
        // Memory cleanup
        node.data = null;
        node = node.prev = node.next = null;
    
        --size;
    
        // Return the data in the node we just removed
        return data;
    }
  
    // Remove a node at a particular index, O(n)
    public T removeAt(int index) {
        // Make sure the index provided is valid
        if (index < 0 || index >= size) {
            throw new IllegalArgumentException();
        }
    
        int i;
        Node<T> trav;
    
        // Search from the front of the list
        if (index < size / 2) {
            for (i = 0, trav = head; i != index; i++) {
                trav = trav.next;
            }
            // Search from the back of the list
        } else
            for (i = size - 1, trav = tail; i != index; i--) {
                trav = trav.prev;
            }
    
        return remove(trav);
    }
  
    // Remove a particular value in the linked list, O(n)
    public boolean remove(Object obj) {
        Node<T> trav = head;
    
        // Support searching for null
        if (obj == null) {
            for (trav = head; trav != null; trav = trav.next) {
                if (trav.data == null) {
                    remove(trav);
                    return true;
                }
            }
            // Search for non null object
        } else {
            for (trav = head; trav != null; trav = trav.next) {
                if (obj.equals(trav.data)) {
                    remove(trav);
                    return true;
                }
            }
        }
        return false;
    }
  
    // Find the index of a particular value in the linked list, O(n)
    public int indexOf(Object obj) {
        int index = 0;
        Node<T> trav = head;
    
        // Support searching for null
        if (obj == null) {
            for (; trav != null; trav = trav.next, index++) {
                if (trav.data == null) {
                    return index;
                }
            }
            // Search for non null object
        } else
            for (; trav != null; trav = trav.next, index++) {
                if (obj.equals(trav.data)) {
                    return index;
                }
            }
    
        return -1;
    }
  
    // Check is a value is contained within the linked list
    public boolean contains(Object obj) {
        return indexOf(obj) != -1;
    }
  
    @Override
    public java.util.Iterator<T> iterator() {
        return new java.util.Iterator<T>() {
            private Node<T> trav = head;
    
            @Override
            public boolean hasNext() {
                return trav != null;
            }
    
            @Override
            public T next() {
                T data = trav.data;
                trav = trav.next;
                return data;
            }
    
            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
        };
    }
  
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("[ ");
        Node<T> trav = head;
        while (trav != null) {
            sb.append(trav.data + ", ");
            trav = trav.next;
        }
        sb.append(" ]");
        return sb.toString();
    }
}