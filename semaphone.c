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

int main() {
  char w[1024];
  // SHARED MEMORY HERE

  // Opening File
  int f = open("story.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
  printf("File: %d\n", f); // Debugging

  // Soliciting Input
  printf("Please add to the story:\n");
  fgets(w, 1024, stdin);
  //if (w[strlen(w)-1] == '\n') 
  //  w[strlen(w)-1] = 0;

  // Writing To File
  write(f, w, strlen(w));
  close(f);

  return 0;
}
