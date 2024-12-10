#include "cvec/cvec.h"

static inline int charprinter(void* element, FILE* stream)
{
    return (fprintf(stream, "%c", *(char*)element));
}

int main()
{
    cvec cvecstr = cvecCreate(0, 1, NULL, NULL, charprinter, NULL, NULL);
    char teststr[] = "Jackson";
    cvecstrInsert(cvecstr, cvecGetElementlen(cvecstr), "Hello Alex!!", 0);
    cvecstrPrint(cvecstr);
    putchar('\n');
    cvecstrReplace(cvecstr, 6, 4, teststr, 0);
    cvecstrPrint(cvecstr);
    putchar('\n');
    // expected result: "Hello Jackson!!"
    cvecDestory(cvecstr);
    return 0;
}
