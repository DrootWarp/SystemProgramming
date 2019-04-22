#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <list>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

template<typename T>
std::string toString(T value)
{
    std::ostringstream oss;
    oss<< value;
    return oss.str();

}

void* threadFunction(void* arg);

int main(int argc, char* argv[]) {
  
  int status;
  pthread_t tid;
  list<pthread_t> threadList;
  pthread_mutex_t mutex; 

  pthread_mutex_init(&mutex,NULL);

  cout << "This is the parent process." << endl;
  cout << "create new process'+'" << endl;
  cout << "delete last process'-'" << endl;
  cout << "quit 'q'" << endl;

  while (true) {
    switch (cin.get()) {
      case '+':
        status = pthread_create(&tid, NULL, threadFunction, &mutex);
        if(status != 0){
         
              cout<<"Error with creating thread"<<endl;
              return -1;        
        }
        threadList.push_back(tid);
        break;
      case '-':
        if (threadList.empty()) {
            cout << "No threads to delete" << endl;

        }
        pthread_cancel(threadList.back());
        threadList.pop_back();
        break;
      case 'q':
        for(int i = 0,size = threadList.size() ; i < size ; i++){
            pthread_cancel(threadList.back());
            threadList.pop_back();        
        }       

        pthread_mutex_destroy(&mutex);
        return 0;
    }
  }
}

void* threadFunction(void* arg) {

  string message = "Work thread with tid: ";
         message += toString(pthread_self());   

  while (true) {

    pthread_testcancel();
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_mutex_lock((pthread_mutex_t*)(arg));

    for(int i = 0 ;i < message.length();i++,usleep(1000/60)){
	   cout<<message[i];
           cout.flush();
    }
    cout<<endl;
    
    pthread_mutex_unlock((pthread_mutex_t*)(arg));
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //sleep(1);
  }

}
