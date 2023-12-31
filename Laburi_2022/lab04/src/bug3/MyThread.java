package bug3;

/**
 * Why is value set correct even though we use different locks in
 * different threads?
 *
 * Modify a single line of code to obtain the expected behaviour (race condition since the monitors are different).
 */
public class MyThread implements Runnable {
    // Pentru ca a si b sunt initial referinte catre acelasi sir "LOCK", le face aceeasi referinta.
    // Daca se schimba unul dintre siruri, apar race conditionuri pentru ca
    // threadurile vor fi sincronizate dupa obiecte diferite.
    static final String a = "LOCKa";
    static final String b = "LOCKb";
    int id;
    static int value = 0;

    MyThread(int id) {
        this.id = id;
    }

    @Override
    public void run() {
        if (id == 0) {
            synchronized (a) {
                for (int i = 0; i < Main.N; i++)
                    value = value + 3;
            }
        } else {
            synchronized (b) {
                for (int i = 0; i < Main.N; i++)
                    value = value + 3;
            }
        }
    }
}
