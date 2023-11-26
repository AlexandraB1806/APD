package bug2;

/**
 * DO NOT MODIFY
 */

// Un thread nu poate obtine lock-ul monitorului (obiectului) daca este deja detinut de un alt thread.
// Dar, un thread poate accesa de mai multe ori acelasi lock si NU apare un DEADLOCK.
// Fenomenul se numeste reentrant synchronization.

// Explicatii suplimentare:
// https://docs.oracle.com/javase/tutorial/essential/concurrency/locksync.html

// In plus, orice clasa are in mod automat lock pe ea insasi.

public class Main {

    public static void main(String[] args) {
        Thread thread = new Thread(new MyThread());
        thread.start();
        try {
            thread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(MyThread.i);
    }
}
