package test_practic;

public class ProductThread implements Runnable {
	public static int start;
	public static int end;

	public ProductThread(int start, int end) {
		ProductThread.start = start;
		ProductThread.end = end;
	}

	@Override
	public void run() {
		for (int i = start; i < end; i++) {
			Testapd.product *= Testapd.v[i];
		}
		System.out.println("Produsul: " + Testapd.product);
	}
}
