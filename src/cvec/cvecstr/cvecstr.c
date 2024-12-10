#include "cvecstr.h"
#include <string.h>

#define LOGGING_PROBLEM_HOLDER_FILE cvecstr->errorprintoutputstream
#include "simpleheaders/Logging/Logging.h"

cvecstatus cvecstrInsert(
    cvec* const cvecstr,
    const cveclen elementindex,
    char* string,
    cveclen stringlen
)
{
    NULLCHECKER(if(!cvecstr)
	{
	    return 1;
    })
    if(cvecstr->elementsize != sizeof(char))
	{
	    PRINTF_ERROR_INFO(
		"elementsize must == sizeof(char), or %zu", sizeof(char)
	    );
	    return 1;
    }
    if(stringlen == 0) stringlen = strlen(string);

    return (cvecInsert(cvecstr, elementindex, string, stringlen)); //
}

cvecstatus cvecstrPrint(const cvec* const cvecstr)
{
    NULLCHECKER(if(!cvecstr)
	{
	    return 1;
    }
    if(cvecstr->Printer == NULL)
	{
	    return 1;
    })
    return (printvecCvec(cvecstr, "", "", "", 1));
}

cvecstatus cvecstrReplace(
    cvec* const cvecstr,
    const cveclen elementindex,
    const cveclen elementspan,
    char* string,
    cveclen stringlen
)
{
    NULLCHECKER(if(!cvecstr)
	{
	    return 1;
    })
    if(cvecstr->elementsize != sizeof(char))
	{
	    PRINTF_ERROR_INFO(
		"elmentsize must == sizeof(char), or %zu", sizeof(char)
	    );
	    return 1;
    }
    if(!string)
	{
	    PRINTF_ERROR_INFO("string cannot be NULL");
	    return 1;
    }
    if(stringlen == 0) stringlen = strlen(string);
    cveclen offset = 0;
    // should we remove of should we just go ahead and insert
    if(elementindex >= cvecstr->elementlen || elementspan == 0)
	{ // span does not matter as it is all empty space
	mylabel:
	    return (cvecstrInsert(
		cvecstr,
		elementindex + offset,
		string + offset,
		stringlen - offset
	    ));

    } else
	{ // we need to overwrite, then insert
	    if(elementindex + elementspan > cvecstr->elementlen)
		{
		    cvecSetElementsData(
			cvecstr,
			elementindex,
			cvecstr->elementlen - elementindex,
			string
		    );
		    offset = cvecstr->elementlen - elementindex;
	    } else
		{
		    cvecSetElementsData(
			cvecstr, elementindex, elementspan, string
		    );
		    offset = elementspan;
		}
	    goto mylabel;
	}
}
