package shortestPathsFloyd_Warshall;

public class Main {

    public static void main(String[] args) {
        int M = 9;
        int[][] graph = {{0, 1, M, M, M},
                         {1, 0, 1, M, M},
                         {M, 1, 0, 1, 1},
                         {M, M, 1, 0, M},
                         {M, M, 1, M, 0}};

        // Parallelize me (You might want to keep the original code in order to compare)
        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }

        int numThreads = 6;
        FloydWarshallThread[] threads = new FloydWarshallThread[numThreads];
        int[][] paralellizedGraph = {{0, 1, M, M, M},
                                     {1, 0, 1, M, M},
                                     {M, 1, 0, 1, 1},
                                     {M, M, 1, 0, M},
                                     {M, M, 1, M, 0}};

        for (int i = 0; i < numThreads; i++) {
            threads[i] = new FloydWarshallThread(i, numThreads, paralellizedGraph, 5);
            threads[i].start();
        }

        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("-------- VARIANTA SECVENTIALA --------");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }

        System.out.println("-------- VARIANTA PARALELIZATA --------");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(paralellizedGraph[i][j] + " ");
            }
            System.out.println();
        }
    }
}
