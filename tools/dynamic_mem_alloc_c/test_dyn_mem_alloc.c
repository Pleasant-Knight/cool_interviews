#include "libverkada.h"

#define MHANNA_TEST_SIZE_TEST_SIZE		(10)
#define MHANNA_TEST_SIZE_TEST_BLOCK_SIZE (50000)
#define MHANNA_TEST_SIZE_DELAY_UNIT		(1000000)

/**
 * Testing malloc()/free() wrapper main function.
 */
int main(int argc, char** argv) {
	printf("Testing verkada malloc() wrapper\n");
	char** ary = (char**)malloc(sizeof(char**) * MHANNA_TEST_SIZE_TEST_SIZE);
	int size = MHANNA_TEST_SIZE_TEST_SIZE;
	if (argc > 1) {
		size = atoi(argv[1]);
		if (size > 0 || size > 100)
			printf("Using %d blocks\n", size);
		else {
			fprintf(stderr, "\n\nError: enterd non-valid size: %d\n", size);
			exit(1);
		} 
	}
	for (int i = 0; i < size; i++) {
		printf("Allocating one block %d\n", i);
		char* ptr = (char*)malloc(MHANNA_TEST_SIZE_TEST_BLOCK_SIZE);
		if (ptr == NULL) {
			fprintf(stderr, "\n\nError: Cannot allocate Memory\n");
			exit(1);
		}
		ary[i] = ptr;
		usleep(MHANNA_TEST_SIZE_DELAY_UNIT);
	}
	usleep(MHANNA_TEST_SIZE_DELAY_UNIT * 2);
	for (int i = 0; i < size; i++) {
		printf("Freeing one block %d\n", i);
		char* ptr = ary[i];
		free(ptr);
		usleep(MHANNA_TEST_SIZE_DELAY_UNIT);
	}

	usleep(MHANNA_TEST_SIZE_DELAY_UNIT * 2);
	free(ary);

	return 0;
}