#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096

  /* Fork a new process to play with */
  /* We don't have a good way to list all pids in the system
     so forking a new process works for testing */ 
void test_dump() 
{
 int pid = fork();
 int size = (int)sbrk(0);
 void* buffer = malloc(size);
 void* addr=0;
 int c=0; 
 if (pid == 0) {
    /* child spins and yields */
    while (1) {
       sleep(5);
    };
  }
dump(pid, addr, buffer, size);  
int *ptr = (int*) buffer;
while(c<size){
printf(1,"0x%x:",c);
printf(1,"0x%x\t", *ptr);
ptr+=1;
printf(1,"0x%x\t", *ptr);
ptr+=1;
printf(1,"0x%x\t", *ptr);
ptr+=1;
printf(1,"0x%x\n", *ptr);
ptr+=1;
c+=16;
}
}
int main(int argc, char *argv[])
{
  test_dump();
  exit();
}
