package multipleProducersMultipleConsumers;

import java.util.concurrent.Semaphore;

public class Buffer {
    private int a;
    private static int BUFFER_CAPACITY = 5;
    private int[] v = new int[BUFFER_CAPACITY];
    private Semaphore empty = new Semaphore(BUFFER_CAPACITY);
    private Semaphore full = new Semaphore(0);
    private int pos = 0;

    public void put(int value) {
        try {
            empty.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        synchronized (Main.class) {
            pos = (pos + 1) % BUFFER_CAPACITY;
            v[pos] = value;
        }

        full.release();
    }

    public int get() {
        try {
            full.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        synchronized (Main.class) {
            a = v[pos];

            pos = pos - 1;
            if (pos < 0) {
                pos = BUFFER_CAPACITY - 1;
            }
        }

        empty.release();

        return a;
    }
}
