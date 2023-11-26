package bug1;

/**
 * Why is value set correct even though we do not use
 * locks/synchronized?
 * DO NOT MODIFY
 */
public class MyThread implements Runnable {
    public static int value = 0;

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            // Inainte:
            value = value + 3;
            // Dupa:
            // value este static, de aceea punem clasa
//            synchronized (Main.class) {
//                value += 3;
//            }
        }
    }
}
