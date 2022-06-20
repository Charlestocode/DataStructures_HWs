public class HW05_4107054018_2 extends LLK {
    
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

        HW05_4107054018_2 test = new HW05_4107054018_2();

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


class HashMap<K, V> implements Iterable<K> {
    
    protected float loadFactor;
    protected int capacity, threshold, modificationCount;

    // 'usedBuckets' counts the total number of used buckets inside the
    // hash-table (includes cells marked as deleted). While 'keyCount'
    // tracks the number of unique keys currently inside the hash-table.
    protected int usedBuckets, keyCount;

    // These arrays store the key-value pairs.
    protected K[] keys;
    protected V[] values;

    // Special marker token used to indicate the deletion of a key-value pair
    @SuppressWarnings("unchecked")
    protected final K TOMBSTONE = (K) (new Object());


    private static final int DEFAULT_CAPACITY = 1 << 4;
    private static final float DEFAULT_LOAD_FACTOR = 0.75f;

    protected HashMap() {
        this(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
    }

    protected HashMap(int capacity, float loadFactor) {
        if (capacity <= 0) throw new IllegalArgumentException("Illegal capacity" + capacity);

        if (loadFactor <= 0 || Float.isNaN(loadFactor) || Float.isInfinite(loadFactor))
            throw new IllegalArgumentException("Illegal loadFactor: " + loadFactor);

        this.loadFactor = loadFactor;
        this.capacity = Math.max(DEFAULT_CAPACITY, capacity);
        adjustCapacity();
    }


    // Given a number this method finds the next
    // power of two above this value.
    private static int nextPowerOfTwo(int n) {
        return Integer.highestOneBit(n) << 1;
    }

    protected int probe(int x) {
        // Quadratic probing function (x^2+x)/2
        return (x * x + x) >> 1;
    }

    protected void increaseCapacity() {
        capacity = nextPowerOfTwo(capacity);
    }

    protected void adjustCapacity() {
        int pow2 = Integer.highestOneBit(capacity);
        if (capacity == pow2) return;
        increaseCapacity();
    }


    // Double the size of the hash-table
    protected void resizeTable() {
        increaseCapacity();
        adjustCapacity();

        threshold = (int) (capacity * loadFactor);

        @SuppressWarnings("unchecked")
        K[] oldKeyTable = (K[]) new Object[capacity];
        @SuppressWarnings("unchecked")
        V[] oldValueTable = (V[]) new Object[capacity];

        // Perform key table pointer swap
        K[] keyTableTmp = keys;
        keys = oldKeyTable;
        oldKeyTable = keyTableTmp;

        // Perform value table pointer swap
        V[] valueTableTmp = values;
        values = oldValueTable;
        oldValueTable = valueTableTmp;

        // Reset the key count and buckets used since we are about to
        // re-insert all the keys into the hash-table.
        keyCount = usedBuckets = 0;

        for (int i = 0; i < oldKeyTable.length; i++) {
            if (oldKeyTable[i] != null && oldKeyTable[i] != TOMBSTONE)
                insert(oldKeyTable[i], oldValueTable[i]);
            oldValueTable[i] = null;
            oldKeyTable[i] = null;
        }
    }


    // Converts a hash value to an index. Essentially, this strips the
    // negative sign and places the hash value in the domain [0, capacity)
    protected final int normalizeIndex(int keyHash) {
        return (keyHash & 0x7FFFFFFF) % capacity;
    }

    // Finds the greatest common denominator of a and b.
    protected static final int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }


    public V put(K key, V value) {
        return insert(key, value);
    }

    public V remove(K key) {
        if (key == null) throw new IllegalArgumentException("Null key");

        final int offset = normalizeIndex(key.hashCode());

        // Starting at the hash linearly probe until we find a spot where
        // our key is or we hit a null element in which case our element does not exist
        for (int i = offset, x = 1; ; i = normalizeIndex(offset + probe(x++))) {

            // Ignore deleted cells
            if (keys[i] == TOMBSTONE) continue;

            // Key was not found in hash-table.
            if (keys[i] == null) return null;

            // The key we want to remove is in the hash-table!
            if (keys[i].equals(key)) {
                keyCount--;
                modificationCount++;
                V oldValue = values[i];
                keys[i] = TOMBSTONE;
                values[i] = null;
                return oldValue;
            }
        }
    }

    public boolean containsKey(K key) {
        return hasKey(key);
    }

    public void clear() {
        for (int i = 0; i < capacity; i++) {
            keys[i] = null;
            values[i] = null;
        }
        keyCount = usedBuckets = 0;
        modificationCount++;
    }


    // Place a key-value pair into the hash-table. If the value already
    // exists inside the hash-table then the value is updated
    public V insert(K key, V val) {
        if (key == null) throw new IllegalArgumentException("Null key");
        if (usedBuckets >= threshold) resizeTable();

        final int offset = normalizeIndex(key.hashCode());

        for (int i = offset, j = -1, x = 1; ; i = normalizeIndex(offset + probe(x++))) {

            // The current slot was previously deleted
            if (keys[i] == TOMBSTONE) {
                if (j == -1) j = i;

                // The current cell already contains a key
            } else if (keys[i] != null) {
                // The key we're trying to insert already exists in the hash-table,
                // so update its value with the most recent value
                if (keys[i].equals(key)) {

                V oldValue = values[i];
                if (j == -1) {
                    values[i] = val;
                } else {
                    keys[i] = TOMBSTONE;
                    values[i] = null;
                    keys[j] = key;
                    values[j] = val;
                }
                modificationCount++;
                return oldValue;
                }

                // Current cell is null so an insertion/update can occur
            } else {
                // No previously encountered deleted buckets
                if (j == -1) {
                usedBuckets++;
                keyCount++;
                keys[i] = key;
                values[i] = val;

                // Previously seen deleted bucket. Instead of inserting
                // the new element at i where the null element is insert
                // it where the deleted token was found.
                } else {
                keyCount++;
                keys[j] = key;
                values[j] = val;
                }

                modificationCount++;
                return null;
            }
        }
    }


    // Returns true/false on whether a given key exists within the hash-table
    public boolean hasKey(K key) {
        if (key == null) throw new IllegalArgumentException("Null key");

        final int offset = normalizeIndex(key.hashCode());

        // Starting at the original hash linearly probe until we find a spot where
        // our key is or we hit a null element in which case our element does not exist.
        for (int i = offset, j = -1, x = 1; ; i = normalizeIndex(offset + probe(x++))) {

            // Ignore deleted cells, but record where the first index
            // of a deleted cell is found to perform lazy relocation later.
            if (keys[i] == TOMBSTONE) {

                if (j == -1) j = i;

                // We hit a non-null key, perhaps it's the one we're looking for.
            } else if (keys[i] != null) {

                // The key we want is in the hash-table!
                if (keys[i].equals(key)) {

                    // If j != -1 this means we previously encountered a deleted cell.
                    // We can perform an optimization by swapping the entries in cells
                    // i and j so that the next time we search for this key it will be
                    // found faster. This is called lazy deletion/relocation.
                    if (j != -1) {
                        // Swap the key-value pairs of positions i and j.
                        keys[j] = keys[i];
                        values[j] = values[i];
                        keys[i] = TOMBSTONE;
                        values[i] = null;
                    }
                    return true;
                }

                // Key was not found in the hash-table :/
            } else return false;
        }
    }


    @Override
    public Iterator<K> iterator() {
        // Before the iteration begins record the number of modifications
        // done to the hash-table. This value should not change as we iterate
        // otherwise a concurrent modification has occurred :0
        final int MODIFICATION_COUNT = modificationCount;

        return new Iterator<K>() {
            int index, keysLeft = keyCount;

            @Override
            public boolean hasNext() {
                // The contents of the table have been altered
                if (MODIFICATION_COUNT != modificationCount) throw new ConcurrentModificationException();
                return keysLeft != 0;
            }

            // Find the next element and return it
            @Override
            public K next() {
                while (keys[index] == null || keys[index] == TOMBSTONE) index++;
                keysLeft--;
                return keys[index++];
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
        };
    }
}
