/*dad_mem_mutex.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h> 
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

pthread_spinlock_t fridge_lock;
pthread_mutex_t mutex; /*初始化互斥锁*/

void *mom(){
    int fd;
    printf("Mom comes home.\n");
    sleep(rand()%2+1);
    printf("Mom checks the fridge.\n");

    // pthread_spin_lock(&fridge_lock);
    pthread_mutex_lock(&mutex);
    fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
    if(lseek(fd,0,SEEK_END)==0){
        printf("Mom goes to buy milk...\n");
        sleep(rand()%2+1);
        printf("Mon comes back.\n");
        if(lseek(fd,0,SEEK_END)>0)
            printf("What a waste of food! The fridge can not hold so much milk!\n");
        else{
            write(fd,"milk",4);
            printf("Mom puts milk in fridge and leaves.\n");
        }
    }else{
        printf("Mom closes the fridge and leaves.\n");
    }
    
    close(fd);
    // pthread_spin_unlock(&fridge_lock);
    pthread_mutex_unlock(&mutex);

}

void *dad(){
    int fd;
    printf("Dad comes home.\n");
    sleep(rand()%2+1);
    printf("Dad checks the fridge.\n");
    // pthread_spin_lock(&fridge_lock);
    pthread_mutex_lock(&mutex);
    fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
    if(lseek(fd,0,SEEK_END)==0){
        printf("Dad goes to buy milk...\n");
        sleep(rand()%2+1);
        printf("Dad comes back.\n");
        if(lseek(fd,0,SEEK_END)>0)
            printf("What a waste of food! The fridge can not hold so much milk!\n");
        else{
            write(fd,"milk",4);
            printf("Dad puts milk in fridge and leaves.\n");
        }
    } else{
        printf("Dad closes the fridge and leaves.\n");
    }
    close(fd);

    // pthread_spin_unlock(&fridge_lock);
    pthread_mutex_unlock(&mutex);

}

int main(int argc, char * argv[]) {
    // pthread_spin_init(&fridge_lock, PTHREAD_PROCESS_PRIVATE);
    pthread_mutex_init (&mutex, PTHREAD_PROCESS_PRIVATE);
    

    srand(time(0));
    pthread_t p1, p2;
    int fd = open("fridge", O_CREAT|O_RDWR|O_TRUNC , 0777);  //empty the fridge
    close(fd);

    // Create two threads (both run func)  
    pthread_create(&p1, NULL, mom, NULL); 
    pthread_create(&p2, NULL, dad, NULL); 
  
    // Wait for the threads to end. 
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 

    // pthread_spin_destroy(&fridge_lock);
    pthread_mutex_destroy(&mutex);
}
