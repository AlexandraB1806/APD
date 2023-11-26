package doubleVectorElements;

public class Main {

    public static void main(String[] args) {
        // Nr threaduri date de la tastatura
//        if (args.length < 1) {
//            System.out.println("Not enough arguments.");
//            System.exit(1);
//        }
//
//        // The program should work for any P <= N
//        int P = Integer.parseInt(args[0]);

        // Nr de threaduri random
        int P = 4;
        int N = 100000013;
        int[] v = new int[N];
        // Vectorul de threaduri cu P threaduri
        MultiplyVect[] threads = new MultiplyVect[P];

        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

        // Parallelize me using P threads
        for (int i = 0; i < P; i++) {
            // v[i] = v[i] * 2;
            threads[i] = new MultiplyVect(v, i, P, N);
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }
}
