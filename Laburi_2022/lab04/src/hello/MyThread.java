package hello;

public class MyThread extends Thread {
	// camp ID specific clasei threadului MyThread
	private final int id;

	// Dau ID-ul ca si camp constructorului
	public MyThread (int id) {
		this.id = id;
	}

	@Override
	public void run() {
		System.out.println("Hello from thread#" + id);
	}
}
