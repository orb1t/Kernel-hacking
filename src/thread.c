/*
 * Both spinlocks and mutex are implemented in this same file. By default mutex works here.
 * In order to test spinlocks, please uncomment the spinlock function calls (line: 73,78,99) and comment the mutex function calls (line: 74,79,100).
 * Conditional Variables are implemented in thread_cv.c
 */
#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

struct balance {
    char name[32];
    int amount;
};
volatile int total_balance = 0;

struct thread_spinlock {
  uint locked;      
}lock;

struct thread_mutex {
  uint locked;       
}mutex;

void thread_spin_init(struct thread_spinlock *lk)
{
	lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk)
{
	while(xchg(&lk->locked, 1) != 0)  // Loops until lock is 0 and set it to 1.
	    ;
}

void thread_spin_unlock(struct thread_spinlock *lk)
{
	xchg(&lk->locked, 0);
}

void thread_mutex_init(struct thread_mutex *m)
{
        m->locked = 0;
}

void thread_mutex_lock(struct thread_mutex *m)
{
        while(xchg(&m->locked, 1) != 0)
           sleep(1) ;					// saves CPU cycles
}

void thread_mutex_unlock(struct thread_mutex *m)
{
        xchg(&m->locked, 0);
}

volatile unsigned int delay (unsigned int d) {
   unsigned int i; 
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;   
}

void do_work(void *arg){
    int i; 
    int old;
   
    struct balance *b = (struct balance*) arg; 
    printf(1, "Starting do_work: s:%s\n", b->name);

    for (i = 0; i < b->amount; i++) { 
     //thread_spin_lock(&lock);   // Spinlock function call to acquire the lock, commented in order to test mutex lock
	 thread_mutex_lock(&mutex);
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
     //thread_spin_unlock(&lock); // Spinlock function call to release the lock, commented in order to test mutex unlock
	thread_mutex_unlock(&mutex);
    }
  
    printf(1, "Done s:%s\n", b->name);

    thread_exit();
    return;
}

int main(int argc, char *argv[]) {

  struct balance b1 = {"b1", 3200};
  struct balance b2 = {"b2", 2800};
 
  void *s1, *s2;
  int t1, t2, r1, r2;

  s1 = malloc(4096);
  s2 = malloc(4096);

  //thread_spin_init(&lock);
  thread_mutex_init(&mutex);
  t1 = thread_create(do_work, (void*)&b1, s1);
  t2 = thread_create(do_work, (void*)&b2, s2); 

  r1 = thread_join();
  r2 = thread_join();
  
  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n", 
      t1, r1, t2, r2, total_balance);

  exit();
}
