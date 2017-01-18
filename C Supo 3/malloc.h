
#include <sys/types.h>

void *mySbrk(int increment);
void *myMalloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *calloc(size_t nelem, size_t elsize);
int main(int argc, char *argv[]);
