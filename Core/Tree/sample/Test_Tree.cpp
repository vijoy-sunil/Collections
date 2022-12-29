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
#include "../inc/Tree.h"
#include "../../../Common/LibTest/inc/LibTest.h"

using namespace Collections;

#define ENABLE_INTERNAL_TESTS   1

// test params to verify tree structure
typedef struct {
    size_t  numNodes;
    size_t  depth;
    size_t* levels;
    size_t* numChildren;

    size_t* ids;
    size_t* numDescendants;
    size_t* parents;
}s_verifyParams;

template <typename T>
bool verifyTree (Memory::Tree <T>* tree, s_verifyParams params) {
    if (tree-> TREE_SIZE != params.numNodes)    
        return false;   
    
    if (tree-> TREE_DEPTH != params.depth)
        return false;

    int idx = 0;
    tree-> TREE_PEEK_SET_ROOT;

    while (tree-> TREE_PEEK_NODE != NULL) {
        if (tree-> TREE_PEEK_LEVEL != params.levels[idx])
            return false;

        if (tree-> TREE_PEEK_NODE-> child.size() != params.numChildren[idx])
            return false;

        if (tree-> TREE_PEEK_NODE-> id != params.ids[idx])
            return false;

        if (tree-> TREE_PEEK_NODE-> numDescendants != params.numDescendants[idx])
            return false;

        // skip checking the parent of the root node
        if (tree-> TREE_PEEK_NODE-> parent != NULL) {
            if (tree-> TREE_PEEK_NODE-> parent-> id != params.parents[idx])
                return false;
        }
            
        // move to next node (level-wise traversal)
        tree-> TREE_PEEK_SET_NEXT;
        // ground truth array index
        idx++;
    }
    return true;
}

LIB_TEST_CASE (0, "multiple instances") {
    auto myTree0 = TREE_INIT (0, int);
    auto myTree1 = TREE_INIT (1, char);

    myTree0-> TREE_ADD_ROOT (15, 0);
    myTree1-> TREE_ADD_ROOT (25, 'a');

    // close instance and reuse
    TREE_CLOSE (1);
    auto myTree2 = TREE_INIT (1, float);

    myTree2-> TREE_ADD_ROOT (35, 10.01);

    TREE_MGR_DUMP;
    TREE_CLOSE_ALL;
    TREE_MGR_DUMP;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "add to root") {
    auto myTree = TREE_INIT (1, int);
    std::pair <int, int> input[] = { {0, 10}, {1, 20}, {2, 30}, {3, 40} };
    // create tree
    for (auto i : input)
        myTree-> TREE_ADD_ROOT (i.first, i.second);

    size_t numNodes = 4;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 4 };
    size_t numChildren[] =          { 1, 1, 1, 0 };
    size_t ids[] =                  { 3, 2, 1, 0 };
    size_t numDescendants[] =       { 3, 2, 1, 0 };
    size_t parents[] =              { 0, 3, 2, 1 };   
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "add children") {
    auto myTree = TREE_INIT (2, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},           {3, 30}, 
                                        {4, 40}, {5, 50},  {6, 60}, {7, 70},
                                        {8, 80},                    {9, 90} };

    int idx = 0;
    // create tree
    myTree-> TREE_ADD_ROOT (input[idx].first, input[idx].second);
    idx++;

    for (auto i : { 1, 2, 3 }) {
        myTree-> TREE_PEEK_SET (i);

        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;
        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;
    }

    for (auto i : { 4, 7 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;
    }

    size_t numNodes = 9;                                        
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 3, 3, 3, 3, 4, 4 };
    size_t numChildren[] =          { 2,
                                      2,                 2,
                                      1,       0,        0,       1,
                                      0,                          0 };
    size_t ids[] =                  { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    size_t numDescendants[] =       { 8,
                                      3,                 3,
                                      1,       0,        0,       1,
                                      0,                          0 };
    size_t parents[] =              { 0, 1, 1, 2, 2, 3, 3, 4, 7 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "add parent") {
    auto myTree = TREE_INIT (3, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},           {3, 30}, 
                                        {4, 40}, {5, 50},  {6, 60}, {7, 70},
                                        {8, 80},                    {9, 90} };

    int idx = 0;
    // create tree
    myTree-> TREE_ADD_ROOT (input[idx].first, input[idx].second);
    idx++;

    for (auto i : { 1, 2, 3 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;

        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;
    }

    for (auto i : { 4, 7 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_CHILD (input[idx].first, input[idx].second);
        idx++;
    }

    std::pair <size_t, int> newNodes[] = { {0, 9}, {16, 79}, {25, 49} };
    /*                                  {0, 9}
     *                                  |
     *                                  {1, 10}
     *                                  |
     *                          -------------------
     *                      {2, 20}             {3, 30}
     *                      |                   |
     *             -------------------          -------------------
     *          {4, 40}             {25, 49}    {6, 60}            {7, 70}
     *          |                   |                              |
     *          {16, 79}            {5, 50}                        {9, 90}
     *          |
     *          {8, 80}
    */
    // add nodes
    idx = 0;
    for (auto i : { 1, 8, 5 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_PARENT (newNodes[idx].first, newNodes[idx].second);
        idx++;
    }
    myTree-> TREE_DUMP;

    size_t numNodes = 12;
    size_t depth = 6;
    size_t levels[] =               { 1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6 };
    size_t numChildren[] =          { 1,
                                      2,
                                      2,                 2,
                                      1,       1,        0,        1,
                                      1,       0,                  0,
                                      0 };
    size_t ids[] =                  { 0, 1, 2, 3, 4, 25, 6, 7, 16, 5, 9, 8 };
    size_t numDescendants[] =       { 11,
                                      10,
                                      5,                 3,
                                      2,       1,        0,       1,
                                      1,       0,                 0,
                                      0 };
    size_t parents[] =              { 0, 0, 1, 1, 2, 2, 3, 3, 4, 25, 7, 16 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;
   
    TREE_CLOSE (3); 
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "remove with id (no adopt)") {
    auto myTree = TREE_INIT (4, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},
                                        {3, 30}, {4, 40}, {5, 50}, {6, 60}, {7, 50},
                                        {8, 80},                   {9, 90},
                                        {10, 100} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (6);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second);

    // remove 3 tail nodes
    for (auto i : { 6, 3, 7 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_REMOVE;
    }

    size_t numNodes = 4;
    size_t depth = 3;
    size_t levels[] =               { 1, 2, 3, 3 };
    size_t numChildren[] =          { 1,
                                      2,                
                                      0,       0 };
    size_t ids[] =                  { 1, 2, 4, 5 };
    size_t numDescendants[] =       { 3,
                                      2,
                                      0,       0 };
    size_t parents[] =              { 0, 1, 2, 2 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (4);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (5, "remove root (no adopt)") {
    auto myTree = TREE_INIT (5, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},
                                        {3, 30}, {4, 40}, {5, 50}, {6, 60}, {7, 50},
                                        {8, 80},                   {9, 90},
                                        {10, 100} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (6);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second);

    myTree-> TREE_DUMP;

    // remove root
    myTree-> TREE_PEEK_SET_ROOT;
    myTree-> TREE_REMOVE;
    myTree-> TREE_DUMP;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (5);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (6, "remove with id (adopt)") {
    auto myTree = TREE_INIT (6, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},
                                        {3, 30}, 
                                        {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                        {8, 80},                   {9, 90},
                                        {10, 8}, {11, 9} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);    

    myTree-> TREE_DUMP;
    
    // create new trees (adoptors)
    auto adoptor0 = TREE_INIT (7, int);
    auto adoptor1 = TREE_INIT (8, int);

    // remove
    myTree-> TREE_PEEK_SET (4);
    adoptor0-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);
    adoptor0-> TREE_DUMP;

    myTree-> TREE_PEEK_SET (9);
    adoptor1-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);
    adoptor1-> TREE_DUMP;

    // original tree
    myTree-> TREE_DUMP;

    // verify original tree
    size_t numNodes = 6;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 4, 4, 4 };
    size_t numChildren[] =          { 1,
                                      1,                
                                      3,       
                                      0,      0,      0 };
    size_t ids[] =                  { 1, 2, 3, 5, 6, 7 };
    size_t numDescendants[] =       { 5,
                                      4,
                                      3,
                                      0,      0,      0};
    size_t parents[] =              { 0, 1, 2, 3, 3, 3 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;

    // verify adoptor 0
    size_t numNodes_a0 = 4;
    size_t depth_a0 = 3;
    size_t levels_a0[] =            { 1, 2, 3, 3 };
    size_t numChildren_a0[] =       { 1,
                                      2,                  
                                      0,      0 };
    size_t ids_a0[] =               { 4, 8, 10, 11 };
    size_t numDescendants_a0[] =    { 3,
                                      2,
                                      0,      0 };
    size_t parents_a0[] =           { 0, 4, 8, 8 };
    
    if (verifyTree <int> (adoptor0, 
        {
            numNodes_a0,
            depth_a0,
            levels_a0,
            numChildren_a0,
            ids_a0,
            numDescendants_a0,
            parents_a0
        }) == false)
        return Quality::Test::FAIL;

    // verify adoptor 1
    size_t numNodes_a1 = 1;
    size_t depth_a1 = 1;
    size_t levels_a1[] =            { 1 };
    size_t numChildren_a1[] =       { 0 };
    size_t ids_a1[] =               { 9 };
    size_t numDescendants_a1[] =    { 0 };
    size_t parents_a1[] =           { 0 };
    
    if (verifyTree <int> (adoptor1, 
        {
            numNodes_a1,
            depth_a1,
            levels_a1,
            numChildren_a1,
            ids_a1,
            numDescendants_a1,
            parents_a1
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE_ALL;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (7, "remove root (adopt)") {
    auto myTree = TREE_INIT (7, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);  

    // create new tree (adoptor)
    auto adoptor = TREE_INIT (8, int);

    // remove
    myTree-> TREE_PEEK_SET_ROOT;
    adoptor-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);
    adoptor-> TREE_DUMP;   
    // original tree
    myTree-> TREE_DUMP;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    // verify adoptor
    size_t numNodes = 8;
    size_t depth = 6;
    size_t levels[] =               { 1, 2, 2, 3, 4, 4, 5, 6 };
    size_t numChildren[] =          { 2,
                                      1,      0,
                                      2,
                                      1,      0,
                                      1,
                                      0 };
    size_t ids[] =                  { 1, 2, 3, 4, 5, 6, 7, 8 };
    size_t numDescendants[] =       { 7,
                                      5,      0,
                                      4,
                                      2,      0,
                                      1,
                                      0 };
    size_t parents[] =              { 0, 1, 1, 2, 4, 4, 5, 7 };
    
    if (verifyTree <int> (adoptor, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE_ALL;                                       
    return Quality::Test::PASS;
}

LIB_TEST_CASE (8, "custom type") {
    // define custom type
    typedef struct {
        float thresh;
        int box;
    }s_customType;

    auto myTree = TREE_INIT (8, s_customType);
    std::pair <size_t, s_customType> input[] = {
        { 1, { 0.1, 100 } },
        { 2, { 0.2, 200 } },
        { 3, { 0.3, 300 } }
    };

    // create tree
    for (auto i : input)
        myTree-> TREE_ADD_ROOT (i.first, i.second);
    
    // define custom dump lambda
    auto lambda_customType = [](s_customType* currentNode, std::ostream& ost) { 
                                    ost << currentNode-> thresh << ", " << currentNode-> box;
                                };
    myTree-> TREE_DUMP_CUSTOM (lambda_customType);

    size_t numNodes = 3;
    size_t depth = 3;
    size_t levels[] =               { 1, 2, 3 };
    size_t numChildren[] =          { 1, 1, 0 };
    size_t ids[] =                  { 3, 2, 1 };
    size_t numDescendants[] =       { 2, 1, 0 };
    size_t parents[] =              { 0, 3, 2 };
    
    if (verifyTree <s_customType> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL; 

    TREE_CLOSE (8);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "path between nodes") {
    auto myTree = TREE_INIT (9, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},
                                        {3, 30}, 
                                        {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                        {8, 80},                   {9, 90},
                                        {10, 8}, {11, 9} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);  

    std::pair <size_t, size_t> pathPairs[] =    { {1, 10}, {10, 1}, {1, 1}, {4, 7}, {9, 11}, {11, 9}, {8, 9} };
    std::vector <size_t> truePaths[] =          { { 10, 8, 4, 3, 2, 1 },
                                                  { 10, 8, 4, 3, 2, 1 },
                                                  { 1 },
                                                  { 7, 3, 4 },
                                                  { 11, 8, 4, 3, 7, 9 },
                                                  { 11, 8, 4, 3, 7, 9 },
                                                  { 9, 7, 3, 4, 8 } };

    int idx = 0;
    for (auto [start, end] : pathPairs) {
        std::vector <size_t> path = myTree-> TREE_PATH (start, end);

        // check size
        if (path.size() != truePaths[idx].size())
            return Quality::Test::FAIL;

        // check generated path
        for (size_t i = 0; i < path.size(); i++) {
            if (path[i] != truePaths[idx][i])
                return Quality::Test::FAIL;
        }
        idx++;
    }
    TREE_CLOSE (9);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (10, "swap nodes") {
    auto myTree = TREE_INIT (10, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20},
                                        {3, 30}, 
                                        {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                        {8, 80},                   {9, 90},
                                        {10, 8}, {11, 9},          {12, 5}, {13, 6}, {14, 7} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);     

    myTree-> TREE_PEEK_SET (9);
    myTree-> TREE_ADD_CHILD (input[11].first, input[11].second); 
    myTree-> TREE_ADD_CHILD (input[12].first, input[12].second); 
    myTree-> TREE_ADD_CHILD (input[13].first, input[13].second);   

    std::pair <size_t, size_t> swapIds[]  = { {1, 1}, {11, 11}, {8, 9}, {9, 8}, {1, 7}, {7, 1}, {5, 6} };

    const size_t numNodes = 14;
    size_t depth = 6;
    size_t levels[] =                       { 1, 2, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 6, 6 };
    size_t numChildren[] =                  { 1, 1, 4, 1, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0 };  
    size_t ids[][numNodes] =                { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
                                              { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
                                              { 1, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13, 14 }, 
                                              { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
                                              { 7, 2, 3, 4, 5, 6, 1, 8, 9, 10, 11, 12, 13, 14 },
                                              { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
                                              { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10, 11, 12, 13, 14 } };                          
    size_t numDescendants[] =               { 13, 12, 11, 3, 0, 0, 4, 2, 3, 0, 0, 0, 0, 0 };          
    size_t parents[][numNodes] =            { { 0, 1, 2, 3, 3, 3, 3, 4, 7, 8, 8, 9, 9, 9 },
                                              { 0, 1, 2, 3, 3, 3, 3, 4, 7, 8, 8, 9, 9, 9 },
                                              { 0, 1, 2, 3, 3, 3, 3, 4, 7, 9, 9, 8, 8, 8 },
                                              { 0, 1, 2, 3, 3, 3, 3, 4, 7, 8, 8, 9, 9, 9 },
                                              { 0, 7, 2, 3, 3, 3, 3, 4, 1, 8, 8, 9, 9, 9 },
                                              { 0, 1, 2, 3, 3, 3, 3, 4, 7, 8, 8, 9, 9, 9 },
                                              { 0, 1, 2, 3, 3, 3, 3, 4, 7, 8, 8, 9, 9, 9 } };
    
    int idx = 0;
    for (auto [idA, idB] : swapIds) {
        myTree-> TREE_SWAP (idA, idB);

        if (verifyTree <int> (myTree, 
            {
                numNodes,
                depth,
                levels,
                numChildren,
                ids[idx],
                numDescendants,
                parents[idx]
            }) == false)
            return Quality::Test::FAIL;   

        idx++;
    }

    TREE_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "unique node ids") {
    auto myTree = TREE_INIT (11, int);
    std::pair <int, int> input[] = { {1, 10}, 
                                     {1, 20},   // <----- invalid id, user needs to make sure ids are unique 
                                     {2, 30}, 
                                     {3, 40} };

    myTree-> TREE_ADD_ROOT (input[0].first, input[1].second);

    // check if id in doubt (id #1) already exists in tree
    myTree-> TREE_PEEK_SET (1);

    // if peek at id#1 returns NULL, then the id doesn't exist in the tree
    if (myTree-> TREE_PEEK_NODE== NULL)
        return Quality::Test::FAIL;
    
    TREE_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (12, "reset tree") {
    auto myTree = TREE_INIT (12, int);
    std::pair <int, int> input[] = { {0, 10}, {1, 20}, {2, 30}, {3, 40} };

    // create tree
    for (auto i : input)
        myTree-> TREE_ADD_ROOT (i.first, i.second);

    myTree-> TREE_RESET;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (13, "tree traversal starting from id") {
    auto myTree = TREE_INIT (13, int);
    std::pair <size_t, int> input[] =   { {1, 10}, 
                                          {2, 20},
                                          {3, 30}, 
                                          {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                          {8, 80},                   {9, 90},
                                          {10, 8}, {11, 9},          {12, 5}, {13, 6}, {14, 7},
                                                   {15, 1},
                                                   {16, 2},
                                                   {17, 3} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);     

    myTree-> TREE_PEEK_SET (9);
    myTree-> TREE_ADD_CHILD (input[11].first, input[11].second); 
    myTree-> TREE_ADD_CHILD (input[12].first, input[12].second); 
    myTree-> TREE_ADD_CHILD (input[13].first, input[13].second);  

    myTree-> TREE_PEEK_SET (11);
    myTree-> TREE_ADD_CHILD (input[14].first, input[14].second); 

    myTree-> TREE_PEEK_SET (15);
    myTree-> TREE_ADD_CHILD (input[15].first, input[15].second); 

    myTree-> TREE_PEEK_SET (16);
    myTree-> TREE_ADD_CHILD (input[16].first, input[16].second);  

    const size_t numNodes = 17;
    size_t peekSetIds[] =           { 1, 3, 5, 7, 12, 14, 17 };
    int outputs[][numNodes] =       { { 10, 20, 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 5, 6, 7, 1, 2, 3 },
                                      { 7, 1, 2, 3 },
                                      { 3 } };

    int k = 0;
    for ( auto i : peekSetIds ) {
        // set peek
        myTree-> TREE_PEEK_SET (i);
        // traverse till end of tree
        int idx = 0;
        while (myTree-> TREE_PEEK_NODE != NULL) {

            if (myTree-> TREE_PEEK_NODE-> data != outputs[k][idx])
                return Quality::Test::FAIL;

            myTree-> TREE_PEEK_SET_NEXT;
            idx++;
        }
        k++;
    }

    TREE_CLOSE (13);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (14, "tree update (add child) amid traversal") {
    auto myTree = TREE_INIT (14, int);
    std::pair <size_t, int> input[] =   { {1, 10}, 
                                          {2, 20},
                                          {3, 30}, 
                                          {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                          {8, 80},                   {9, 90},
                                          {10, 8}, {11, 9},          {12, 5}, {13, 6}, {14, 7},
                                                   {15, 1},
                                                   {16, 2},
                                                   {17, 3} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);     

    myTree-> TREE_PEEK_SET (9);
    myTree-> TREE_ADD_CHILD (input[11].first, input[11].second); 
    myTree-> TREE_ADD_CHILD (input[12].first, input[12].second); 
    myTree-> TREE_ADD_CHILD (input[13].first, input[13].second);  

    myTree-> TREE_PEEK_SET (11);
    myTree-> TREE_ADD_CHILD (input[14].first, input[14].second); 

    myTree-> TREE_PEEK_SET (15);
    myTree-> TREE_ADD_CHILD (input[15].first, input[15].second); 

    myTree-> TREE_PEEK_SET (16);
    myTree-> TREE_ADD_CHILD (input[16].first, input[16].second);  

    // update tree in the middle of traversal
    size_t peekSetId = 3;
    std::pair <size_t, int> newChild = { 69, 69 };
    int output[]  = { 30, 40, 50, 60, 70, 69, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 };

    myTree-> TREE_PEEK_SET (peekSetId);

    int idx = 0;
    bool addChildPending = true;

    while (myTree-> TREE_PEEK_NODE != NULL) {
        if (myTree-> TREE_PEEK_NODE-> data != output[idx])
               return Quality::Test::FAIL;

        // add a new child to node id#3 while traversing
        if (addChildPending) {
            myTree-> TREE_PEEK_SET (3);
            myTree-> TREE_ADD_CHILD (newChild.first, newChild.second);             
            addChildPending = false;

            /* the stepper queue has not been updated with the newly added child (yet), to do this we could either reset 
             * the peek to the parent (that can see the change) or to the root
             */
            myTree-> TREE_PEEK_SET (3);
            continue;
        }
        myTree-> TREE_PEEK_SET_NEXT;
        idx++;        
    }

    myTree-> TREE_DUMP;

    TREE_CLOSE (14);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (15, "tree update (remove node) amid traversal") {
    auto myTree = TREE_INIT (15, int);
    std::pair <size_t, int> input[] =   { {1, 10}, 
                                          {2, 20},
                                          {3, 30}, 
                                          {4, 40}, {5, 50}, {6, 60}, {7, 70},
                                          {8, 80},                   {9, 90},
                                          {10, 8}, {11, 9},          {12, 5}, {13, 6}, {14, 7},
                                                   {15, 1},
                                                   {16, 2},
                                                   {17, 3} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second);

    myTree-> TREE_PEEK_SET (3);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second); 
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second);

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[8].first, input[8].second);

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_CHILD (input[9].first, input[9].second); 
    myTree-> TREE_ADD_CHILD (input[10].first, input[10].second);     

    myTree-> TREE_PEEK_SET (9);
    myTree-> TREE_ADD_CHILD (input[11].first, input[11].second); 
    myTree-> TREE_ADD_CHILD (input[12].first, input[12].second); 
    myTree-> TREE_ADD_CHILD (input[13].first, input[13].second);  

    myTree-> TREE_PEEK_SET (11);
    myTree-> TREE_ADD_CHILD (input[14].first, input[14].second); 

    myTree-> TREE_PEEK_SET (15);
    myTree-> TREE_ADD_CHILD (input[15].first, input[15].second); 

    myTree-> TREE_PEEK_SET (16);
    myTree-> TREE_ADD_CHILD (input[16].first, input[16].second);  

    // start traversing from id#3
    size_t peekSetId = 3;
    int output[]  = { 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 };

    myTree-> TREE_PEEK_SET (peekSetId);

    int idx = 0;
    bool removeChildPending = true;
    size_t removeId = 3;

    while (myTree-> TREE_PEEK_NODE != NULL) {
        if (myTree-> TREE_PEEK_NODE-> data != output[idx])
               return Quality::Test::FAIL;

        if (removeChildPending) {
            myTree-> TREE_PEEK_SET (removeId);
            myTree-> TREE_REMOVE;         
            removeChildPending = false;

            /* the stepper queue has not been updated with the newly added child (yet), to do this we could either reset 
             * the peek to the parent (that can see the change) or to the root
             */
            myTree-> TREE_PEEK_SET (3);
            continue;
        }
        myTree-> TREE_PEEK_SET_NEXT;
        idx++;        
    }

    myTree-> TREE_DUMP;

    TREE_CLOSE (15);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (16, "append tree") {
    auto myTree = TREE_INIT (16, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };    

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 

    // append from same tree (ids will be unique after append operation)
    size_t appendId = 1;
    size_t adoptId = 7;

    myTree-> TREE_PEEK_SET (adoptId);
    auto subTree = myTree-> TREE_ADOPT.first;

    myTree-> TREE_PEEK_SET (appendId);
    myTree-> TREE_APPEND (subTree);

    size_t numNodes = 8;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 2, 3, 3, 4, 4 };
    size_t numChildren[] =          { 3, 1, 0, 1, 2, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 7, 4, 8, 5, 6 };
    size_t numDescendants[] =       { 7, 3, 0, 1, 2, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 1, 2, 7, 4, 4 };

    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents
        }) == false)
        return Quality::Test::FAIL;  

    TREE_CLOSE (16);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (17, "empty tree") {
    auto myTree = TREE_INIT (17, int);
    TREE_MGR_DUMP;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    // peek set with id
    myTree-> TREE_PEEK_SET (2);
    if (myTree-> TREE_PEEK_NODE != NULL)
        return Quality::Test::FAIL; 

    // add child
    if (myTree-> TREE_ADD_CHILD (1, 10) != false)
        return Quality::Test::FAIL;

    // append an empty tree/node
    if (myTree-> TREE_APPEND (NULL) != false)
        return Quality::Test::FAIL;

    // add parent
    if (myTree-> TREE_ADD_PARENT (1, 10) != false)
        return Quality::Test::FAIL;

    // remove
    myTree-> TREE_PEEK_SET_ROOT;
    if ((myTree-> TREE_REMOVE).second != false)
        return Quality::Test::FAIL;

    // adopt
    if ((myTree-> TREE_ADOPT).second != false)
        return Quality::Test::FAIL;

    // swap with invalid ids
    if (myTree-> TREE_SWAP (1, 2) == true)
        return Quality::Test::FAIL;

    // find path with invalid ids
    if (myTree-> TREE_PATH (1, 2).size() != 0)
        return Quality::Test::FAIL;

    // importing/resetting an empty tree to an empty tree shouldn't change anything
    myTree-> TREE_IMPORT (NULL);
    myTree-> TREE_RESET;

    myTree-> TREE_DUMP;    

    TREE_CLOSE (17);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (18, "invalid peek set next") {
    auto myTree = TREE_INIT (18, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 

    // reset peek position to test
    myTree-> TREE_PEEK_SET (99);

    if (myTree-> TREE_PEEK_NODE != NULL)
        return Quality::Test::FAIL;  

    // we will get the depth of tree if we passed an invalid id as peek position
    if (myTree-> TREE_PEEK_LEVEL != 6)
        return Quality::Test::FAIL;         

    // this should set peek params to { NULL, 0 }
    myTree-> TREE_PEEK_SET_NEXT;

    if (myTree-> TREE_PEEK_NODE != NULL)
        return Quality::Test::FAIL;  

    if (myTree-> TREE_PEEK_LEVEL != 0)
        return Quality::Test::FAIL;   

    TREE_CLOSE (18);
    return Quality::Test::PASS;
}

#if ENABLE_INTERNAL_TESTS == 1
LIB_TEST_CASE (19, "[internal validation] peek pair after peek set") {
    auto myTree = TREE_INIT (19, int);
    size_t depth = 6;

    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 

    /* sticky peek pairs could either be
     * { node, level }  -> if the id passed in is valid
     * { NULL, depth }  -> otherwise, returns a NULL pointer and depth of tree
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output = { 4, 3 };

    myTree-> TREE_PEEK_SET (output.first);

    if (myTree-> TREE_PEEK_NODE-> id != output.first ||
        myTree-> TREE_PEEK_LEVEL != output.second)     
        return Quality::Test::FAIL;   

    myTree-> TREE_DUMP;

    // invalid id
    myTree-> TREE_PEEK_SET (100);

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != depth)
        return Quality::Test::FAIL;

    myTree-> TREE_DUMP;

    TREE_CLOSE (19);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (20, "[internal validation] peek pair after peek set root") {
    auto myTree = TREE_INIT (20, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 

    /* sticky peek pairs could either be
     * { NULL, 0 }      -> if root doesn't exist
     * { root, 1 }      -> valid root with level set to 1
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output = { 1, 1 };

    myTree-> TREE_PEEK_SET_ROOT;

    if (myTree-> TREE_PEEK_NODE-> id != output.first ||
        myTree-> TREE_PEEK_LEVEL != output.second)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    // invalid root
    myTree-> TREE_RESET;
    myTree-> TREE_PEEK_SET_ROOT;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)
        return Quality::Test::FAIL;

    myTree-> TREE_DUMP;

    TREE_CLOSE (20);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (21, "[internal validation] peek pair after peek set next") {
    auto myTree = TREE_INIT (21, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs could either be
     * { NULL, 0 }      -> end of tree reached
     * { node, level}   -> node with level
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output = { 8, 6 };

    myTree-> TREE_PEEK_SET (output.first);

    if (myTree-> TREE_PEEK_NODE-> id != output.first ||
        myTree-> TREE_PEEK_LEVEL != output.second)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    // end of tree
    myTree-> TREE_PEEK_SET_NEXT;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)
        return Quality::Test::FAIL;

    myTree-> TREE_DUMP;

    TREE_CLOSE (21);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (22, "[internal validation] peek pair after add parent") {
    auto myTree = TREE_INIT (22, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pair set to
     * { node, level }  -> of child node if we are adding to the root, else
     * { node, level }  -> of newly added parent
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output_0 = { 1, 1 };

    myTree-> TREE_PEEK_SET (output_0.first);
    myTree-> TREE_ADD_PARENT (0, 69);

    if (myTree-> TREE_PEEK_NODE-> id != output_0.first ||
        myTree-> TREE_PEEK_LEVEL != output_0.second)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    std::pair <size_t, size_t> output_1 = { 9, 7 };

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_ADD_PARENT (output_1.first, 90);

    if (myTree-> TREE_PEEK_NODE-> id != output_1.first ||
        myTree-> TREE_PEEK_LEVEL != output_1.second)
        return Quality::Test::FAIL;

    myTree-> TREE_DUMP;

    TREE_CLOSE (22);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (23, "[internal validation] peek pair after remove/adopt") {
    auto myTree = TREE_INIT (23, int);
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs set to
     * { NULL, 0 }      -> reset pair
    */

    myTree-> TREE_PEEK_SET (8);
    myTree-> TREE_REMOVE;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADOPT;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    TREE_CLOSE (23);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (24, "[internal validation] peek pair after depth") {
    auto myTree = TREE_INIT (24, int);
    size_t depth = 6;

    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs set to
     * { NULL, depth }
    */

    myTree-> TREE_DEPTH;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != depth)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    TREE_CLOSE (24);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (25, "[internal validation] peek pair after swap") {
    auto myTree = TREE_INIT (25, int);
    size_t depth = 6;

    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs set to
     * { nodeB, levelB }    -> if valid
     * { NULL, depth }      -> otherwise
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output = { 4, 3 };

    myTree-> TREE_SWAP (3, output.first);

    if (myTree-> TREE_PEEK_NODE-> id != output.first ||
        myTree-> TREE_PEEK_LEVEL != output.second)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    myTree-> TREE_SWAP (22, 23);

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != depth)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    TREE_CLOSE (25);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (26, "[internal validation] peek pair after path") {
    auto myTree = TREE_INIT (26, int);
    size_t depth = 6;
    
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs set to
     * { nodeB, levelB }    -> if valid
     * { NULL, depth }      -> otherwise
    */

    // output peek node id, peek level
    std::pair <size_t, size_t> output = { 4, 3 };

    myTree-> TREE_PATH (3, output.first);

    if (myTree-> TREE_PEEK_NODE-> id != output.first ||
        myTree-> TREE_PEEK_LEVEL != output.second)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    myTree-> TREE_SWAP (22, 23);

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != depth)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    TREE_CLOSE (26);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (27, "[internal validation] peek pair after import/reset") {
    auto myTree = TREE_INIT (27, int);   
    std::pair <size_t, int> input[] = { {1, 10}, 
                                        {2, 20}, {3, 30},
                                        {4, 40}, 
                                        {5, 50}, {6, 60},
                                        {7, 70},  
                                        {8, 80} };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_CHILD (input[1].first, input[1].second);
    myTree-> TREE_ADD_CHILD (input[2].first, input[2].second); 

    myTree-> TREE_PEEK_SET (2);
    myTree-> TREE_ADD_CHILD (input[3].first, input[3].second);    

    myTree-> TREE_PEEK_SET (4);
    myTree-> TREE_ADD_CHILD (input[4].first, input[4].second);  
    myTree-> TREE_ADD_CHILD (input[5].first, input[5].second); 

    myTree-> TREE_PEEK_SET (5);
    myTree-> TREE_ADD_CHILD (input[6].first, input[6].second);  

    myTree-> TREE_PEEK_SET (7);
    myTree-> TREE_ADD_CHILD (input[7].first, input[7].second); 
    
    /* sticky peek pairs set to
     * { NULL, 0 }      -> reset pair
    */

    myTree-> TREE_IMPORT (NULL);

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    myTree-> TREE_RESET;

    if (myTree-> TREE_PEEK_NODE != NULL ||
        myTree-> TREE_PEEK_LEVEL != 0)     
        return Quality::Test::FAIL;  

    myTree-> TREE_DUMP;

    TREE_CLOSE (27);
    return Quality::Test::PASS;
}
#endif  // ENABLE_INTERNAL_TESTS

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    LIB_TEST_RUN_ALL;

    return 0;
}