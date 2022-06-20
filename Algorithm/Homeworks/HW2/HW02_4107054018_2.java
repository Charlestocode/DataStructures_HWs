import java.util.Arrays;

public class HW02_4107054018_2 extends ThreeSum {
    
    // Find all possible combination with two other pointer in the sorted list.
    @Override
    public int T_sum(int[] arr) {

        Arrays.sort(arr);
        
        int count = 0;
        int left, right;

        for (int i = 0; i < arr.length - 2; i++) {
            left = i + 1;
            right = arr.length - 1;

            while (left < right) {
                // If these three elements sum up to zero, we increment the value of `left` and decrement the value of `right`
                // to find if there is any other combination.
                if (arr[i] + arr[left] + arr[right] == 0) {
                    count++;
                    left++;
                    right--;
                }
                else if (arr[i] + arr[left] + arr[right] < 0) {
                    left++;
                }
                else {
                    right--;
                }
            }
        }

        return count;
    }

    public static void main(String[] args) {

        HW02_4107054018_2 test = new HW02_4107054018_2();

        int[] arr = new int[] {-1, 1, 2, 4, 8, -3};
        System.out.println("Number of combination: " + test.T_sum(arr));
    }
}
