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
    LOG_INIT (0, Quality::Log::INFO, Quality::Log::TO_CONSOLE);
    auto myLog = GET_LOG (0);

    LOG_INFO (myLog) << "Hello World! " 
                     << "This is a test message. " 
                     << 123 
                     << "," 
                     << 10.1010 
                     << std::endl; 
    LOG_CLOSE (0);

    LOG_INIT (0, Quality::Log::WARNING, Quality::Log::TO_CONSOLE);
    auto myAnotherLog = GET_LOG (0);

    LOG_WARNING (myAnotherLog) << "Hello World! " 
                               << "This is a test message. " 
                               << 123 
                               << "," 
                               << 10.1010 
                               << std::endl;

    LOG_INIT (1, Quality::Log::VERBOSE, Quality::Log::TO_CONSOLE);
    auto myDifferentLog = GET_LOG (1);

    LOG_WARNING (myDifferentLog) << "Hello World! " 
                                 << "This is a test message. " 
                                 << 123 
                                 << "," 
                                 << 10.1010 
                                 << std::endl;
    LOG_WARNING (myDifferentLog) << "My name is " 
                                 << "John Adams " 
                                 << 123 
                                 << "," 
                                 << 10.1010 
                                 << std::endl;
    LOG_CLOSE (0);
    LOG_CLOSE (1);

    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "filter log level test") {
    LOG_INIT (2, Quality::Log::NONE, Quality::Log::TO_FILE_IMMEDIATE);
    LOG_INIT (3, Quality::Log::INFO, Quality::Log::TO_FILE_IMMEDIATE);
    LOG_INIT (4, Quality::Log::WARNING, Quality::Log::TO_FILE_IMMEDIATE);
    LOG_INIT (5, Quality::Log::ERROR, Quality::Log::TO_FILE_IMMEDIATE);
    LOG_INIT (6, Quality::Log::VERBOSE, Quality::Log::TO_FILE_IMMEDIATE);

    auto myLog2 = GET_LOG (2);
    auto myLog3 = GET_LOG (3);
    auto myLog4 = GET_LOG (4);
    auto myLog5 = GET_LOG (5);
    auto myLog6 = GET_LOG (6);

    LOG_INFO    (myLog2)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (myLog2)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_ERROR   (myLog2)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_INFO    (myLog3)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (myLog3)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_ERROR   (myLog3)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_INFO    (myLog4)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (myLog4)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_ERROR   (myLog4)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_INFO    (myLog5)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (myLog5)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_ERROR   (myLog5)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_INFO    (myLog6)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_WARNING (myLog6)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;
    LOG_ERROR   (myLog6)    << "My name is " << "John Adams " << 123 << "," << 10.1010 << std::endl;

    LOG_MGR_DUMP;
    LOG_CLOSE_ALL;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "sink test") {
    LOG_INIT (7, Quality::Log::VERBOSE, Quality::Log::TO_FILE_IMMEDIATE);
    LOG_INIT (8, Quality::Log::VERBOSE, Quality::Log::TO_CONSOLE);
    // you need to specify buffer capacity for circular buffer sink
    LOG_INIT (9, Quality::Log::VERBOSE, Quality::Log::TO_FILE_BUFFER_CIRCULAR, 10);

    auto myLog7 = GET_LOG (7);
    auto myLog8 = GET_LOG (8);
    auto myLog9 = GET_LOG (9);

    int numMessages = 15;
    while (numMessages != 0) {
        LOG_WARNING (myLog7) << "My name is " << "John Adams " << numMessages << "," << 10.1010 << std::endl;  
        LOG_WARNING (myLog8) << "My name is " << "John Adams " << numMessages << "," << 10.1010 << std::endl;  
        LOG_WARNING (myLog9) << "My name is " << "John Adams " << numMessages << "," << 10.1010 << std::endl;  

        numMessages--;
    }

    LOG_CLOSE_ALL;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "mgr dump") {
    LOG_INIT (10, Quality::Log::VERBOSE, Quality::Log::TO_CONSOLE);
    LOG_INIT (11, Quality::Log::INFO, Quality::Log::TO_FILE_BUFFER_CIRCULAR, 100);

    LOG_MGR_DUMP;

    LOG_CLOSE (10);
    LOG_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "special messages") {
    LOG_INIT (12, Quality::Log::VERBOSE, Quality::Log::TO_FILE_IMMEDIATE | Quality::Log::TO_CONSOLE);
    auto myLog = GET_LOG (12);
  
    int input_0 = 1000;
    char input_1 = 'a';
    float input_2 = 10.234;
    std::string input_3 = "John Adams";        
    float *ptr = &input_2;

    LOG_INFO (myLog) << input_0 << " " << input_1 << std::endl;
    LOG_INFO (myLog) << input_2 << " " << input_3 << " " << *ptr << std::endl;

    LOG_WARNING (myLog) << "test format specifiers like \\n \\t etc." 
                        << "\t\t\t" 
                        << "does it work?"
                        << "\n" 
                        << "guess it does!"
                        << std::endl;

    LOG_INIT (13, Quality::Log::VERBOSE, 
              Quality::Log::TO_FILE_IMMEDIATE | 
              Quality::Log::TO_FILE_BUFFER_CIRCULAR | 
              Quality::Log::TO_CONSOLE, 5);
    auto mySecondLog = GET_LOG (13);

    /* for all sinks except circular buffer, when you omit std::endl at the end of a log message and start another message
     * it is considered as different log entries (indicated by headers that are appended to the beginning of each message)
     * evn though they are not on the same line.
    */
    LOG_ERROR (mySecondLog) << "testing multi line logs without std::endl";
    LOG_ERROR (mySecondLog) << "even if i use \\n" << "\n";
    LOG_ERROR (mySecondLog) << "I am still a different message even though we are on the same line" << std::endl;

    // for circular buffered sinks, std::endl marks the end of a single entry into the buffer
    int count = 0;
    for (int i = 0; i < 5; i++) {
        count++;
        LOG_INFO (mySecondLog) << "count: " << count << std::endl;
    }

    LOG_CLOSE (12);
    LOG_CLOSE (13);
    return Quality::Test::PASS;
}

int main(void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);

    LIB_TEST_RUN_ALL;
    return 0;
}