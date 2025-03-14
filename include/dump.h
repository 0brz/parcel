#ifndef _TOOLS_DUMP_H_
#define _TOOLS_DUMP_H_

#include "parcel.h"

// dump_hook
namespace parcel {
    namespace dump {
        //void lextable(offset_table<LinkedLex *> *table);
        void hook(parcel::tokens::token *h);
    }
}

#endif