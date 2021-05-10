public class BinarySemaphore {

    boolean flag;

    BinarySemaphore(boolean initValue) {
        flag = initValue;
    }

    public synchronized void P() {
        while (flag == false) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        flag = false;
    }

    public synchronized void V() {
        flag = true;
        notify();
    }
}