/*
* Author: Alexandros Perrakis
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "restaurant.h"

#define TRUE 1

sem_t chef_ready;
sem_t customer_ready;
sem_t modify_globals;

int customers_left;
int customers_waiting = 0;
int is_logged_in_Facebook = 0;
int customer_is_being_served = 0;

void log_in_facebook()
{
    printf("\033[0;34mChef logged into Facebook.\033[0m \n");
    is_logged_in_Facebook = 1;
}

void log_out_of_facebook()
{
    printf("\033[0;33mChef logged out of Facebook.\033[0m \n");
    is_logged_in_Facebook = 0;
}

void serve_customer()
{   
    int s = 100 + (rand() % 901); /* Random number between 100 and 1000 (miliseconds)*/
    s = s * 1000; /* Convert miliseconds to microseconds*/
    
    usleep(s);
}

void * chef()
{
    printf("\033[0;32mChef Thread was created. \033[0m\n");

    while(customers_left > 0) // Exit Chef thread if no more customers are coming (Fixed value N from arguments)
    {
        if(customers_waiting == 0) //If there are no more customers currently inside or outside of the restaurant, log in Facebook
            log_in_facebook();

        sem_wait(&customer_ready); // Check or wait for a customer
       
        customer_is_being_served = 1; // We need this to stop customer thread exiting before being served
        
        if(is_logged_in_Facebook) 
            log_out_of_facebook();   

        sem_post(&chef_ready); // Signal that Chef is now ready to serve the customer
        
        printf("A customer is being served.\n");
        serve_customer();
        printf("Customer was served and left.\n");

        customer_is_being_served = 0;
    }

    pthread_exit(NULL);
}

void * customer()
{
    long unsigned int thread_id = pthread_self(); // Thread ID
    printf("\033[0;32mCustomer Thread with id = %ld was created. \033[0m \n", thread_id);
    
    sem_post(&customer_ready); // Signal that current customer is ready to get served
    sem_wait(&chef_ready);     // Wait for Chef to get ready 

    // Semaphore synchronization for modifying global variables that all threads have access
    sem_wait(&modify_globals); 
    customers_left--;
    customers_waiting--;
    sem_post(&modify_globals);

    while(customer_is_being_served); // Wait for customer to be served before exiting thread
    printf("\033[0;31mCustomer Thread with id = %ld was destroyed. \033[0m \n", thread_id);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    // Second Argument as N (Total number of customers)
    int N = atoi(argv[1]);

    // Customers left number for Chef thread to know when to exit
    customers_left = N;

    pthread_t chef_pthread;
    pthread_t customer_pthread[N];
    int rc; // Return Code  

    // Initialization of semaphores
    sem_init(&chef_ready, 0, 0);
    sem_init(&customer_ready, 0, 0); 
    sem_init(&modify_globals, 0, 1);

    // Creation of Chef Thread
    rc = pthread_create(&chef_pthread, NULL, chef, NULL);
    if(rc) 
    {
        printf("Error creating thread.\n"); 
        exit(-1);
    }        

    int i;
    int N_third = N / 3;
    // Wait 5 seconds every time we reach one third of N so that all customers are not created at the same time.
    for(i = 0; i < N; i++)
    {
        if(i == N_third || i == N_third * 2)
            sleep(5);

        rc = pthread_create(&customer_pthread[i], NULL, customer, NULL);
        if(rc) 
        {
            printf("Error creating thread.\n"); 
            exit(-1);
        }
        customers_waiting++; // Increase this every time a customer thread is created
    }

    // Pthread Joins - Wait for all threads to finish.
    pthread_join(chef_pthread, NULL);
    for(i = 0; i < N; i++)
        pthread_join(customer_pthread[i], NULL);
        
    return 0;
}
