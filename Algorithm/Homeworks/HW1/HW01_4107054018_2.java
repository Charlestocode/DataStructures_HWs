/* When input the array, we sort the array in natural ordering. */
import java.util.Arrays;

public class HW01_4107054018_2 extends ArrayData {

    public HW01_4107054018_2(int[] array) {
        this.A = array;
        Arrays.sort(this.A);
    }

    @Override
    public int max() {
        return A[A.length - 1];
    }

    @Override
    public int min() {
        return A[0];
    }

    public static void main(String[] args) {
        int[] array = {-100, 5, 2222, 45, 666, 90, 87, -55, 123, -88888};
        HW01_4107054018_1 test = new HW01_4107054018_1(array);
        System.out.println("max: " + test.max());
        System.out.println("min: " + test.min());
    }
}
