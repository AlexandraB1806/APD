package readersWriters.readerPriority;

import java.util.concurrent.BrokenBarrierException;

public class Reader extends Thread {
    private final int id;

    public Reader(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        do {
            // TODO
            try {
                Main.numberOfReaders.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            Main.currentReaders++;

            if (Main.currentReaders == 1) {
                try {
                    Main.readWrite.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            Main.numberOfReaders.release();

            // Citire din resursa comuna
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Reader " + id + " is reading");
            Main.hasRead[id] = true;

            // TODO
            try {
                Main.numberOfReaders.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            Main.currentReaders--;

            if (Main.currentReaders == 0) {
                Main.readWrite.release();
            }

            Main.numberOfReaders.release();

        } while (!Main.hasRead[id]);
    }
}
