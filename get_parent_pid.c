#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

  int saved_ebx;

  int pid0, pid1, pid2;
  printf(1, "test program: forking childs\n\n");
  pid0 = fork();
  if (pid0) {
    asm volatile(
    "movl %%ebx, %0;"
    "movl %1, %%ebx;"
    : "=r" (saved_ebx)
    : "r"(pid0)
    );
    printf(1, "User: printing ancestors for pid: %d\n" , pid0);
    int parent_pid = get_parent_pid();

    printf(1, "pid: %d, parent_pid: %d\n\n", pid0, parent_pid);
    asm("movl %0, %%ebx" : : "r"(saved_ebx));
  }
  while(wait() != -1);
    
  pid1 = fork();
  if (pid1 && pid0 == 0) {
    printf(1, "User: printing ancestors for pid: %d\n" , pid1);

    asm volatile(
    "movl %%ebx, %0;"
    "movl %1, %%ebx;"
    : "=r" (saved_ebx)
    : "r"(pid1)
    );
    int parent_pid = get_parent_pid();

    printf(1, "pid: %d, parent_pid: %d\n\n", pid1, parent_pid);
    asm("movl %0, %%ebx" : : "r"(saved_ebx));
  }
  while(wait() != -1);

  pid2 = fork();

  if(pid0 == 0 && pid1 == 0 && pid2){
    asm volatile(
    "movl %%ebx, %0;"
    "movl %1, %%ebx;"
    : "=r" (saved_ebx)
    : "r"(pid2)
  );

  int parent_pid = get_parent_pid();

    printf(1, "pid: %d, parent_pid: %d\n\n", pid2, parent_pid);
    asm("movl %0, %%ebx" : : "r"(saved_ebx));
  }  
  while(wait() != -1);
  
    
  exit();
}