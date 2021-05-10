package test;

public class LockSet extends Thread{

    static final int N = 99999;
	static int count = 0;
	
	@Override
	public void run() {
		for (int i = 0; i < N; i++) {
			count++;
		}
	}

	public static void main(String args[]) throws Exception {
		final LockSet t1 = new LockSet();
		final LockSet t2 = new LockSet();
		t1.start();
		t2.start();
		t1.join();
		t2.join();
		System.out.println("Is it " + (N * 2) + "? " + count);
	}
}
