/*
 *Conditional variables and Mutex is implemented here. A small queue is used to test the solution.
 */

#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

struct thread_mutex {
  uint locked;
}mutex;

struct thread_cond {
	uint chan;   //channel
	uint count;
	};

struct q {
   struct thread_cond cv;
   struct thread_mutex m;
   void *ptr;
};
struct q *q;

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

void thread_cond_init(struct thread_cond *c)
{
  c->chan = 0;
  c->count = 0;
}

void thread_cond_wait(struct thread_cond *c, struct thread_mutex *m)
{
	  thread_mutex_unlock(m);
	  cond_sleep(&(c->chan));
	  thread_mutex_lock(m);
}

void thread_cond_signal(struct thread_cond *c)
{
  cond_wake(&(c->chan));
}

// Thread 1 (sender)
void
send(struct q *q)
{
	void *p = malloc(4);
	printf(1, "Sender started\n");
   thread_mutex_lock(&q->m);
//    while(q->ptr != 0)    			// for interviews
// 	    thread_cond_wait(&q->cv, &q->m);
   q->ptr = p;
   thread_cond_signal(&q->cv);
   thread_mutex_unlock(&q->m);
	printf(1, "Sent 'p' in 'q->ptr'\n");
   thread_exit();
}

// Thread 2 (receiver)
void*
recv(struct q *q)
{
  void *p;
  printf(1, "Receiver started\n");
  thread_mutex_lock(&q->m);

  while((p = q->ptr) == 0)
    thread_cond_wait(&q->cv, &q->m);
  q->ptr = 0;
  thread_mutex_unlock(&q->m);
  printf(1, "Received 'p' from 'q->ptr.\n");
  thread_exit();
  return p;
}

int main(int argc, char *argv[]) {

	q->ptr=0;
	thread_mutex_init(&q->m);
	thread_cond_init(&q->cv);

	void *s1, *s2;
	  int t1, t2;

	  s1 = malloc(4096);
	  s2 = malloc(4096);

	  t1 = thread_create(send, (void*)q, s1);
	  t2 = thread_create(recv, (void*)q, s2);

	  thread_join();
	  thread_join();

	  printf(1, "Cond Variable - Threads finished: %d, %d\n", t1, t2);
	  exit();
}
