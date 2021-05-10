### CmpE 436 - Hw3  
Name: Abdullah Furkan Ilisu  
ID: 2013400201  

***

### Question1

>

	- example multithreaded Java programs where the Lockset based race detection algorithm finds a potential race: 

		x = x+1;
	
	- The lockset-based detectors never produces false negatives means never misses a real race 
	
>

	- example multithreaded Java programs where the Happens-before based race detection algorithm finds a potential race: 

		x = x+1;
	
	- another example where it does not find the data race: 

		thread1(){
			--counter;
			mutex.lock();
			y = (counter+1) / 2;
			mutex.unlock();
		}

		thread2(){
			mutex.lock();
			y = (counter-1) / 2;
			mutex.unlock();
			++counter;
		}

		there is no data race on 'y'
		execution of threads (thread2, thread1) there is a data race on 'counter'
		however, the happens-before based detector does not predict this (false negative)
	
>

	Road Runner tool supports various race detection algorithm including the above two. 
	
***

### Question2

![Promela_Model_Code](https://github.com/frknilisu/CmpE436/tree/master/hw3/promela_model_code.pml)

![Promela_Model_View](https://github.com/frknilisu/CmpE436/tree/master/hw3/promela_model_view.png)

![Promela_Model_Verification](https://github.com/frknilisu/CmpE436/tree/master/hw3/promela_model_verification.out)

***

### Question3

![Q3-Solution](https://github.com/frknilisu/CmpE436/tree/master/hw3/q3.jpg)
