#include "cvec.h"

#define LOGGING_PROBLEM_HOLDER_FILE vec->errorprintoutputstream
#include "simpleheaders/Logging/Logging.h"

#define CVEC_DEFAULT_VEC_ALLOCATION_SIZE 10
#define CVEC_DEFAULT_VEC_ALLOCATION_INC 10

cvec* const cvecCreate(
    const cveclen elementallocationlen,
    const cveclen elementsize,
    const cvecReallocatorFunction Reallocator,
    const cvecDeallocatorFunction Deallocator,
    const cvecPrintelementFunction Printer,
    FILE* const outputstream,
    FILE* const erroroutputstream
)
{

    cvec* vec = malloc(sizeof(cvec));
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO_SYS("Could not malloc for vec");
	    return NULL;
    })

    if(!Printer)
	{ // means that the user will not want to print this vector
	    vec->Printer = NULL;
	    vec->printoutputstream =
		NULL; // this makes the simpleheader logging stop doing its thing
	    vec->errorprintoutputstream = NULL;
    } else
	{
	    vec->Printer = Printer;
	    if(!outputstream)
		{
		    vec->printoutputstream = stdout;
	    } else vec->printoutputstream = outputstream;
	    if(!erroroutputstream)
		{
		    vec->errorprintoutputstream = stderr;
	    } else vec->errorprintoutputstream = erroroutputstream;
	    // else it is good and should be a valid stream for as long as the vec will live(until vec destroy)
	}

    if(elementsize == 0)
	{
	    PRINTF_ERROR_INFO("elementsize cannot equal 0\n");
	    return NULL;
    }

    if(Reallocator && Deallocator)
	{
	    vec->Reallocator = Reallocator;
	    vec->Deallocator = Deallocator;
    } else
	{
	    if(!Reallocator && Deallocator)
		{
		    PRINTF_WARNING_INFO(
			"Invalid Reallocator, but valid Deallocator: "
			"continuing with std realloc and std free\n"
		    );
	    } else if(Reallocator && !Deallocator)
		{
		    PRINTF_WARNING_INFO(
			"Valid Reallocator, but invalid Deallocator: "
			"continuing with std realloc and std free\n"
		    );
	    }

	    // if both invalid, dont print anything

	    vec->Reallocator = (cvecReallocatorFunction)(&realloc);
	    vec->Deallocator = (cvecDeallocatorFunction)(&free);
	}

    const cveclen elementallocationlen_ =
	(elementallocationlen == 0 ? CVEC_DEFAULT_VEC_ALLOCATION_SIZE
                                   : elementallocationlen);
    void* const data =
	(*vec->Reallocator)(NULL, elementallocationlen * elementsize);
    // calling realloc with NULL as ptr is = to malloc(size)
    if(!data)
	{
	    PRINTF_ERROR_INFO_SYS("Could not allocate memory for data");
	    return NULL;
    }

    vec->data = data;
    vec->elementallocationlen = elementallocationlen_;
    vec->elementlen = 0;
    vec->elementsize = elementsize;

    return vec;
}

cvecstatus cvecDestory(cvec* const vec)
{
	// Not with NULLCHECKER because this shouldn't
	// ever be that high impact, and if it is then
	// you are 100% doing it in such a
	// way that branch prediction 
	// (or even simple compiler optimization)
	// will fix it...
    if(!cvecIsValid(vec))
	{
	    PRINTF_ERROR_INFO("Given invalid vec\n");
	    return 1;
    }

    (*vec->Deallocator)(vec->data);

    vec->elementlen = 0;
    vec->elementsize = 0;
    vec->data = NULL;
    vec->Deallocator = NULL;
    vec->Reallocator = NULL;
    vec->elementallocationlen = 0;

    free(vec);
    return 0;
}

cvecstatus cvecInsert(
    cvec* const vec,
    const cveclen elementindex,
    void* const data,
    const cveclen elementlen
)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL");
	    return 1;
    })

    {
	const cveclen ret = CheckIfEnoughSpaceCvec(vec, elementlen);
	if(ret != 0)
	    { // we need to increase vec length by ret elements
		if(cvecReserve(vec, ret))
		    {
			PRINTF_ERROR_INFO_SYS(
			    "could not reserve more space for vec. wanted "
			    "{" cveclen_FMT "} more elements, or " cveclen_FMT
			    " bytes\n",
			    elementlen,
			    vec->elementsize * elementlen
			);
			return 1;
		}
	}
    }

    if(elementindex == vec->elementlen)
	{
	    memcpy(
		cvecGetElement_unsafe(vec, elementindex),
		data,
		vec->elementsize * elementlen
	    );
	    vec->elementlen += elementlen;
	    return 0;
    }
    if(elementindex > vec->elementlen)
	{
	    // === === === === --- --- 3--- ---
	    // +++ +++ +++ +++
	    memset(
		cvecGetElement_unsafe(vec, vec->elementlen),
		0,
		vec->elementsize * (elementindex - vec->elementlen)
	    );
	    vec->elementlen = elementindex;
	    cvecInsert(vec, vec->elementlen, data, elementlen);
	    return 0;
    }

    // === === === === 3=== === === -------
    // +++ +++ +++ +++

    // === === === === 3+++ +++ +++ +++ ----
    // === === ===

    // === === === === 3+++ +++ +++ +++ === === === -
    //
    // for now, if we need to insert something in the middle of the vec,
    // we do the simple option, which is to copy all of the data above it,
    // put the data we need in, then copy it all to the end
    void* const startptr = cvecGetElement_unsafe(vec, elementindex);
    void* const tmpdata =
	malloc((vec->elementlen - elementindex) * vec->elementsize);
    // ? do we always need to malloc, or can we figure out some portable stack push thing
	// ? for low (like <256 or some <MAX_STACK_ALLOC) sized vecs
    if(!tmpdata)
	{
	    PRINTF_ERROR_INFO_SYS("could not malloc for temporary buffer");
	    return 1;
    }

    memcpy(
	tmpdata, startptr, (vec->elementlen - elementindex) * vec->elementsize
    );
    memcpy(startptr, data, elementlen * vec->elementsize);
    const cveclen oldlen = vec->elementlen;
    vec->elementlen = elementindex + elementlen;
    cvecInsert(vec, vec->elementlen, tmpdata, oldlen - elementindex);
    free(tmpdata);

    return 0;
}

cvecstatus cvecRemove(
    cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen
)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL\n");
	    return 1;
    })
    if(!(elementindex < vec->elementlen))
	{
	    PRINTF_ERROR_INFO(
		"given index out of bounds. given: { " cveclen_FMT
		" }, should be less than { " cveclen_FMT " }\n",
		elementindex,
		vec->elementlen
	    );
	    return 1;
    }
    if(!(elementindex + elementlen <= vec->elementlen))
	{
	    PRINTF_ERROR_INFO(
		"tried to copy out of bounds. tried to copy "
		"[index: { " cveclen_FMT " },{ " cveclen_FMT " })\n",
		elementindex,
		elementindex + elementlen
	    );
	    return 1;
    }

    if(elementindex == vec->elementlen)
	{
	    vec->elementlen -= elementlen;
	    return 0;
    }
    // === === === 3=== ===| === === ===
    // === === === 3xxx xxx === === ===
    cveclen index_offset = 0;
    while(index_offset < vec->elementlen - (elementindex + elementlen))
	{ // should probably be replaced with memmove
	    memcpy(
		cvecGetElement(vec, elementindex + index_offset),
		cvecGetElement(vec, elementindex + index_offset + elementlen),
		vec->elementsize
	    );
	    index_offset++;
	}
    vec->elementlen -= elementlen;
    return 0;
}

cvecstatus cvecReserve(cvec* const vec, const cveclen elementlen)
{
    if(elementlen == 0)
	{
	    return 0;
    }
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL\n");
	    return 1;
    })

    const cveclen wanted_allocation_size =
	vec->elementallocationlen + elementlen * vec->elementsize;
    void* const ndata = (*vec->Reallocator)(
	vec->data,
	wanted_allocation_size +
	    CVEC_DEFAULT_VEC_ALLOCATION_INC * vec->elementsize
    );
    if(!ndata)
	{
	    PRINTF_ERROR_INFO_SYS("Could not reallocate");
	    return 1;
    }
    vec->data = ndata;
    vec->elementallocationlen = wanted_allocation_size;
    return 0;
}

cvecstatus cvecPrint(const cvec* const vec)
{
    NULLCHECKER(if(!vec)
	{
	    PRINTF_ERROR_INFO("vec cannot be NULL\n");
	    return 1;
    }
	// this is probably easier to mess up and might be a little hard to 
	// discern that there just isn't a printer set up, but
	// that's the fun you get when turning the option off
    if(vec->Printer == NULL)
	{ // if Printer is NULL, then we cannot print anything
	    return 1;
    })
    return (printvecCvec(vec, "{ ", " }\n", ", ", 1));
}

cvecstatus cvecSetElementsData(
    cvec* const vec,
    const cveclen elementindex,
    const cveclen elementlen,
    void* const data
)
{
    NULLCHECKER(if(!vec)
	{
	    return 1;
    })

    if(elementindex + elementlen > vec->elementallocationlen)
	{
	    if(!cvecReserve(vec, elementindex + elementlen)) return 1;
    }
    memcpy(
	cvecGetElement_unsafe(vec, elementindex),
	data,
	elementlen * vec->elementsize
    );
    return 0;
}

cvec* const cvecCreateDuplicate(const cvec* const vec)
{
    cvec* const retvec = cvecCreate(
	vec->elementallocationlen,
	vec->elementsize,
	vec->Reallocator,
	vec->Deallocator,
	vec->Printer,
	vec->printoutputstream,
	vec->errorprintoutputstream
    );
    if(!retvec) return NULL;
    if(cvecPush(retvec, vec->data, vec->elementlen) != 0)
	{
	    cvecDestory(retvec);
	    return NULL;
    }
    return retvec;
}
