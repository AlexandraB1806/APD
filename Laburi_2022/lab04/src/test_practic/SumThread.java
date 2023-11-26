package test_practic;

public class SumThread implements Runnable {
	public static int start;
	public static int end;

	public SumThread(int start, int end) {
		SumThread.start = start;
		SumThread.end = end;
	}

	@Override
	public void run() {
		for (int i = start; i < end; i++) {
			Testapd.sum += Testapd.v[i];
		}
		System.out.println("Suma: " + Testapd.sum);
	}
}
