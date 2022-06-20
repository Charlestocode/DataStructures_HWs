import java.util.Collections;
import java.util.ArrayList;


/* Treat the class HW09_4107054018_1 as a Graph class */
public class HW09_4107054018_1 extends LSD {

    private class Graph {
        private int V;
        private ArrayList<Integer>[] adj;
    
        // After receiving edges input, create the adjacency list with O(n) time.
        Graph(int[][] edges, int V) {
    
            // Create an empty graph of 50,000 length.
            this.V = V;
            adj = (ArrayList<Integer>[]) new ArrayList[this.V];
            for (int i = 0; i < this.V; i++) {
                adj[i] = new ArrayList<Integer>();
            }
    
    
            // Read the 2D edge array.
            int v, w;   // To store two vertex indices for edge.
            for (int i = 0; i < edges.length; i++) {
                v = edges[i][0];
                w = edges[i][1];
    
                addEdge(v, w);
            }
        }	
    
        // O(1)
        void addEdge(int v, int w) {
    
            adj[v].add(w);
            adj[w].add(v);
        }	
    
        // O(1)
        ArrayList<Integer> adj(int v) {
            return adj[v];
        }
    
        int V() {
            return this.V;
        }
    }


    private class ConnectedComponent {

        private boolean[] marked;
        private int[] id;
        private int count;
    
        ConnectedComponent(Graph G) {
    
            marked = new boolean[G.V()];
            id = new int[G.V()];
    
            for (int v = 0; v < G.V(); v++) {
                if (!marked[v]) {
                    dfs(G, v);
                    count++;
                }
            }
        }
    
        // time complexity: O(1)
        boolean connected(int v, int w) {
            return id[v] == id[w];
        }
    
        // time complexity: O(E)
        void dfs(Graph G, int v) {
            marked[v] = true;
            id[v] = count;
    
            for (int w : G.adj(v)) {
                if (!marked[w]) {
                    dfs(G, w);
                }
            }
        }
    }


    private class BFSDistance {
        private boolean[] marked;
        private int[] distTo;
        private int maxDist;
        
        BFSDistance(Graph G) {
            marked = new boolean[G.V()];
            distTo = new int[G.V()];
            maxDist = 0;
        }
    
        void bfs(Graph G, int s) {
            ArrayList<Integer> queue = new ArrayList<Integer>();
            queue.add(s);
            marked[s] = true;
            distTo[s] = 0;
    
            while (!queue.isEmpty()) {
                int v = queue.remove(0);
                
                for (int w : G.adj(v)) {
                    if (!marked[w]) {
                        queue.add(w);
    
                        marked[w] = true;
                        distTo[w] = distTo[v] + 1;
    
                        // Update the Largest Shortest Distance in the component.
                        if (distTo[w] > maxDist) {
                            maxDist = distTo[w];
                        }
                    }
                }
            }
        }
    
        int distTo(int v) {
            return distTo[v];
        }
    
        int LSDInComponent() {
            return maxDist;
        }
    }


    @Override
    public int Distance(int[][] edges) {

        int maxVertexIndex = 0;
        // Store all vertex indices in a natural order.
        ArrayList<Integer> vertices = new ArrayList<>();
        for (int i = 0; i < edges.length; i++) {
            vertices.add(edges[i][0]);
            vertices.add(edges[i][1]);

            if (edges[i][0] > maxVertexIndex || edges[i][1] > maxVertexIndex) {
                if (edges[i][0] > edges[i][1]) {
                    maxVertexIndex = edges[i][0];
                }
                else {
                    maxVertexIndex = edges[i][1];
                }
            }
        }
        Collections.sort(vertices);


        int maxDist = 0;
        Graph graph = new Graph(edges, maxVertexIndex + 1);
        ConnectedComponent connectedComponent = new ConnectedComponent(graph);
        BFSDistance distance = new BFSDistance(graph);

        

        // Take two vertices to find the LSD.
        int vertex1, vertex2;
        for (int i = 0; i < vertices.size() - 1; i++) {
            vertex1 = vertices.get(i);
            distance.bfs(graph, vertex1);

            for (int j = i + 1; j < vertices.size(); j++) {
                vertex2 = vertices.get(j);

                if (connectedComponent.connected(vertex1, vertex2)) {
                    if (distance.distTo(vertex2) > maxDist) {
                        maxDist = distance.distTo(vertex2);
                    }
                }
            }
        }

        return maxDist;
    }

    public static void main(String[] args) {

        HW09_4107054018_1 test = new HW09_4107054018_1();
        int[][] edges = new int[][]{{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 4}, {2, 5}, {6, 7}};

        System.out.println("LSD: " + test.Distance(edges));
    }
}









