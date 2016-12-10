// Semaphone Driver File
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>

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

int main() {
  char w[1024];
  int sfd = getSem(); // Retrieving Semaphore
  int sd = getMem(); // Retrieving Memory

  printf("Please be patient; someone may be accessing the story right now!\n\n");
  
  struct sembuf sb = {0, -1, SEM_UNDO};
  semop(sfd, &sb, 1);
  // =================== INSIDE ===================  
  // Getting Memory
  int * shm = shmat(sd, 0, 0); // Get Memory

  // Opening File
  int fw = open("story.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
  int fr = open("story.txt", O_RDONLY);
  // printf("File: %d\n", f); // Debugging

  // Displaying Past Input
  int len = *(shm);
  char disp[len+1];
  lseek(fr, -1 * len, SEEK_END);
  read(fr, disp, len);
  disp[len] = 0;
  close(fr);
  printf("You listen to the last storyteller tell his tale...\n");
  if (len < 2) // Empty or simply newline.
    printf("... but nobody came. (Perhaps, no storyteller has come yet. Or, perhaps, the last storyteller only spoke in whispers.)\n\n");
  else
    printf("%s\n", disp);

  // Soliciting Input
  printf("Please add to the story:\n");
  fgets(w, 1024, stdin);

  // Writing To File
  write(fw, w, strlen(w));
  close(fw);
  *shm = strlen(w); // Updating Last Length

  sb.sem_num = 1;
  semop(sfd, &sb, 1);
  // =================== OUTSIDE ==================  

  return 0;
}
