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

LIB_TEST_CASE (0, "int type w/ no overwrite") {
    int input[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    /* since this is a no overwrite test case, with capacity = 5, the buffer contents will be 10 ... 6 even if we iterate
     * through the full input[] array, and the push operation should return without fail
    */
    const int capacity = 5;
    int output[] = { 10, 9, 8, 7, 6 };

    BUFFER_INIT (0, Memory::CIRCULAR_NO_OVERWRITE, int, capacity);
    for (auto i : input) 
        BUFFER_PUSH (0, int) << i;

    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (0, int) != output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (0);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "float type w/ no overwrite") {
    float input[] = { 10.1, 9.2, 8.3, 7.4, 6.5, 5.6, 4.7, 3.8, 2.9, 1.10 };

    const int capacity = 5;
    float output[] = { 10.1, 9.2, 8.3, 7.4, 6.5 };

    BUFFER_INIT (1, Memory::CIRCULAR_NO_OVERWRITE, float, capacity);
    for (auto i : input) 
        BUFFER_PUSH (1, float) << i;

    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (1, float) != output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "string type w/ no overwrite") {
    std::string input[] = { "ab", "cd", "ef", "gh", "ij", "kl", "mn", "op", "qr", "st" };

    const int capacity = 5;
    std::string output[] = { "ab", "cd", "ef", "gh", "ij" };

    BUFFER_INIT (2, Memory::CIRCULAR_NO_OVERWRITE, std::string, capacity);
    for (auto i : input) 
        BUFFER_PUSH (2, std::string) << i;

    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (2, std::string) != output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "char type w/ overwrite") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    const int capacity = 5;
    char output[] = { 'f', 'g', 'h', 'i', 'j' };

    BUFFER_INIT (3, Memory::CIRCULAR_OVERWRITE, char, capacity);
    for (auto i : input) 
        BUFFER_PUSH (3, char) << i;

    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (3, char) != output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (3);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "reuse instance id") {
    const int capacity = 5;
    BUFFER_INIT (4, Memory::CIRCULAR_OVERWRITE, char, capacity); 
    BUFFER_PUSH (4, char) << 'a';
    if (*BUFFER_POP (4, char) != 'a')
        return Quality::Test::FAIL;

    // you are able to reuse the instance #4 for another buffer after you close it
    BUFFER_CLOSE (4);

    BUFFER_INIT (4, Memory::CIRCULAR_NO_OVERWRITE, int, capacity); 
    BUFFER_PUSH (4, int) << 1010;
    if (*BUFFER_POP (4, int) != 1010)
        return Quality::Test::FAIL;

    BUFFER_CLOSE (4); 

    return Quality::Test::PASS; 
}

LIB_TEST_CASE (5, "buffer mgr dump") {
    const int capacity = 5;

    BUFFER_INIT (5, Memory::CIRCULAR_NO_OVERWRITE, int, capacity);
    BUFFER_INIT (6, Memory::CIRCULAR_NO_OVERWRITE, float, capacity);
    BUFFER_INIT (7, Memory::CIRCULAR_NO_OVERWRITE, int, capacity); 

    // mgr dump shows all opened buffer instances
    BUFFER_MGR_DUMP;

    BUFFER_CLOSE_ALL;

    BUFFER_MGR_DUMP;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (6, "buffer dump") {
    int input[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    const int capacity = 5;
    BUFFER_INIT (8, Memory::CIRCULAR_OVERWRITE, int, capacity);
    for (auto i : input) 
        BUFFER_PUSH (8, int) << i;

    BUFFER_DUMP (8, int);

    // pop few and redump
    BUFFER_POP (8, int);
    BUFFER_POP (8, int);

    BUFFER_DUMP (8, int);
    BUFFER_CLOSE (8);

    return Quality::Test::PASS;
}

LIB_TEST_CASE (7, "full-fill half-flush full-fill w/ no overwrite") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    const int capacity = 6;
    char half_flush_output[] = { 'a', 'b', 'c' };
    char full_flush_output[] = { 'd', 'e', 'f', 'a', 'b', 'c'};

    // full fill
    BUFFER_INIT (9, Memory::CIRCULAR_NO_OVERWRITE, char, capacity);
    for (auto i : input) 
        BUFFER_PUSH (9, char) << i;

    // half flush
    for (int i = 0; i < capacity/2; i++) {
        if (*BUFFER_POP (9, char) != half_flush_output[i])
            return Quality::Test::FAIL;
    }

    // re full fill
    for (auto i : input)
        BUFFER_PUSH (9, char) << i;

    // full flush
    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (9, char) != full_flush_output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (9);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (8, "full-fill half-flush full-fill w/ overwrite") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    const int capacity = 6;
    char half_flush_output[] = { 'e', 'f', 'g'};
    char full_flush_output[] = { 'e', 'f', 'g', 'h', 'i', 'j' };

    // full fill
    BUFFER_INIT (10, Memory::CIRCULAR_OVERWRITE, char, capacity);
    for (auto i : input) 
        BUFFER_PUSH (10, char) << i;

    BUFFER_DUMP (10, char);

    // half flush
    for (int i = 0; i < capacity/2; i++) {
        if (*BUFFER_POP (10, char) != half_flush_output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_DUMP (10, char);

    // re full fill
    for (auto i : input)
        BUFFER_PUSH (10, char) << i;

    BUFFER_DUMP (10, char);

    // full flush
    for (int i = 0; i < capacity; i++) {
        if (*BUFFER_POP (10, char) != full_flush_output[i])
            return Quality::Test::FAIL;
    }

    BUFFER_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "buffer reset") {
    float input[] = { 10.1, 9.2, 8.3, 7.4, 6.5, 5.6, 4.7, 3.8, 2.9, 1.10 };

    const int capacity = 6;
    BUFFER_INIT (11, Memory::CIRCULAR_OVERWRITE, float, capacity);
    for (auto i : input) 
        BUFFER_PUSH (11, float) << i;

    BUFFER_DUMP (11, float);

    BUFFER_RESET (11, float);
    BUFFER_DUMP (11, float);
    BUFFER_CLOSE (11);

    return Quality::Test::PASS;
}

LIB_TEST_CASE (10, "buffer peek") {
    char input[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

    char output_full_fill_peek[] = { 'e', 'j' };
    char output_half_flush_peek[] = { 'h', 'j' };

    const int capacity = 6;
    // full fill
    BUFFER_INIT (12, Memory::CIRCULAR_OVERWRITE, char, capacity);
    for (auto i : input) 
        BUFFER_PUSH (12, char) << i;

    // peek
    if (*BUFFER_PEEK_FIRST (12, char) != output_full_fill_peek[0])
        return Quality::Test::FAIL;
    if (*BUFFER_PEEK_LAST (12, char) != output_full_fill_peek[1])
        return Quality::Test::FAIL;

    // half flush
    for (int i = 0; i < capacity/2; i++)
        BUFFER_POP (12, char);
    
    // peek
    if (*BUFFER_PEEK_FIRST (12, char) != output_half_flush_peek[0])
        return Quality::Test::FAIL;
    if (*BUFFER_PEEK_LAST (12, char) != output_half_flush_peek[1])
        return Quality::Test::FAIL;

    // re full fill
    for (auto i : input)
        BUFFER_PUSH (12, char) << i;

    // peek
    if (*BUFFER_PEEK_FIRST (12, char) != output_full_fill_peek[0])
        return Quality::Test::FAIL;
    if (*BUFFER_PEEK_LAST (12, char) != output_full_fill_peek[1])
        return Quality::Test::FAIL;

    BUFFER_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "buffer availability") {
    int input[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    const int capacity = 6;

    int output_full_fill = 0;
    int output_half_flush = capacity/2;

    // full fill
    BUFFER_INIT (13, Memory::CIRCULAR_NO_OVERWRITE, int, capacity);
    for (auto i : input) 
        BUFFER_PUSH (13, int) << i; 

    if (BUFFER_AVAILABILITY (13, int) != output_full_fill)
        return Quality::Test::FAIL;

    // half flush
    for (int i = 0; i < capacity/2; i++)
        BUFFER_POP (13, int);

    if (BUFFER_AVAILABILITY (13, int) != output_half_flush)
        return Quality::Test::FAIL;

    // re full fill
    for (auto i : input) 
        BUFFER_PUSH (13, int) << i; 

    if (BUFFER_AVAILABILITY (13, int) != output_full_fill)
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

    s_customType input[] = { {0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"} };
    
    const int capacity = 3;
    s_customType output_full_flush[] = { {2, "two"}, {3, "three"}, {4, "four"} };

    BUFFER_INIT (14, Memory::CIRCULAR_OVERWRITE, s_customType, capacity);

    // full fill
    for (auto i : input)
        BUFFER_PUSH (14, s_customType) << i;

    // define lambda function, this tells the custom dump function on how to unravel the custom type
    auto lambda_customType = [](s_customType* readPtr, std::ostream& ost) { 
                                    ost << readPtr->data << "," << readPtr->desc;
                                };
    BUFFER_DUMP_CUSTOM (14, s_customType, lambda_customType);

    // full flush
    for (int i = 0; i < capacity; i++) {
        s_customType out = *BUFFER_POP (14, s_customType);

        if (out.data != output_full_flush[i].data ||
            out.desc != output_full_flush[i].desc)
            return Quality::Test::FAIL;
    }

    BUFFER_DUMP_CUSTOM (14, s_customType, lambda_customType);

    BUFFER_CLOSE (14);
    return Quality::Test::PASS;
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    // run all tests
    LIB_TEST_RUN_ALL;

    return 0;
}