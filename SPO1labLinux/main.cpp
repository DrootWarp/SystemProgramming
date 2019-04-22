#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

int main(int argc, char* argv[] ) {
  initscr();
  FILE *fChild;
  // char CHILD_PATH[50] = "/home/lahome/Desktop/SPOlabs/SPO1labLinux/";

  if(argc < 3){
    argv[1]="Child";
    argv[2]="10";
    argv[3]="Child.txt";
      
  }

  
  srand(time(0));
  pid_t pid = fork();
  switch (pid){
     case -1:{
       
       printw("Child process wasn't created");
       refresh();
       break;
       
     }
     case 0:{
       
       if( execv(argv[1], argv) == -1){
	 
	 printw("Error,cann't find Childe file");
	 
       }
       break;
       
     }
     default:{
       
       int pStatus;
       while(1){
	 
	 if(waitpid(pid,&pStatus,0)>0){
	   
	   fChild = fopen(argv[3],"r");
	   int i = 0, array[atoi(argv[2])];
           printw("Work Parent process: %d\n",getpid());
           refresh();
	   
	   while(fscanf(fChild,"%d ",&array[i] ) != EOF ){

	     printw("%d ",array[i]);
	     i++;

	   }

	   fclose(fChild);	   
	   break;
	 }	   
       }
       printw("\n Child Process was terminated with Status: ");
       refresh();
       if(WIFEXITED(pStatus)){
	 printw("%d",(int)WEXITSTATUS(pStatus));
       }
       refresh();

        }

    }
    getch();
    endwin();
    return 0;
}
