public class HW04_4107054018_1 extends One_0k_rock {
    
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

            // Split the string into two parts equally.
            String firstHalfSubstring = strArray[i].substring(0, strArray[i].length() / 2);
            if (firstHalfSubstring.contains("1")) {
                resultArray[i] = false;
                continue;
            }

            String secondHalfSubstring = strArray[i].substring(strArray[i].length() / 2, strArray[i].length());
            if (secondHalfSubstring.contains("0")) {
                resultArray[i] = false;
                continue;
            }

            resultArray[i] = true;
        }
        
        return resultArray;
    }
}
