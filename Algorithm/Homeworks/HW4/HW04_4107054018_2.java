public class HW04_4107054018_2 extends One_0k_rock {
    
    @Override
    public boolean[] one0k(String[] strArray) {

        boolean[] resultArray = new boolean[strArray.length];

        // Traverse each item in the string array.
        for (int i = 0; i < strArray.length; i++) {

            // If the length of the string is odd, it must be false.
            if (strArray[i].length() % 2 == 1) {
                resultArray[i] = false;
                continue;
            }


            boolean isIllegal = false;
            int leftChecker = strArray[i].length() / 2 - 1;
            int rightChecker = strArray[i].length() / 2;
            while (leftChecker >= 0 && rightChecker < strArray[i].length()) {

                // Check the left part substring from the rightmost to the leftmost.
                if (strArray[i].charAt(leftChecker) == '1') {
                    resultArray[i] = false;
                    isIllegal = true;
                    break;
                }
                // Check the right part substring from the leftmost to the rightmost.
                if (strArray[i].charAt(rightChecker) == '0') {
                    resultArray[i] = false;
                    isIllegal = true;
                    break;
                }

                leftChecker--;
                rightChecker++;
            }

            if (isIllegal == false) {
                resultArray[i] = true;
            }
        }

        return resultArray;
    }
}
