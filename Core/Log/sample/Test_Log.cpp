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
#include "../inc/Log.h"
#include "../../../Common/LibTest/inc/LibTest.h"

using namespace Collections;

LIB_TEST_CASE (0, "multiple log instances") {
    LOG_INIT (0, Log::INFO, Log::TO_CONSOLE);
    LOG_INFO (0) << "Hello World! " << "This is a test message. " << 123 << "," << 10.1010 << std::endl; 
    LOG_CLOSE (0);

    LOG_INIT (0, Log::WARNING, Log::TO_CONSOLE);
    LOG_WARNING (0) << "Hello World! " << "This is a test message. " << 123 << "," << 10.1010 << std::endl;

    LOG_INIT (1, Log::VERBOSE, Log::TO_CONSOLE);
    LOG_WARNING (1) << "Hello World! " << "This is a test message. " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (1) << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_CLOSE (0);
    LOG_CLOSE (1);

    return Quality::PASS;
}

LIB_TEST_CASE (1, "filter log level test") {
    // there are a total of 5 log levels
    const int instances[] = { 2, 3, 4, 5, 6 };

    LOG_INIT (2, Log::NONE, Log::TO_FILE_IMMEDIATE);
    LOG_INIT (3, Log::INFO, Log::TO_FILE_IMMEDIATE);
    LOG_INIT (4, Log::WARNING, Log::TO_FILE_IMMEDIATE);
    LOG_INIT (5, Log::ERROR, Log::TO_FILE_IMMEDIATE);
    LOG_INIT (6, Log::VERBOSE, Log::TO_FILE_IMMEDIATE);

    for (auto i : instances) {
        LOG_INFO (i)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
        LOG_WARNING (i) << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
        LOG_ERROR (i)   << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    }

    LOG_CLOSE_ALL;
    return Quality::PASS;
}

LIB_TEST_CASE (2, "sink test") {
    // there are a total of 3 log sinks
    const int instances[] = { 7, 8, 9 };

    LOG_INIT (7, Log::VERBOSE, Log::TO_FILE_IMMEDIATE);
    LOG_INIT (8, Log::VERBOSE, Log::TO_CONSOLE);
    // you need to specify buffer capacity for circular buffer sink
    LOG_INIT (9, Log::VERBOSE, Log::TO_FILE_BUFFER_CIRCULAR, 10);

    for (auto i : instances) {
        int numMessages = 15;
        while (numMessages != 0) {
            LOG_WARNING (i) << "My name is " << "John Adams " << numMessages << "," << 10.1010 << std::endl;    
            numMessages--;
        }
    }

    LOG_CLOSE_ALL;
    return Quality::PASS;
}

LIB_TEST_CASE (3, "conditional calls") {
    LOG_INIT (10, Log::ERROR, Log::TO_CONSOLE);
    int count = 0;

    // this code block executes as an info message
    IF_LOG (10, Log::INFO) {
        for (int i = 0; i < 10; i++)
            count++;
    }

    IF_NOT_LOG (10, Log::WARNING) {
        for (int i = 0; i < 10; i++)
            count += 2;
    }
    
    LOG_ERROR (10) << count << std::endl;
    LOG_CLOSE (10);
    return Quality::PASS;
}

LIB_TEST_CASE (4, "mgr dump") {
    LOG_INIT (11, Log::VERBOSE, Log::TO_CONSOLE);
    LOG_INIT (12, Log::INFO, Log::TO_FILE_BUFFER_CIRCULAR, 100);

    LOG_MGR_DUMP;

    LOG_CLOSE (11);
    LOG_CLOSE (12);
    return Quality::PASS;
}

LIB_TEST_CASE (5, "special messages") {
    LOG_INIT (13, Log::VERBOSE, Log::TO_FILE_IMMEDIATE | Log::TO_CONSOLE);
  
    int input_0 = 1000;
    char input_1 = 'a';
    float input_2 = 10.234;
    std::string input_3 = "John Adams";        
    float *ptr = &input_2;

    LOG_INFO (13) << input_0 << " " << input_1 << std::endl;
    LOG_INFO (13) << input_2 << " " << input_3 << " " << *ptr << std::endl;

    LOG_WARNING (13) << "test format specifiers like \\n \\t etc." 
                     << "\t\t\t" 
                     << "does it work?"
                     << "\n" 
                     << "guess it does!"
                     << std::endl;

    LOG_INIT (14, Log::VERBOSE, Log::TO_FILE_IMMEDIATE | Log::TO_FILE_BUFFER_CIRCULAR | Log::TO_CONSOLE, 5);
    /* for all sinks except circular buffer, when you omit std::endl at the end of a log message and start another message
     * it is considered as different log entries (indicated by headers that are appended to the beginning of each message)
     * evn though they are not on the same line.
    */
    LOG_ERROR (14) << "testing multi line logs without std::endl";
    LOG_ERROR (14) << "even if i use \\n" << "\n";
    LOG_ERROR (14) << "I am still a different message even though we are on the same line" << std::endl;

    /* for circular buffered sinks, std::endl marks the end of a single entry into the buffer
    */
    int count = 0;
    for (int i = 0; i < 5; i++) {
        count++;
        LOG_INFO (14) << "count: " << count << std::endl;
    }

    LOG_CLOSE (13);
    LOG_CLOSE (14);
    return Quality::PASS;
}

int main(void) {
    LIB_TEST_INIT (Quality::TO_CONSOLE | Quality::TO_FILE);

    LIB_TEST_RUN_ALL;
    return 0;
}