#include "cvec.h"
#define LOGGING_PROBLEM_HOLDER_FILE vec->errorprintoutputstream
#include "simpleheaders/Logging/Logging.h"

int cvecIsValid(const cvec* const vec)
{
    if(!vec) return 0;
    else if((!vec->data) || (!vec->Deallocator) || (!vec->Reallocator) || (!vec->elementsize) || (!vec->elementallocationlen))
	return 0;
    else return 1;
}

cveclen cvecGetElementlen(const cvec* const vec)
{
	NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL, returning 0\n");
	    return 0;
    })
    return vec->elementlen; //
}

cveclen cvecGetElementsize(const cvec* const vec)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL, returning 0\n");
	    return 0;
    })
    return vec->elementsize; //
}

cveclen cvecGetElementAllocationlength(const cvec* const vec)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL, returning 0\n");
	    return 0;
    })
    return vec->elementallocationlen;
}

void* const cvecGetElement(const cvec* const vec, const cveclen elementindex)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL\n");
	    return NULL;
    })
    if(!(elementindex < vec->elementlen))
	{
	    PRINTF_ERROR_INFO(
		"elementindex must be in bounds. given: " cveclen_FMT
		", should be less than " cveclen_FMT "\n",
		elementindex,
		vec->elementlen
	    );
	    return NULL;
    }
    return cvecGetElement_unsafe(vec, elementindex);
}

cvecstatus cvecCopyElements(
    const cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const buffer
)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL");
	    return 1;
    })
    if(!(elementindex < vec->elementlen))
	{
	    PRINTF_ERROR_INFO(
		"elementindex out of bounds. given: " cveclen_FMT
		", should be less than " cveclen_FMT "\n",
		elementindex,
		vec->elementlen
	    );
	    return 1;
    }
    if(!(elementindex + elementlen < vec->elementlen))
	{
	    PRINTF_ERROR_INFO(
		"tried to copy out of bounds. tried to copy "
		"[index: { " cveclen_FMT " },{ " cveclen_FMT " })\n",
		elementindex,
		elementindex + elementlen
	    );
    }
    memcpy(
	buffer,
	cvecGetElement_unsafe(vec, elementindex),
	elementlen * vec->elementsize
    );
    return 0;
}

#pragma region // aliases

void* const cvecGetLastElement(const cvec* const vec)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_INFO_INFO("vec cannot be NULL\n");
	    return NULL;
    })
    return (cvecGetElement_unsafe(vec, cvecGetElementlen(vec) - 1)); //
}

cvecstatus cvecPop(cvec* const vec, const cveclen elementlen)
{
    return (cvecRemove(vec, cvecGetElementlen(vec) - 1, elementlen));
}

cvecstatus cvecPush(cvec* const vec, void* const data, const cveclen elementlen)
{
    return (cvecInsert(vec, cvecGetElementlen(vec), data, elementlen));
}

#pragma endregion // aliases
