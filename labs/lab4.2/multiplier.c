#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include "logger.h"

#define ROW_LEN 2000

int NUM_BUFFERS;
long **buffers;
pthread_mutex_t *mutexes;
long * result;

long * readMatrix(char *filename);
long * getColumn(int col, long *matrix);
long * getRow(int row, long *matrix);
int getLock();
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long * multiply(long *matA, long *matB);
int saveResultMatrix(long *result);

int main(int argc, char *argv[]) {
	NUM_BUFFERS = atoi(argv[2]);
	long *matA,
		 *matB,
		 *result;
	pthread_mutex_t mtxs[NUM_BUFFERS];

	omp_set_num_threads(2);

	if(argc != 3 || strcmp(argv[1], "-n") != 0) {
		perror("Usage: ./multiplier -n <NUM_BUFFERS>");
		return 1;
	}

	buffers = (long **) malloc(sizeof(long **) * NUM_BUFFERS);

	for(int i = 0; i < NUM_BUFFERS; i++) {
		pthread_mutex_init(&mtxs[i], NULL);
	}

	mutexes = mtxs;

	matA = readMatrix("matA.dat");
	matB = readMatrix("matB.dat");

	result = multiply(matA, matB);

/*	for(int i = 0; i < 10; i++) {
		printf("%ld\n", buffers[0][i]);
	}
*/
	free(matA);
	free(matB);
	free(buffers);

	for(int i = 0; i < NUM_BUFFERS; i++) {
		pthread_mutex_destroy(&mutexes[i]);
	}

	return 0;
}

long * readMatrix(char *filename) {
	FILE *fp;
	long *mat,
		  element = 0;
	char num[10],
		 *ptr,
		 c;
	int i = 0;

	mat = (long *)malloc(sizeof(long) * 4000000);

	fp = fopen(filename, "r");

	while((c = getc(fp)) != EOF) {
		if(c == '\n' || c == EOF) {
			mat[element] = strtol(num, &ptr, 10);
			memset(num, 0, 10);
			element++;
			i = 0;
		} else {
			num[i] = c;
			i++;
		}
	}

	fclose(fp);
	return mat;
}

long * getColumn(int col, long *matrix) {
	long *column = (long *)malloc(sizeof(long) * 2);

	for(int i = 0; i < 2; i++) {
		column[i] = matrix[col + (i * 2)];
	}

	return column;
}

long * getRow(int row, long *matrix) {
	long *r = (long *)malloc(sizeof(long) * 2);

	for(int i = 0; i < 2; i++) {
		r[i] = matrix[(row * 2) + i];
	}

	return r;
}

int getLock() {
	int buffer = -1;
	while(buffer == -1) {
		for(int i = 0; i < NUM_BUFFERS; i++) {
			if(pthread_mutex_trylock(&mutexes[i]) == 0) {
				buffer = i;
				break;
			}
		}
		wait();
	}
	return buffer;
}

int releaseLock(int lock) {
	int res = pthread_mutex_unlock(&mutexes[lock]);
	return res;
}

long dotProduct(long *vec1, long *vec2) {
	long res = 0;
	int buff,
		buff2;

	for(int i = 0; i < 2; i++) {
		res += vec1[i] * vec2[i];
	}

	releaseLock();
	return res;
}

long * multiply(long *matA, long *matB) {
	long *result;

	result = (long *)malloc(sizeof(long) * 4000000);

#pragma omp parallel for
	for(int i = 0; i < 2; i++) {
#pragma omp parallel for
		for(int j = 0; j < 2; j++) {
			result[(i*2) + j] = dotProduct(getRow(i, matA), getColumn(j, matB));
		}
	}
	printf("%lu\n", result[0]);
	return result;
}

int saveResultMatrix(long *result) {
	return 0;
}
