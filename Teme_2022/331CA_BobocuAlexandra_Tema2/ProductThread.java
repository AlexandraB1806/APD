import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.concurrent.locks.ReentrantLock;

public class ProductThread extends Thread {
	ReentrantLock lockProduct;
	// Citirea si scrierea in fisier pentru produse
	final BufferedReader productReader;
	final BufferedWriter productWriter;

	int numProducts;
	String orderId;

	// Fiecare linie din fisierul "order_products.txt"
	String eachLineFromProductFile;
	// Informatiile extrase din fiecare linie a fisierului "order_products.txt"
	String currOrderId;
	String currProductId;

	public ProductThread(ReentrantLock lockProduct, BufferedReader productReader,
						 BufferedWriter productWriter, int numProducts, String orderId) {
		this.lockProduct = lockProduct;
		this.productReader = productReader;
		this.productWriter = productWriter;
		this.numProducts = numProducts;
		this.orderId = orderId;
	}

	@Override
	public void run() {
		try {
			Tema2.semaphore.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		while (true) {
			// Doar un thread de tip Product va citi zona cu care
			// lucreaza linie cu linie
			lockProduct.lock();
			try {
				try {
					eachLineFromProductFile = productReader.readLine();

					// Am ajuns la finalul fisierului
					if (eachLineFromProductFile == null) {
						break;
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			} finally {
				lockProduct.unlock();
			}

			// Extrag din linia citita informatiile utile
			StringTokenizer st = new StringTokenizer(eachLineFromProductFile, ",");

			while (st.hasMoreTokens()) {
				currOrderId = st.nextToken();
				currProductId = st.nextToken();
			}

			// Am gasit comanda pentru produsul procesat
			if (currOrderId.equals(orderId)) {
				lockProduct.lock();
				try {
					try {
						productWriter.write(eachLineFromProductFile + ",shipped\n");
					} catch (IOException e) {
						e.printStackTrace();
					}
				} finally {
					lockProduct.unlock();
				}
			}
		}
		Tema2.semaphore.release();
	}
}
