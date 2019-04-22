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
        
        void* shMemoryAdress;
        struct shmid_ds shm_s;
        int pid;
        int bufferSize = 10, semAmount = 4;
        short semNumArray[semAmount] = {0};
        int sem_id, shm_id, clientStatus;
        int length, numberOfBLocks;

        if(argc < 3){
               argv[1] = "Client";
               argv[2] = "0265";
               argv[3] = "0264";
        }

	sem_id = semget(atoi(argv[2]), semAmount, IPC_CREAT|0007);
        semctl(sem_id, 0, SETALL, semNumArray);
	if(sem_id < 0)
	{
		perror("Semaphores is not created.");
		return -1;
	}

        shm_id = shmget(atoi(argv[3]), bufferSize, IPC_CREAT|0007);        
        if (shm_id < 0) 
	{
		perror("shmget error\n");
                semctl(sem_id,0,IPC_RMID,NULL); //delete semaphore set
		return -1;
	} 

        shMemoryAdress = shmat(shm_id, NULL, 0);

        if(shMemoryAdress == NULL){

                perror("Error");

                semctl(sem_id,0,IPC_RMID,NULL); //delete semaphore set 
                shmctl(shm_id,IPC_RMID,&shm_s);  
                            
                return -1;
        }
        
        pid = fork();
	switch(pid)
	{
	case -1:{
       
		perror("New process is not created");

                semctl(sem_id,0,IPC_RMID,NULL); //delete semaphore set
                shmdt(shMemoryAdress);      
                shmctl(shm_id,IPC_RMID,&shm_s);
		return -1;
        
	}	
	case 0:{
	
         	if (execv(argv[1],argv) == -1){

			perror("Error! Cannot open Client file");
                        return -1;
		}

                break;
        }
        //sem index 0-server,1-client,2-quit,3-error
	default:{
                                          
               string bufferString;                        
               bufferString.resize(bufferSize);

               while(true){

                        
                         cout<<"(Server)"<<"Enter the string: ";
                         getline(cin, bufferString);
                      
                         length = bufferString.size();

                         numberOfBLocks = ceil((double)length / (double)bufferSize);

                         *(int*)shMemoryAdress = numberOfBLocks;                          

                         releaseSemaphore(sem_id,0);
                         waitSemaphore(sem_id,1);

                         if(semctl(sem_id, 3, GETVAL) > 0){

                                    break;

                         }
                          
                         for(int i = 0; i < numberOfBLocks; i++){

                                    bufferString.copy((char*)(shMemoryAdress), bufferSize, i*bufferSize);
                                    //strcpy((char*)shMemoryAdress, const_cast<char*>(buffString.c_str()));
 
                                    releaseSemaphore(sem_id,0);
                                    waitSemaphore(sem_id,1);
                         }
                        
                         *(int*)shMemoryAdress = length;
                         releaseSemaphore(sem_id,0);
                         waitSemaphore(sem_id, 1);

                         if(bufferString == "quit"){
                                                                       
                                    releaseSemaphore(sem_id, 2);
                                    releaseSemaphore(sem_id, 0);
                                    waitpid(pid, &clientStatus,0);

                                    break;

                         }                          

                         bufferString.clear();
                         cin.clear();
                         cout<<endl;
                         //releaseSemaphore(sem_id, 0);
                         
         
               }
	       
        }
	  
	}

        if(WIFEXITED(clientStatus)){

                   cout << "Client was terminated with status: "
                        << WEXITSTATUS(clientStatus) << endl;

        }

        semctl(sem_id,0,IPC_RMID,NULL); //delete semaphore set
        shmdt(shMemoryAdress);      
        shmctl(shm_id,IPC_RMID,&shm_s);

        return 0;
}
