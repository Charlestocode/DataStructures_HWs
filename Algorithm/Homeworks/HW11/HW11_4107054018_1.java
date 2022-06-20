public class HW11_4107054018_1 extends SunMoonLake {

    private int waterVolume;
    private boolean[][] visited1;

    private static class Cell implements Comparable<Cell> {
        private int row;
        private int col;
        private int value;

        public Cell(int r, int c, int v) {
            this.row = r;
            this.col = c;
            this.value = v;
        }

        @Override
        public int compareTo(Cell other) {
            return this.value - other.value;
        }
    }


    private void visit(int[][] height, Cell start, minPQ<Cell> walls) {
        fill(height, start.row + 1, start.col, walls, start.value);
        fill(height, start.row - 1, start.col, walls, start.value);
        fill(height, start.row, start.col + 1, walls, start.value);
        fill(height, start.row, start.col - 1, walls, start.value);
    }
    
    private void fill(int[][] height, int row, int col, minPQ<Cell> walls, int min) {
        if (row < 0 || col < 0) return;
        else if (row >= height.length || col >= height[0].length) return;
        else if (visited1[row][col]) return;
        else if (height[row][col] >= min) {
            walls.add(new Cell(row, col, height[row][col]));
            visited1[row][col] = true;
            return;
        } 
        else {
        	//System.out.println(row + ", " + col + " height = " + height[row][col] + ", bar = " + min);
            waterVolume += min - height[row][col];
            visited1[row][col] = true;
            fill(height, row + 1, col, walls, min);
            fill(height, row - 1, col, walls, min);
            fill(height, row, col + 1, walls, min);
            fill(height, row, col - 1, walls, min);
        }
    }


    @Override
    public int count_water_volume(int[][] heightMap) {

        if (heightMap.length == 0) return 0;
        int N = heightMap.length * heightMap[1].length;

        minPQ<Cell> walls = new minPQ<Cell>(N);
        waterVolume = 0;
        visited1 = new boolean[heightMap.length][heightMap[0].length];
        int rows = heightMap.length, cols = heightMap[0].length;

        // Build wall.
        for (int c = 0; c < cols; c++) {
        	walls.add(new Cell(0, c, heightMap[0][c]));
        	walls.add(new Cell(rows - 1, c, heightMap[rows - 1][c]));
        	visited1[0][c] = true;
        	visited1[rows - 1][c] = true;
        }
        for (int r = 1; r < rows - 1; r++) {
        	walls.add(new Cell(r, 0, heightMap[r][0]));
        	walls.add(new Cell(r, cols - 1, heightMap[r][cols - 1]));
        	visited1[r][0] = true;
        	visited1[r][cols - 1] = true;
        }

        // End building wall.
        while(walls.size() > 0) {
            Cell min = walls.poll();
            visit(heightMap, min, walls);
        }
        return waterVolume;
    }


    /*
    public static void main(String[] args) {

        HW11_4107054018_1 test = new HW11_4107054018_1();

        int[][] heightMap1 = new int[][]{{2, 5, 3}, {7, 1, 6}, {9, 8, 4}};
        System.out.println("Volume: " + test.count_water_volume(heightMap1));

        int[][] heightMap2 = new int[][]{{1, 3, 2, 4, 5, 2}, {2, 1, 3, 2, 1, 4}, {2, 3, 4, 5, 7, 8}};
        System.out.println("Volume: " + test.count_water_volume(heightMap2));

        int[][] heightMap3 = new int[][]{{3, 3, 4, 5, 6}, {4, 2, 4, 3, 5}, {5, 1, 0, 4, 4}, {4, 1, 1, 2, 3}, {1, 5, 4, 5, 4}};
        System.out.println("Volume: " + test.count_water_volume(heightMap3));

        int[][] heightMap4 = new int[][]{{3, 3, 4, 5, 6}, {4, 2, 2, 1, 5}, {5, 1, 4, 0, 4}, {4, 1, 1, 2, 3}, {4, 5, 4, 5, 4}};
        System.out.println("Volume: " + test.count_water_volume(heightMap4));

        int[][] heightMap5 = new int[][]{{3, 3, 3, 3, 3}, {3, 2, 2, 2, 3}, {3, 2, 1, 2, 3}, {3, 2, 2, 2, 3}, {3, 3, 3, 3, 3}};
        System.out.println("Volume: " + test.count_water_volume(heightMap5));
    }
    */
}


class minPQ<T extends Comparable<T>> {

    private T[] pq; // heap-ordered complete binary tree
    private int N = 0; // in pq[1..N] with pq[0] unused

    public minPQ(int maxN) {
        pq = (T[]) new Comparable[maxN + 1];
    }

    public boolean isEmpty() {
        return N == 0;
    }

    public int size() {
        return N;
    }

    public void add(T v) {
        pq[++N] = v;
        swim(N);
    }

    public T poll() {
        T max = pq[1];  // Retrieve max key from top.
        exch(1, N--);   // Exchange with last item.
        pq[N+1] = null;     // Avoid loitering.
        sink(1);    // Restore heap property.
        return max;
    }

    
    // See pages 145-147 for implementations of these helper methods.
    private boolean less(int i, int j) {

        if (i > N || j > N) return false;
        return pq[i].compareTo(pq[j]) < 0;
    }

    private void exch(int i, int j) {
        T temp = pq[i];
        pq[i] = pq[j];
        pq[j] = temp;
    }

    private void swim(int k) {

        while (k > 1 && less(k, k / 2)) {
            exch(k, k / 2);
            k = k / 2;
        }
    }

    private void sink(int k) {

        while (2 * k <= N) {
            int j = 2 * k;  // left child index
            if (j < N && less(j + 1, j)) j++;
            if (!less(j, k)) break;
            exch(k, j);
            k = j;
        }
    }
}