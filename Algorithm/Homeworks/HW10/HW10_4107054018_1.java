public class HW10_4107054018_1 extends StringSort {
    
    private static int R = 256;
    private static final int M = 15;
    private static String[] aux;

    @Override
    public String[] checkString(String[] array) {

        int N = array.length;
        aux = new String[N];
        sort(array, 0, N - 1, 0);
        return array;
    }


    private static int charAt(String s, int d) {

        if (d < s.length()) {
            return s.charAt(d);
        }
        else {
            return -1;
        }
    }

    private static void sort(String[] array, int low, int high, int d) {

        if (high < low) {
            //Insertion.sort(array, low, high, d);
            return;
        }

        int[] count = new int[R + 2];
        // Compute the frequency.
        for (int i = low; i <= high; i++) {
            count[charAt(array[i], d) + 2]++;
        }

        // Transform counts to indices.
        for (int r = 0; r < R + 1; r++) {
            count[r + 1] += count[r];
        }

        // Distribute.
        // We always distribute the item to int array aux from the first element of aux.
        for (int i = low; i <= high; i++) {
            aux[count[charAt(array[i], d) + 1]++] = array[i];
        }

        // Copy back.
        for (int i = low; i <= high; i++) {
            array[i] = aux[i - low];
        }

        // Recursively sort for each character value.
        for (int r = 0; r < R; r++) {
            sort(array, low + count[r], low + count[r + 1] - 1, d + 1);
        }
    }

    
    /*
    public static void main(String[] args) {

        HW10_4107054018_1 test = new HW10_4107054018_1();
        String[] array1 = new String[]{"Once", "a", "upon", "time"};
        String[] array2 = new String[]{"aaaa", "aa", "a", "aaa"};

        System.out.println("Array 1: ");
        String[] sorted1 = test.checkString(array1);
        for (int i = 0; i < sorted1.length; i++) {
            System.out.println(sorted1[i]);
        }

        String[] sorted2 = test.checkString(array2);
        System.out.println("Array 2: ");
        for (int i = 0; i < sorted1.length; i++) {
            System.out.println(sorted2[i]);
        }
    }
    */
}

/*
class Insertion {

    static void sort(String[] array, int low, int high, int d) {
        for (int i = low; i <= high; i++) {
            for (int j = i; j > low && less(array[j], array[j - 1], d); j--) {
                exch(array, j, j - 1);
            }
        }
    }

    private static void exch(String[] array, int index1, int index2) {
        String tmp = array[index1];
        array[index1] = array[index2];
        array[index1] = tmp;
    }

    private static boolean less(String v, String w, int d) {
        return v.substring(d).compareTo(w.substring(d)) < 0;
    }
}
*/