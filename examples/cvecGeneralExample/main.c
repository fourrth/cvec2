#include <stdio.h>
#include "cvec/cvec.h"
#include <stdlib.h>

// given a pointer to an element of the same as is in the vec,
// prints it in which ever format
// return is defined the same as printf

static inline int myvalueprinter(void* const element, FILE* stream)
{
    const int a = *(int*)element;
    return (fprintf(stream, "%i", a));
}

int main(void)
{
    FILE* errorlog = fopen("cvecExampleErrorloggingfile.txt", "w");
    if(!errorlog) exit(-1);

    cvec vec = cvecCreate(
	1, sizeof(int), NULL, NULL, myvalueprinter, stdout, errorlog
    );
    unsigned int abc[] = {120, 240, 360, 1080};
    cvecPush(vec, &abc, 3);

    cvecPrint(vec);

    cvecInsert(vec, 0, &abc[3], 1);
    cvecPrint(vec);
    cvecRemove(vec, 0, 1);
    cvecRemove(vec, 100, 1); // error code
    cvecPrint(vec);
    cvec duplicate = cvecCreateDuplicate(vec);
    unsigned int* getval = cvecGetElement(vec, 100); // error code
    cvecInsert(vec, 10, &abc[0], 1);
    cvecPush(duplicate, &abc[0], 4);
    cvecPush(duplicate, &abc[0], 4);
    fputs("original vec: ", stdout);
    cvecPrint(vec);
    fputs("duplicate vec: ", stdout);
    cvecPrint(duplicate);

    for(cveclen cx = 0; cx < cvecGetElementlen(vec); cx++)
	{
	    cvecPop(duplicate, 1);
	}
    cvecPrint(duplicate);

    cvecDestory(vec);
    cvecDestory(duplicate);
    fclose(errorlog);
    putchar('\n');
    return 0;
}
