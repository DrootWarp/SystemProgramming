#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <stdio.h>

#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <csignal>

using namespace std;

int main(int argc, char* argv[])
{
    int sig;  
    char symbol;
    pid_t pid;
    list<pid_t> pidList;
    sem_t* lab2Semaphore;
    if(argc < 2){
      argv[1] = "Child";
      argv[2] = "/lab2Semaphore";
    }

    cout << "This is the parent process." << endl;
    cout << "create new process'+'" << endl;
    cout << "delete last process'-'" << endl;
    cout << "quit 'q'" << endl;

    lab2Semaphore = sem_open(argv[2], O_CREAT, 0777,1);
     if (lab2Semaphore == SEM_FAILED) {
   	cout<< "Error with creating semaphore"<<endl;
     return 1;  
     }

    while(true)
    {
        cin.get(symbol);
        switch(symbol)
        {
            case '+':
                pid = fork();
                switch(pid)
                {
                case -1:{
                    cout << "Erorr while creating child process! (fork)" << endl << endl;
                    exit(1);
		}
                case 0:{
                    execv(argv[1], argv);
                    cout << "Error while loading child process (excec)!" << endl << endl;
                    exit(2);
		}
                default:{
                    pidList.push_back(pid);
		    // sem_post(lab2Semaphore);
                    break;
		}
                }
                break;

            case '-':
                if(pidList.empty())
                {
                    cout << "There are no chidren to delete!" << endl;
                }
                else
                {
                    sem_wait(lab2Semaphore);
                    kill(pidList.back(), SIGKILL);
                    pidList.pop_back();
                    sem_post(lab2Semaphore);
                }
                break;

            case 'q':
	       sem_close(lab2Semaphore);
               sem_unlink(argv[2]);
               kill(-getpid(), SIGKILL);
                return 0;

            default:
                continue;

	   
        }
        cin.ignore();
    }
}
