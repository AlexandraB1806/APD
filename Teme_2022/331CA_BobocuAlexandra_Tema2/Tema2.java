import java.io.*;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class Tema2 {
	static Semaphore semaphore;
	static String inFolder;

	public static void main(String[] args) throws IOException {
		inFolder = args[0];
		int numThreads = Integer.parseInt(args[1]);
		BufferedReader orderReader = new BufferedReader(new FileReader(inFolder + "/orders.txt"));
		BufferedWriter orderWriter = new BufferedWriter(new FileWriter("orders_out.txt"));
		BufferedWriter productWriter = new BufferedWriter(new FileWriter("order_products_out.txt"));

		// Semaforul va permite doar la numThreads threaduri
		// de tip Product sa ruleze la un moment dat
		semaphore = new Semaphore(numThreads);

		// Creez lock-uri diferite pentru fiecare tip de thread
		ReentrantLock lockOrder = new ReentrantLock();
		ReentrantLock lockProduct = new ReentrantLock();

		// Lansez doar numThreads threaduri de tip Order care sa se ocupe
		// de fisierul "orders.txt"
		Thread[] threads = new Thread[numThreads];

		for (int i = 0; i < numThreads; i++) {
			threads[i] = new OrderThread(lockOrder, lockProduct, orderReader, orderWriter, productWriter);
			threads[i].start();
		}

		for (int i = 0; i < numThreads; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		orderReader.close();
		orderWriter.close();
		productWriter.close();
	}
}
