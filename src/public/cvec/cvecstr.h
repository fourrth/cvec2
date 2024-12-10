#ifndef PUBLIC_CVEC_CVECSTR_H_
#define PUBLIC_CVEC_CVECSTR_H_

#include "cvec.h"

/**
 * @brief inserts a char* into the vec
 * carries most of the same semantices as cvecInsert(),
 * but is meant for cstr
 * @param vec a non-null pointer to the vec
 * note that the vec must have an element size of sizeof(char)
 * and will fail if not
 * @param elementindex index to insert at
 * @param string a non-null string to copy
 * @param stringlen the length of the string to copy
 * if 0, then assumes that the string is null-terminated
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecstrInsert(
    const cvec vec,
    const cveclen elementindex,
    char* string,
    cveclen stringlen
);

/**
 * @brief prints the cvec as a string using provided printer
 * @param vec a non-null pointer to the vec
 * this function does not require the elementsize to be sizeof(char)
 * @return cvecstatus
 * if != 0, then failure
 * else success
 */
cvecstatus cvecstrPrint(const cvec vec);

/**
 * @brief Replaces a section of the vec with the specified string, 
 * then pushes the vec to fit the rest
 * 
 * works just like if you removed the section that is [elementindex,elementindex+elementspan),
 * then inserted on elementindex
 * @param cvecstr a non-null pointer to the vec
 * this function requires the elementsize to be sizeof(char)
 * @param elementindex the point to insert at
 * if elementindex is past the length of the vec, 
 * the behavior is exactly cvecInsert(cvecstr,elementindex,string,stringlen)
 * @param elementspan the span to 'remove' from the vec
 * for the abovementioned case, the span will not do anything
 * as there is nothing to remove: it's is all empty space

 * elementspan is directly link to how much to remove,
 * so an elementspan of 0 is equivalent to just inserting at the index,
 * like the above shown example function call
 * @param string non-null string to be put into the vec
 * @param stringlen the length of the string
 * if 0, then assumes it is null-terminated
 * @return cvecstatus
 * if !=0, then failure
 * else success
 */
cvecstatus cvecstrReplace(
    const cvec cvecstr,
    const cveclen elementindex,
    const cveclen elementspan,
    char* string,
    const cveclen stringlen
);
#endif // PUBLIC_CVEC_CVECSTR_H_
