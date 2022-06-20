public class HW06_4107054018_3 extends MedianOfArrays {
    
    @Override
    public double find_median(int[][] arrays) {

        // Get the number of all elemetns in the input `arrays`.
        int numElems = 0;
        for (int i = 0; i < arrays.length; i++) {
            numElems += arrays[i].length;
        }
          
        int[] array = new int[numElems];
        int index = 0;
        for (int i = 0; i < arrays.length; i++) {
            for (int j = 0; j < arrays[i].length; j++) {
                array[index++] = arrays[i][j];
            }
        }

        sort(array);

        int mid = numElems / 2;
        if (numElems % 2 == 0) {
            return (double) (array[mid - 1] + array[mid]) / 2;
        }
        else {
            return array[mid];
        }
    }

    private static void merge(int[] array, int[] aux, int low, int mid, int high) {

        for (int k = low; k <= high; k++) {
            aux[k] = array[k];
        }

        int i = low, j = mid + 1;
        for (int k = low; k <= high; k++) {
            if (i > mid)    array[k] = aux[j++];
            else if (j > high)  array[k] = aux[i++];
            else if (aux[j] < aux[i])   array[k] = aux[j++];
            else    array[k] = aux[i++];
        }
    }

    private static void sort(int[] array, int[] aux, int low, int high) {

        if (high <= low) return;
        int mid = (low + high) / 2;

        sort(array, aux, low, mid);
        sort(array, aux, mid + 1, high);
        merge(array, aux, low, mid, high);
    }

    static void sort(int[] array) {
        int[] aux = new int[array.length];
        sort(array, aux, 0, array.length - 1);
    }
}
