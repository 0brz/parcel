#ifndef _PARCEL_CALLBACKS_
#define _PARCEL_CALLBACKS_

#include <string>

namespace parcel {
    typedef void(*build_callback)(bool is_ok, std::string error);
};

#endif