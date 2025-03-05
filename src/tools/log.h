#ifndef _TOOLS_LOG_H_
#define _TOOLS_LOG_H_

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KWHT  "\x1B[37m"

#include <iostream>
#include <string>

using namespace std;

namespace parcel {
    namespace tools {
        
        static class Logger {
            private:
            void write(const char* colorPrefix, const char* msg) {
                cout << colorPrefix << msg << "\033[0m" << endl;
            };

            public:
            void Link(const char* msg) {write(KBLU, msg);};
            void Info(const char* msg) {write(KWHT, msg);};
            void Error(const char* msg) {write(KRED, msg);};
            void Warning(const char* msg) {write(KYEL, msg);};
        } Log;
    }
}

#endif