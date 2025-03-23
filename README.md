parcel - custom language for parsing texts, data.



### Build (CMAKE)
install tests/googletest:
1. cd tests
2. run `git clone https://github.com/google/googletest.git`

main build (in root)
1. mkdir build
2. cmake ..
3. run `./tests/core`


features:
    hooks
    funcs
    or/and value
    if
    data.to()
    multithreading

- vardefs
- hooks
- build-in funcs
- MT
- lang


goals:
- lexing..
- lex_graph
- parser_graph
- value_graph
- MT
