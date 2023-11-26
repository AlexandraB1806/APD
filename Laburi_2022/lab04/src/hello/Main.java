package hello;

public class Main {
	public static int numCores = Runtime.getRuntime().availableProcessors();

	public static void main(String[] args) throws InterruptedException {
		// Crearea unui vector de threaduri cu numCores elemente
		Thread[] threads = new Thread[numCores];

		// Creez instante pt fiecare thread in parte si pornesc rularea paralela
		for (int i = 0; i < numCores; i++) {
			threads[i] = new MyThread(i);
			threads[i].start();
		}

		// La final termin executia threadurilor
		for (int i = 0; i < numCores; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
