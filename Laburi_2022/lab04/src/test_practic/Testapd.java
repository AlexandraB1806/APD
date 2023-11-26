package test_practic;

public class Testapd {
	static int[] v;
	static int N;
	static int P;
	static int sum;
	static long product = 1;
	static int min = Integer.MAX_VALUE;
	static int max = Integer.MIN_VALUE;
	static double average;

	public static void main(String[] args) {
		N = Integer.parseInt(args[0]);
		P = Integer.parseInt(args[1]);
		v = new int[N];

		// Initializare vector
		for (int i = 2; i < args.length; i++) {
			v[i - 2] = Integer.parseInt(args[i]);
		}

		Thread[] threads = new Thread[P];

		for (int i = 0; i < P; i++) {
			int start = i * N / P;
			int end = (i + 1) * N / P;
			// In functie de valoarea lui P
			switch (i) {
				case 0:
					threads[i] = new Thread(new SumThread(start, end));
					threads[i].start();
					break;
				case 1:
					threads[i] = new Thread(new ProductThread(start, end));
					threads[i].start();
					break;
				case 2:
					threads[i] = new Thread(new MinThread(start, end));
					threads[i].start();
					break;
				case 3:
					threads[i] = new Thread(new MaxThread(start, end));
					threads[i].start();
					break;
				case 4:
					threads[i] = new Thread(new AverageThread(start, end));
					threads[i].start();
					break;
			}
		}

		for (Thread t : threads) {
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
