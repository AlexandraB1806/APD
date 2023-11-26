package readersWriters.readerPriority;

import java.util.concurrent.BrokenBarrierException;

public class Writer extends Thread {
    private final int id;

    public Writer(int id) {
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
            try {
                Main.readWrite.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // Scriere in resursa comuna
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            System.out.println("Writer " + id + " is writing");
            Main.hasWritten[id] = true;

            Main.readWrite.release();

        } while (!Main.hasWritten[id]);
    }
}
