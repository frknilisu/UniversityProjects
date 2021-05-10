package q3;

public class Race {

	static int count = 0;

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Thread thread1 = new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				for (int i = 0; i < 99999; i++) {
					int tmp = count;
					tmp++;
					count = tmp;
				}
			}
		});

		Thread thread2 = new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				for (int i = 0; i < 99999; i++) {
					int tmp = count;
					tmp++;
					count = tmp;
				}
			}
		});

		Thread thread3 = new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				for (int i = 0; i < 99999; i++) {
					int tmp = count;
					tmp++;
					count = tmp;
				}
			}
		});

		thread1.start();
		thread2.start();
		thread3.start();

		try {
			thread1.join();
			thread2.join();
			thread3.join();
		} catch (Exception e) {
			// TODO: handle exception
		}
		System.out.println(count);

	}

}
