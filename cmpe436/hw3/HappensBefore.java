package test;

public class HappensBefore {

    static final int N = 9999;
    static int x = 0;
    static int y = 0;

	static Lock lock = new Dekker();


	public static void main(String args[]) throws Exception {

		Thread t1 = new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				x = 10;
				lock.requestCS(0);
				y = (x+1) / 2;
				lock.releaseCS(0);
			}
		});
		
		
		Thread t2 = new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				lock.requestCS(1);
				y = (x-1) / 2;
				lock.releaseCS(1);
				x = 20;
			}
		});
		
		
		t1.start();
		t2.start();
		
		t1.join();
		t2.join();
	}
}

interface Lock {
    public void requestCS(int pid); //may block
    public void releaseCS(int pid);
}

class Dekker implements Lock {
    boolean wantCS[] = {false, false};
    int turn = 1;
    public void requestCS(int i) { // entry protocol
        int j = 1 - i;
        wantCS[i] = true;
        while (wantCS[j]) {
            if (turn == j) {
                wantCS[i] = false;
                while (turn == j) ;// busy wait
                wantCS[i] = true;
            }
        }
    }
    public void releaseCS(int i) { // exit protocol
        turn = 1 - i;
        wantCS[i] = false;
    }


}

