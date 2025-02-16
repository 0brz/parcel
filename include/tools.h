#ifdef PARCEL_DEV

#include "src/tools/lexer/lex.h"
#include "src/tools/logger.h"

#endif

#include "src/tools/logger.h"

#ifndef PARCEL_BUILD

int tols_build()
{
    return 5;
}

#endif
