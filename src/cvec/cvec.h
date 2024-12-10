#ifndef CVEC_H_
#define CVEC_H_
/* - - - PRIVATE HEADER - - - */

#include <stddef.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#if cvec_BUILD_NULLCHECK
#define NULLCHECKER(x) \
    x;
#else 
#define NULLCHECKER(x)
#endif

#define cveclen_FMT "%zu"
typedef size_t cveclen; // sizeof(cveclen) < sizeof(unsigned long)
typedef int cvecstatus;

typedef void* (*cvecReallocatorFunction)(void*, size_t);
typedef void (*cvecDeallocatorFunction)(void*);
typedef int (*cvecPrintelementFunction)(void*, FILE*);

typedef struct cvec
{
    cveclen elementlen;
    cveclen elementsize;
    cveclen elementallocationlen;
    void* data;
    cvecReallocatorFunction Reallocator;
    cvecDeallocatorFunction Deallocator;
    cvecPrintelementFunction Printer;
    FILE* printoutputstream;
    FILE* errorprintoutputstream;
} cvec;

// same as not unsafe, except just doesn't do parameter checking/bounds checking
static inline void* const
cvecGetElement_unsafe(const cvec* const vec, const cveclen elementindex)
{
    return (void*)((uint8_t*)(vec->data) + elementindex * vec->elementsize);
}

// returns 0 if there is enough space
// else, returns how many elements over it is
static inline cveclen
CheckIfEnoughSpaceCvec(const cvec* const vec, const cveclen elementlen)
{
    const cveclen allocation_wanted = vec->elementlen + elementlen;
    if(allocation_wanted > vec->elementallocationlen)
	{
	    return allocation_wanted;
    } else return 0;
}

cvec* const cvecCreate(
    const cveclen elementallocationlen,
    const cveclen elementsize,
    const cvecReallocatorFunction Reallocator,
    const cvecDeallocatorFunction Deallocator,
    const cvecPrintelementFunction printer,
    FILE* const outputstream,
    FILE* const erroroutputstream
);

cvecstatus cvecDestory(cvec* const vec);

cvecstatus cvecInsert(
    cvec* const vec,
    const cveclen elementindex,
    void* const data,
    const cveclen elementlen
);

cvecstatus cvecRemove(
    cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen
);
// removes the data by copying the data infront of it, to it

cvecstatus cvecReserve(cvec* const vec, const cveclen elementlen);

cveclen cvecGetElementlen(const cvec* const vec);

cveclen cvecGetElementsize(const cvec* const vec);

cveclen cvecGetElementAllocationlength(const cvec* const vec);

void* const
cvecGetElement_unsafe(const cvec* const vec, const cveclen elementindex);
void* const cvecGetElement(const cvec* const vec, const cveclen elementindex);

void* const cvecGetLastElement(const cvec* const vec);

cvecstatus cvecPop(cvec* const vec, const cveclen elementlen);

cvecstatus
cvecPush(cvec* const vec, void* const data, const cveclen elementlen);

cvecstatus cvecCopyElements(
    const cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const buffer
);

int cvecIsValid(const cvec* const vec);

cvecstatus cvecPrint(const cvec* const vec);

cvecstatus cvecSetElementsData(
    cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const data
);

static inline cvecstatus printvecCvec(
    const cvec* const vec,
    const char* const left_character,
    const char* const right_character,
    const char* const seperator_character,
    const int should_flush
)
{
    if(vec->elementlen == 0)
	{
	    fputs(left_character, vec->printoutputstream);
	    putc(' ', vec->printoutputstream);
	    fputs(right_character, vec->printoutputstream);
	    return 0;
    }
    fputs(left_character, vec->printoutputstream);
    for(cveclen cx = 0; cx < vec->elementlen - 1; cx++)
	{
	    (*vec->Printer)(cvecGetElement(vec, cx), vec->printoutputstream);
	    fputs(seperator_character, vec->printoutputstream);
	}
    (*vec->Printer)(cvecGetLastElement(vec), vec->printoutputstream);
    fputs(right_character, vec->printoutputstream);

    if(should_flush) fflush(vec->printoutputstream);
    return 0;
}

// for right now, no options but that could change in the future
cvec* const cvecCreateDuplicate(const cvec* const vec);

#endif // CVEC_H_
