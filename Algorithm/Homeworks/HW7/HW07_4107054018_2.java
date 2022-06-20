public class HW07_4107054018_2 extends Buy_Phone {

    static int[][] tempArr = new int[100000][2];

    private static void quickSort(int[][] A, int left, int right) {

        if (right <= left) return;

        int pivotIndex = partition(A, left, right);

        quickSort(A, left, pivotIndex - 1);
        quickSort(A, pivotIndex + 1, right);
    }

    private static int partition(int[][] A, int left, int right) {

        int i = left, j = right + 1;

        int pivot = A[left][0];

        while (true) {
            while (A[++i][0] < pivot) {
                if (i == right) break;
            }

            while (A[--j][0] > pivot) {
                if (j == left) break;
            }

            if (i >= j) break;
            exch(A, i, j);
        }

        exch(A, left, j);

        return j;
    }

    private static void exch(int[][] A, int index1, int index2) {

        int[] temp;
        temp = A[index1];
        A[index1] = A[index2];
        A[index2] = temp;
    }


    @Override
    public int[][] bestPhone(int[][] inputArr){

        int len = inputArr.length;
        int ansIndex = 0, maxX, maxY;

        quickSort(inputArr,0,len-1);

        for (int[] an : inputArr) {
            System.out.println(an[0] + "," + an[1]);
        }
        System.out.println("--------------------------");

        tempArr[0] = inputArr[len-1];
        maxX = inputArr[len-1][0];
        maxY = inputArr[len-1][1];

        for(int i = len-2; i > -1; i--){
            maxY = Math.max(maxY, inputArr[i][1]);
            if(inputArr[i][0]<maxX && inputArr[i][1] < maxY) continue;
            if(inputArr[i][0] != tempArr[ansIndex][0] && inputArr[i][1]!=tempArr[ansIndex][1]){
                tempArr[++ansIndex] = inputArr[i];
            }
        }

        int[][]result = new int [ansIndex+1][2];
        for(int i=0; i <= ansIndex; i++){
            result[i] = tempArr[i];
        }

        return result;
    }

    public static void main(String[] args) {
        int[][] inputArr = {{100,101},{100,10},{100,100},{101,4},{4,8},{5,5},{8,4},{10,2},{10,1}};
        HW07_4107054018_2 test = new HW07_4107054018_2();
        int[][] ans = test.bestPhone(inputArr);
        for (int[] an : ans) {
            System.out.println(an[0] + "," + an[1]);
        }
    }
}
