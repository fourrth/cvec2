#ifndef PUBLIC_CVEC_CVEC_H_
#define PUBLIC_CVEC_CVEC_H_
/* - - - PUBLIC HEADER - - - */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef void* cvec;

typedef size_t cveclen; // sizeof(cveclen) < sizeof(unsigned long)
typedef int cvecstatus;

/**
 * @brief the same as std's realloc()
 */
typedef void* (*cvecReallocatorFunction)(void*, size_t);

/**
 * @brief the same as std's free() 
 */
typedef void (*cvecDeallocatorFunction)(void*);

/**
 * @brief given a pointer to an element in a vec, print it in the specified stream
 * @return the amount of bytes transmitted to stream
 * what printf would return
 */
typedef int (*cvecPrintelementFunction)(void*, FILE*);

/**
 * @brief Creates a cvec
 * @param elementallocationlen starting amount of space that you want to allocate
 * if 0, then does the standard amount
 * @param elementsize the size of each individual element in the cvec
 * fails if 0
 * @param Reallocator a pointer to a user defined allocator
 * this allocator should be have the same conventions as std's realloc()
 * @param Deallocator a pointer to a user defined deallocator
 * this should have the same conventions as std's free()
 * if either Reallocator or Deallocator are NULL, 
 * then it will default to using realloc() and free()
 * @param Printer a pointer to a user defined printer
 * this should print an individual element however the user wants
 * only a single element: everything else is taken care of
 * if NULL, that means that the user will not be able to print the cvec
 * @param outputstream a pointer to the stream to output print calls to
 * if NULL, defaults to stdout
 * @param erroroutputstream a pointer to the stream to output runtime errors to
 * if NULL, defaults to stderr
 * @return a pointer to the cvec
 * if NULL, failure
 * else success
 */
const cvec cvecCreate(
    const cveclen elementallocationlen,
    const cveclen elementsize,
    const cvecReallocatorFunction Reallocator,
    const cvecDeallocatorFunction Deallocator,
    const cvecPrintelementFunction Printer,
    FILE* const outputstream,
    FILE* const erroroutputstream
);

/**
 * @brief cleans up the cvec given
 * @param vec a non-null pointer to the cvec
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecDestory(const cvec vec);

/**
 * @brief Insert an element into the cvec
 * if elementindex is in the middle of the cvec
 * i.e., elementindex != elementlen
 * then it will do a copy and shift 
 * the stuff to the 'right' over to the left
 * however, if it is to the 'right', then we
 * put all 0's before where the user's specified index
 * @param vec a non-null pointer to the cvec
 * @param elementindex what index to insert into
 * @param data a pointer to the data to insert
 * if NULL, then assume it is just all 0's
 * for however long elementlength is
 * @param elementlen how many elements to insert from data
 * failure if 0
 * @return cvecstatus 
 * if != 0, then failure
 * else success
 */
cvecstatus cvecInsert(
    const cvec vec,
    const cveclen elementindex,
    void* const data,
    const cveclen elementlen
);

/**
 * @brief Removes an element from the cvec
 * if the index is in the middle of the cvec
 * i.e., elementindex != cvec's len -1
 * then the elements to the 'right' of elementindex 
 * will be shifted 'left' to fill the space left
 * @param vec a non-null pointer to the cvec
 * @param elementindex what index to remove at
 * failure if out of bounds
 * @param elementlen how many elements from that index to remove
 * [ elementindex, elementindex + elementlen )
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecRemove(
    const cvec vec,
    const cveclen elementindex,
    const cveclen elementlen
);

/**
 * @brief Increases the allocationlength of the cvec
 * calles the cvec's reallocator and get storage for
 * the current amount of storage, plus elementlen
 * @param vec pointer to the cvec
 * @param elementlen how many more elements should the new allocation have
 * if 0, just passes the function and immediately returns
 * if vec is NULL, but elementlen is 0, then it will ignore it all 
 * and just return 0, as the 'elementlen = 0' cond requires
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecReserve(const cvec vec, const cveclen elementlen);

/**
 * @brief Getter for getting the cvec's element length
 * @param vec a non-null pointer to the cvec
 * if NULL, then just returns 0
 * @return cveclen
 * the length (in terms of elements) of the cvec
 */
cveclen cvecGetElementlen(const cvec vec);

/**
 * @brief Getter for getting the cvec's element size
 * @param vec a non-null pointer to the cvec
 * if NULL, then just returns 0
 * @return cveclen
 * the size in bytes of each element in the cvec
 */
cveclen cvecGetElementsize(const cvec vec);

/**
 * @brief Getter for getting the current allocation len
 * @param vec a non-null pointer to the cvec
 * if NULL, then just return 0
 * @return cveclen
 * the size (in terms of elements) of the current allocation in the cvec
 */
cveclen cvecGetElementAllocationlen(const cvec vec);

/**
 * @brief Getter for getting the specified element in the cvec
 * @param vec a non-null pointer to the cvec
 * @param elementindex what index to retrieve from
 * failure if out of bounds
 * @return pointer to element
 * if NULL, then failure
 * else success
 */
void* const cvecGetElement(const cvec vec, const cveclen elementindex);

/**
 * @brief Getter for getting the last element in the cvec
 * @param vec a non-null pointer to the cvec
 * @return pointer to element
 * if NULL, then failure
 * else success
 */
void* const cvecGetLastElement(const cvec vec);

/**
 * @brief Removes the last elements from the cvec
 * @param vec a non-null pointer to the cvec
 * @param elementlen how many elements to remove at the end
 * must be less than cvec len
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecPop(const cvec vec, const cveclen elementlen);

/**
 * @brief Pushes elements onto the cvec
 * @param vec a non-null pointer to the cvec
 * @param data a non-null pointer to the data to be added to the cvec
 * @param elementlen the length (in terms of elements) of the data
 * if 0, just passes the function and immediately returns
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecPush(const cvec vec, void* const data, const cveclen elementlen);

/**
 * @brief Copies elements from the cvec into given buffer
 * @param vec a non-null pointer to the cvec
 * @param elementindex where to start copying from in the cvec
 * failure if out of bounds
 * @param elementlen how many elements to copy from the cvec
 * failure if out of bounds
 * @param buffer a non-null pointer to copy into
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecCopyElements(
    const cvec vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const buffer
);

/**
 * @brief Does the same as cvecInsert(), but will not push elements to the right
 * instead just overwrites them
 * 
 * is guaranteed to do the write, unlike memset() or whatever
 * @param vec a non-null pointer to the vec
 * @param elementindex index to start setting at
 * if it is greater than the elementlen, but less than allocation length,
 * it will set the elements but not increment the vec: like presetting it
 * if it is greater than allocation length (as in it wants to write in unallocated memory)
 * it will reserve more space and then do the above behavior
 * @param elementlen how many elements to copy from data
 * @param data a non-null pointer to pull the new elements from
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecSetElementsData(
    const cvec vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const data
);

/**
 * @brief says if the given cvec is valid
 * @param vec a non-null pointer to the vec
 * @return int bool
 * 1 is valid, 0 if invalid
 */
int cvecIsValid(const cvec* const vec);

/**
 * @brief prints out the given vec
 * @param vec a non-null pointer to the vec
 * @return cvecstatus
 * if !=0, then failure
 */
cvecstatus cvecPrint(const cvec* const vec);

/**
 * @brief Duplicated the given cvec and returns a new one
 * it will inherit everything the old cvec had
 * NOTE: this is a new cvec and as such must be destroyed 
 * when you are done with it. It is a full copy, and has no
 * relation to the vec it took from 
 * (no dangling pointers if that vec it took from is destroyed),
 * however it does take the output streams,reallocators, and other such
 * so those could become dangling pointers if you were to destroy them aswell
 * @param vec a non-null pointer to the vec
 * @return a pointer to the cvec
 * if NULL failure,
 * else success
 */
const cvec cvecCreateDuplicate(const cvec vec);

#if cvec_BUILD_CVECSTR
#include "cvecstr.h"
#endif // cvec_BUILD_CVECSTR

#endif // PUBLIC_CVEC_CVEC_H_
