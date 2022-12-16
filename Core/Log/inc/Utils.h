/*
 Copyright 2022, Author: VIJOY SUNIL KUMAR
 
 All rights reserved. No part of this source code may be reproduced or distributed by any means without prior written permission of
 the copyright owner. It is strictly prohibited to publish any parts of the source code to publicly accessible repositories or
 websites. The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include "Record.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Collections {
namespace Log {
namespace Utils {
    std::string levelToString (e_level level) {
        std::string result;
        switch (level) {
            case INFO:
                result = "INFO";
                break;

            case WARNING:
                result = "WARN";
                break;

            case ERROR:
                result = "ERRO";
                break;

            default:
                result = "UNDF";
                break;
        }
        return result;
    }

    std::string getLocalTimestamp (void) {
        /* the string stream associates a string object with a string. Using this we can read from string as if it were a 
         * stream like cin
        */
        std::stringstream stream;
        
        std::chrono::time_point <std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::time_t t_c = std::chrono::system_clock::to_time_t (now);
        // https://en.cppreference.com/w/cpp/io/manip/put_time
        stream << std::put_time (std::localtime (&t_c), "%F %T");
        return stream.str();
    }

    inline bool filter (Record* c_record, e_level level) {
        return c_record->getLevel() & level;
    }

    std::string getHeader (size_t instanceId, 
                           e_level level, 
                           const char* file, const char* function, const size_t line) {

        std::string header = "[ " + std::to_string (instanceId) + " ]" + " " +
                             getLocalTimestamp() + " " +
                             "[ " + levelToString (level) + " ]" + " " +
                             file + " " +
                             function + " " +
                             std::to_string (line) +  " ";

        return header;
    }
}   // namespace Utils
}   // namespace Log
}   // namespace Collections
#endif  // LOG_UTILS_H