// Semaphore Control File

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

// /*
union semun {
  int val;
  struct semid_ds * buf;
  unsigned short * array;
  struct seminfo * _buf;
} su;
// */

int getSem() {
  int sfd = semget( ftok("makefile", 314159),
		    1,
		    IPC_CREAT | 0644 );
  return sfd; // Semaphore Descriptor
}

int getMem() {
  int sd = shmget( ftok("makefile", 1729),
		   4, // int
		   IPC_CREAT | 0644 );
  return sd; // Shared Memory Descriptor
}

int main(int argc, char * argv[]) {
  if (argc == 1)
    return 0;

  if (strncmp(argv[1],"-c",strlen(argv[1])) == 0) { // create or access
    int sfd = getSem(); // Creating Semaphore
    int sd = getMem(); // Creating Shared Memory
    union semun su; 
    su.val = 1;
    semctl (sfd, 0, SETVAL, su);
    int f =  open("story.txt",O_CREAT | O_TRUNC, 0644);
    close(f);
    // printf("Retrieved Semaphore: %d\n", sfd); // Debugging
    // printf("Retrieved Shared Memory: %d\n", sd); // Debugging
  }

  if (strncmp(argv[1],"-r",strlen(argv[1])) == 0) { // remove
    int sfd = getSem();
    int sd = getMem();
    int f = open ("story.txt", O_RDONLY, 0664);
    struct stat *fileinfo = (struct stat*) malloc (sizeof(struct stat));
    stat("story.txt", fileinfo);
    if (fileinfo->st_size != 0) {
      char s[fileinfo->st_size];
      read(f, s, fileinfo->st_size);
      printf("%s\n", s ); // Print Story
    }
    close(f); // Closing File
    semctl(sfd, 0, IPC_RMID); // Removing Semaphore
    shmctl(sd, IPC_RMID, 0); // Removing Shared Memory
    free(fileinfo); // Freeing Fileinfo
    // printf("Removed Semaphore: %d\n", sfd); // Debugging
    // printf("Removed Shared Memory: %d\n", sd); // Debugging
  } 
  
  if (strncmp(argv[1],"-v",strlen(argv[1])) == 0) { // value
    int sfd = getSem();
    int v = semctl(sfd, 0, GETVAL);
    //view contents of the story file
    int f = open ("story.txt", O_RDONLY, 0664);
    struct stat *fileinfo = (struct stat*) malloc (sizeof(struct stat));
    stat("story.txt", fileinfo);
    if (fileinfo->st_size != 0) {
      char s[fileinfo->st_size];
      read(f, s, fileinfo->st_size);
      printf("%s\n", s ); // Print Story
    }
    close(f);
    free(fileinfo); // Freeing Fileinfo
    // printf("Semaphore %d Value: %d\n", sfd , v); // Debugging
  }

  return 0;
}
