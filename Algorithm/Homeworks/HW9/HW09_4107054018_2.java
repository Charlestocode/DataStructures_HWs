//package HW09;

public class HW09_4107054018_2 extends LSD {

	final class VertexIndex {
        final VertexIndex Next; final int K, V;
        VertexIndex(final VertexIndex next, final int k, final int v) {
            Next = next; K = k; V = v;
        }
    }

    final class AdjacencyVertex {
        final AdjacencyVertex Next; final int K; int N = 0, A[] = new int[16];
        AdjacencyVertex(final AdjacencyVertex next, final int k, final int v) {
            Next = next; K = k; A[0] = v;
        }
    }


    final public int Distance(final int[][] array) {

        int i, max = -1, maxK = -1, head = 0, tail = 0;
        final int END = (1<<32-Integer.numberOfLeadingZeros(array.length-1))-1;
        final int queue[] = new int[END+1];
        final int AEND = (1<<32-Integer.numberOfLeadingZeros(array.length<<1))-1;
        final AdjacencyVertex[] ADJ = new AdjacencyVertex[AEND+1]; 
        AdjacencyVertex ant; 
        boolean put;

        for (final int[] n : array) {
            final int a = n[0], b = n[1];

            for (put = true, ant = ADJ[a&AEND]; ant != null; ant = ant.Next) {
                if (ant.K == a) {
                    final AdjacencyVertex AN = ant;
                    if (++AN.N > max) { maxK = a; max = AN.N; }
                    if (AN.N == AN.A.length) {
                        final int[] newList = new int[AN.N<<1];
                        System.arraycopy(AN.A, 0, newList, 0, AN.N);
                        AN.A = newList;
                    }
                    AN.A[AN.N] = b;
                    put = false; break;
                }
            }

            if(put) ADJ[a&AEND] = new AdjacencyVertex(ADJ[a&AEND], a, b);
            for (put = true, ant = ADJ[b&AEND]; ant != null; ant = ant.Next) {
                if (ant.K == b) {
                    final AdjacencyVertex AN = ant;
                    if (++AN.N > max) { maxK = b; max = AN.N; }
                    if (AN.N == AN.A.length) {
                        final int[] newList = new int[AN.N<<1];
                        System.arraycopy(AN.A, 0, newList, 0, AN.N);
                        AN.A = newList;
                    }
                    AN.A[AN.N] = a;
                    put = false; break;
                }
            }

            if(put) ADJ[b&AEND] = new AdjacencyVertex(ADJ[b&AEND], b, a);
        }


        max = -1; 
        VertexIndex snt;

        final VertexIndex SET[] = new VertexIndex[END+1];
        SET[maxK&END] = new VertexIndex(SET[maxK&END], maxK, 1);
        queue[tail] = maxK; tail = (tail+1)&END;
        while (head != tail) {
            maxK = queue[head]; head = (head+1)&END;
            for (i = 1, snt = SET[maxK&END]; snt != null; snt = snt.Next)
                if (snt.K == maxK){ i = snt.V + 1; break; }
            final int depth = i;
            if (depth > max) max = depth;
            for (ant = ADJ[maxK&AEND]; ant != null && ant.K != maxK; ant = ant.Next);
            final int N = ant.N+1, A[] = ant.A;
            for (i = 0; i < N;) {
                final int K = A[i++];
                for (put = true, snt = SET[K&END]; snt != null; snt = snt.Next)
                    if (snt.K == K){ put = false; break; }
                if(put){
                    SET[K&END] = new VertexIndex(SET[K&END], K, depth);
                    queue[tail] = K; tail = (tail+1)&END;
                }
            }
        }
        return max-2;
    }

    public static void main(String[] args) {

        HW09_4107054018_2 test = new HW09_4107054018_2();
        int[][] edges = new int[][]{{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 4}, {2, 5}, {6, 7}};

        System.out.println("LSD: " + test.Distance(edges));
    }
}

