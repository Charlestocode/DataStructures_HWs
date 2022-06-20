public class HW03_4107054018_1 extends HillFinding {

    HW03_4107054018_1() { }
    
    @Override
    public int H_Finding(int[] arr) {
        
        int maxElemIndex = 0;
        
        int low = 0;
        int high = arr.length - 1;
        int mid;

        while (low <= high) {
            mid = (low + high) / 2;

            if (arr[mid] >= arr[maxElemIndex]) {
                maxElemIndex = mid;
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return arr.length - maxElemIndex - 2;
    }


    public static void main(String[] args) {

        HW03_4107054018_1 test = new HW03_4107054018_1();

        int arr1[] = new int[]{5, 6, 7, 1, 2, 3, 4};
        System.out.println("Cutting posistion: " + test.H_Finding(arr1));

        int arr2[] = new int[]{5, 7, 8, 8, 1, 3, 4};
        System.out.println("Cutting posistion: " + test.H_Finding(arr2));
    }
}
