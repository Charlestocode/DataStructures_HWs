import java.util.ArrayList;

public class HW05_4107054018_4 extends LLK {
    
    @Override
    public boolean checkLLK(int[][] array) {
        // If there are only two points. they must be collinear
        if (array.length <= 2) {
            return true;
        }

        // Create a slope-count map to record the counts of each slope.
        ArrayList<Float> slopeCountSet = new ArrayList<>();
        
        // Declare a variable slope to store each slope after getting a new slope.
        Float slope;

        // Outer loop index i is for pivot points. Inner loop index j is for any other points.
        for (int i = 0; i < array.length; i++) {
            for (int j = 0; j < array.length; j++) {
                if (i != j) {
                    slope = findSlope(array[i], array[j]);
                    
                    if (slopeCountSet.contains(slope)) {
                        return true;
                    }
                    else {
                        slopeCountSet.add(slope);
                    }
                }
            }
            // Before changing the pivot point, we have to clear the whole sets in `slopeCountSet`.
            slopeCountSet.clear();
        }

        return false;
    }


    private Float findSlope(int[] point1, int[] point2) {
        // If the variation of x coordinate is zero, the slope does not exist, so we return MAX_VALUE.
        if (point2[0] == point1[0]) {
            return Float.MAX_VALUE;
        }
        return Float.valueOf(((float) point2[1] - point1[1]) / ((float) point2[0] - point1[0]));
    }


    public static void main(String[] args) {

        HW05_4107054018_1 test = new HW05_4107054018_1();

        int[][] arr1 = new int[][]{{1, 2}, {1, 3}, {1, 4}};
        System.out.println(test.checkLLK(arr1));

        int[][] arr2 = new int[][]{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
        System.out.println(test.checkLLK(arr2));
    }
}
