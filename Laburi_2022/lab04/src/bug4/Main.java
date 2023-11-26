package bug4;

/**
 * Why doesn't this program end? (Hint: volatile)
 * Fix the problem changing a single line of code.
 */

// Din cauza optimizarilor automate pe care le face Java, modificarea variabilei boolene keepRunning
// este cunoscuta doar de threadul MAIN, nu si de cel secundar. Modificandu-i tipul la unul volatile,
// variabila nu se cacheuieste, iar modificarile sunt intotdeauna scrise in RAM,
// de unde sunt vizibile pentru toate threadurile.
public class Main extends Thread {
    private volatile boolean keepRunning = true;

    public void run() {
        long count = 0;
        while (keepRunning) {
            count++;
        }

        System.out.println("Thread terminated." + count);
    }

    public static void main(String[] args) throws InterruptedException {
        Main t = new Main();

        t.start();

        Thread.sleep(1000);

        t.keepRunning = false;

        System.out.println("keepRunning set to false.");
    }
}
