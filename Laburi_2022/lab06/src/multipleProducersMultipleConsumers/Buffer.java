package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	ArrayBlockingQueue<Integer> buffer = new ArrayBlockingQueue<>(5);

	int value;

	void put(int value) {
		try {
			buffer.put(value);
		} catch (IllegalStateException | InterruptedException | NullPointerException e) {
			e.printStackTrace();
		}
	}

	int get() {
		int retVal = -1;
		try {
			retVal = buffer.take();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		return retVal;
	}
}
