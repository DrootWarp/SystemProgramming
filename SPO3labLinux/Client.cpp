#include <iostream>
#include <stdio.h>
#include <termios.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include <string.h>
#include <math.h>
#include <limits>

using namespace std;

int waitSemaphore(int sem_id, int num) {
	struct sembuf buf;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	buf.sem_num = num;
	return semop(sem_id, &buf, 1);
}

int releaseSemaphore(int sem_id, int num) {
	struct sembuf buf;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;
	buf.sem_num = num;
	return semop(sem_id, &buf, 1);
}

int main(int argc, char* argv[]){

        void *shMemoryAdress;
        int bufferSize = 10, sem_id, shm_id;
        int numberOfBlocks;
        string bufferString;
        
        sem_id = semget(atoi(argv[2]), 0, 0007);
	if(sem_id < 0)
	{
		perror("Semaphores is not opened.");
		return -1;
	}

        shm_id = shmget(atoi(argv[3]), bufferSize, 0007);        
        if (shm_id < 0) 
	{
		perror("shmget is not opened\n");
                releaseSemaphore(sem_id,3);
		return -1;
	} 
        
        shMemoryAdress = shmat(shm_id, NULL, 0);
        //semSet: 0-server,1-client,2-quit,3-error
        while(true){

               
                waitSemaphore(sem_id, 0);

                if(semctl(sem_id, 2, GETVAL) == 1){

                        shmdt(shMemoryAdress);
                        break;

                 }                                

                if(shMemoryAdress == NULL){

                         perror("Error");
                         releaseSemaphore(sem_id,3);
                         releaseSemaphore(sem_id,1);
                         return -1;   

                 }
                 
                 numberOfBlocks = *(int*)shMemoryAdress;
                 releaseSemaphore(sem_id, 1);
                 
                 for(int i = 0; i < numberOfBlocks; i++ ){

                          waitSemaphore(sem_id, 0); 
                           
                          bufferString.append((char*)shMemoryAdress, bufferSize);

                          releaseSemaphore(sem_id, 1);

                 }
                 
                 waitSemaphore(sem_id, 0);
                 bufferString.resize(*(int*)shMemoryAdress);
                 //releaseSemaphore(sem_id, 1);
                 //waitSemaphore(sem_id, 0);

                 cout<<"\nClient process got: "<< bufferString << endl;

                 releaseSemaphore(sem_id, 1);
                
                 bufferString.clear();
                                   
        }

        return 0;

}
