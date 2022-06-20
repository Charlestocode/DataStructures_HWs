public class HW03_4107054018_2 extends HillFinding {

    @Override
    public int H_Finding(int[] arr) {

        int peak = 0;

        int left = 0;
        int right = arr.length - 1;

        for (int i = 0; i < arr.length >> 1; i++) {
            // Check if the `left` pointer find the peak element.
            if (arr[left + 1] >= arr[left]) {
                left++;
            }
            else {
                peak = left;
                break;
            }

            // Check if the `left` pointer find the peak element.
            if (arr[right - 1] <= arr[right]) {
                right--;
            }
            else {
                peak = right - 1;
                break;
            }
        }
        
        return arr.length - peak - 2;
    }


    public static void main(String[] args) {

        HW03_4107054018_1 test = new HW03_4107054018_1();

        int arr1[] = new int[]{5, 6, 7, 1, 2, 3, 4};
        System.out.println("Cutting posistion: " + test.H_Finding(arr1));

        int arr2[] = new int[]{5, 7, 8, 8, 1, 3, 4};
        System.out.println("Cutting posistion: " + test.H_Finding(arr2));
    }
}
