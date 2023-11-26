package test_practic;

public class AverageThread implements Runnable {
	public static int start;
	public static int end;

	public AverageThread(int start, int end) {
		AverageThread.start = start;
		AverageThread.end = end;
	}

	@Override
	public void run() {
		for (int i = start; i < end; i++) {
			Testapd.average += Testapd.v[i];
		}
		Testapd.average /= Testapd.N;
		System.out.println("Media: " + Testapd.average);
	}
}
