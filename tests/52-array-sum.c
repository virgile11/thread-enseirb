#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

/* Somme des éléments d'un tableau méthode diviser pour régner
 *
 * Ce programme stocke dans un tableau de taille N les entiers de 1 à N et
 * calcule leur somme. On fait la vérification du résultat avec N*(N+1)/2
 */

struct bundle {
	int *array;
	int start;
	int end;
};


static void * sum(void *arg)
{
	struct bundle *b = (struct bundle *) arg;

	int err, middle;
	void *res, *res1, *res2;
	thread_t th1, th2;
	struct bundle b1, b2;

	if (b->end == b->start) {
		res = (void *) b->array[b->start];
	}
	
	else {
		middle = b->start + (b->end - b->start) / 2;

		b1.array = b->array;
		b1.start = b->start;
		b1.end   = middle;

		b2.array = b->array;
		b2.start = middle + 1;
		b2.end   = b->end;

		//fprintf(stderr, "split1: %d to %d | split2: %d to %d\n",
		//		b1.start, b1.end,
		//		b2.start, b2.end);

		err = thread_create(&th1, sum, &b1);
		assert(!err);
		err = thread_create(&th2, sum, &b2);
		assert(!err);

		err = thread_join(th1, &res1);
		assert(!err);
		err = thread_join(th2, &res2);
		assert(!err);

		res = (void *) ((int) res1 + (int) res2);
	}

	return res;
}


int main(int argc, char *argv[])
{
	int *array;
	unsigned int size;

	if (argc < 2) {
		printf("argument manquant\n");
		return -1;
	}

	size = atoi(argv[1]);
	array = malloc(size * (sizeof *array));

	int i;
	for (i = 0; i < size; ++i) {
		array[i] = i+1;
	}

	struct bundle b = { array, 0, size-1 };
	int res = (int) sum(&b);

	free(array);

	assert(res == size * (size+1) / 2);
	printf("somme des entiers de 1 à %d = %d\n", size, res);

	return 0;
}