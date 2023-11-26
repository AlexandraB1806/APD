package shortestPathsFloyd_Warshall;

public class FloydWarshallThread extends Thread {
	private final int id;
	private final int[][] graph;
	private final int N;
	private final int numThreads;

	public FloydWarshallThread(int id, int numThreads, int[][] graph, int N) {
		this.id = id;
		this.numThreads = numThreads;
		this.graph = graph;
		this.N = N;
	}

	@Override
	public void run() {
		final int start = id * (int)Math.ceil((double) N / numThreads);
		final int end = (int) Math.min((id + 1) * Math.ceil((double) N / numThreads), N);

		for (int k = start; k < end; k++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                	synchronized (this) {
						graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
					}
                }
            }
        }
	}
}
