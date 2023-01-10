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
#include "../../LibTest/inc/LibTest.h"

using namespace Collections;

// test params to verify tree structure
typedef struct {
    size_t  numNodes;
    size_t  depth;
    size_t* levels;
    size_t* numChildren;

    size_t* ids;
    size_t* numDescendants;
    size_t* parents;

    size_t* nullPos;
}s_groundTruth;

/* this method is used to verify the input tree structure by comparing various parameters against the  ground truth params
 * passed in by the user through s_groundTruth. The following methods are used to verify the given tree
 * 
 * [utils]
 * TREE_SIZE
 * TREE_DEPTH
 * 
 * [set]
 * TREE_PEEK_SET_ROOT
 * TREE_PEEK_SET_NEXT       [*]
 * 
 * [execute]
 * TREE_PEEK_NODE           [*]
 * TREE_PEEK_LEVEL          [*]
 * TREE_PEEK_CHILD_COUNT    [*]
 * TREE_PEEK_IS_END
 * 
 * [*]  -> these methods are only called when the tree is non-empty
*/
template <typename T>
bool verifyTree (Memory::Tree <T>* tree, s_groundTruth params) {
    if (tree-> TREE_SIZE    != params.numNodes ||   
        tree-> TREE_DEPTH   != params.depth)
        return false;

    int idx = 0;
    tree-> TREE_PEEK_SET_ROOT;

    // loop till end of tree
    while (!tree-> TREE_PEEK_IS_END) {

        if (tree-> TREE_PEEK_LEVEL                      != params.levels[idx] ||
            tree-> TREE_PEEK_CHILD_COUNT                != params.numChildren[idx])
            return false;

        if (tree-> TREE_PEEK_NODE                       != NULL) {
            if (tree-> TREE_PEEK_NODE-> id              != params.ids[idx] ||
                tree-> TREE_PEEK_NODE-> numDescendants  != params.numDescendants[idx])
                return false;

            // skip checking the parent of the root node
            if (tree-> TREE_PEEK_NODE-> parent          != NULL) {
                if (tree-> TREE_PEEK_NODE-> parent-> id != params.parents[idx])
                    return false;
            }

            // verify node at this position is not NULL
            if (params.nullPos[idx] != 0)
                return false;
        }

        // NULL node position validate
        else {
            if (params.nullPos[idx] != 1)
                return false;
        }
            
        // move to next node (level-wise traversal)
        tree-> TREE_PEEK_SET_NEXT;
        // ground truth array index
        idx++;
    }
    return true;
}

LIB_TEST_CASE (16, "empty tree tests") {
    auto myTree = TREE_INIT (16, int);

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    /* test all available methods on an empty tree
     *
     * [set]
     * TREE_PEEK_SET
     * TREE_PEEK_SET_ROOT
     * TREE_PEEK_SET_NEXT
     * 
     * [execute]
     * TREE_PEEK_NODE
     * TREE_PEEK_LEVEL
     * TREE_PEEK_CHILD_COUNT
     * TREE_PEEK_IS_END
     * 
     * TREE_ADD_CHILD
     * TREE_ADD_NULL_CHILD
     * TREE_APPEND
     * TREE_ADD_PARENT
     * TREE_REMOVE_NODE
     * TREE_ADOPT_NODE
     * TREE_REMOVE
     * TREE_ADOPT
     * 
     * [utils]
     * TREE_SWAP
     * TREE_PATH
     * TREE_TAILS
     * TREE_IMPORT
     * TREE_RESET
     * 
     * [dump]
     * TREE_MGR_DUMP
     * TREE_DUMP
    */

    // set peek to root and move next
    myTree-> TREE_PEEK_SET_ROOT;
    myTree-> TREE_PEEK_SET_NEXT;

    if (myTree-> TREE_PEEK_NODE             != NULL     ||
        myTree-> TREE_PEEK_LEVEL            != 0        ||
        myTree-> TREE_PEEK_CHILD_COUNT      != 0        ||
        myTree-> TREE_PEEK_IS_END           != true)
        return Quality::Test::FAIL; 

    // seek peek to id
    myTree-> TREE_PEEK_SET (2);

    if (myTree-> TREE_PEEK_NODE             != NULL     ||
        myTree-> TREE_PEEK_LEVEL            != 0        ||
        myTree-> TREE_PEEK_CHILD_COUNT      != 0        ||
        myTree-> TREE_PEEK_IS_END           != true)
        return Quality::Test::FAIL; 

    if (myTree-> TREE_ADD_CHILD (1, 10)     != false    ||
        myTree-> TREE_ADD_NULL_CHILD        != false    ||
        myTree-> TREE_APPEND (NULL)         != false    ||
        myTree-> TREE_ADD_PARENT (1, 10)    != false)
        return Quality::Test::FAIL;

    if (myTree-> TREE_REMOVE_NODE.first     != NULL     ||
        myTree-> TREE_REMOVE_NODE.second    != false    ||
        myTree-> TREE_ADOPT_NODE.first      != NULL     ||
        myTree-> TREE_ADOPT_NODE.second     != false)
        return Quality::Test::FAIL;

    if (myTree-> TREE_REMOVE.first          != NULL     ||
        myTree-> TREE_REMOVE.second         != false    ||
        myTree-> TREE_ADOPT.first           != NULL     ||
        myTree-> TREE_ADOPT.second          != false)
        return Quality::Test::FAIL;

    if (myTree-> TREE_SWAP (1, 2)           != false    ||
        myTree-> TREE_PATH (1, 2).size()    != 0        ||
        myTree-> TREE_TAILS.size()          != 0)
        return Quality::Test::FAIL;

    // importing/resetting an empty tree to an empty tree shouldn't change anything
    myTree-> TREE_IMPORT (NULL);
    myTree-> TREE_RESET;

    TREE_MGR_DUMP;
    myTree-> TREE_DUMP;    

    TREE_CLOSE (16);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (17, "peek pair validation") {
    auto myTree = TREE_INIT (17, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 }, { 3, 30 },
                                        { 4, 40 }, 
                                        { 5, 50 }, { 6, 60 },
                                        { 7, 70 },  
                                        { 8, 80 } };  

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

    for (auto i : { 1, 3, 6, 8 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {3, 30}                 {NULL}
     *                          |                   |
     *                          {4, 40}             {NULL}
     *                          |
     *                  ---------------------
     *                  |                   |
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    /* peek pair values after each operation are tested here
     * { NULL, 0 }          end of tree
     * { NULL, depth }      invalid id
     * { node, level }      otherwise
    */

    // peek set root test
    auto emptyTree = TREE_INIT (18, int);

    emptyTree-> TREE_PEEK_SET_ROOT;
    if (emptyTree-> TREE_PEEK_NODE      != NULL             ||
        emptyTree-> TREE_PEEK_LEVEL     != 0)
        return Quality::Test::FAIL;  

    std::pair <size_t, size_t> rootPair     = { 1, 1 };

    myTree-> TREE_PEEK_SET_ROOT;
    if (myTree-> TREE_PEEK_NODE-> id    != rootPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != rootPair.second)
        return Quality::Test::FAIL;   

    // peek set test
    std::pair <size_t, size_t> validPair    = { 5, 4 };
    size_t invalidId                        = 99;
    size_t depth                            = 7;

    myTree-> TREE_PEEK_SET (validPair.first);
    if (myTree-> TREE_PEEK_NODE-> id    != validPair.first  ||
        myTree-> TREE_PEEK_LEVEL        != validPair.second)
        return Quality::Test::FAIL;

    myTree-> TREE_PEEK_SET (invalidId);
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != depth)
        return Quality::Test::FAIL;    

    // peek set next test
    std::pair <size_t, size_t> lastPair     = { 8, 6 }; 

    myTree-> TREE_PEEK_SET (lastPair.first);  
    myTree-> TREE_PEEK_SET_NEXT; 
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != depth)
        return Quality::Test::FAIL; 

    myTree-> TREE_PEEK_SET_NEXT;
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != 0)
        return Quality::Test::FAIL;  

    // add parent test
    size_t peekIds[]                        = { 1, 3 };
    std::pair <size_t, int> newParents[]    = { { 100, 0 }, 
                                                { 101, 0 } };

    myTree-> TREE_PEEK_SET (peekIds[0]);
    myTree-> TREE_ADD_PARENT (newParents[0].first, newParents[0].second);

    rootPair = { 100, 1 };
    
    if (myTree-> TREE_PEEK_NODE-> id    != rootPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != rootPair.second)
        return Quality::Test::FAIL;

    myTree-> TREE_PEEK_SET (peekIds[1]);
    myTree-> TREE_ADD_PARENT (newParents[1].first, newParents[1].second);

    if (myTree-> TREE_PEEK_NODE-> id    != newParents[1].first   ||
        myTree-> TREE_PEEK_LEVEL        != 3)   // the new parent (id# 101 is at level 3) 
        return Quality::Test::FAIL;

    /*                                  {100, 0}
     *                                  |
     *                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {101, 0}                {NULL}
     *                          |                   |
     *                          {4, 40}             {3, 30} 
     *                          |                   |
     *                  ---------------------       |
     *                  |                   |       |
     *               {5, 50}             {6, 60}    {NULL}
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    // remove/adopt node test
    size_t removeIds[]                      = { 100, 101, 3 };

    myTree-> TREE_PEEK_SET (removeIds[0]);
    std::pair <size_t, size_t> lastPeekPair = { 100, 1 };

    myTree-> TREE_REMOVE_NODE;
    if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
        return Quality::Test::FAIL; 

    myTree-> TREE_PEEK_SET (removeIds[1]);
    myTree-> TREE_REMOVE_NODE;
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != 0)
        return Quality::Test::FAIL;  

    /*                                  {100, 0}
     *                                  |
     *                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {3, 30}                 {NULL}
     *                          |                   |
     *                          {4, 40}             {NULL} 
     *                          |                   
     *                  ---------------------       
     *                  |                   |       
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    // remove test
    myTree-> TREE_PEEK_SET (removeIds[2]);
    myTree-> TREE_REMOVE;
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != 0)
        return Quality::Test::FAIL; 

    /*                                  {100, 0}
     *                                  |
     *                                  {1, 10}
     *                                  |
     *                          ---------------------
     *                          |                   |
     *                          {2, 20}             {NULL}
     *                          |                   
     *                          {4, 40}             
     *                          |                   
     *                  ---------------------       
     *                  |                   |       
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    // depth test
    myTree-> TREE_PEEK_SET (2);
    lastPeekPair                            = { 2, 3 };
    
    myTree-> TREE_DEPTH;
    if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
        return Quality::Test::FAIL;   

    // swap test
    std::pair <size_t, size_t> swapIds[] = { { 5, 5 },
                                             { 0, 9 },
                                             { 7, 8 },
                                             { 1, 4 } };

    for (auto i : swapIds) {
        myTree-> TREE_SWAP (i.first, i.second);
        if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
            myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
            return Quality::Test::FAIL;       
    }

    // path test
    std::pair <size_t, size_t> pathPairs[]  = { { 5, 2 },
                                                { 4, 9 },
                                                { 3, 3 } };

    for (auto i : pathPairs) {
        myTree-> TREE_PATH (i.first, i.second);
        if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
            myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
            return Quality::Test::FAIL; 
    }

    // tail ids test
    myTree-> TREE_TAILS;
    if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
        return Quality::Test::FAIL;  

    // import/reset test
    myTree-> TREE_RESET;
    if (myTree-> TREE_PEEK_NODE         != NULL             ||
        myTree-> TREE_PEEK_LEVEL        != 0)
        return Quality::Test::FAIL;  

    // dump test
    myTree-> TREE_ADD_ROOT (1, 1);
    myTree-> TREE_PEEK_SET_ROOT;
    lastPeekPair = { 1, 1 };

    /*                                  {1, 1}
     *                                  |
    */

    myTree-> TREE_DUMP;
    if (myTree-> TREE_PEEK_NODE-> id    != lastPeekPair.first   ||
        myTree-> TREE_PEEK_LEVEL        != lastPeekPair.second)
        return Quality::Test::FAIL;    

    TREE_CLOSE (17);
    TREE_CLOSE (18);
    return Quality::Test::PASS;
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
    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 }, 
                                        { 2, 30 }, 
                                        { 3, 40 } };
    // create tree
    for (auto i : input)
        myTree-> TREE_ADD_ROOT (i.first, i.second);
    
    // add null child
    myTree-> TREE_PEEK_SET (0);
    myTree-> TREE_ADD_NULL_CHILD;

    /*                                  {3, 40}
     *                                  |
     *                                  {2, 30}
     *                                  |
     *                                  {1, 20}
     *                                  |
     *                                  {0, 10}
     *                                  |
     *                                  {NULL}
    */

    myTree-> TREE_DUMP;

    size_t numNodes = 4;
    size_t depth = 5;
    /*                                              
     * null node position corresponds to this index when 
     * you traverse the tree. The tree verify function
     * does not check numChildren, id, numDescendants and
     * parents for the null child, thus it is set as 0 at
     * this index. 
     * 
     * The nullPos at this index is set to '1' to indicate
     * that the null child is at this position in the tree
     *                                                  |
     *                                                  |
     *                                                  V
    */
    size_t levels[] =               { 1, 2, 3, 4,       5 };
    size_t numChildren[] =          { 1, 1, 1, 0,       0 };
    size_t ids[] =                  { 3, 2, 1, 0,       0 };
    size_t numDescendants[] =       { 3, 2, 1, 0,       0 };
    size_t parents[] =              { 0, 3, 2, 1,       0 }; 
    size_t nullPos[] =              { 0, 0, 0, 0,       1 };  
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "add children") {
    auto myTree = TREE_INIT (2, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },              { 3, 30 }, 
                                        { 4, 40 }, { 5, 50 },   { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                         { 9, 90 } };

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

    for (auto i : { 1, 3, 5 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                    {2, 20}                   {3, 30}                 {NULL}
     *                    |                         |
     *          ----------------------          -----------------------------------------
     *          |                    |          |                   |                   |
     *          {4, 40}         {5, 50}         {6, 60}             {7, 70}             {NULL}      
     *          |               |                                   |
     *          {8, 80}         {NULL}                              {9, 90}
    */

    myTree-> TREE_DUMP;

    size_t numNodes = 9;                                        
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4 };
    size_t numChildren[] =          { 2, 2, 2, 0, 1, 0, 0, 1, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 0, 4, 5, 6, 7, 0, 8, 0, 9 };
    size_t numDescendants[] =       { 8, 3, 3, 0, 1, 0, 0, 1, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 0, 2, 2, 3, 3, 0, 4, 0, 7 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "add parent") {
    auto myTree = TREE_INIT (3, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },              { 3, 30 }, 
                                        { 4, 40 }, { 5, 50 },   { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                         { 9, 90 } };

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

    for (auto i : { 1, 3, 5 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                    ---------------------------------------------------
     *                    |                         |                       |
     *                    {2, 20}                   {3, 30}                 {NULL}
     *                    |                         |
     *          ----------------------          -----------------------------------------
     *          |                    |          |                   |                   |
     *          {4, 40}         {5, 50}         {6, 60}             {7, 70}             {NULL}      
     *          |                    |                              |
     *          {8, 80}         {NULL}                              {9, 90}
    */

 
    std::pair <size_t, int> newNodes[] = { { 0, 9 }, { 16, 79 }, { 25, 49 } };

    // add nodes
    idx = 0;
    for (auto i : { 1, 8, 4 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_PARENT (newNodes[idx].first, newNodes[idx].second);
        idx++;
    }

    /*                                  {0, 9}
     *                                  |
     *                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                    {2, 20}                   {3, 30}                 {NULL}
     *                    |                         |
     *          ----------------------          -----------------------------------------
     *          |                    |          |                   |                   |
     *          {25, 49}        {5, 50}         {6, 60}             {7, 70}             {NULL}      
     *          |               |                                   |
     *          {4, 40}         {NULL}                              {9, 90}
     *          |
     *          {16, 79}
     *          |
     *          {8, 80} 
    */

    myTree-> TREE_DUMP;

    size_t numNodes = 12;
    size_t depth = 7;
    size_t levels[] =               { 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
    size_t numChildren[] =          { 1, 2, 2, 2, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0 };
    size_t ids[] =                  { 0, 1, 2, 3, 0, 25, 5, 6, 7, 0, 4, 0, 9, 16, 8 };
    size_t numDescendants[] =       { 11, 10, 5, 3, 0, 3, 0, 0, 1, 0, 2, 0, 0, 1, 0 };
    size_t parents[] =              { 0, 0, 1, 1, 0, 2, 2, 3, 3, 0, 25, 0, 7, 4, 16 };
    size_t nullPos[] =              { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;
   
    TREE_CLOSE (3); 
    return Quality::Test::PASS;
}

LIB_TEST_CASE (4, "remove with id (no adopt)") {
    auto myTree = TREE_INIT (4, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 100 } };

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

    for (auto i : { 2, 3, 9 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |                |                   |                  |               |               |
     *          {3, 30}          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                       |
     *          ----------------------                                  |
     *          |                    |                                  |
     *          {8, 80}         {NULL}                                  {9, 90}
     *          |                                                       |
     *          {10, 100}                                               {NULL}
    */

    // remove nodes
    for (auto i : { 6, 3, 7 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_REMOVE;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *              ----------------------------------------
     *              |                   |                  |
     *              {4, 40}             {5, 50}            {NULL}       
    */

    myTree-> TREE_DUMP;

    size_t numNodes = 4;
    size_t depth = 3;
    size_t levels[] =               { 1, 2, 3, 3, 3 };
    size_t numChildren[] =          { 1, 2, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 4, 5, 0 };
    size_t numDescendants[] =       { 3, 2, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 2, 2, 0 };
    size_t nullPos[] =              { 0, 0, 0, 0, 1 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (4);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (5, "remove root (no adopt)") {
    auto myTree = TREE_INIT (5, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 100 } };

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

    for (auto i : { 2, 3, 9 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |                |                   |                  |               |               |
     *          {3, 30}          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                       |
     *          ----------------------                                  |
     *          |                    |                                  |
     *          {8, 80}         {NULL}                                  {9, 90}
     *          |                                                       |
     *          {10, 100}                                               {NULL}
    */

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
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (5);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (6, "remove with id (adopt)") {
    auto myTree = TREE_INIT (6, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, 
                                        { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 8 }, { 11, 9 } };

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

    for (auto i : { 3, 4, 9, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ------------------------------------------------------------------------
     *          |                   |                  |               |               |
     *          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 | 
     *          {10, 8}        {11, 9}                                 {NULL}
     *          |
     *          {NULL}
    */

    // create new trees (adoptors)
    auto adoptor0 = TREE_INIT (7, int);
    auto adoptor1 = TREE_INIT (8, int);

    // remove
    myTree-> TREE_PEEK_SET (4);
    adoptor0-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);

    myTree-> TREE_PEEK_SET (9);
    adoptor1-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ----------------------------------------------------
     *          |                  |               |               |
     *          {5, 50}            {6, 60}         {7, 70}         {NULL}
    */ 

    myTree-> TREE_DUMP;

    // verify original tree
    size_t numNodes = 6;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 4, 4, 4, 4 };
    size_t numChildren[] =          { 1, 1, 3, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 5, 6, 7, 0 };
    size_t numDescendants[] =       { 5, 4, 3, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 2, 3, 3, 3, 0 };
    size_t nullPos[] =              { 0, 0, 0, 0, 0, 0, 1 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    /*                                  {4, 40}            
     *                                  |                                                      
     *                        ----------------------                                 
     *                        |                    |                                 
     *                        {8, 80}              {NULL}                                 
     *                        |                                                      
     *              ----------------------                                 
     *              |                    |                                  
     *              {10, 8}              {11, 9}                                 
     *              |
     *              {NULL}
    */

    adoptor0-> TREE_DUMP;

    // verify adoptor 0
    size_t numNodes_a0 = 4;
    size_t depth_a0 = 4;
    size_t levels_a0[] =            { 1, 2, 2, 3, 3, 4 };
    size_t numChildren_a0[] =       { 1, 2, 0, 0, 0, 0 };
    size_t ids_a0[] =               { 4, 8, 0, 10, 11, 0 };
    size_t numDescendants_a0[] =    { 3, 2, 0, 0, 0, 0 };
    size_t parents_a0[] =           { 0, 4, 0, 8, 8, 0 };
    size_t nullPos_a0[] =           { 0, 0, 1, 0, 0, 1 };
    
    if (verifyTree <int> (adoptor0, 
        {
            numNodes_a0,
            depth_a0,
            levels_a0,
            numChildren_a0,
            ids_a0,
            numDescendants_a0,
            parents_a0,
            nullPos_a0
        }) == false)
        return Quality::Test::FAIL;

    /*                                  {9, 90}            
     *                                  | 
     *                                  {NULL}
    */

    adoptor1-> TREE_DUMP;

    // verify adoptor 1
    size_t numNodes_a1 = 1;
    size_t depth_a1 = 2;
    size_t levels_a1[] =            { 1, 2 };
    size_t numChildren_a1[] =       { 0, 0 };
    size_t ids_a1[] =               { 9, 0 };
    size_t numDescendants_a1[] =    { 0, 0 };
    size_t parents_a1[] =           { 0, 0 };
    size_t nullPos_a1[] =           { 0, 1 };

    if (verifyTree <int> (adoptor1, 
        {
            numNodes_a1,
            depth_a1,
            levels_a1,
            numChildren_a1,
            ids_a1,
            numDescendants_a1,
            parents_a1,
            nullPos_a1
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE_ALL;
    return Quality::Test::PASS;
}

LIB_TEST_CASE (7, "remove root (adopt)") {
    auto myTree = TREE_INIT (7, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 }, { 3, 30 },
                                        { 4, 40 }, 
                                        { 5, 50 }, { 6, 60 },
                                        { 7, 70 },  
                                        { 8, 80 } };

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

    for (auto i : { 1, 3, 6, 8 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {3, 30}                 {NULL}
     *                          |                   |
     *                          {4, 40}             {NULL}
     *                          |
     *                  ---------------------
     *                  |                   |
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    // create new tree (adoptor)
    auto adoptor = TREE_INIT (8, int);

    // remove
    myTree-> TREE_PEEK_SET_ROOT;
    adoptor-> TREE_IMPORT ((myTree-> TREE_ADOPT).first);

    // empty tree
    myTree-> TREE_DUMP;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }) == false)
        return Quality::Test::FAIL;

    /*                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {3, 30}                 {NULL}
     *                          |                   |
     *                          {4, 40}             {NULL}
     *                          |
     *                  ---------------------
     *                  |                   |
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    adoptor-> TREE_DUMP; 

    // verify adoptor
    size_t numNodes = 8;
    size_t depth = 7;
    size_t levels[] =               { 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7 };
    size_t numChildren[] =          { 2, 1, 0, 0, 2, 0, 1, 0, 1, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 0, 4, 0, 5, 6, 7, 0, 8, 0 };
    size_t numDescendants[] =       { 7, 5, 0, 0, 4, 0, 2, 0, 1, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 0, 2, 0, 4, 4, 5, 0, 7, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 };
    
    if (verifyTree <int> (adoptor, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
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

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_NULL_CHILD;

    /*                                  {3, {0.3, 300}}
     *                                  |
     *                                  {2, {0.2, 200}}
     *                                  |
     *                                  {1, {0.1, 100}}
     *                                  |
     *                                  {NULL}
    */

    // define custom dump lambda
    auto lambda_customType = [](s_customType* currentNode, std::ostream& ost) { 
                                    ost << currentNode-> thresh << ", " << currentNode-> box;
                                };
    myTree-> TREE_DUMP_CUSTOM (lambda_customType);

    size_t numNodes = 3;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 4 };
    size_t numChildren[] =          { 1, 1, 0, 0 };
    size_t ids[] =                  { 3, 2, 1, 0 };
    size_t numDescendants[] =       { 2, 1, 0, 0 };
    size_t parents[] =              { 0, 3, 2, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1 };
    
    if (verifyTree <s_customType> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL; 

    TREE_CLOSE (8);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "path between nodes") {
    auto myTree = TREE_INIT (9, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, 
                                        { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 8 }, { 11, 9 } };

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

    for (auto i : { 3, 4, 9, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ------------------------------------------------------------------------
     *          |                   |                  |               |               |
     *          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 | 
     *          {10, 8}        {11, 9}                                 {NULL}
     *          |
     *          {NULL}
    */

    myTree-> TREE_DUMP;

    std::pair <size_t, size_t> pathPairs[] =    { { 1, 10 },
                                                  { 10, 1 }, 
                                                  { 1, 1 }, 
                                                  { 4, 7 }, 
                                                  { 9, 11 }, 
                                                  { 11, 9 }, 
                                                  { 8, 9 }, 
                                                  { 8, 10 } };
    std::vector <size_t> truePaths[] =          { { 10, 8, 4, 3, 2, 1 },
                                                  { 10, 8, 4, 3, 2, 1 },
                                                  { 1 },
                                                  { 7, 3, 4 },
                                                  { 11, 8, 4, 3, 7, 9 },
                                                  { 11, 8, 4, 3, 7, 9 },
                                                  { 9, 7, 3, 4, 8 },
                                                  { 10, 8 } };

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
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, 
                                        { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 8 }, { 11, 9 },            { 12, 5 }, { 13, 6 }, { 14, 7 } };

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

    for (auto i : { 3, 4, 10, 11, 12 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ------------------------------------------------------------------------
     *          |                   |                  |               |               |
     *          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                     -------------------------- 
     *          |                    |                     |           |            |
     *          {10, 8}        {11, 9}                     {12, 5}     {13, 6}      {14, 7}
     *          |                    |                     |
     *          {NULL}          {NULL}                     {NULL}
    */

    myTree-> TREE_DUMP;

    std::pair <size_t, size_t> swapIds[]  = { { 1, 1 }, 
                                              { 11, 11 }, 
                                              { 8, 9 }, 
                                              { 9, 8 }, 
                                              { 1, 7 }, 
                                              { 7, 1 }, 
                                              { 5, 6 },
                                              { 8, 11 },
                                              { 12, 9 } };

    size_t numNodes = 14;
    size_t depth = 7;
    size_t levels[] =                       { 1, 2, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7 };
    size_t numChildren[] =                  { 1, 1, 4, 1, 0, 0, 1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0 };  
    size_t ids[][19] =                      { { 1, 2, 3, 4, 5, 6, 7, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 5, 6, 7, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 5, 6, 7, 0, 9, 0, 8, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 5, 6, 7, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 7, 2, 3, 4, 5, 6, 1, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 5, 6, 7, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 6, 5, 7, 0, 8, 0, 9, 10, 11, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 6, 5, 7, 0, 11, 0, 9, 10, 8, 12, 13, 14, 0, 0, 0 },
                                              { 1, 2, 3, 4, 6, 5, 7, 0, 11, 0, 12, 10, 8, 9, 13, 14, 0, 0, 0 } };                          
    size_t numDescendants[] =               { 13, 12, 11, 3, 0, 0, 4, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0 };          
    size_t parents[][19] =                  { { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 9, 9, 8, 8, 8, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 7, 2, 3, 3, 3, 3, 0, 4, 0, 1, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 8, 8, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 11, 11, 9, 9, 9, 0, 0, 0 },
                                              { 0, 1, 2, 3, 3, 3, 3, 0, 4, 0, 7, 11, 11, 12, 12, 12, 0, 0, 0 } };
    size_t nullPos[] =                      { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
    
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
                parents[idx],
                nullPos
            }) == false)
            return Quality::Test::FAIL;   

        idx++;
    }

    TREE_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "unique node ids") {
    auto myTree = TREE_INIT (11, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 1, 20 },   // <----- invalid id, user needs to make sure ids are unique 
                                        { 2, 30 }, 
                                        { 3, 40 } };

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
    std::pair <size_t, int> input[] = { { 0, 10 }, 
                                        { 1, 20 }, 
                                        { 2, 30 }, 
                                        { 3, 40 } };

    // create tree
    for (auto i : input)
        myTree-> TREE_ADD_ROOT (i.first, i.second);

    myTree-> TREE_PEEK_SET (0);
    myTree-> TREE_ADD_NULL_CHILD;

    /*                                  {3, 40}
     *                                  |
     *                                  {2, 30}
     *                                  |
     *                                  {1, 20}
     *                                  |
     *                                  {0, 10}
     *                                  |
     *                                  {NULL}
    */

    myTree-> TREE_RESET;

    // empty tree
    myTree-> TREE_DUMP;

    if (verifyTree <int> (myTree, 
        {
            0,
            0,
            NULL,
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
    std::pair <size_t, int> input[] =   { { 1, 10 }, 
                                          { 2, 20 },
                                          { 3, 30 }, 
                                          { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                          { 8, 80 },                       { 9, 90 },
                                          { 10, 8 }, { 11, 9 },            { 12, 5 }, { 13, 6 }, { 14, 7 },
                                                     { 15, 1 },
                                                     { 16, 2 },
                                                     { 17, 3 } };

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

    for (auto i : { 3, 4, 10, 12 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ------------------------------------------------------------------------
     *          |                   |                  |               |               |
     *          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                     -------------------------- 
     *          |                    |                     |           |            |
     *          {10, 8}        {11, 9}                     {12, 5}     {13, 6}      {14, 7}
     *          |                    |                     |
     *          {NULL}         {15, 1}                     {NULL}
     *                               |
     *                         {16, 2}
     *                               |
     *                         {17, 3}
    */

    myTree-> TREE_DUMP;

    const size_t numNodes = 17;
    size_t peekSetIds[] =           { 1, 3, 5, 7, 9, 10, 12, 13, 14, 17 };
    int outputs[][numNodes] =       { { 10, 20, 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 90, 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 8, 9, 5, 6, 7, 1, 2, 3 },
                                      { 5, 6, 7, 1, 2, 3 },
                                      { 6, 7, 1, 2, 3 },
                                      { 7, 1, 2, 3 },
                                      { 3 } };

    // verify tree data instead of tree structure
    int k = 0;
    for (auto i : peekSetIds) {
        // set peek
        myTree-> TREE_PEEK_SET (i);

        // output index for kth input
        int idx = 0;
        // traverse till end of tree
        while (!myTree-> TREE_PEEK_IS_END) {

            if (myTree-> TREE_PEEK_NODE != NULL) {
                if (myTree-> TREE_PEEK_NODE-> data != outputs[k][idx++])
                    return Quality::Test::FAIL;
            }

            myTree-> TREE_PEEK_SET_NEXT;
        }
        k++;
    }

    TREE_CLOSE (13);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (14, "tree update (add child) amid traversal") {
    auto myTree = TREE_INIT (14, int);
    std::pair <size_t, int> input[] =   { { 1, 10 }, 
                                          { 2, 20 },
                                          { 3, 30 }, 
                                          { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                          { 8, 80 },                       { 9, 90 },
                                          { 10, 8 }, { 11, 9 },            { 12, 5 }, { 13, 6 }, { 14, 7 },
                                                     { 15, 1 },
                                                     { 16, 2 },
                                                     { 17, 3 } };

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

    for (auto i : { 3, 4, 10, 12 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *                                  {3, 30}
     *                                  |
     *          ------------------------------------------------------------------------
     *          |                   |                  |               |               |
     *          {4, 40}             {5, 50}            {6, 60}         {7, 70}         {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                     -------------------------- 
     *          |                    |                     |           |            |
     *          {10, 8}        {11, 9}                     {12, 5}     {13, 6}      {14, 7}
     *          |                    |                     |
     *          {NULL}         {15, 1}                     {NULL}
     *                               |
     *                         {16, 2}
     *                               |
     *                         {17, 3}
    */

    myTree-> TREE_DUMP;

    size_t peekSetId = 3;
    std::pair <size_t, int> newChild = { 69, 69 };
    int outdated_output[]  = { 30, 40, 50, 60, 70, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 };
    int updated_output[]   = { 30, 40, 50, 60, 70, 69, 80, 90, 8, 9, 5, 6, 7, 1, 2, 3 };

    myTree-> TREE_PEEK_SET (peekSetId);
    /* any update to the tree that is done after the peek has been set may not be seen while traversing, this is due to
     * the stepper queue not being updated with the newly added node
    */
    myTree-> TREE_ADD_CHILD (newChild.first, newChild.second); 

    int idx = 0;
    while (!myTree-> TREE_PEEK_IS_END) {
        if (myTree-> TREE_PEEK_NODE != NULL) {
            if (myTree-> TREE_PEEK_NODE-> data != outdated_output[idx++])
                return Quality::Test::FAIL;
        }
        myTree-> TREE_PEEK_SET_NEXT;       
    }

    /* to refresh the stepper queue, we either set the peek back to the root or atleast back to the parent that can see
     * the tree update
    */
    myTree-> TREE_PEEK_SET (peekSetId);

    idx = 0;
    while (!myTree-> TREE_PEEK_IS_END) {
        if (myTree-> TREE_PEEK_NODE != NULL) {
            if (myTree-> TREE_PEEK_NODE-> data != updated_output[idx++])
                return Quality::Test::FAIL;
        }
        myTree-> TREE_PEEK_SET_NEXT;       
    }

    TREE_CLOSE (14);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (15, "append tree") {
    auto myTree = TREE_INIT (15, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 }, { 3, 30 },
                                        { 4, 40 }, 
                                        { 5, 50 }, { 6, 60 },
                                        { 7, 70 },  
                                        { 8, 80 } };    

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

    for (auto i : { 1, 3, 6, 8 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                          ---------------------------------------------
     *                          |                   |                       |
     *                          {2, 20}             {3, 30}                 {NULL}
     *                          |                   |
     *                          {4, 40}             {NULL}
     *                          |
     *                  ---------------------
     *                  |                   |
     *               {5, 50}             {6, 60}    
     *                  |                   |
     *               {7, 70}             {NULL}
     *                  |
     *               {8, 80}  
     *                  |
     *               {NULL}                 
    */

    // append from same tree
    size_t appendId = 1;
    size_t adoptId = 7;

    myTree-> TREE_PEEK_SET (adoptId);
    auto subTree = myTree-> TREE_ADOPT.first;

    myTree-> TREE_PEEK_SET (appendId);
    myTree-> TREE_APPEND (subTree);

    /*                                  {1, 10}
     *                                  |
     *                          -------------------------------------------------------------
     *                          |                   |                       |               |
     *                          {2, 20}             {3, 30}                 {NULL}          {7, 70}
     *                          |                   |                                       |
     *                          {4, 40}             {NULL}                                  {8, 80}
     *                          |                                                           |
     *                  ---------------------                                               |
     *                  |                   |                                               |   
     *               {5, 50}             {6, 60}                                            {NULL}
     *                                      |
     *                                   {NULL}        
    */

    myTree-> TREE_DUMP;   

    size_t numNodes = 8;
    size_t depth = 5;
    size_t levels[] =               { 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5 };
    size_t numChildren[] =          { 3, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 0, 7, 4, 0, 8, 5, 6, 0, 0 };
    size_t numDescendants[] =       { 7, 3, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 0, 1, 2, 0, 7, 4, 4, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 };

    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;  

    TREE_CLOSE (15);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (18,"remove node (no adopt)") {
    auto myTree = TREE_INIT (18, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 100 } };

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

    for (auto i : { 2, 3, 8, 9, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |                   |                  |               |               |                |
     *          {3, 30}             {4, 40}            {5, 50}         {6, 60}         {7, 70}          {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 | 
     *          {10, 100}       {NULL}                                 {NULL}
     *          |
     *          {NULL}
    */

    // remove nodes
    for (auto i : { 6, 3, 7, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_REMOVE_NODE;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |               |                   |                   |               |               |
     *          {8, 80}         {NULL}              {4, 40}             {5, 50}         {9, 90}         {NULL}
     *          |                                                                       |
     *          ----------------                                                        |
     *          |               |                                                       |
     *          {NULL}          {NULL}                                                  {NULL}
    */  

    myTree-> TREE_DUMP;

    size_t numNodes = 6;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4 };
    size_t numChildren[] =          { 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 8, 0, 4, 5, 9, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 2, 0, 2, 2, 2, 0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1 }; 
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    return Quality::Test::PASS;    
}

// dependent test case (uses tree instance from a different test case)
LIB_TEST_CASE (19, "remove root node (no adopt)") {
    auto myTree = GET_TREE (18, int);
  
    // remove root node, should return false
    myTree-> TREE_PEEK_SET_ROOT;
    if (myTree-> TREE_REMOVE_NODE.second == true)
        return Quality::Test::FAIL;

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |               |                   |                   |               |               |
     *          {8, 80}         {NULL}              {4, 40}             {5, 50}         {9, 90}         {NULL}
     *          |                                                                       |
     *          ----------------                                                        |
     *          |               |                                                       |
     *          {NULL}          {NULL}                                                  {NULL}
    */  

    myTree-> TREE_DUMP;

    size_t numNodes = 6;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4 };
    size_t numChildren[] =          { 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 8, 0, 4, 5, 9, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 2, 0, 2, 2, 2, 0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1 }; 
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (18);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (20, "remove node (with adopt)") {
    auto myTree = TREE_INIT (20, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 100 } };

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

    for (auto i : { 2, 3, 8, 9, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |                   |                  |               |               |                |
     *          {3, 30}             {4, 40}            {5, 50}         {6, 60}         {7, 70}          {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 | 
     *          {10, 100}       {NULL}                                 {NULL}
     *          |
     *          {NULL}
    */

    // remove nodes
    for (auto i : { 3, 4, 5, 6, 7 }) {
        myTree-> TREE_PEEK_SET (i);

        auto adoptor = TREE_INIT (21, int);
        adoptor-> TREE_IMPORT ((myTree-> TREE_ADOPT_NODE).first);
        adoptor-> TREE_DUMP;

        TREE_CLOSE (21);
    } 

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------
     *          |               |                   |               |
     *          {8, 80}         {NULL}              {9, 90}         {NULL}
     *          |                                   |
     *          ----------------                    |
     *          |               |                   |
     *          {10, 100}       {NULL}              {NULL}
     *          |
     *          {NULL}
    */    

    myTree-> TREE_DUMP;

    size_t numNodes = 5;
    size_t depth = 5;
    size_t levels[] =               { 1, 2, 3, 3, 3, 3, 4, 4, 4, 5 };
    size_t numChildren[] =          { 1, 2, 1, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 8, 0, 9, 0, 10, 0, 0, 0 };
    size_t numDescendants[] =       { 4, 3, 1, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 2, 0, 2, 0, 8, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 0, 1, 0, 1, 1, 1 };
    
    if (verifyTree <int> (myTree, 
        {
            numNodes,
            depth,
            levels,
            numChildren,
            ids,
            numDescendants,
            parents,
            nullPos
        }) == false)
        return Quality::Test::FAIL;

    TREE_CLOSE (20);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (21, "tail node ids") {
    auto myTree = TREE_INIT (21, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 },
                                        { 3, 30 }, { 4, 40 }, { 5, 50 }, { 6, 60 }, { 7, 70 },
                                        { 8, 80 },                       { 9, 90 },
                                        { 10, 100 } };

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

    for (auto i : { 2, 3, 8, 9, 10 }) {
        myTree-> TREE_PEEK_SET (i);
        myTree-> TREE_ADD_NULL_CHILD;
    }

    /*                                  {1, 10}
     *                                  |
     *                                  {2, 20}
     *                                  |
     *          -----------------------------------------------------------------------------------------
     *          |                   |                  |               |               |                |
     *          {3, 30}             {4, 40}            {5, 50}         {6, 60}         {7, 70}          {NULL}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 |
     *          {8, 80}         {NULL}                                 {9, 90}
     *          |                                                      |
     *          ----------------------                                 |
     *          |                    |                                 | 
     *          {10, 100}       {NULL}                                 {NULL}
     *          |
     *          {NULL}
    */  
 
    // tail ids when you traverse level order
    std::vector <size_t> output  = { 4, 5, 7, 9, 10 };
    std::vector <size_t> tailIds = myTree-> TREE_TAILS;

    if (tailIds.size() != output.size())
        return Quality::Test::FAIL;

    for (size_t i = 0; i < tailIds.size(); i++) {
        if (tailIds[i] != output[i])
            return Quality::Test::FAIL;
    }

    TREE_CLOSE (21);
    return Quality::Test::PASS;  
}

LIB_TEST_CASE (22, "tail node id on 1 node tree") {
    auto myTree = TREE_INIT (22, int);
    std::pair <size_t, int> input[] = { { 1, 10 } };

    // create tree
    myTree-> TREE_ADD_ROOT (input[0].first, input[0].second);

    myTree-> TREE_PEEK_SET (1);
    myTree-> TREE_ADD_NULL_CHILD;

    /*                                  {1, 10}
     *                                  |
     *                                  {NULL}
    */  
 
    myTree-> TREE_DUMP;

    // tail ids when you traverse level order
    std::vector <size_t> output  = { 1 };
    std::vector <size_t> tailIds = myTree-> TREE_TAILS;

    if (tailIds.size() != output.size())
        return Quality::Test::FAIL;

    for (size_t i = 0; i < tailIds.size(); i++) {
        if (tailIds[i] != output[i])
            return Quality::Test::FAIL;
    }

    TREE_CLOSE (22);
    return Quality::Test::PASS;  
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    LIB_TEST_RUN_ALL;

    return 0;
}