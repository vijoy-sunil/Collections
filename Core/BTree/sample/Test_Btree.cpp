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
#include "../inc/BTree.h"
#include "../../../Common/LibTest/inc/LibTest.h"

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

template <typename T>
bool verifyTree (Memory::BTree <T>* tree, s_groundTruth params) {
    if (tree-> BTREE_SIZE   != params.numNodes ||   
        tree-> BTREE_DEPTH  != params.depth)
        return false;

    int idx = 0;
    tree-> BTREE_PEEK_SET_ROOT;

    // loop till end of tree
    while (!tree-> BTREE_PEEK_IS_END) {

        if (tree-> BTREE_PEEK_LEVEL                         != params.levels[idx] ||
            tree-> BTREE_PEEK_CHILD_COUNT                   != params.numChildren[idx])
            return false;

        if (tree-> BTREE_PEEK_NODE                          != NULL) {
            if (tree-> BTREE_PEEK_NODE-> id                 != params.ids[idx] ||
                tree-> BTREE_PEEK_NODE-> numDescendants     != params.numDescendants[idx])
                return false;

            // skip checking the parent of the root node
            if (tree-> BTREE_PEEK_NODE-> parent             != NULL) {
                if (tree-> BTREE_PEEK_NODE-> parent-> id    != params.parents[idx])
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
        tree-> BTREE_PEEK_SET_NEXT;
        // ground truth array index
        idx++;
    }
    return true;
}

LIB_TEST_CASE (0, "empty tree tests") {
    auto myTree = BTREE_INIT (0, int);

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

    /* methods testsed here
     *
     * [mgr]
     * BTREE_MGR_DUMP
     * 
     * [set]
     * BTREE_PEEK_SET
     * BTREE_PEEK_SET_ROOT
     * BTREE_PEEK_SET_NEXT
     *
     * [peek]
     * BTREE_PEEK_LEFT
     * BTREE_PEEK_RIGHT
     * 
     * [update]
     * BTREE_ADD_LEFT
     * BTREE_ADD_RIGHT
     * BTREE_ADD_TAIL
     * BTREE_ADD_PARENT
     * BTREE_REMOVE_NODE
     * BTREE_ADOPT_NODE
     * BTREE_REMOVE
     * BTREE_ADOPT
     * 
     * [utils]
     * BTREE_DEEPEST
     * BTREE_DUMP
    */

    myTree-> BTREE_PEEK_SET (1);

    if (myTree-> BTREE_PEEK_LEFT            != NULL     ||
        myTree-> BTREE_PEEK_RIGHT           != NULL)
        return Quality::Test::FAIL;

    myTree-> BTREE_PEEK_SET_ROOT;
    myTree-> BTREE_PEEK_SET_NEXT;

    if (myTree-> BTREE_PEEK_LEFT            != NULL     ||
        myTree-> BTREE_PEEK_RIGHT           != NULL)
        return Quality::Test::FAIL;

    std::pair <size_t, int> input[] = { { 1, 10 } };
    if (myTree-> BTREE_ADD_LEFT   (input[0].first, input[0].second)     != false    ||
        myTree-> BTREE_ADD_RIGHT  (input[0].first, input[0].second)     != false    ||
        myTree-> BTREE_ADD_TAIL   (input[0].first, input[0].second)     != false    ||
        myTree-> BTREE_ADD_PARENT (input[0].first, input[0].second      != false))
        return Quality::Test::FAIL;

    if (myTree-> BTREE_REMOVE_NODE.second   != false    ||
        myTree-> BTREE_ADOPT_NODE.second    != false    ||
        myTree-> BTREE_REMOVE.second        != false    ||
        myTree-> BTREE_ADOPT.second         != false)
        return Quality::Test::FAIL;

    if (myTree-> BTREE_DEEPEST              != NULL)
        return Quality::Test::FAIL;

    myTree-> BTREE_DUMP;
    BTREE_MGR_DUMP;

    BTREE_CLOSE (0);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (1, "left-right tests") {
    auto myTree = BTREE_INIT (1, int);
    std::pair <size_t, int> input = { 1, 10 };

    // create tree
    myTree-> BTREE_ADD_ROOT (input.first, input.second);

    /*                                  {1, 10}
     *                                  |
     *                      -------------------------
     *                      |                       |
     *                      {NULL}             {NULL}
    */

    myTree-> BTREE_PEEK_SET (1);
    if (myTree-> BTREE_PEEK_LEFT    != NULL ||
        myTree-> BTREE_PEEK_RIGHT   != NULL)
        return Quality::Test::FAIL;

    // add children
    std::pair <size_t, size_t> left  = { 5, 50 };
    std::pair <size_t, size_t> right = { 6, 60 }; 

    myTree-> BTREE_ADD_LEFT (left.first, left.second);
    myTree-> BTREE_ADD_RIGHT (right.first, right.second);

    /*                                  {1, 10}
     *                                  |
     *                      -------------------------
     *                      |                       |
     *                      {5, 50}           {6, 60}
     *                      |                       |
     *          -------------------------   -------------------------
     *          |                       |   |                       |
     *          {NULL}             {NULL}   {NULL}             {NULL}
    */

    if (myTree-> BTREE_PEEK_LEFT-> id       != 5 ||
        myTree-> BTREE_PEEK_RIGHT-> id      != 6)
        return Quality::Test::FAIL;  

    // add children again
    if (myTree-> BTREE_ADD_LEFT (left.first, left.second)       != false  ||
        myTree-> BTREE_ADD_RIGHT (right.first, right.second)    != false)
        return Quality::Test::FAIL;

    myTree-> BTREE_DUMP;

    size_t numNodes = 3;                                        
    size_t depth = 3;
    size_t levels[] =               { 1, 2, 2, 3, 3, 3, 3 };
    size_t numChildren[] =          { 2, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 5, 6, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 2, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 1, 1, 1, 1 };

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

    BTREE_CLOSE (1);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (14, "peek pair validation") {
    auto myTree = BTREE_INIT (14, int);
    std::pair <size_t, int> root                = { 1, 10 };

    // initial peek value
    if (myTree-> BTREE_PEEK_NODE    != NULL    ||
        myTree-> BTREE_PEEK_LEVEL   != 0)
        return Quality::Test::FAIL;

    // add root (peek has not been set yet)
    myTree-> BTREE_ADD_ROOT (root.first, root.second);
    if (myTree-> BTREE_PEEK_NODE    != NULL    ||
        myTree-> BTREE_PEEK_LEVEL   != 0)
        return Quality::Test::FAIL;

    /*                                  {1, 10}
     *                                  |
     *                      -------------------------
     *                      |                       |
     *                      {NULL}             {NULL}
    */

    // add parent
    std::pair <size_t, int> newParent           = { 2, 20 };
    std::pair <size_t, size_t> parentPair       = { 2, 1 };

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_PARENT (newParent.first, newParent.second); 

    if (myTree-> BTREE_PEEK_NODE-> id   != parentPair.first   ||
        myTree-> BTREE_PEEK_LEVEL       != parentPair.second)
        return Quality::Test::FAIL;

    /*                                  {2, 20}
     *                                  |
     *                          -----------------
     *                          |               |
     *                          {1, 10}         {NULL}
     *                          |
     *                  -----------------
     *                  |               |
     *                  {NULL}          {NULL}
    */

    // remove/adopt node
    size_t removeIds[] = { 2, 1 };
    myTree-> BTREE_PEEK_SET (removeIds[0]);
    myTree-> BTREE_REMOVE_NODE;

    /*                                  {1, 10}
     *                                  |
     *                      -------------------------
     *                      |                       |
     *                      {NULL}             {NULL}
    */

    if (myTree-> BTREE_PEEK_NODE    != NULL    ||
        myTree-> BTREE_PEEK_LEVEL   != 0)
        return Quality::Test::FAIL;

    // remove
    myTree-> BTREE_PEEK_SET (removeIds[1]);
    myTree-> BTREE_REMOVE;
    
    if (myTree-> BTREE_PEEK_NODE    != NULL    ||
        myTree-> BTREE_PEEK_LEVEL   != 0)
        return Quality::Test::FAIL;

    // repopulate tree for next test
    myTree-> BTREE_ADD_ROOT (root.first, root.second);
    myTree-> BTREE_PEEK_SET_ROOT;
    myTree-> BTREE_ADD_PARENT (newParent.first, newParent.second); 

    /*                                  {2, 20}
     *                                  |
     *                          -----------------
     *                          |               |
     *                          {1, 10}         {NULL}
     *                          |
     *                  -----------------
     *                  |               |
     *                  {NULL}          {NULL}
    */

    // deepest node
    std::pair <size_t, size_t> lastPeekPair     = { 2, 1 };
    myTree-> BTREE_DEEPEST;
    if (myTree-> BTREE_PEEK_NODE-> id   != lastPeekPair.first    ||
        myTree-> BTREE_PEEK_LEVEL       != lastPeekPair.second)
        return Quality::Test::FAIL;   

    myTree-> BTREE_DUMP;

    BTREE_CLOSE (14);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (2, "add to root") {
    auto myTree = BTREE_INIT (2, int);
    std::pair <size_t, int> input[] = { { 1, 10 }, 
                                        { 2, 20 }, 
                                        { 3, 30 }, 
                                        { 4, 40 } };
    // create tree
    for (auto i : input)
        myTree-> BTREE_ADD_ROOT (i.first, i.second);

    /*                                  {4, 40}
     *                                  |
     *                      -------------------------
     *                      |                       |
     *                      {3, 30}                 {NULL}
     *                      |
     *              -----------------
     *              |               |
     *              {2, 20}         {NULL}
     *              |
     *      -----------------
     *      |               |
     *      {1, 10}         {NULL}
     *      |
     *      -----------------
     *      |               |
     *      {NULL}          {NULL}
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 4;
    size_t depth = 5;
    size_t levels[] =               { 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    size_t numChildren[] =          { 1, 1, 0, 1, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 4, 3, 0, 2, 0, 1, 0, 0, 0 };
    size_t numDescendants[] =       { 3, 2, 0, 1, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 4, 0, 3, 0, 2, 0, 0, 0 };   
    size_t nullPos [] =             { 0, 0, 1, 0, 1, 0, 1, 1, 1 };
    
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

    BTREE_CLOSE (2);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (3, "binary tree using add left, add right") {
    auto myTree = BTREE_INIT (3, int);
    std::pair <size_t, int> root    =   { 1, 10 };
    std::pair <size_t, int> input[] = { { 2, 20 },            { 3, 30 },
                                        { 4, 40 },            { 5, 50 },
                                        { 6, 60 },            { 7, 70 },
                                        { 8, 80 }, { 9, 90 }, { 10, 1 }, { 11, 2 },
                                                   { 12, 3 }, { 13, 4 } };

    // create tree
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_LEFT (input[0].first, input[0].second);
    myTree-> BTREE_ADD_RIGHT (input[1].first, input[1].second);

    myTree-> BTREE_PEEK_SET (2);
    myTree-> BTREE_ADD_LEFT (input[2].first, input[2].second);

    myTree-> BTREE_PEEK_SET (3);
    myTree-> BTREE_ADD_RIGHT (input[3].first, input[3].second);

    myTree-> BTREE_PEEK_SET (4);
    myTree-> BTREE_ADD_RIGHT (input[4].first, input[4].second);

    myTree-> BTREE_PEEK_SET (5);
    myTree-> BTREE_ADD_LEFT (input[5].first, input[5].second);

    myTree-> BTREE_PEEK_SET (6);
    myTree-> BTREE_ADD_RIGHT (input[7].first, input[7].second);
    myTree-> BTREE_ADD_LEFT (input[6].first, input[6].second);

    myTree-> BTREE_PEEK_SET (7);
    myTree-> BTREE_ADD_LEFT (input[8].first, input[8].second);
    myTree-> BTREE_ADD_RIGHT (input[9].first, input[9].second);

    myTree-> BTREE_PEEK_SET (9);
    myTree-> BTREE_ADD_RIGHT (input[10].first, input[10].second);

    myTree-> BTREE_PEEK_SET (10);
    myTree-> BTREE_ADD_LEFT (input[11].first, input[11].second);

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {6, 60}                           {7, 70}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}           {11, 2}
     *                      |                       |   |                       |
     *          -------------           -------------   -------------           -------------
     *          |           |           |           |   |           |           |           |
     *     {NULL}      {NULL}      {NULL}     {12, 3}   {13, 4}     {NULL}      {NULL}      {NULL}
     *                                              |   |
     *                                  -------------   -------------
     *                                  |           |   |           |
     *                             {NULL}      {NULL}   {NULL}      {NULL}
    */

    myTree-> BTREE_DUMP;
    
    size_t numNodes = 13;
    size_t depth = 7;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 
                                      5, 5, 5, 5, 
                                      6, 6, 6, 6, 6, 6, 6, 6, 
                                      7, 7, 7, 7 };
    size_t numChildren[] =          { 2, 
                                      1, 1, 
                                      1, 0, 0, 1, 
                                      0, 2, 2, 0, 
                                      0, 1, 1, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0 };
    size_t ids[] =                  { 1, 
                                      2, 3, 
                                      4, 0, 0, 5, 
                                      0, 6, 7, 0, 
                                      8, 9, 10, 11, 
                                      0, 0, 0, 12, 13, 0, 0, 0, 
                                      0, 0, 0, 0 };
    size_t numDescendants[] =       { 12, 
                                      5, 5, 
                                      4, 0, 0, 4, 
                                      0, 3, 3, 0, 
                                      0, 1, 1, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0 };
    size_t parents[] =              { 0, 
                                      1, 1, 
                                      2, 0, 0, 3, 
                                      0, 4, 5, 0, 
                                      6, 6, 7, 7,
                                      0, 0, 0, 9, 10, 0, 0, 0, 
                                      0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 
                                      0, 0, 
                                      0, 1, 1, 0, 
                                      1, 0, 0, 1, 
                                      0, 0, 0, 0, 
                                      1, 1, 1, 0, 0, 1, 1, 1, 
                                      1, 1, 1, 1 };   

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

LIB_TEST_CASE (4, "binary tree from array") {
    auto myTree = BTREE_INIT (4, int);
    std::pair <size_t, int> root =      { 1, 10 };
    std::pair <size_t, int> input[] = { { 2, 20 },                  { 3, 30 },
                                        { 4, 40 },  { 5, 50 },      { 6, 60 }, { 7, 70 },
                                        { 8, 80 } };

    // create root 
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET_ROOT;
    for (auto i : input) {
        if (myTree-> BTREE_ADD_TAIL (i.first, i.second) == false)
            return Quality::Test::FAIL;
    }

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}           {5, 50}   {6, 60}            {7, 70}
     *                      |                       |   |                       |
     *      -----------------       -----------------   -----------------       -----------------
     *      |               |       |               |   |               |       |               |
     *      {8, 80}    {NULL}       {NULL}     {NULL}   {NULL}     {NULL}       {NULL}     {NULL}
     *      |
     *      -----------------
     *      |               |
     *      [NULL]     {NULL}
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 8;
    size_t depth = 5;
    size_t levels[] =               { 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5 };
    size_t numChildren[] =          { 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 7, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 2, 2, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };   
    size_t nullPos[] =              { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    
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

// use tree from test case #3
LIB_TEST_CASE (5, "remove node #1") {
    auto myTree = GET_BTREE (3, int);

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {6, 60}                           {7, 70}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}           {11, 2}
     *                      |                       |   |                       |
     *          -------------           -------------   -------------           -------------
     *          |           |           |           |   |           |           |           |
     *     {NULL}      {NULL}      {NULL}     {12, 3}   {13, 4}     {NULL}      {NULL}      {NULL}
     *                                              |   |
     *                                  -------------   -------------
     *                                  |           |   |           |
     *                             {NULL}      {NULL}   {NULL}      {NULL}
    */

    // remove root
    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_REMOVE_NODE;

    /*                                              {13, 4}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {6, 60}                           {7, 70}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}           {11, 2}
     *                      |                       |   |                       |
     *          -------------           -------------   -------------           -------------
     *          |           |           |           |   |           |           |           |
     *     {NULL}      {NULL}      {NULL}     {12, 3}   {NULL}      {NULL}      {NULL}      {NULL}
     *                                              |   
     *                                  -------------   
     *                                  |           |   
     *                             {NULL}      {NULL}   
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 12;
    size_t depth = 7;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 
                                      5, 5, 5, 5, 
                                      6, 6, 6, 6, 6, 6, 6, 6, 
                                      7, 7 };
    size_t numChildren[] =          { 2, 
                                      1, 1, 
                                      1, 0, 0, 1, 
                                      0, 2, 2, 0, 
                                      0, 1, 0, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0 };
    size_t ids[] =                  { 13, 
                                      2, 3, 
                                      4, 0, 0, 5, 
                                      0, 6, 7, 0, 
                                      8, 9, 10, 11, 
                                      0, 0, 0, 12, 0, 0, 0, 0, 
                                      0, 0 };
    size_t numDescendants[] =       { 11, 
                                      5, 4, 
                                      4, 0, 0, 3, 
                                      0, 3, 2, 0, 
                                      0, 1, 0, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0 };
    size_t parents[] =              { 0, 
                                      13, 13, 
                                      2, 0, 0, 3, 
                                      0, 4, 5, 0, 
                                      6, 6, 7, 7, 
                                      0, 0, 0, 9, 0, 0, 0, 0, 
                                      0, 0 };
    size_t nullPos[] =              { 0, 
                                      0, 0, 
                                      0, 1, 1, 0, 
                                      1, 0, 0, 1, 
                                      0, 0, 0, 0, 
                                      1, 1, 1, 0, 1, 1, 1, 1, 
                                      1, 1 };   
    
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

    size_t removeIds[] =            { 6, 7 };
    for (auto i : removeIds) {
        myTree-> BTREE_PEEK_SET (i);
        myTree-> BTREE_REMOVE_NODE;
    }

    /*                                              {13, 4}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {12, 3}                           {11, 2}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}            {NULL}
     *                      |                       |   |                       
     *          -------------           -------------   -------------           
     *          |           |           |           |   |           |           
     *     {NULL}      {NULL}      {NULL}      {NULL}   {NULL}      {NULL}                                                   
    */

    myTree-> BTREE_DUMP;

    size_t numNodes_0 = 10;
    size_t depth_0 = 6;
    size_t levels_0[] =             { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 
                                      5, 5, 5, 5, 
                                      6, 6, 6, 6, 6, 6 };
    size_t numChildren_0[] =        { 2, 
                                      1, 1, 
                                      1, 0, 0, 1, 
                                      0, 2, 1, 0, 
                                      0, 0, 0, 0, 
                                      0, 0, 0, 0, 0, 0 };
    size_t ids_0[] =                { 13, 
                                      2, 3, 
                                      4, 0, 0, 5, 
                                      0, 12, 11, 0, 
                                      8, 9, 10, 0, 
                                      0, 0, 0, 0, 0, 0 };
    size_t numDescendants_0[] =     { 9, 
                                      4, 3, 
                                      3, 0, 0, 2, 
                                      0, 2, 1, 0, 
                                      0, 0, 0, 0, 
                                      0, 0, 0, 0, 0, 0 };
    size_t parents_0[] =            { 0, 
                                      13, 13, 
                                      2, 0, 0, 3, 
                                      0, 4, 5, 0, 
                                      12, 12, 11, 0, 
                                      0, 0, 0, 0, 0, 0 };
    size_t nullPos_0[] =            { 0, 
                                      0, 0, 
                                      0, 1, 1, 0, 
                                      1, 0, 0, 1, 
                                      0, 0, 0, 1, 
                                      1, 1, 1, 1, 1, 1 };   
    
    if (verifyTree <int> (myTree, 
        {
            numNodes_0,
            depth_0,
            levels_0,
            numChildren_0,
            ids_0,
            numDescendants_0,
            parents_0,
            nullPos_0
        }) == false)
        return Quality::Test::FAIL;

    return Quality::Test::PASS;
}

// use tree from test case #4
LIB_TEST_CASE (6, "remove node #2") {
    auto myTree = GET_BTREE (4, int);

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}           {5, 50}   {6, 60}            {7, 70}
     *                      |                       |   |                       |
     *      -----------------       -----------------   -----------------       -----------------
     *      |               |       |               |   |               |       |               |
     *      {8, 80}    {NULL}       {NULL}     {NULL}   {NULL}     {NULL}       {NULL}     {NULL}
     *      |
     *      -----------------
     *      |               |
     *      [NULL]     {NULL}
    */

    size_t removeIds[] = { 1, 3, 4, 7 };
    for (auto i : removeIds) {
        myTree-> BTREE_PEEK_SET (i);
        myTree-> BTREE_REMOVE_NODE;
    }

    /*                                              {8, 80}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {5, 50}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {6, 60}            {NULL}   {NULL}             {NULL}
     *                      |                                                
     *      -----------------              
     *      |               |             
     *      {NULL}     {NULL}             
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 4;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 3, 3, 3, 3, 4, 4 };
    size_t numChildren[] =          { 2, 1, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 8, 2, 5, 6, 0, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 3, 1, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 8, 8, 2, 0, 0, 0, 0, 0 };   
    size_t nullPos[] =              { 0, 0, 0, 0, 1, 1, 1, 1, 1 };
    
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

// use tree from test case #5
LIB_TEST_CASE (7, "adopt node #1") {
    auto myTree = GET_BTREE (3, int);

    /*                                              {13, 4}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {12, 3}                           {11, 2}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}            {NULL}
     *                      |                       |   |                       
     *          -------------           -------------   -------------           
     *          |           |           |           |   |           |           
     *     {NULL}      {NULL}      {NULL}      {NULL}   {NULL}      {NULL}                                                   
    */

    size_t adoptIds[] = { 13, 4, 12, 10 }; 
    for (auto i : adoptIds) {
        myTree-> BTREE_PEEK_SET (i);

        auto adoptor = BTREE_INIT (7, int);
        adoptor-> BTREE_IMPORT ((myTree-> BTREE_ADOPT_NODE).first);
        adoptor-> BTREE_DUMP;
        BTREE_CLOSE (7);
    }

    /*                                              {11, 2}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {9, 90}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {8, 80}                           {NULL}             {NULL}
     *                                  |                           
     *                      -------------------------   
     *                      |                       |   
     *                      {NULL}             {NULL}                                                       
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 6;
    size_t depth = 5;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 
                                      5, 5 };
    size_t numChildren[] =          { 2, 
                                      1, 1, 
                                      1, 0, 0, 0, 
                                      0, 0, 0, 0,
                                      0, 0 };
    size_t ids[] =                  { 11, 
                                      2, 3, 
                                      9, 0, 0, 5, 
                                      0, 8, 0, 0, 
                                      0, 0 };
    size_t numDescendants[] =       { 5, 
                                      2, 1, 
                                      1, 0, 0, 0, 
                                      0, 0, 0, 0, 
                                      0, 0 };
    size_t parents[] =              { 0, 
                                      11, 11, 
                                      2, 0, 0, 3, 
                                      0, 9, 0, 0, 
                                      0, 0 };
    size_t nullPos[] =              { 0, 
                                      0, 0, 
                                      0, 1, 1, 0, 
                                      1, 0, 1, 1, 
                                      1, 1 };   
    
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

    BTREE_CLOSE (3);
    return Quality::Test::PASS;
}

// use tree from test case #6
LIB_TEST_CASE (8, "adopt node #2") {
    auto myTree = GET_BTREE (4, int);

    /*                                              {8, 80}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {5, 50}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {6, 60}            {NULL}   {NULL}             {NULL}
     *                      |                                                
     *      -----------------              
     *      |               |             
     *      {NULL}     {NULL}             
    */

    size_t adoptIds[] = { 8, 5 };
    for (auto i : adoptIds) {
        myTree-> BTREE_PEEK_SET (i);

        auto adoptor = BTREE_INIT (8, int);
        adoptor-> BTREE_IMPORT ((myTree-> BTREE_ADOPT_NODE).first);
        adoptor-> BTREE_DUMP;
        BTREE_CLOSE (8);
    }    

    /*                                              {6, 60}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}            {NULL}
     *                                  |                       
     *                      -------------------------   
     *                      |                       |   
     *                      {NULL}             {NULL}                                             
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 2;
    size_t depth = 3;
    size_t levels[] =               { 1, 2, 2, 3, 3 };
    size_t numChildren[] =          { 1, 0, 0, 0, 0 };
    size_t ids[] =                  { 6, 2, 0, 0, 0 };
    size_t numDescendants[] =       { 1, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 6, 0, 0, 0 };   
    size_t nullPos[] =              { 0, 0, 1, 1, 1 };
    
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

    BTREE_CLOSE (4);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (9, "remove tree") {
    auto myTree = BTREE_INIT (9, int);

    std::pair <size_t, int> root    =   { 1, 10 };
    std::pair <size_t, int> input[] = { { 2, 20 },            { 3, 30 },
                                        { 4, 40 },            { 5, 50 },
                                        { 6, 60 },            { 7, 70 },
                                        { 8, 80 }, { 9, 90 }, { 10, 1 }, { 11, 2 },
                                                   { 12, 3 }, { 13, 4 } };

    // create tree
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_LEFT (input[0].first, input[0].second);
    myTree-> BTREE_ADD_RIGHT (input[1].first, input[1].second);

    myTree-> BTREE_PEEK_SET (2);
    myTree-> BTREE_ADD_LEFT (input[2].first, input[2].second);

    myTree-> BTREE_PEEK_SET (3);
    myTree-> BTREE_ADD_RIGHT (input[3].first, input[3].second);

    myTree-> BTREE_PEEK_SET (4);
    myTree-> BTREE_ADD_RIGHT (input[4].first, input[4].second);

    myTree-> BTREE_PEEK_SET (5);
    myTree-> BTREE_ADD_LEFT (input[5].first, input[5].second);

    myTree-> BTREE_PEEK_SET (6);
    myTree-> BTREE_ADD_RIGHT (input[7].first, input[7].second);
    myTree-> BTREE_ADD_LEFT (input[6].first, input[6].second);

    myTree-> BTREE_PEEK_SET (7);
    myTree-> BTREE_ADD_LEFT (input[8].first, input[8].second);
    myTree-> BTREE_ADD_RIGHT (input[9].first, input[9].second);

    myTree-> BTREE_PEEK_SET (9);
    myTree-> BTREE_ADD_RIGHT (input[10].first, input[10].second);

    myTree-> BTREE_PEEK_SET (10);
    myTree-> BTREE_ADD_LEFT (input[11].first, input[11].second);

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {6, 60}                           {7, 70}            {NULL}
     *                                  |                           |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {8, 80}           {9, 90}   {10, 1}           {11, 2}
     *                      |                       |   |                       |
     *          -------------           -------------   -------------           -------------
     *          |           |           |           |   |           |           |           |
     *     {NULL}      {NULL}      {NULL}     {12, 3}   {13, 4}     {NULL}      {NULL}      {NULL}
     *                                              |   |
     *                                  -------------   -------------
     *                                  |           |   |           |
     *                             {NULL}      {NULL}   {NULL}      {NULL}
    */

    size_t removeIds[] = { 6, 7 };
    for (auto i : removeIds) {
        myTree-> BTREE_PEEK_SET (i);
        myTree-> BTREE_REMOVE;
    }    

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}             {NULL}                           {NULL}             {NULL}
    */ 

    myTree-> BTREE_DUMP;

    size_t numNodes = 5;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4 };
    size_t numChildren[] =          { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 3, 4, 0, 0, 5, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 1, 2, 0, 0, 3, 0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1 };   

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

    // remove root
    myTree-> BTREE_PEEK_SET_ROOT;
    myTree-> BTREE_REMOVE;
    myTree-> BTREE_DUMP;

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

    BTREE_CLOSE (9);
    return Quality::Test::PASS;    
}

LIB_TEST_CASE (10, "adopt tree") {
    auto myTree = BTREE_INIT (10, int);
    std::pair <size_t, int> root =      { 1, 10 };
    std::pair <size_t, int> input[] = { { 2, 20 },                  { 3, 30 },
                                        { 4, 40 },  { 5, 50 },      { 6, 60 }, { 7, 70 },
                                        { 8, 80 } };

    // create root 
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET_ROOT;
    for (auto i : input) {
        if (myTree-> BTREE_ADD_TAIL (i.first, i.second) == false)
            return Quality::Test::FAIL;
    }

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}           {5, 50}   {6, 60}           {7, 70}
     *                      |                       |   |                       |
     *      -----------------       -----------------   -----------------       -----------------
     *      |               |       |               |   |               |       |               |
     *      {8, 80}    {NULL}       {NULL}     {NULL}   {NULL}     {NULL}       {NULL}     {NULL}
     *      |
     *      -----------------
     *      |               |
     *      [NULL]     {NULL}
    */   

    size_t adoptIds[] = { 8, 3, 5 };
    for (auto i : adoptIds) {
        myTree-> BTREE_PEEK_SET (i);

        auto adoptor = BTREE_INIT (11, int);
        adoptor-> BTREE_IMPORT ((myTree-> BTREE_ADOPT).first);
        adoptor-> BTREE_DUMP;
        BTREE_CLOSE (11);
    }

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}            {NULL}
     *                                  |                       
     *                      -------------------------   
     *                      |                       |   
     *                      {4, 40}            {NULL}   
     *                      |                         
     *      -----------------       
     *      |               |         
     *      {NULL}     {NULL}         
    */   

    myTree-> BTREE_DUMP;

    size_t numNodes = 3;
    size_t depth = 4;
    size_t levels[] =               { 1, 2, 2, 3, 3, 4, 4 };
    size_t numChildren[] =          { 1, 1, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 2, 0, 4, 0, 0, 0 };
    size_t numDescendants[] =       { 2, 1, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 1, 0, 2, 0, 0, 0 };
    size_t nullPos[] =              { 0, 0, 1, 0, 1, 1, 1 };   

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

    BTREE_CLOSE (10);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (11, "weird tree via add tail, add left, add right") {
    auto myTree = BTREE_INIT (11, int);
    std::pair <size_t, int> root        =   { 1, 10 };
    std::pair <size_t, int> input_0[]   = { { 2, 20 }, { 3, 30 },
                                            { 4, 40 },            
                                            { 5, 50 },
                                            { 6, 60 },            
                                            { 7, 70 } };

    // create tree
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_LEFT (input_0[0].first, input_0[0].second);
    myTree-> BTREE_ADD_RIGHT (input_0[1].first, input_0[1].second);

    int idx = 2;
    for (auto i : { 2, 4, 5, 6 }) {
        myTree-> BTREE_PEEK_SET (i);
        myTree-> BTREE_ADD_LEFT (input_0[idx].first, input_0[idx].second);

        idx++;
    }

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}             {NULL}
     *                      |                                                   
     *          -------------------------                           
     *          |                       |                           
     *          {5, 50}            {NULL}                       
     *          |
     *          -------------------------
     *          |                       |
     *          {6, 60}            {NULL}  
     *          |
     *          -------------------------
     *          |                       |
     *          {7, 70}            {NULL}           
     *          |
     *          -------------------------
     *          |                       |
     *          {NULL}             {NULL}
    */

    myTree-> BTREE_DUMP;

    // second stage of inputs to be added using addTail()
    std::pair <size_t, int> input_1[] = { { 12, 2 },                  { 13, 3 },
                                          { 14, 4 },  { 15, 5 },      { 16, 6 }, { 17, 7 },
                                          { 18, 8 } };
    
    myTree-> BTREE_PEEK_SET (3);
    for (auto i : input_1) 
        myTree-> BTREE_ADD_TAIL (i.first, i.second);
    
    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {12, 2}           {13, 3}
     *                      |                           |                       |                          
     *          -------------------------   -------------------------   -------------------------                           
     *          |                       |   |                       |   |                       |                    
     *          {5, 50}            {NULL}   {14, 4}           {15, 5}   {16, 6}           {17, 7}
     *          |                           |                       |   |                       |
     *          -------------------------   ---------       ---------   ---------       ---------
     *          |                       |   |       |       |       |   |       |       |       |
     *          {6, 60}            {NULL}   {18, 8} {N}     {N}    {N}  {N}    {N}      {N}    {N}
     *          |                           |
     *          -------------------------   ---------
     *          |                       |   |       |
     *          {7, 70}            {NULL}   {N}    {N} 
     *          |
     *          -------------------------
     *          |                       |
     *          {NULL}             {NULL}
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 14;
    size_t depth = 7;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 4, 4, 
                                      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
                                      6, 6, 6, 6,
                                      7, 7 };
    size_t numChildren[] =          { 2, 
                                      1, 2, 
                                      1, 0, 2, 2, 
                                      1, 0, 1, 0, 0, 0,
                                      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0,
                                      0, 0 };
    size_t ids[] =                  { 1, 
                                      2, 3, 
                                      4, 0, 12, 13, 
                                      5, 0, 14, 15, 16, 17, 
                                      6, 0, 18, 0, 0, 0, 0, 0, 0, 0,
                                      7, 0, 0, 0,
                                      0, 0 };
    size_t numDescendants[] =       { 13, 
                                      4, 7, 
                                      3, 0, 3, 2, 
                                      2, 0, 1, 0, 0, 0, 
                                      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0,
                                      0, 0 };
    size_t parents[] =              { 0, 
                                      1, 1, 
                                      2, 0, 3, 3, 
                                      4, 0, 12, 12, 13, 13, 
                                      5, 0, 14, 0, 0, 0, 0, 0, 0, 0, 
                                      6, 0, 0, 0,
                                      0, 0 };
    size_t nullPos[] =              { 0, 
                                      0, 0, 
                                      0, 1, 0, 0, 
                                      0, 1, 0, 0, 0, 0, 
                                      0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 
                                      0, 1, 1, 1,
                                      1, 1 };   

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

    BTREE_CLOSE (11);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (12, "another add tail test") {
    auto myTree = BTREE_INIT (12, int);
    std::pair <size_t, int> root      =   { 1, 10 };
    std::pair <size_t, int> input_0[] = { { 2, 20 }, { 3, 30 },
                                          { 4, 40 }, { 5, 50 },
                                          { 6, 60 }, { 7, 70 } };

    // create tree
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_LEFT (input_0[0].first, input_0[0].second);
    myTree-> BTREE_ADD_RIGHT (input_0[1].first, input_0[1].second);

    myTree-> BTREE_PEEK_SET (2);
    myTree-> BTREE_ADD_LEFT (input_0[2].first, input_0[2].second);

    myTree-> BTREE_PEEK_SET (3);
    myTree-> BTREE_ADD_RIGHT (input_0[3].first, input_0[3].second);

    myTree-> BTREE_PEEK_SET (4);
    myTree-> BTREE_ADD_RIGHT (input_0[4].first, input_0[4].second);

    myTree-> BTREE_PEEK_SET (5);
    myTree-> BTREE_ADD_LEFT (input_0[5].first, input_0[5].second);

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}            {5, 50}
     *                      |                                                   |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |    
     *          {NULL}            {6, 60}                           {7, 70}            {NULL}
     *                                  |                           |
     *          -------------------------                           -------------------------
     *          |                       |                           |                       |
     *          {NULL}             {NULL}                           {NULL}             {NULL}
    */

    // second stage of inputs to be added using addTail()
    std::pair <size_t, int> input_1[] = { { 12, 2 }, { 13, 3 },
                                          { 14, 4 }, { 15, 5 },      
                                          { 16, 6 }, { 17, 7 }, { 18, 8 } };
    
    myTree-> BTREE_PEEK_SET_ROOT;
    for (auto i : input_1) 
        myTree-> BTREE_ADD_TAIL (i.first, i.second);    

    /*                                                  {1, 10}
     *                                                  |
     *                                  -------------------------------------
     *                                  |                                   |
     *                                  {2, 20}                       {3, 30}
     *                                  |                                   |
     *          -------------------------                                   -------------------------
     *          |                       |                                   |                       |
     *          {4, 40}            {12, 2}                                  {13, 3}           {5, 50}
     *          |                       |                                   |                       |
     *  -------------------------   -------------------------   -------------------------   -------------------------
     *  |                       |   |                       |   |                       |   |                       |
     *  {14, 4}           {6, 60}   {15, 5}           {16, 6}   {17, 7}           {18, 8}   {7, 70}            {NULL}
     *  |                       |   |                       |   |                       |   |               
     *  -----------   -----------   -----------   -----------   -----------   -----------   -----------
     *  |         |   |         |   |         |   |         |   |         |   |         |   |         |
     *  {N}     {N}   {N}     {N}   {N}     {N}   {N}     {N}   {N}     {N}   {N}     {N}   {N}     {N}
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 14;
    size_t depth = 5;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 3, 3, 
                                      4, 4, 4, 4, 4, 4, 4, 4, 
                                      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
    size_t numChildren[] =          { 2, 
                                      2, 2, 
                                      2, 2, 2, 1, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t ids[] =                  { 1, 
                                      2, 3, 
                                      4, 12, 13, 5, 
                                      14, 6, 15, 16, 17, 18, 7, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t numDescendants[] =       { 13, 
                                      6, 5, 
                                      2, 2, 2, 1, 
                                      0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t parents[] =              { 0, 
                                      1, 1, 
                                      2, 2, 3, 3, 
                                      4, 4, 12, 12, 13, 13, 5, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t nullPos[] =              { 0, 
                                      0, 0, 
                                      0, 0, 0, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 1, 
                                      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };   

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

    BTREE_CLOSE (12);
    return Quality::Test::PASS;
}

LIB_TEST_CASE (13, "add parent") {
    auto myTree = BTREE_INIT (13, int);
    std::pair <size_t, int> root        =   { 1, 10 };
    std::pair <size_t, int> input_0[]   = { { 2, 20 }, { 3, 30 },
                                            { 4, 40 },            
                                            { 5, 50 },
                                            { 6, 60 },            
                                            { 7, 70 } };

    // create tree
    myTree-> BTREE_ADD_ROOT (root.first, root.second);

    myTree-> BTREE_PEEK_SET (1);
    myTree-> BTREE_ADD_LEFT (input_0[0].first, input_0[0].second);
    myTree-> BTREE_ADD_RIGHT (input_0[1].first, input_0[1].second);

    int idx = 2;
    for (auto i : { 2, 4, 5, 6 }) {
        myTree-> BTREE_PEEK_SET (i);
        myTree-> BTREE_ADD_LEFT (input_0[idx].first, input_0[idx].second);

        idx++;
    }

    /*                                              {1, 10}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {3, 30}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {NULL}             {NULL}
     *                      |                                                   
     *          -------------------------                           
     *          |                       |                           
     *          {5, 50}            {NULL}                       
     *          |
     *          -------------------------
     *          |                       |
     *          {6, 60}            {NULL}  
     *          |
     *          -------------------------
     *          |                       |
     *          {7, 70}            {NULL}           
     *          |
     *          -------------------------
     *          |                       |
     *          {NULL}             {NULL}
    */

    std::pair <size_t, int> newParents[] = { { 10, 1 },
                                             { 20, 2 },
                                             { 30, 3 },
                                             { 40, 4 } };
    size_t peekIds[] = { 7, 5, 3, 1 };
    idx = 0;
    for (auto i : newParents) {
        myTree-> BTREE_PEEK_SET (peekIds[idx++]);
        myTree-> BTREE_ADD_PARENT (i.first, i.second);
    }

    /*                                              {40, 4}
     *                                              |
     *                                              -------------------------
     *                                              |                       |
     *                                              {1, 10}            {NULL}
     *                                              |
     *                                  -------------------------
     *                                  |                       |
     *                                  {2, 20}           {30, 3}
     *                                  |                       |
     *                      -------------------------   -------------------------
     *                      |                       |   |                       |
     *                      {4, 40}            {NULL}   {3, 30}            {NULL}
     *                      |                           |                            
     *          -------------------------               -------------------------           
     *          |                       |               |                       |
     *          {20, 2}            {NULL}               {NULL}             {NULL}         
     *          |
     *          -------------------------
     *          |                       |
     *          {5, 50}            {NULL}
     *          |
     *          -------------------------
     *          |                       |
     *          {6, 60}            {NULL}  
     *          |
     *          -------------------------
     *          |                       |
     *          {10, 1}            {NULL}           
     *          |
     *          -------------------------
     *          |                       |
     *          {7, 70}            {NULL}
     *          |
     *          -------------------------
     *          |                       |
     *          {NULL}             {NULL}
    */

    myTree-> BTREE_DUMP;

    size_t numNodes = 11;
    size_t depth = 10;
    size_t levels[] =               { 1, 
                                      2, 2, 
                                      3, 3, 
                                      4, 4, 4, 4, 
                                      5, 5, 5, 5,
                                      6, 6,
                                      7, 7,
                                      8, 8,
                                      9, 9,
                                      10, 10 };
    size_t numChildren[] =          { 1, 
                                      2, 0, 
                                      1, 1,
                                      1, 0, 0, 0,
                                      1, 0, 0, 0,
                                      1, 0,
                                      1, 0,
                                      1, 0,
                                      0, 0,
                                      0, 0 };
    size_t ids[] =                  { 40,
                                      1, 0,
                                      2, 30,
                                      4, 0, 3, 0,
                                      20, 0, 0, 0,
                                      5, 0,
                                      6, 0, 
                                      10, 0,
                                      7, 0, 
                                      0, 0 };
    size_t numDescendants[] =       { 10,
                                      9, 0,
                                      6, 1,
                                      5, 0, 0, 0,
                                      4, 0, 0, 0,
                                      3, 0,
                                      2, 0,
                                      1, 0, 
                                      0, 0,
                                      0, 0 };
    size_t parents[] =              { 0,
                                      40, 0,
                                      1, 1,
                                      2, 0, 30, 0,
                                      4, 0, 0, 0,
                                      20, 0,
                                      5, 0,
                                      6, 0,
                                      10, 0,
                                      0, 0 };
    size_t nullPos[] =              { 0,
                                      0, 1, 
                                      0, 0,
                                      0, 1, 0, 1,
                                      0, 1, 1, 1,
                                      0, 1, 
                                      0, 1,
                                      0, 1,
                                      0, 1,
                                      1, 1 };   

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

    BTREE_CLOSE (13);
    return Quality::Test::PASS;
}

int main (void) {
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);
    LIB_TEST_RUN_ALL;

    return 0;
}