package bugConcurrentHashMap;

import java.util.concurrent.ConcurrentHashMap;

public class MyThread implements Runnable {
	public static ConcurrentHashMap<Integer, Integer> hashMap = new ConcurrentHashMap<>();
	private final int id;

	public MyThread(int id) {
		this.id = id;
	}

	private void addValue(int key, int value) {
		// Daca valoarea asociata cheii exista deja, se va pune inca o data in prevVal.
		// Daca nu, prevVal = null, dar pt cheia respectiva, s-a pus valoarea in HashMAp.
		Integer prevVal = hashMap.putIfAbsent(key, value);

		if (prevVal != null) {
			// Actualizam valoarea
			hashMap.put(key, prevVal + value);
		}
	}
	
	@Override
	public void run() {
		if (id == 0) {
			for (int i = 0; i < Main.N; i++) {
				addValue(i, 2 * i);
			}
		} else {
			for (int i = 0; i < Main.N; i++) {
				addValue(i, 3 * i);
			}
		}
	}
}
