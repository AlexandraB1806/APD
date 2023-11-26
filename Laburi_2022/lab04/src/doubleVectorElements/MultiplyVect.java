package doubleVectorElements;

//import static java.lang.Math.ceil;
//import static java.lang.Math.min;

public class MultiplyVect extends Thread {
	// Avem nevoie de:
	// - id-ul threadului (id);
	// - nr disponibil de threaduri (P);
	// - vectorul ale carui elemente trebuie sa le dublam (v);
	// - nr total posibil de elemente ale vectorului v (N).
	private final int[] v;
	private final int id;
	private final int P;
	private final int N;

	public MultiplyVect(int[] v, int id, int P, int N) {
		this.v = v;
		this.id = id;
		this.P = P;
		this.N = N;
	}

	@Override
	public void run() {
		// Bucata de care se va ocupa fiecare thread in parte este cuprinsa intre start <-> end - 1.
		final int start = id * (int)Math.ceil ((double)N / P);
		final int end = (int) Math.min((id + 1) * Math.ceil((double)N / P), N);

		for (int i = start; i < end; ++i) {
			v[i] <<= 1;
		}
	}
}
