package q1;

public class BinarySemaphore {

	boolean value;
	
	public BinarySemaphore(boolean value) {
		this.value=value;
	}
	
	public synchronized void P() {
		while(value == false) {
			try {
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}	// add process to the queue of blocked processes
		}
		value=false;
	}
	
	public synchronized void V() {
		value=true;
		notify();	// wake up a process from the queue
	}

}
