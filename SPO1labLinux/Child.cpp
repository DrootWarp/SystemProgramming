#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>


int main(int argc, char* argv[]){
  initscr();
  int i = 0;
  FILE * fChild;
  int array[atoi(argv[2])] = {};
  srand(time(0));
  for(int i = 0; i < atoi(argv[2]) ; i++ ){
    array[i] = 1 + rand() % 10;
  }
  fChild = fopen(argv[3],"w");
  i = 0;

  while(i < atoi(argv[2])){
    fprintf(fChild,"%d ",array[i]);
    i++;
  }

  printw("Work CHild %d, his Parent: %d",getpid(),getppid());
  refresh();

  fclose(fChild);
  sleep(3);
  endwin();
  return 1;
}
