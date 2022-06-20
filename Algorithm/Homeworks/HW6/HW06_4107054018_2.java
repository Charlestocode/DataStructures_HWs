public class HW06_4107054018_2 extends MedianOfArrays {
    
    @Override
    public double find_median(int[][] arrays) {

        double median = -1;

        int[] arrPtrs = new int[arrays.length];
        int arrayToIncIndex = 0;

        int count = -1;
        int curMin, prevMin = Integer.MAX_VALUE;

        // Get the number of all elemetns in the input `arrays`.
        int numElems = 0;
        for (int i = 0; i < arrays.length; i++) {
            numElems += arrays[i].length;
        }
        
        // We just need the first `numElems / 2` elements to find the median value.
        for (int i = 0; i < numElems; i++) {

            curMin = Integer.MAX_VALUE;
            for (int j = 0; j < arrays.length; j++) {
                if (arrPtrs[j] >= arrays[j].length) continue;

                if (arrays[j][arrPtrs[j]] < curMin) {
                    curMin = arrays[j][arrPtrs[j]];
                    arrayToIncIndex = j;
                }
            }
            
            // When we get to the middle of the elements, return the answer.
            if (++count == numElems / 2) {
                // If the number of elements is even
                if (numElems % 2 == 0) {
                    median = (double) (curMin + prevMin) / 2;
                }
                // If the number of elements is odd
                else if (numElems % 2 == 1){
                    median = curMin;
                }
                break;
            }

            arrPtrs[arrayToIncIndex]++;
            prevMin = curMin;
        }

        return median;
    }
}
