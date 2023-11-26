package test_practic;

public class MaxThread implements Runnable {
	public static int start;
	public static int end;

	public MaxThread(int start, int end) {
		MaxThread.start = start;
		MaxThread.end = end;
	}

	@Override
	public void run() {
		for (int i = start; i < end; i++) {
			Testapd.max = Math.max(Testapd.max, Testapd.v[i]);
		}
		System.out.println("Maximul: " + Testapd.max);
	}
}
