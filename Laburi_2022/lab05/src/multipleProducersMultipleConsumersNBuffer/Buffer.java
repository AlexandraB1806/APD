package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {
    
    Queue<Integer> queue;

	Semaphore consumerSem = new Semaphore(0);
	Semaphore producerSem = new Semaphore(Main.N_PRODUCERS);
    
    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
    }

	public void put(int value) {
		try {
			producerSem.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		synchronized (Main.class) {
			queue.add(value);
		}

		consumerSem.release();
	}

	public int get() {
		try {
			consumerSem.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		int a = -1;
		Integer productBeforeRelease;

		synchronized (Main.class) {
			productBeforeRelease = queue.poll();
		}

		producerSem.release();

		if (productBeforeRelease != null) {
			a = productBeforeRelease;
		}

		return a;
	}
}
