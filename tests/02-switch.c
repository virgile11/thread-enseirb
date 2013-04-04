#include <stdio.h>
#include <assert.h>
#include "thread.h"

/* test de switchs.
 *
 * les affichages doivent être dans le bon ordre (fifo)
 * le programme doit retourner correctement.
 * valgrind doit être content.
 *
 * support nécessaire:
 * - thread_create()
 * - thread_yield() depuis ou vers le main
 * - thread_exit()
 * - thread_join() avec récupération de la valeur de retour
 */

static void * thfunc(void *id)
{
  int err, i;
  for(i=0; i<10; i++) {
    fprintf(stderr, "%s yield vers le main\n", (char*) id);
    err = thread_yield();
    assert(!err);
  }

  fprintf(stderr, "%s terminé\n", (char*) id);
  thread_exit(NULL);
}

int main()
{
  thread_t th1,th2,th3;
  void *res;
  int err, i;

  err = thread_create(&th1, thfunc, "fils1");
  assert(!err);
  err = thread_create(&th2, thfunc, "fils2");
  assert(!err);
  err = thread_create(&th3, thfunc, "fils3");
  assert(!err);
  /* des switchs avec l'autre thread */
  for(i=0; i<20; i++) {
    fprintf(stderr, "le main yield vers un fils\n");
    err = thread_yield();
    assert(!err);
  }

  fprintf(stderr, "--- Join th3 ----\n");
  err = thread_join(th3, &res);
  assert(!err);
  assert(res == NULL);
  fprintf(stderr, "--- Join th2 ----\n");
  err = thread_join(th2, &res);
  assert(!err);
  assert(res == NULL);
  fprintf(stderr, "--- Join th1 ----\n");
  err = thread_join(th1, &res);
  assert(!err);
  assert(res == NULL);

  printf("main terminé\n");
  return 0;
}
