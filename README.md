# parcel (parsing language lib)
parcel is a statically typed language inside cpp for describing the structure and rules of source data parsing.

## Language keywords & features

#### user type defs (links):
- `@sample_link: ...` - describes the type for further reuse

#### getting data (hooks):
- `&sample_hook: ...` - after parsing, the user can see what data was received. All accepted tokens stored in hooks. User can see them.

#### non-sequence collections:
- `set` - an accumulate collection that accepts one of the subtypes
- `list` - an accumulate collection that accepts only one subtype
#### sequence collections:
- sequence collection describes many subtypes (elements) for sequential build deep sturcture. When all the items inside the collection are built, this structure accepts.
- `vector` - it not contain any other subtypes when builds. Build-progress move to begining after first fail subtype build.
- `seq` - (sequence) could contains any other subtypes when builds. Build-progress is accumulating when parsing.

#### basetypes:
- `word` - accept an word (letters only) like 'sun', 'weather'
- `id` - accept an id (letters + '_' + '-' + nums) like 'bar_100', '__foo'
- `num` - numeric (now, int only)
- `char` - any single char

#### literals:
- `int/float` like 123.50, 1240
- `char` like 'a', 'B'
- `string` like "good weather", "Roboto92<<", "@me--"

## Build (cmake)
1. cloning. `git clone https://github.com/0brz/parcel.git`
2. use root `CMakeLists.txt` to build/link parcellib in your project. 
3. you can build with no tests
4. use public api (parcel/include) in your project. Main is `#include <parcel.h>`
 

## Build/check tests (cmake)
Build (CMAKE)
install tests/googletest:
- `cd tests`
- run `git clone https://github.com/google/googletest.git`

main build (in root)
- `mkdir build`
- `cmake ..`
- run `./tests/core`