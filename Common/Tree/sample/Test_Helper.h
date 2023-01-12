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
#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../inc/Tree.h"

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
 * [set]
 * TREE_PEEK_SET_ROOT
 * TREE_PEEK_SET_NEXT       [*]
 * 
 * [peek]
 * TREE_PEEK_NODE           [*]
 * TREE_PEEK_LEVEL          [*]
 * TREE_PEEK_CHILD_COUNT    [*]
 * TREE_PEEK_IS_END
 * 
 * [utils]
 * TREE_SIZE
 * TREE_DEPTH
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
#endif  // TEST_HELPER_H