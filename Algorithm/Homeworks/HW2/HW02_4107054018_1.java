public class HW02_4107054018_1 extends ThreeSum {
    
    public HW02_4107054018_1() { }


    // Sort the input array by Quick Sort Algorithm.
    public void sort(int[] arr) {
        quickSort(arr, 0, arr.length - 1);
    }

    private static void swap(int[] arr, int index1, int index2) {
        int temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }

    private static int partition(int[] arr, int low, int high) {

        int pivot = arr[high];
        int i, exchangeIndex = low - 1;

        for (i = low; i < high; i++) {
            if (arr[low] < pivot) {
                exchangeIndex++;
                swap(arr, exchangeIndex, i);
            }
        }
        exchangeIndex++;
        swap(arr, exchangeIndex, high);

        return exchangeIndex;
    }

    private static void quickSort(int[] arr, int low, int high) {
        
        while (low < high) {
            // `pivot` is the partitioning index, meaning arr[pivot] is at the right position.
            int pivot = partition(arr, low, high);

            // If the left part of the array is smaller, recur the left part. Otherwise, recur the right part.
            if (pivot - low < high - pivot) {
                quickSort(arr, low, pivot - 1);
                low = pivot + 1;
            }
            else {
                quickSort(arr, pivot + 1, high);
                high = pivot - 1;
            }
        }
    }


    // Find all possible combination with two other pointer in the sorted list.
    @Override
    public int T_sum(int[] arr) {

        sort(arr);
        
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

        HW02_4107054018_1 test = new HW02_4107054018_1();

        int[] arr = new int[] {-1, 1, 2, 4, 8, -3};
        System.out.println("Number of combination: " + test.T_sum(arr));
    }
}
