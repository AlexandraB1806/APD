import java.io.*;
import java.util.StringTokenizer;
import java.util.concurrent.locks.ReentrantLock;

public class OrderThread extends Thread {
	ReentrantLock lockOrder;
	ReentrantLock lockProduct;
	// Citirea si scrierea in fisier pentru comenzi
	final BufferedReader orderReader;
	final BufferedWriter orderWriter;
	// Citirea si scrierea in fisier pentru produse
	BufferedReader productReader;
	BufferedWriter productWriter;

	// Fiecare linie din fisierul "orders.txt"
	String eachLineFromOrderFile;
	// Informatiile extrase din fiecare linie a fisierului "orders.txt"
	String orderId;
	int numProducts;

	public OrderThread (ReentrantLock lockOrder, ReentrantLock lockProduct, BufferedReader orderReader,
						BufferedWriter orderWriter, BufferedWriter productWriter) {
		this.lockOrder = lockOrder;
		this.lockProduct = lockProduct;
		this.orderReader = orderReader;
		this.orderWriter = orderWriter;
		this.productWriter = productWriter;
	}

	@Override
	public void run() {
		while (true) {
			// Doar un thread de tip Order va citi zona cu care
			// lucreaza linie cu linie
			lockOrder.lock();
			try {
				try {
					eachLineFromOrderFile = orderReader.readLine();

					// Am ajuns la finalul fisierului
					if (eachLineFromOrderFile == null) {
						break;
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			} finally {
				lockOrder.unlock();
			}

			// Extrag din linia citita informatiile utile
			StringTokenizer st = new StringTokenizer(eachLineFromOrderFile, ",");

			while (st.hasMoreTokens()) {
				orderId = st.nextToken();
				numProducts = Integer.parseInt(st.nextToken());
			}

			// Daca nu am Empty Order
			if (numProducts != 0) {
				// Se creeaza numProducts threaduri de tip Product care sa
				// gestioneze fiecare cate un produs
				Thread[] threads = new Thread[numProducts];

				try {
					productReader = new BufferedReader
							(new FileReader(Tema2.inFolder + "/order_products.txt"));

					for (int i = 0; i < numProducts; i++) {
						threads[i] = new ProductThread(lockProduct, productReader,
													   productWriter, numProducts,
													   orderId);
						threads[i].start();
					}
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}

				for (int i = 0; i < numProducts; i++) {
					try {
						threads[i].join();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}

				// Scrierea rezultatului in fisierul corespuzator.
				// S-au terminat toate thread-urile
				// de tip Product, deci comanda a fost livrata
				lockOrder.lock();
				try {
					try {
						orderWriter.write(eachLineFromOrderFile + ",shipped\n");
					} catch (IOException e) {
						e.printStackTrace();
					}
				} finally {
					lockOrder.unlock();
				}

				try {
					productReader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
