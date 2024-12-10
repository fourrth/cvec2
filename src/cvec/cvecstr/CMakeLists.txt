set(cvecstr_src
    "cvecstr.h"
    "cvecstr.c"
)
list(TRANSFORM cvecstr_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(cvecstr_public_src
    "cvecstr.h"
)
