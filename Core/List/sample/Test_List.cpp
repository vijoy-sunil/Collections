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
    auto myList0 = LIST_INIT (0, int);
    auto myList1 = LIST_INIT (1, char);
    auto myList2 = LIST_INIT (2, float);
    LIST_INIT (3, std::string);

    // or use GET_ to get list instance
    auto myList3 = GET_LIST (3, std::string);

    // add a node to head with data (26) and node id (1)
    myList0-> LIST_ADD_HEAD (1, 26);

    myList1-> LIST_ADD_HEAD (1, 'z');
    myList2-> LIST_ADD_HEAD (1, 0.95);
    myList3-> LIST_ADD_HEAD (1, "John");

    myList0-> LIST_DUMP;
    myList1-> LIST_DUMP;
    myList2-> LIST_DUMP;
    myList3-> LIST_DUMP;

    // close before reusing an existing id
    LIST_CLOSE (1);
    auto myList4 = LIST_INIT (1, float);

    myList4-> LIST_ADD_HEAD (2, 5.95);
    myList4-> LIST_DUMP;

    LIST_MGR_DUMP;
    LIST_CLOSE_ALL;
    LIST_MGR_DUMP;

    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "add/peek/remove from tail") {
    auto myList = LIST_INIT (1, int);

    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 },
                                        { 4, 40 },
                                        { 5, 50 },
                                        { 6, 60 } };
    int output[] = { 60, 50, 40, 30, 20, 10 };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    // peek and remove
    for (auto i : output) {
        if (myList-> LIST_PEEK_TAIL-> data != i) 
            return Quality::Test::FAIL;

        myList-> LIST_REMOVE_TAIL;
    }

    LIST_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "add/peek/remove from head") {
    auto myList = LIST_INIT (2, int);

    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 },
                                        { 4, 40 },
                                        { 5, 50 },
                                        { 6, 60 } };
    int output[] = { 60, 50, 40, 30, 20, 10 };

    for (auto i : input)
        myList-> LIST_ADD_HEAD (i.first, i.second);

    for (auto i : output) {
        if (myList-> LIST_PEEK_HEAD-> data != i)
            return Quality::Test::FAIL;

        myList-> LIST_REMOVE_HEAD;   
    }

    LIST_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "add before id") {
    auto myList = LIST_INIT (3, int);

    // add one node before testing
    myList-> LIST_ADD_HEAD (0, 50);

    std::pair <size_t, int> input[] = { { 1, 7 },
                                        { 2, 8 },
                                        { 3, 9 } };

    // set peek position
    myList-> LIST_PEEK_SET (0);
    for (auto i : input) {
        if (myList-> LIST_ADD_BEFORE (i.first, i.second) == false)
            return Quality::Test::FAIL;
    }  

    for (auto i : input) {
        if (myList-> LIST_PEEK_HEAD-> data != i.second)
            return Quality::Test::FAIL;

        myList-> LIST_REMOVE_HEAD;
    }
    LIST_CLOSE (3);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "add after id") {
    auto myList = LIST_INIT (4, int);
    // add one node before testing
    myList-> LIST_ADD_HEAD (0, 50);

    std::pair <size_t, int> input[] = { { 1, 7 },
                                        { 2, 8 },
                                        { 3, 9 } };

    // set peek position
    myList-> LIST_PEEK_SET (0);
    for (auto i : input) {
        if (myList-> LIST_ADD_AFTER (i.first, i.second) == false)
            return Quality::Test::FAIL;
    }  

    for (auto i : input) {
        if (myList-> LIST_PEEK_TAIL-> data != i.second)
            return Quality::Test::FAIL;

        myList-> LIST_REMOVE_TAIL;
    }
    LIST_CLOSE (4);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (5, "add/remove invalid id") {
    auto myList = LIST_INIT (5, int);
    // add one node before testing
    myList-> LIST_ADD_HEAD (0, 50);   

    int invalidId = 100;
    myList-> LIST_PEEK_SET (invalidId);

    if (myList-> LIST_ADD_AFTER (1, 10) == true)
        return Quality::Test::FAIL;

    if (myList-> LIST_ADD_BEFORE (1, 10) == true)
        return Quality::Test::FAIL;

    if (myList-> LIST_REMOVE == true)
        return Quality::Test::FAIL;

    myList-> LIST_DUMP;
    LIST_CLOSE (5);
    return Quality::Test::PASS; 
}

LIB_TEST_CASE (6, "peek/remove/dump from empty list") {
    auto myList = LIST_INIT (6, int);

    if (myList-> LIST_PEEK_HEAD != NULL)
        return Quality::Test::FAIL;

    if (myList-> LIST_PEEK_TAIL != NULL)
        return Quality::Test::FAIL;

    myList-> LIST_PEEK_SET (0);
    if (myList-> LIST_REMOVE != false)
        return Quality::Test::FAIL;

    if (myList-> LIST_REMOVE_HEAD != false)
        return Quality::Test::FAIL;

    if (myList-> LIST_REMOVE_TAIL != false)
        return Quality::Test::FAIL;

    myList-> LIST_DUMP;

    LIST_CLOSE (6);
    return Quality::Test::PASS; 
}

LIB_TEST_CASE (7, "swap head node with another node") {
    auto myList = LIST_INIT (7, int);   

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int swap_ids[] = { 0, 2 };
    int output_new_head = 30;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    myList-> LIST_DUMP;

    myList-> LIST_SWAP (swap_ids[0], swap_ids[1]);
    myList-> LIST_DUMP;

    if (myList-> LIST_PEEK_HEAD-> data != output_new_head)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (7);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (8, "swap tail node with another node") {
    auto myList = LIST_INIT (8, int);   

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int swap_ids[] = { 5, 2 };
    int output_new_tail = 30;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    myList-> LIST_DUMP;

    myList-> LIST_SWAP (swap_ids[0], swap_ids[1]);
    myList-> LIST_DUMP;

    if (myList-> LIST_PEEK_TAIL-> data != output_new_tail)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (8);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "swap head node with tail node") {
    auto myList = LIST_INIT (9, int);   

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int swap_ids[] = { 0, 5 };
    int output_new_tail = 10;
    int output_new_head = 60;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    myList-> LIST_DUMP;

    myList-> LIST_SWAP (swap_ids[0], swap_ids[1]);
    myList-> LIST_DUMP;

    if (myList-> LIST_PEEK_HEAD-> data != output_new_head)
        return Quality::Test::FAIL;

    if (myList-> LIST_PEEK_TAIL-> data != output_new_tail)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (9);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (10, "swap nodes with invalid id") {
    auto myList = LIST_INIT (10, int);   
    int swap_ids[] = { 1, 11 };

    // swap in an empty list, 2 invalid ids
    if (myList-> LIST_SWAP (swap_ids[0], swap_ids[1]) != false)
        return Quality::Test::FAIL;

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    // 1 invalid id
    if (myList-> LIST_SWAP (swap_ids[0], swap_ids[1]) != false)
        return Quality::Test::FAIL;

    // 2 valid ids - both the same doesn't fail
    if (myList-> LIST_SWAP (swap_ids[0], swap_ids[0]) != true)
        return Quality::Test::FAIL;
    
    LIST_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "swap non boundary adjacent nodes") {
    auto myList = LIST_INIT (11, int);  

    std::pair <size_t, int> input_0[] = { { 0, 10 }, 
                                          { 1, 20 },
                                          { 2, 30 },
                                          { 3, 40 },
                                          { 4, 50 },
                                          { 5, 60 } };
    
    for (auto i : input_0)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    int swap_ids_0[] = { 2, 3 };
    myList-> LIST_SWAP (swap_ids_0[0], swap_ids_0[1]);
    myList-> LIST_DUMP;

    // swap again but with 1st node is after the 2nd node
    int swap_ids_1[] = { 3, 2 };
    myList-> LIST_SWAP (swap_ids_1[0], swap_ids_1[1]);
    myList-> LIST_DUMP;

    myList-> LIST_RESET;
    std::pair <size_t, int> input_1[] = { { 0, 10 },
                                          { 1, 60 } };

    for (auto i : input_1)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    myList-> LIST_DUMP;
    
    // swap boundary adjacent nodes
    int swap_ids_2[] = { 0, 1 };
    myList-> LIST_SWAP (swap_ids_2[0], swap_ids_2[1]);
    myList-> LIST_DUMP;    

    LIST_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (12, "swap non boundary non adjacent nodes") {
    auto myList = LIST_INIT (12, int);   

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int swap_ids[] = { 2, 4 };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    myList-> LIST_SWAP (swap_ids[0], swap_ids[1]);
    myList-> LIST_DUMP;

    LIST_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (13, "reverse list") {
    auto myList = LIST_INIT (13, int);   
    
    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int new_head = 60;
    int new_tail = 10;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    
    myList-> LIST_REVERSE;
    myList-> LIST_DUMP;

    // peek 
    if (myList-> LIST_PEEK_HEAD-> data != new_head)
        return Quality::Test::FAIL;

    if (myList-> LIST_PEEK_TAIL-> data != new_tail)
        return Quality::Test::FAIL;    

    LIST_CLOSE (13);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (14, "reverse list with only 2 nodes") {
    auto myList = LIST_INIT (14, int);   

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 } };
    int new_head = 20;
    int new_tail = 10;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    
    myList-> LIST_REVERSE;
    myList-> LIST_DUMP;

    // peek 
    if (myList-> LIST_PEEK_HEAD-> data != new_head)
        return Quality::Test::FAIL;

    if (myList-> LIST_PEEK_TAIL-> data != new_tail)
        return Quality::Test::FAIL;  

    LIST_CLOSE (14);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (15, "reverse list with only 1 node") {
    auto myList = LIST_INIT (15, int);   

    std::pair <size_t, int> input[] = { { 0, 10 } };
    int new_head = 10;
    int new_tail = 10;

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);
    
    myList-> LIST_REVERSE;
    myList-> LIST_DUMP;

    // peek 
    if (myList-> LIST_PEEK_HEAD-> data != new_head)
        return Quality::Test::FAIL;

    if (myList-> LIST_PEEK_TAIL-> data != new_tail)
        return Quality::Test::FAIL;  

    LIST_CLOSE (15);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (16, "reverse empty list") {
    auto myList = LIST_INIT (16, int);    

    myList-> LIST_REVERSE;
    myList-> LIST_DUMP;

    LIST_CLOSE (16);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (17, "custom types") {
    // define custom type
    typedef struct {
        std::string name;
        float thresh;
    }s_customType;

    auto myList = LIST_INIT (17, s_customType);

    std::pair <size_t, s_customType> input[] = { { 0, { "John", 0.55 } }, 
                                                 { 1, {"Adams", 0.75 } } };

    for (auto i : input)
        myList-> LIST_ADD_HEAD (i.first, i.second);

    auto lambda_customType = [](s_customType* currentNode, std::ostream& ost) { 
                                    ost << currentNode-> name << ", " << currentNode-> thresh;
                                };
    myList-> LIST_DUMP_CUSTOM (lambda_customType);
    LIST_CLOSE (17);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (18, "list reset") {
    auto myList = LIST_INIT (18, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    myList-> LIST_RESET;
    myList-> LIST_DUMP;

    // list size should be zero
    if (myList-> LIST_SIZE != 0)
        return Quality::Test::FAIL;

    LIST_CLOSE (18);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (19, "list traversal head to tail") {
    auto myList = LIST_INIT (19, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 } };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    // traverse from head to tail, first step is to set the peek position
    myList-> LIST_PEEK_SET_HEAD;
    myList-> LIST_DUMP;

    int i = 0;
    while (myList-> LIST_PEEK_CURRENT != NULL) {
        if (myList-> LIST_PEEK_CURRENT-> data != input[i++].second)
            return Quality::Test::FAIL;

        // move peek pointer
        myList-> LIST_PEEK_SET_NEXT;
    }

    LIST_CLOSE (19);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (20, "list traversal tail to head") {
    auto myList = LIST_INIT (20, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 } };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    // set the peek position to tail
    myList-> LIST_PEEK_SET_TAIL;
    myList-> LIST_DUMP;

    int i = 2;
    while (myList-> LIST_PEEK_CURRENT != NULL) {
        if (myList-> LIST_PEEK_CURRENT-> data != input[i--].second)
            return Quality::Test::FAIL;

        // move peek pointer
        myList-> LIST_PEEK_SET_PREVIOUS;
    }

    LIST_CLOSE (20);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (21, "list traversal starting at id") {
    auto myList = LIST_INIT (21, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 } };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    // set the peek position (the id of node (data = 30) is 2)
    myList-> LIST_PEEK_SET (2);
    myList-> LIST_DUMP;

    // traverse forward
    int i = 2;
    while (myList-> LIST_PEEK_CURRENT != NULL) {
        if (myList-> LIST_PEEK_CURRENT-> data != input[i++].second)
            return Quality::Test::FAIL;

        // move peek pointer
        myList-> LIST_PEEK_SET_NEXT;
    }

    // reset peek position
    myList-> LIST_PEEK_SET (3);
    myList-> LIST_DUMP;

    // traverse backward
    i = 3;
    while (myList-> LIST_PEEK_CURRENT != NULL) {
        if (myList-> LIST_PEEK_CURRENT-> data != input[i--].second)
            return Quality::Test::FAIL;

        // move peek pointer
        myList-> LIST_PEEK_SET_PREVIOUS;
    }    

    LIST_CLOSE (21);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (22, "list traversal empty list") {
    auto myList = LIST_INIT (22, int);

    // set peek position to tail
    myList-> LIST_PEEK_SET_TAIL;
    if (myList-> LIST_PEEK_CURRENT != NULL)
        return Quality::Test::FAIL;

    // set peek position to head
    myList-> LIST_PEEK_SET_HEAD;
    if (myList-> LIST_PEEK_CURRENT != NULL)
        return Quality::Test::FAIL;

    // set peek position using id
    myList-> LIST_PEEK_SET (1);
    if (myList-> LIST_PEEK_CURRENT != NULL)
        return Quality::Test::FAIL;

    LIST_CLOSE (22);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (23, "combining two lists") {
    auto myList0 = LIST_INIT (23, int);

    std::pair <size_t, int> input_0[] = { { 0, 10 }, 
                                          { 1, 20 },
                                          { 2, 30 } };

    for (auto i : input_0)
        myList0-> LIST_ADD_TAIL (i.first, i.second);

    auto myList1 = LIST_INIT (24, int);

    std::pair <size_t, int> input_1[] = { { 0, 40 }, 
                                          { 1, 50 },
                                          { 2, 60 } };

    for (auto i: input_1)
        myList1-> LIST_ADD_TAIL (i.first, i.second);

    // combine list 23 and list 24
    myList0-> LIST_PEEK_SET_TAIL;
    myList1-> LIST_PEEK_SET_HEAD;

    // make sure newly added nodes does not share the same id as the nodes in the parent list
    size_t id = 3;
    while (myList1-> LIST_PEEK_CURRENT != NULL) {
        // add current node data of 24 to tail of 23
        myList0-> LIST_ADD_TAIL (id++, myList1-> LIST_PEEK_CURRENT-> data);
        myList1-> LIST_PEEK_SET_NEXT;
    }

    // after combining, delete list 24
    LIST_CLOSE (24);
    LIST_MGR_DUMP;

    std::pair <size_t, int> output[] = { { 0, 10 }, 
                                         { 1, 20 },
                                         { 2, 30 },
                                         { 3, 40 },
                                         { 4, 50 },
                                         { 5, 60 } };
    
    int i = 0;
    myList0-> LIST_PEEK_SET_HEAD;

    while (myList0-> LIST_PEEK_CURRENT != NULL) {
        if (myList0-> LIST_PEEK_CURRENT-> data != output[i++].second)
            return Quality::Test::FAIL;

        myList0-> LIST_PEEK_SET_NEXT;
    }

    myList0-> LIST_DUMP;

    LIST_CLOSE (23);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (24, "remove using id") {
    auto myList = LIST_INIT (24, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 },
                                        { 3, 40 },
                                        { 4, 50 },
                                        { 5, 60 } };
    int removeId = 2;
    int output[] = { 10, 20, 40, 50, 60 };

    for (auto i : input)
        myList-> LIST_ADD_TAIL (i.first, i.second);

    myList-> LIST_PEEK_SET (removeId);
    myList-> LIST_REMOVE;
    myList-> LIST_DUMP;

    for (auto i : output) {
        if (myList-> LIST_PEEK_HEAD-> data != i) 
            return Quality::Test::FAIL;
        myList-> LIST_REMOVE_HEAD;
    }

    LIST_CLOSE (24);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (25, "reset empty list") {
    auto myList = LIST_INIT (25, int);

    myList-> LIST_ADD_HEAD (0, 100);

    // remove only node in the list
    myList-> LIST_PEEK_SET (0);
    myList-> LIST_REMOVE;
    myList-> LIST_DUMP;

    // reset empty list
    myList-> LIST_RESET;
    myList-> LIST_DUMP;

    LIST_CLOSE (25);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (26, "unique node ids") {
    auto myList = LIST_INIT (26, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 0, 20 },      // <----- invalid id, user needs to make sure ids are unique
                                        { 2, 30 } };

    myList-> LIST_ADD_HEAD (input[0].first, input[1].second);

    // check if id in doubt (id #0) already exists in tree
    myList-> LIST_PEEK_SET (0);

    // if peek at id #0 returns NULL, then the id doesn't exist in the list
    if (myList-> LIST_PEEK_CURRENT == NULL)
        return Quality::Test::FAIL;

    LIST_CLOSE (26);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (27, "list as queue") {
    auto myQueue = LIST_INIT (27, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 } };

    for (auto i : input)
        myQueue-> LIST_ADD_TAIL (i.first, i.second);

    int idx = 0;
    while (myQueue-> LIST_SIZE != 0) {
        // queue follows first in - first out when reading
        if (myQueue-> LIST_PEEK_HEAD-> data != input[idx++].second)
            return Quality::Test::FAIL;

        myQueue-> LIST_REMOVE_HEAD;
    }
     
    LIST_CLOSE (27);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (28, "list as stack") {
    auto myStack = LIST_INIT (28, int);

    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 },
                                        { 2, 30 } };  

    for (auto i : input)
        myStack-> LIST_ADD_TAIL (i.first, i.second);

    int idx = 2;
    while (myStack-> LIST_SIZE != 0) {
        // stack follows last in - first out when reading
        if (myStack-> LIST_PEEK_TAIL-> data != input[idx--].second)
            return Quality::Test::FAIL;

        myStack-> LIST_REMOVE_TAIL;
    }

    LIST_CLOSE (28);
    return Quality::Test::PASS;                                         
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE, "./Build/Save/List/");
    LIB_TEST_RUN_ALL;

    return 0;
}
