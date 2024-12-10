#ifndef CVEC_CVECSTR_CVECSTR_H_
#define CVEC_CVECSTR_CVECSTR_H_

#include "cvec.h"

cvecstatus cvecstrInsert(
    cvec* const cvecstr,
    const cveclen elementindex,
    char* string,
    const cveclen stringlen
);

cvecstatus cvecstrPrint(const cvec* const cvecstr);

cvecstatus cvecstrReplace(
    cvec* const cvecstr,
    const cveclen elementindex,
    const cveclen elementspan,
    char* string,
    const cveclen stringlen
);
#endif // CVEC_CVECSTR_CVECSTR_H_
