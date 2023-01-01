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
#include "../inc/Buffer.h"
// import lib test
#include "../../LibTest/inc/LibTest.h"
using namespace Collections;

LIB_TEST_CASE (0, "int type w/ no overflow") {
    int input[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    /* since this is a no overflow test case, with capacity = 5, the buffer contents will be 10 ... 6 even if we iterate
     * through the full input[] array, and the push operation should return without fail
    */
    size_t capacity = 5;
    int output[] = { 10, 9, 8, 7, 6 };

    // init buffer and get instance
    auto myBuffer = BUFFER_INIT (0, Memory::WITHOUT_OVERFLOW, int, capacity);
    // or use GET_ method to get instance
    // auto myBuffer = GET_BUFFER (0, int);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    // pop contents
    for (auto i : output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (0);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "float type w/ no overflow") {
    float input[] = { 10.1, 9.2, 8.3, 7.4, 6.5, 5.6, 4.7, 3.8, 2.9, 1.10 };

    size_t capacity = 5;
    float output[] = { 10.1, 9.2, 8.3, 7.4, 6.5 };

    auto myBuffer = BUFFER_INIT (1, Memory::WITHOUT_OVERFLOW, float, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    for (auto i : output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "string type w/ no overflow") {
    std::string input[] = { "ab", "cd", "ef", "gh", "ij", "kl", "mn", "op", "qr", "st" };

    size_t capacity = 5;
    std::string output[] = { "ab", "cd", "ef", "gh", "ij" };

    auto myBuffer = BUFFER_INIT (2, Memory::WITHOUT_OVERFLOW, std::string, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    for (auto i : output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "char type w/ overflow") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    size_t capacity = 5;
    char output[] = { 'f', 'g', 'h', 'i', 'j' };

    auto myBuffer = BUFFER_INIT (3, Memory::WITH_OVERFLOW, char, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    for (auto i : output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (3);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "reuse instance id") {
    size_t capacity = 5;
    auto myBuffer = BUFFER_INIT (4, Memory::WITH_OVERFLOW, char, capacity); 

    myBuffer-> BUFFER_PUSH ('a');
    if (* (myBuffer-> BUFFER_POP_FIRST) != 'a')
        return Quality::Test::FAIL;

    // you are able to reuse the instance #4 for another buffer after you close it
    BUFFER_CLOSE (4);

    auto mySecondBuffer = BUFFER_INIT (4, Memory::WITHOUT_OVERFLOW, int, capacity); 

    mySecondBuffer-> BUFFER_PUSH (1010);
    if (* (mySecondBuffer-> BUFFER_POP_FIRST) != 1010)
        return Quality::Test::FAIL;

    BUFFER_CLOSE (4); 
    return Quality::Test::PASS; 
}

LIB_TEST_CASE (5, "buffer mgr dump") {
    const int capacity = 5;

    BUFFER_INIT (5, Memory::WITHOUT_OVERFLOW, int, capacity);
    BUFFER_INIT (6, Memory::WITHOUT_OVERFLOW, float, capacity);
    BUFFER_INIT (7, Memory::WITHOUT_OVERFLOW, int, capacity); 

    // mgr dump shows all opened buffer instances
    BUFFER_MGR_DUMP;

    BUFFER_CLOSE_ALL;

    BUFFER_MGR_DUMP;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (6, "buffer dump") {
    int input[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    size_t capacity = 5;
    auto myBuffer = BUFFER_INIT (8, Memory::WITH_OVERFLOW, int, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    myBuffer-> BUFFER_DUMP;

    // pop few and redump
    myBuffer-> BUFFER_POP_FIRST;
    myBuffer-> BUFFER_POP_FIRST;

    myBuffer-> BUFFER_DUMP;
    
    BUFFER_CLOSE (8);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (7, "full-fill half-flush full-fill w/ no overflow") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    size_t capacity = 6;
    char half_flush_output[] = { 'a', 'b', 'c' };
    char full_flush_output[] = { 'd', 'e', 'f', 'a', 'b', 'c' };

    // full fill
    auto myBuffer = BUFFER_INIT (9, Memory::WITHOUT_OVERFLOW, char, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    // half flush
    for (auto i : half_flush_output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    // re full fill
    for (auto i : input)
        myBuffer-> BUFFER_PUSH (i);

    // full flush
    for (auto i : full_flush_output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (9);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (8, "full-fill half-flush full-fill w/ overflow") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    size_t capacity = 6;
    char half_flush_output[] = { 'e', 'f', 'g' };
    char full_flush_output[] = { 'e', 'f', 'g', 'h', 'i', 'j' };

    // full fill
    auto myBuffer = BUFFER_INIT (10, Memory::WITH_OVERFLOW, char, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    myBuffer-> BUFFER_DUMP;

    // half flush
    for (auto i : half_flush_output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    myBuffer-> BUFFER_DUMP;

    // re full fill
    for (auto i : input)
        myBuffer-> BUFFER_PUSH (i);

    myBuffer-> BUFFER_DUMP;

    // full flush
    for (auto i : full_flush_output) {
        if (* (myBuffer-> BUFFER_POP_FIRST) != i)
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "buffer reset") {
    float input[] = { 10.1, 9.2, 8.3, 7.4, 6.5, 5.6, 4.7, 3.8, 2.9, 1.10 };

    const int capacity = 6;
    auto myBuffer = BUFFER_INIT (11, Memory::WITH_OVERFLOW, float, capacity);
    
    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    myBuffer-> BUFFER_DUMP;

    myBuffer-> BUFFER_RESET;
    myBuffer-> BUFFER_DUMP;

    BUFFER_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (10, "buffer peek") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    char output_full_fill_peek[] = { 'e', 'j' };
    char output_half_flush_peek[] = { 'h', 'j' };

    size_t capacity = 6;
    // full fill
    auto myBuffer = BUFFER_INIT (12, Memory::WITH_OVERFLOW, char, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    // peek
    if (* (myBuffer-> BUFFER_PEEK_FIRST) != output_full_fill_peek[0])
        return Quality::Test::FAIL;
    if (* (myBuffer-> BUFFER_PEEK_LAST) != output_full_fill_peek[1])
        return Quality::Test::FAIL;

    // half flush
    for (size_t i = 0; i < capacity/2; i++)
        myBuffer-> BUFFER_POP_FIRST;
    
    // peek
    if (* (myBuffer-> BUFFER_PEEK_FIRST) != output_half_flush_peek[0])
        return Quality::Test::FAIL;
    if (* (myBuffer-> BUFFER_PEEK_LAST) != output_half_flush_peek[1])
        return Quality::Test::FAIL;

    // re full fill
    for (auto i : input)
        myBuffer-> BUFFER_PUSH (i);

    // peek
    if (* (myBuffer-> BUFFER_PEEK_FIRST) != output_full_fill_peek[0])
        return Quality::Test::FAIL;
    if (* (myBuffer-> BUFFER_PEEK_LAST) != output_full_fill_peek[1])
        return Quality::Test::FAIL;

    BUFFER_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "buffer availability") {
    int input[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    size_t capacity = 6;

    size_t output_full_fill = 0;
    size_t output_half_flush = capacity/2;

    // full fill
    auto myBuffer = BUFFER_INIT (13, Memory::WITHOUT_OVERFLOW, int, capacity);

    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    if (myBuffer-> BUFFER_AVAILABILITY != output_full_fill)
        return Quality::Test::FAIL;

    // half flush
    for (size_t i = 0; i < capacity/2; i++)
        myBuffer-> BUFFER_POP_FIRST;

    if (myBuffer-> BUFFER_AVAILABILITY != output_half_flush)
        return Quality::Test::FAIL;

    // re full fill
    for (auto i : input) 
        myBuffer-> BUFFER_PUSH (i);

    if (myBuffer-> BUFFER_AVAILABILITY != output_full_fill)
        return Quality::Test::FAIL;

    BUFFER_CLOSE (13);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (12, "custom type") {
    // define custom type
    typedef struct {
        int data;
        std::string desc;
    }s_customType;

    s_customType input[] = { { 0, "zero" }, 
                             { 1, "one" }, 
                             { 2, "two" }, 
                             { 3, "three" }, 
                             { 4, "four" } };
    
    size_t capacity = 3;
    s_customType output_full_flush[] = { { 2, "two" }, 
                                         { 3, "three" }, 
                                         { 4, "four" } };

    auto myBuffer = BUFFER_INIT (14, Memory::WITH_OVERFLOW, s_customType, capacity);

    // full fill
    for (auto i : input)
        myBuffer-> BUFFER_PUSH (i);

    // define lambda function, this tells the custom dump function on how to unravel the custom type
    auto lambda_customType = [](s_customType* readPtr, std::ostream& ost) { 
                                    ost << readPtr-> data << ", " << readPtr-> desc;
                                };
    myBuffer-> BUFFER_DUMP_CUSTOM (lambda_customType);

    // full flush
    for (auto i : output_full_flush) {
        s_customType out = *(myBuffer-> BUFFER_POP_FIRST);

        if (out.data != i.data ||
            out.desc != i.desc)
            return Quality::Test::FAIL;
    }

    myBuffer-> BUFFER_DUMP_CUSTOM (lambda_customType);

    BUFFER_CLOSE (14);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (13, "buffer as stack") {
    int input[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int output_0[] = { 9, 8, 7, 6, 5 };
    int output_1[] = { 5, 4, 3, 2, 1 };
    size_t capacity = 5;

    auto myStack = BUFFER_INIT (15, Memory::WITH_OVERFLOW, int, capacity);
    for (auto i : input)
        myStack-> BUFFER_PUSH (i);

    int idx = 0;
    // a stack follows last in first out protocol
    while (myStack-> BUFFER_AVAILABILITY != capacity) {
        if (* (myStack-> BUFFER_POP_LAST) != output_0[idx++])
            return Quality::Test::FAIL;
    }

    // lets try the same with no overflow
    auto myAnotherStack = BUFFER_INIT (16, Memory::WITHOUT_OVERFLOW, int, capacity);
    for (auto i : input)
        myAnotherStack-> BUFFER_PUSH (i);

    idx = 0;
    while (myAnotherStack-> BUFFER_AVAILABILITY != capacity) {
        if (* (myAnotherStack-> BUFFER_POP_LAST) != output_1[idx++])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE_ALL;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (14, "buffer as queue") {
    int input[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int output_0[] = { 5, 6, 7, 8, 9 };
    int output_1[] = { 1, 2, 3, 4, 5 };
    size_t capacity = 5;

    auto myQueue = BUFFER_INIT (17, Memory::WITH_OVERFLOW, int, capacity);
    for (auto i : input)
        myQueue-> BUFFER_PUSH (i);

    int idx = 0;
    // a queue follows first in first out protocol
    while (myQueue-> BUFFER_AVAILABILITY != capacity) {
        if (* (myQueue-> BUFFER_POP_FIRST) != output_0[idx++])
            return Quality::Test::FAIL;
    }

    // lets try the same with no overflow
    auto myAnotherQueue = BUFFER_INIT (18, Memory::WITHOUT_OVERFLOW, int, capacity);
    for (auto i : input)
        myAnotherQueue-> BUFFER_PUSH (i);

    idx = 0;
    while (myAnotherQueue-> BUFFER_AVAILABILITY != capacity) {
        if (* (myAnotherQueue-> BUFFER_POP_FIRST) != output_1[idx++])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE_ALL;
    return Quality::Test::PASS;
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    // run all tests
    LIB_TEST_RUN_ALL;

    return 0;
}