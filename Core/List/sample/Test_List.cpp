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
#include "../inc/List.h"
#include "../../../Common/LibTest/inc/LibTest.h"

using namespace Collections;

LIB_TEST_CASE (0, "multiple instances") {
    LIST_INIT (0, int);
    LIST_INIT (1, char);
    LIST_INIT (2, float);
    LIST_INIT (3, std::string);

    LIST_ADD_HEAD (0, int) (1);
    LIST_ADD_HEAD (1, char) ('z');
    LIST_ADD_HEAD (2, float) (0.95);
    LIST_ADD_HEAD (3, std::string) ("John");

    LIST_DUMP (0, int);
    LIST_DUMP (1, char);
    LIST_DUMP (2, float);
    LIST_DUMP (3, std::string);

    // close before reusing an existing id
    LIST_CLOSE (1);
    LIST_INIT (1, float);

    LIST_ADD_HEAD (1, float) (5.95);
    LIST_DUMP (1, float);

    LIST_MGR_DUMP;
    LIST_CLOSE_ALL;
    LIST_MGR_DUMP;

    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "add/peek/remove from tail") {
    LIST_INIT (1, int);

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int output[] = { 60, 50, 40, 30, 20, 10 };

    for (auto i : input)
        LIST_ADD_TAIL (1, int) (i);

    // peek and remove
    for (auto i : output) {
        if (LIST_PEEK_TAIL (1, int) -> data != i) 
            return Quality::Test::FAIL;

        //LIST_DUMP (1, int);
        LIST_REMOVE_TAIL (1, int);
    }

    LIST_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "add/peek/remove from head") {
    LIST_INIT (2, int);

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int output[] = { 60, 50, 40, 30, 20, 10 };

    for (auto i : input)
        LIST_ADD_HEAD (2, int) (i);

    for (auto i : output) {
        if (LIST_PEEK_HEAD (2, int) -> data != i)
            return Quality::Test::FAIL;

        //LIST_DUMP (2, int);
        LIST_REMOVE_HEAD (2, int);   
    }

    LIST_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "add before id") {
    LIST_INIT (3, int);
    // add one node before testing
    LIST_ADD_HEAD (3, int) (50);

    int input[] = { 7, 8 ,9 };

    // set peek position
    LIST_PEEK_SET (3, int) (0);
    for (auto i : input) {
        if (LIST_ADD_BEFORE (3, int) (i) == false)
            return Quality::Test::FAIL;
    }  
    //LIST_DUMP (3, int);

    for (auto i : input) {
        if (LIST_PEEK_HEAD (3, int) -> data != i)
            return Quality::Test::FAIL;

        LIST_REMOVE_HEAD (3, int);
    }
    LIST_CLOSE (3);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "add after id") {
    LIST_INIT (4, int);
    // add one node before testing
    LIST_ADD_HEAD (4, int) (50);

    int input[] = { 7, 8 ,9 };

    // set peek position
    LIST_PEEK_SET (4, int) (0);
    for (auto i : input) {
        if (LIST_ADD_AFTER (4, int) (i) == false)
            return Quality::Test::FAIL;
    }  
    //LIST_DUMP (4, int);

    for (auto i : input) {
        if (LIST_PEEK_TAIL (4, int) -> data != i)
            return Quality::Test::FAIL;

        LIST_REMOVE_TAIL (4, int);
    }
    LIST_CLOSE (4);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (5, "add/remove invalid id") {
    LIST_INIT (5, int);
    // add one node before testing
    LIST_ADD_HEAD (5, int) (50);   

    int invalidId = 100;
    LIST_PEEK_SET (5, int) (invalidId);

    if (LIST_ADD_AFTER (5, int) (10) == true)
        return Quality::Test::FAIL;

    if (LIST_ADD_BEFORE (5, int) (10) == true)
        return Quality::Test::FAIL;

    if (LIST_REMOVE (5, int) == true)
        return Quality::Test::FAIL;

    LIST_DUMP (5, int);
    LIST_CLOSE (5);
    return Quality::Test::PASS; 
}

LIB_TEST_CASE (6, "peek/remove/dump from empty list") {
    LIST_INIT (6, int);

    if (LIST_PEEK_HEAD (6, int) != NULL)
        return Quality::Test::FAIL;

    if (LIST_PEEK_TAIL (6, int) != NULL)
        return Quality::Test::FAIL;

    LIST_PEEK_SET (6, int) (0);
    if (LIST_REMOVE (6, int) != false)
        return Quality::Test::FAIL;

    if (LIST_REMOVE_HEAD (6, int) != false)
        return Quality::Test::FAIL;

    if (LIST_REMOVE_TAIL (6, int) != false)
        return Quality::Test::FAIL;

    LIST_DUMP (6, int);

    LIST_CLOSE (6);
    return Quality::Test::PASS; 
}

LIB_TEST_CASE (7, "swap head node with another node") {
    LIST_INIT (7, int);    

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int swap_ids[] = { 0, 2 };
    int output_new_head = 30;

    for (auto i : input)
        LIST_ADD_TAIL (7, int) (i);
    LIST_DUMP (7, int);

    LIST_SWAP (7, int) (swap_ids[0], swap_ids[1]);
    LIST_DUMP (7, int);

    if (LIST_PEEK_HEAD (7, int) -> data != output_new_head)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (7);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (8, "swap tail node with another node") {
    LIST_INIT (8, int);    

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int swap_ids[] = { 5, 2 };
    int output_new_tail = 30;

    for (auto i : input)
        LIST_ADD_TAIL (8, int) (i);
    LIST_DUMP (8, int);

    LIST_SWAP (8, int) (swap_ids[0], swap_ids[1]);
    LIST_DUMP (8, int);

    if (LIST_PEEK_TAIL (8, int) -> data != output_new_tail)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (8);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "swap head node with tail node") {
    LIST_INIT (9, int);    

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int swap_ids[] = { 0, 5 };
    int output_new_tail = 10;
    int output_new_head = 60;

    for (auto i : input)
        LIST_ADD_TAIL (9, int) (i);
    LIST_DUMP (9, int);

    LIST_SWAP (9, int) (swap_ids[0], swap_ids[1]);
    LIST_DUMP (9, int);

    if (LIST_PEEK_HEAD (9, int) -> data != output_new_head)
        return Quality::Test::FAIL;

    if (LIST_PEEK_TAIL (9, int) -> data != output_new_tail)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (9);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (10, "swap nodes with invalid id") {
    LIST_INIT (10, int);    
    int swap_ids[] = { 1, 11 };

    // swap in an empty list, 2 invalid ids
    if (LIST_SWAP (10, int) (swap_ids[0], swap_ids[1]) != false)
        return Quality::Test::FAIL;

    int input[] = { 10, 20, 30, 40, 50, 60 };
    for (auto i : input)
        LIST_ADD_TAIL (10, int) (i);

    // 1 invalid id
    if (LIST_SWAP (10, int) (swap_ids[0], swap_ids[1]) != false)
        return Quality::Test::FAIL;

    // 2 valid ids - both the same doesn't fail
    if (LIST_SWAP (10, int) (swap_ids[0], swap_ids[0]) != true)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "swap non boundary adjacent nodes") {
    LIST_INIT (11, int);    

    int input_0[] = { 10, 20, 30, 40, 50, 60 };
    
    for (auto i : input_0)
        LIST_ADD_TAIL (11, int) (i);

    int swap_ids_0[] = { 2, 3 };
    LIST_SWAP (11, int) (swap_ids_0[0], swap_ids_0[1]);
    LIST_DUMP (11, int);

    // swap again but with 1st node is after the 2nd node
    int swap_ids_1[] = { 3, 2 };
    LIST_SWAP (11, int) (swap_ids_1[0], swap_ids_1[1]);
    LIST_DUMP (11, int);

    LIST_RESET (11, int);
    int input_1[] = { 10, 60 };

    for (auto i : input_1)
        LIST_ADD_TAIL (11, int) (i);
    LIST_DUMP (11, int);
    
    // swap boundary adjacent nodes
    int swap_ids_2[] = { 0, 1 };
    LIST_SWAP (11, int) (swap_ids_2[0], swap_ids_2[1]);
    LIST_DUMP (11, int);    

    LIST_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (12, "swap non boundary non adjacent nodes") {
    LIST_INIT (12, int);    

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int swap_ids[] = { 2, 4 };

    for (auto i : input)
        LIST_ADD_TAIL (12, int) (i);

    LIST_SWAP (12, int) (swap_ids[0], swap_ids[1]);
    LIST_DUMP (12, int);

    LIST_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (13, "reverse list") {
    LIST_INIT (13, int);    
    
    int input[] = { 10, 20, 30, 40, 50, 60 };
    int new_head = 60;
    int new_tail = 10;

    for (auto i : input)
        LIST_ADD_TAIL (13, int) (i);
    
    LIST_REVERSE (13, int);
    LIST_DUMP (13, int);

    // peek 
    if (LIST_PEEK_HEAD (13, int) -> data != new_head)
        return Quality::Test::FAIL;

    if (LIST_PEEK_TAIL (13, int) -> data != new_tail)
        return Quality::Test::FAIL;    

    LIST_CLOSE (13);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (14, "reverse list with only 2 nodes") {
    LIST_INIT (14, int);    

    int input[] = { 10, 20 };
    int new_head = 20;
    int new_tail = 10;

    for (auto i : input)
        LIST_ADD_TAIL (14, int) (i);
    
    LIST_REVERSE (14, int);
    LIST_DUMP (14, int);

    // peek 
    if (LIST_PEEK_HEAD (14, int) -> data != new_head)
        return Quality::Test::FAIL;

    if (LIST_PEEK_TAIL (14, int) -> data != new_tail)
        return Quality::Test::FAIL;  

    LIST_CLOSE (14);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (15, "reverse list with only 1 node") {
    LIST_INIT (15, int);    

    int input[] = { 10 };
    int new_head = 10;
    int new_tail = 10;

    for (auto i : input)
        LIST_ADD_TAIL (15, int) (i);
    
    LIST_REVERSE (15, int);
    LIST_DUMP (15, int);

    // peek 
    if (LIST_PEEK_HEAD (15, int) -> data != new_head)
        return Quality::Test::FAIL;

    if (LIST_PEEK_TAIL (15, int) -> data != new_tail)
        return Quality::Test::FAIL;  

    LIST_CLOSE (15);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (16, "reverse empty list") {
    LIST_INIT (16, int);    

    LIST_REVERSE (16, int);
    LIST_DUMP (16, int);

    LIST_CLOSE (16);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (17, "custom types") {
    // define custom type
    typedef struct {
        std::string name;
        float thresh;
    }s_customType;

    LIST_INIT (17, s_customType);
    s_customType input[] = { {"John", 0.55}, {"Adams", 0.75} };

    for (auto i : input)
        LIST_ADD_HEAD (17, s_customType) (i);

    auto lambda_customType = [](s_customType* currentNode, std::ostream& ost) { 
                                    ost << currentNode-> name << "," << currentNode-> thresh;
                                };
    LIST_DUMP_CUSTOM (17, s_customType, lambda_customType);
    LIST_CLOSE (17);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (18, "list reset") {
    LIST_INIT (18, int);

    int input[] = { 10, 20, 30, 40, 50, 60 };
    for (auto i : input)
        LIST_ADD_TAIL (18, int) (i);

    LIST_RESET (18, int);
    // list size should be zero
    if (LIST_SIZE (18, int) != 0)
        return Quality::Test::FAIL;

    LIST_CLOSE (18);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (19, "list traversal head to tail") {
    LIST_INIT (19, int);

    int input[] = { 10, 20, 30 };

    for (auto i : input)
        LIST_ADD_TAIL (19, int) (i);

    // traverse from head to tail, first step is to set the peek position
    LIST_PEEK_SET_HEAD (19, int);
    LIST_DUMP (19, int);

    int i = 0;
    while (LIST_PEEK_CURRENT (19, int) != NULL) {
        if (LIST_PEEK_CURRENT (19, int) -> data != input[i++])
            return Quality::Test::FAIL;

        // move peek pointer
        LIST_PEEK_SET_NEXT (19, int);
    }

    LIST_CLOSE (19);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (20, "list traversal tail to head") {
    LIST_INIT (20, int);

    int input[] = { 10, 20, 30 };

    for (auto i : input)
        LIST_ADD_TAIL (20, int) (i);

    // set the peek position to tail
    LIST_PEEK_SET_TAIL (20, int);
    LIST_DUMP (20, int);

    int i = 2;
    while (LIST_PEEK_CURRENT (20, int) != NULL) {
        if (LIST_PEEK_CURRENT (20, int) -> data != input[i--])
            return Quality::Test::FAIL;

        // move peek pointer
        LIST_PEEK_SET_PREVIOUS (20, int);
    }

    LIST_CLOSE (20);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (21, "list traversal starting at id") {
    LIST_INIT (21, int);

    int input[] = { 10, 20, 30, 40, 50 };

    for (auto i : input)
        LIST_ADD_TAIL (21, int) (i);

    // set the peek position (the id of node (data = 30) is 2)
    LIST_PEEK_SET (21, int) (2);
    LIST_DUMP (21, int);

    // traverse forward
    int i = 2;
    while (LIST_PEEK_CURRENT (21, int) != NULL) {
        if (LIST_PEEK_CURRENT (21, int) -> data != input[i++])
            return Quality::Test::FAIL;

        // move peek pointer
        LIST_PEEK_SET_NEXT (21, int);
    }

    // reset peek position
    LIST_PEEK_SET (21, int) (3);
    LIST_DUMP (21, int);

    // traverse backward
    i = 3;
    while (LIST_PEEK_CURRENT (21, int) != NULL) {
        if (LIST_PEEK_CURRENT (21, int) -> data != input[i--])
            return Quality::Test::FAIL;

        // move peek pointer
        LIST_PEEK_SET_PREVIOUS (21, int);
    }    

    LIST_CLOSE (21);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (22, "list traversal empty list") {
    LIST_INIT (22, int);

    // set peek position to tail
    LIST_PEEK_SET_TAIL (22, int);
    if (LIST_PEEK_CURRENT (22, int) != NULL)
        return Quality::Test::FAIL;

    // set peek position to head
    LIST_PEEK_SET_HEAD (22, int);
    if (LIST_PEEK_CURRENT (22, int) != NULL)
        return Quality::Test::FAIL;

    // set peek position using id
    LIST_PEEK_SET (22, int) (1);
    if (LIST_PEEK_CURRENT (22, int) != NULL)
        return Quality::Test::FAIL;

    LIST_CLOSE (22);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (23, "combining two lists") {
    LIST_INIT (23, int);
    int input_0[] = { 10, 20, 30 };

    for (auto i : input_0)
        LIST_ADD_TAIL (23, int) (i);

    LIST_INIT (24, int);
    int input_1[] = { 40, 50, 60 };

    for (auto i: input_1)
        LIST_ADD_TAIL (24, int) (i);

    // combine list 23 and list 24
    LIST_PEEK_SET_TAIL (23, int);
    LIST_PEEK_SET_HEAD (24, int);

    while (LIST_PEEK_CURRENT (24, int) != NULL) {
        // add current node data of 24 to tail of 23
        LIST_ADD_TAIL (23, int) (LIST_PEEK_CURRENT (24, int) -> data);
        LIST_PEEK_SET_NEXT (24, int);
    }

    // after combining, delete list 24
    LIST_CLOSE (24);
    LIST_MGR_DUMP;

    int output[] = { 10, 20, 30, 40, 50, 60 };
    int i = 0;

    LIST_PEEK_SET_HEAD (23, int);
    while (LIST_PEEK_CURRENT (23, int) != NULL) {
        if (LIST_PEEK_CURRENT (23, int) -> data != output[i++])
            return Quality::Test::FAIL;

        LIST_PEEK_SET_NEXT (23, int);
    }

    LIST_CLOSE (23);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (24, "remove using id") {
    LIST_INIT (24, int);

    int input[] = { 10, 20, 30, 40, 50, 60 };
    int removeId = 2;
    int output[] = { 10, 20, 40, 50, 60 };

    for (auto i : input)
        LIST_ADD_TAIL (24, int) (i);

    LIST_PEEK_SET (24, int) (removeId);
    LIST_REMOVE (24, int);
    LIST_DUMP (24, int);

    for (auto i : output) {
        if (LIST_PEEK_HEAD (24, int) -> data != i) 
            return Quality::Test::FAIL;
        LIST_REMOVE_HEAD (24, int);
    }

    LIST_CLOSE (24);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (25, "reset empty list") {
    LIST_INIT (25, int);
    LIST_ADD_HEAD (25, int) (100);

    // remove only node in the list
    LIST_PEEK_SET (25, int) (0);
    LIST_REMOVE (25, int);
    LIST_DUMP (25, int);

    // reset empty list
    LIST_RESET (25, int);
    LIST_DUMP (25, int);

    LIST_CLOSE (25);
    return Quality::Test::PASS;    
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    LIB_TEST_RUN_ALL;

    return 0;
}
