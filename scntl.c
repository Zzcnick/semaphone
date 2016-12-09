// Semaphore Control File

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

/*
struct sembuf {
  short sem_num;
  short sem_op;
  short sem_flag;
};
*/

union semun {
  int val;
  struct semid_ds * buf;
  unsigned short * array;
  struct seminfo * _buf;
} su;

int getSem() {
  int sfd = semget( ftok("makefile", 314159),
		    1,
		    IPC_CREAT | 0644);
  return sfd; // Semaphore Descriptor
}

int main(int argc, char * argv[]) {

  if (argc == 1)
    return 0;

  if (strncmp(argv[1],"-c",strlen(argv[1])) == 0) { // create or access
    int sfd = getSem();
    su.val = 1;
    semctl (sfd, 0, SETVAL, su);
    int f =  open("story.txt",O_CREAT | O_TRUNC, 0644);
    close(f);
    printf("Retrieved Semaphore: %d\n", sfd); // Debugging
  }
  
  /* // addition and other stuff tonight
  if (strncmp(flag,"-u",2) == 0) // up
    semctl(sfd, 0, IPC_SETVAL, semun);
  if (strncmp(flag,"-d",2) == 0) // down
    semun.semnum = -1;
  */

  if (strncmp(argv[1],"-r",strlen(argv[1])) == 0) { // remove
    int sfd = getSem();
    int f = open ("story.txt", O_RDONLY);
    char *s;
    read(f, s);
    
    printf("%s\n", s, );
    semctl(sfd, 0, IPC_RMID);
    printf("Removed Semaphore: %d\n", sfd); // Debugging
  } 
  
  if (strncmp(argv[1],"-v",strlen(argv[1])) == 0) { // value
    int sfd = getSem();
    int v = semctl(sfd, 0, GETVAL);
    printf("Semaphore %d Value: %d\n", sfd , v); // Debugging
  }

  return 0;
}
