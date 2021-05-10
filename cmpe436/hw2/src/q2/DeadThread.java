package q2;

public class DeadThread {

	public static Object resource1 = new Object();
	public static Object resource2 = new Object();
	public static Object resource3 = new Object();

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ThreadDemo1 T1 = new ThreadDemo1();
		ThreadDemo2 T2 = new ThreadDemo2();
		ThreadDemo3 T3 = new ThreadDemo3();
		T1.start();
		T2.start();
		T3.start();
	}

	private static class ThreadDemo1 extends Thread {
		public void run() {
			synchronized (resource1) {
				System.out.println("Thread 1: Holding resource 1...");

				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				System.out.println("Thread 1: Waiting for resource 2...");

				synchronized (resource2) {
					System.out.println("Thread 1: Holding resource 1 & 2...");
				}
			}
		}
	}

	private static class ThreadDemo2 extends Thread {
		public void run() {
			synchronized (resource2) {
				System.out.println("Thread 2: Holding resource 2...");

				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				System.out.println("Thread 2: Waiting for resource 3...");

				synchronized (resource3) {
					System.out.println("Thread 2: Holding lock 2 & 3...");
				}
			}
		}
	}

	private static class ThreadDemo3 extends Thread {
		public void run() {
			synchronized (resource3) {
				System.out.println("Thread 3: Holding resource 3...");

				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				System.out.println("Thread 3: Waiting for resource 1...");

				synchronized (resource1) {
					System.out.println("Thread 3: Holding lock 1 & 3...");
				}
			}
		}
	}

}
