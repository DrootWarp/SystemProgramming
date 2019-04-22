#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <list>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>

#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <csignal>

using namespace std;

template<typename T>
std::string toString(T value)
{
    std::ostringstream oss;
    oss<< value;
    return oss.str();

}

int main(int argc,char* argv[] )
{
        sem_t* lab2Semaphore;
	string message = "Work child process,his pid:  ";
        message += toString(getpid()); 
	lab2Semaphore = sem_open(argv[2],0);

	while (true)
	{
	     sem_wait(lab2Semaphore);
	      for(int i = 0 ;i < message.length();i++,usleep(10000)){
		cout<<message[i];
                cout.flush();
	      }
	      cout<<endl;
	      sem_post(lab2Semaphore);
              sleep(1);
	}
	
	return 0;
}
