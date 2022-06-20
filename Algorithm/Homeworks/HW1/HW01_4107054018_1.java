/* Traverse each element to find maximum and minimum. */
public class HW01_4107054018_1 extends ArrayData {
    
    public HW01_4107054018_1(int[] array) {
        this.A = array;
    }

    @Override
    public int max() {
        int max = A[0];

        for (int i = 1; i < A.length; i++) {
            if (A[i] > max) {
                max = A[i];
            }
        }
        
        return max;
    }

    @Override
    public int min() {
        int min = A[0];
        
        for (int i = 1; i < A.length; i++) {
            if (A[i] < min) {
                min = A[i];
            }
        }

        return min;
    }


    public static void main(String[] args) {
        int[] array = {-100, 5, 2222, 45, 666, 90, 87, -55, 123, -88888};
        HW01_4107054018_1 test = new HW01_4107054018_1(array);
        System.out.println("max: " + test.max());
        System.out.println("min: " + test.min());
    }
}
