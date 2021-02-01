# sleeping-chef
The classic consumer-producer thread synchronization problem, implemented in C, with semaphores.

The problem is:
Take-away in a restaurant, during CoViD-19 limitations, where only 1 customer can be inside the restaurant, anytime.

Chef (Producer) thread: 1 <br>
Customer (Consumer) threads: 0 - N

### Chef procedure:
* If there is a customer outside, Chef invites him in the store and serves him.<br> The customer takes the order and leaves. (thread exits)
* If there are no customers waiting, Chef logs in facebook and waits until there is a customer.

### Compile

* <b> Makefile: </b> <br> with the command make and run with default N = 12 Customers with make run.
* <b> Manually: </b> <br> Compile: gcc -pthread restaurant.c -o restaurant <br> Run: ./restaurant N (where N is the number of the total customers that will arrive)

### Screenshot from a run with N=12 clients:

![alt text](https://github.com/Stolichnayer/sleeping-chef/blob/main/screenshot.png)
