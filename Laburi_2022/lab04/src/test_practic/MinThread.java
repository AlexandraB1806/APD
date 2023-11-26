package test_practic;

public class MinThread implements Runnable {
	public static int start;
	public static int end;

	public MinThread(int start, int end) {
		MinThread.start = start;
		MinThread.end = end;
	}

	@Override
	public void run() {
		for (int i = start; i < end; i++) {
			Testapd.min = Math.min(Testapd.min, Testapd.v[i]);
		}
		System.out.println("Minimul: " + Testapd.min);
	}
}
