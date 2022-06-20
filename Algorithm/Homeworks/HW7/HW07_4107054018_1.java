public class HW07_4107054018_1 extends Buy_Phone {
    
    @Override
    public int[][] bestPhone(int[][] inputArr) {

        int len = inputArr.length, newLen = len, i, j;

        for(i = 0; i < len - 1; i++){

            // If we traverse the null value pair, skip it and decrement the length of new answer array.
            if(inputArr[i] == null){
                newLen--;
                continue;
            }

            // Traverse the feature array from the tail.
            for(j = len - 1; j > i; j--){
                if(inputArr[j] != null) {
                    // Check if the i-th element of 2D feature lists is unbeatable.
                    // If there exists a feature can beat i-th element, then set the i-th element to null.
                    if(inputArr[i][0] <= inputArr[j][0] && inputArr[i][1] <= inputArr[j][1]){
                        inputArr[i] = null;
                        newLen--;
                        break;
                    }
                }
            }
        }

        int[][] ans = new int[newLen][2];
        for(i = 0, j= 0; i < len; i++){
            if(inputArr[i] != null){
                ans[j++] = inputArr[i];
            }
        }

        return ans;
    }
}
