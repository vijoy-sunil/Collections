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
#ifndef TREE_IMPL_H
#define TREE_IMPL_H

#include "../../../Admin/InstanceMgr.h"
#include <queue>

namespace Collections {
namespace Memory {
    template <typename T>
    class Tree: public Admin::NonTemplateBase {
        protected:
            // node definition
            typedef struct Node {
                size_t id;
                size_t numDescendants;
                Node* parent;
                std::vector <Node*> child;
                T data;
            }s_Node;
            
            s_Node* m_rootNode;

        private:
            size_t m_instanceId;
            std::pair <s_Node*, size_t> m_stickyPeek;

            // these two members are necessary to perform efficient tree traversal
            std::queue <s_Node*> m_stepperQueue;
            size_t m_nodesInLevel;

            /* new node created will have
             * [id]             -> note that id validity should be checked by the user
             * numDescendants   -> 0
             * parent           -> NULL
             * child            -> {  }
             * [data]           -> payload
            */
            s_Node* createNode (size_t id, const T& data) {
                s_Node* newNode = new s_Node;

                newNode-> id = id;
                newNode-> numDescendants = 0;
                newNode-> parent = NULL;
                newNode-> data = data;
                return newNode;
            }

            void stepperReset (void) {
                // clear queue
                while (!m_stepperQueue.empty())
                    m_stepperQueue.pop();
            }

            /* queue contents could either be
             * [ ]              -> if tree is empty, or
             * [root]           -> if root node exists
            */
            void stepperStart (void) {
                if (m_rootNode == NULL)
                    return;

                m_stepperQueue.push (m_rootNode);
                m_nodesInLevel = m_stepperQueue.size();
            }
            
            /* returns either of the 3 pairs
             * { node, false }  -> we have a node
             * { NULL, true }   -> no node since we hit level indicator (use this to update level count)
             * { NULL, false }  -> end of tree
            */
            std::pair <s_Node*, bool> stepperNext (void) {
                if (!m_stepperQueue.empty()) {
                    if (m_nodesInLevel == 0) {
                        m_nodesInLevel = m_stepperQueue.size();
                        // return NULL, update level count
                        return { NULL, true };
                    }
                    else
                        m_nodesInLevel--;

                    // get next node in queue
                    s_Node* currentNode = m_stepperQueue.front();
                    m_stepperQueue.pop();

                    // push all children to queue
                    if (currentNode != NULL) {
                        for (auto const& child : currentNode-> child)
                            m_stepperQueue.push (child);

                        // return node, don't update level count
                        return { currentNode, false };
                    }
                    // run stepperNext() again if you encounter a NULL node in queue
                    else
                        return stepperNext();
                }

                // reached end of tree
                return { NULL, false };
            }

            /* returns either of the 2 pairs
             * { node, level }  -> if the id passed in is valid, we get the node and its level
             * { NULL, depth}   -> otherwise, returns a NULL pointer and depth of tree
             * 
             * note that this method also inherently refreshes the stepper queue
            */
            std::pair <s_Node*, size_t> getNode (size_t id) {
                // always reset queue and start from root
                stepperReset();
                stepperStart();

                /* Here, ret could be either of the 2 pairs
                 * { root, false }  -> we have root node
                 * { NULL, false }  -> root doesn't exist (we need to set level to 0)
                */
                std::pair <s_Node*, bool> ret = stepperNext(); 
                size_t level = (ret.first == NULL) ? 0 : 1;

                while (! (ret.first  ==  NULL && 
                          ret.second ==  false) ) {

                    // update level count
                    if (ret.second == true)
                        level++;

                    // we have a node in ret
                    else {
                        s_Node* currentNode = ret.first;
                        if (currentNode-> id == id)
                            return { currentNode, level };
                    }

                    /* Here, ret could be either of the 3 pairs:
                     * { node, false }  -> we have a node
                     * { NULL, true }   -> no node since we hit level indicator
                     * { NULL, false }  -> end of tree
                    */
                    ret = stepperNext();
                }

                // reached end of tree, return depth
                return { NULL, level };
            }

            /* node contents are displayed in the following pattern
             *      {                                           <L3>
             *          id : ?                                  <L4>
             *          descendants count : ?
             *          parent id : ?
             *          child count : ?
             *          child id :  
             *                  {                               <L5>
             *                      ?                           <L6>
             *                      ?
             *                      ...
             *                  }                               <L5>
             *          data : ?, ?
             *          level : ?
             *      }                                           <L3>
            */
            void dumpNode (s_Node* node, 
                           std::ostream& ost, 
                           void (*lambda) (T*, std::ostream&)) {

                if (node == NULL)
                    return; 

                std::string parentId = (node-> parent == NULL) ? "NULL" : std::to_string (node-> parent-> id);

                ost << OPEN_L3;
                ost << TAB_L4   << "id : "                  << node-> id                        << "\n";
                ost << TAB_L4   << "descendants count : "   << node-> numDescendants            << "\n";
                ost << TAB_L4   << "parent id : "           << parentId                         << "\n";
                ost << TAB_L4   << "child count : "         << peekChildCount()                 << "\n";
                ost << TAB_L4   << "child id : "            << "\n";
                
                ost << OPEN_L5;
                for (auto const& child : node-> child) {
                if (child != NULL)
                ost << TAB_L6   << child-> id               << "\n";
                else
                ost << TAB_L6   << "NULL"                   << "\n";
                }
                ost << CLOSE_L5;
                
                ost << TAB_L4   << "data : ";               lambda (& (node-> data), ost);  ost << "\n";
                ost << TAB_L4   << "level : "               << peekLevel()                      << "\n";
                ost << CLOSE_L3;
            }

        public:
            Tree (size_t instanceId) {
                m_instanceId = instanceId;
                m_rootNode   = NULL;
                m_stickyPeek = { NULL, 0 };
                m_nodesInLevel = 0;
            }

            ~Tree (void) {
                // destroy all nodes
                peekSetRoot();
                remove();
            }

            /* sticky peek pairs could either be
             * { node, level }  -> if the id passed in is valid
             * { NULL, depth }  -> otherwise, returns a NULL pointer and depth of tree
            */
            inline void peekSet (size_t id) {
                m_stickyPeek = getNode (id);
            }

            /* sticky peek pairs could either be
             * { NULL, 0 }      -> if root doesn't exist
             * { root, 1 }      -> valid root with level set to 1
            */
            void peekSetRoot (void) {
                // force reset queue
                stepperReset();
                stepperStart();

                /* instead of setting peek node to root node directly, we use the stepper to set it for us (same as in the
                 * peekSet method). This allows us to use peekSetNext method to be used after
                 *
                 *  Here, stepperNext() could return either of the 2 pairs
                 * { root, false } -> we have root node
                 * { NULL, false } -> root doesn't exist (we need to set level to 0)
                */
                s_Node* currentNode = stepperNext().first;
                if (currentNode == NULL)
                    m_stickyPeek = { NULL, 0 };
                else
                    m_stickyPeek = { currentNode, 1 };
            }

            /* this is an efficient way to traverse the tree (vs getNode method) since there is no need to start the
             * traversal every time from the root. Here, we use the stepper method instead (saves the last position into
             * the stepper queue)
             * 
             * sticky peek pairs could either be
             * { NULL, 0 }      -> end of tree reached
             * { node, level}   -> node with level
             *
             * if the tree has been updated, calling peekSetNext() may result in wrong result since the stepper queue may
             * not reflect the changes made during the tree update. (updates that effect this method involves any change
             * in the child vector or the node itself, or any other method that changes the peek position will also effect
             * this method). To combat this, either set peek again to parent or to the root 
            */
            void peekSetNext (void) {
                /* ret could be either of the 3 pairs:
                 * { node, false } -> we have a node
                 * { NULL, true }  -> no node since we hit level indicator (here we update level count and move again)
                 * { NULL, false } -> end of tree
                */
                std::pair <s_Node*, bool> ret = stepperNext();  

                if (ret.second == true) {
                    m_stickyPeek.second++;
                    // move 1 more step to get past the level indicator
                    ret = stepperNext();
                }

                // clear level count
                if (ret.first == NULL)
                    m_stickyPeek = { NULL, 0 };
                else 
                    m_stickyPeek.first = ret.first;
            }

            inline s_Node* peekNode (void) {
                return m_stickyPeek.first;
            }

            inline size_t peekLevel (void) {
                return m_stickyPeek.second;
            }

            size_t peekChildCount (void) {
                s_Node* parentNode = peekNode();
                if (parentNode == NULL)
                    return 0;
                // child vector size minus NULL occurences
                else
                    return parentNode-> child.size() - std::count (parentNode-> child.begin(), 
                                                                   parentNode-> child.end(), 
                                                                   static_cast <s_Node*> (0));
            }

            void addRoot (size_t id, const T& data) {
                s_Node* newNode = createNode (id, data);
                // a root node already exists
                if (m_rootNode != NULL) {
                    newNode-> numDescendants = m_rootNode-> numDescendants + 1;
                    newNode-> child.push_back (m_rootNode);
                    m_rootNode-> parent = newNode;
                }

                // update root node
                m_rootNode = newNode;
            }

            bool addChild (size_t id, const T& data) {
                s_Node* parentNode = peekNode();
                // peek node id not found
                if (parentNode == NULL)
                    return false;

                s_Node* newNode = createNode (id, data);
                // update number of descendants of all parents up to the root node
                s_Node* tempNode = parentNode;
                while (tempNode != NULL) {
                    tempNode-> numDescendants++;
                    tempNode = tempNode-> parent;
                }

                parentNode-> child.push_back (newNode);
                newNode-> parent = parentNode;
                return true; 
            } 

            /* this is different from the addChild method, since, here we take in either a tree or a node as input and link
             * it with a parent node as its child. The user must make sure before hand that the final tree formed has all
             * nodes that are unique in ids
            */
            bool appendTree (s_Node* rootNode) {
                s_Node* parentNode = peekNode();

                // invalid peek node id or invalid root node
                if (parentNode == NULL || rootNode == NULL)
                    return false;

                // update number of descendants of all parents upto root node
                s_Node* tempNode = parentNode;
                while (tempNode != NULL) {
                    tempNode-> numDescendants += rootNode-> numDescendants + 1;
                    tempNode = tempNode-> parent;
                }

                parentNode-> child.push_back (rootNode);
                rootNode-> parent = parentNode;
                return true;
            }

            /* sticky peek pair set to
             * { node, level }  -> of child node if we are adding to the root, else
             * { node, level }  -> of newly added parent
            */
            bool addParent (size_t id, const T& data) {
                s_Node* childNode = peekNode();
                // peek node id not found
                if (childNode == NULL)
                    return false;

                if (childNode == m_rootNode) {
                    addRoot (id, data);
                    return true;
                }

                /* adding a parent to 'childNode' is same as removing (with adopt) the 'childNode', adding a child to the
                 * parent node, and then appending the adopted tree/node to newly added child. Note that the ordering of
                 * the 'childNode' in the parent's child vector may be different, since we are removing and adding back to
                 * the vector
                */
                s_Node* parentNode = childNode-> parent;
                /* we know that the id is valid, so no need to check the validity again in remove(), remove 'childNode' 
                 * which has already been set as the peek node
                */
                s_Node* rootNode = remove (true).first;

                // add new child which will be the parent to the 'childNode'
                peekSet (parentNode-> id);
                addChild (id, data);

                // set peekNode to the newly added child
                peekSet (id);
                return appendTree (rootNode);
            }

            /* return orphaned node/tree if adopt = true, else delete all orphaned nodes and return NULL. Since we also
             * return NULL if the id is invalid, we need a boolean to differentiate between the two NULL return cases
             * 
             * returns either of the 3 pairs
             * { NULL, false }  -> invalid id
             * { node, true }   -> adopted node/tree, valid id
             * { NULL, true }   -> remove complete, valid id
             * 
             * sticky peek pairs set to
             * { NULL, 0 }      -> reset pair
            */
            std::pair <s_Node*, bool> remove (bool adopt = false) {
                s_Node* currentNode = peekNode();
                // peek node id not found
                if (currentNode == NULL) 
                    return { NULL, false };

                // if NOI (node of interest) is not a root node
                if (currentNode != m_rootNode) {
                    s_Node* parentNode = currentNode-> parent;

                    // update descendants of all parents upto root node
                    s_Node* tempNode = parentNode;
                    while (tempNode != NULL) {
                        tempNode-> numDescendants -= currentNode-> numDescendants + 1;
                        tempNode = tempNode-> parent;
                    }

                    parentNode-> child.erase (std::remove (parentNode-> child.begin(), 
                                                           parentNode-> child.end(), 
                                                           currentNode),
                                              parentNode-> child.end());
                    currentNode-> parent = NULL;
                }
                else
                    m_rootNode = NULL;

                if (adopt == true) {
                    // clear dangling variables
                    m_stickyPeek = { NULL, 0 };
                    return { currentNode, true };
                }

                else {
                    // we cannot use the stepper here because we need depth traversal to remove node and its children
                    std::queue <s_Node*> pendingQ;
                    pendingQ.push (currentNode);

                    while (!pendingQ.empty()) {
                        // get pending node
                        s_Node* currentNode = pendingQ.front();
                        // remove from pending queue
                        pendingQ.pop();
            
                        if (currentNode != NULL) {
                            for (auto const& child : currentNode-> child)
                                pendingQ.push (child);

                            delete currentNode;
                        }
                    }

                    // clear dangling variables
                    m_stickyPeek = { NULL, 0 };
                }
                return { NULL, true };
            }

            inline size_t getSize (void) {
                return (m_rootNode == NULL) ? 0 : m_rootNode-> numDescendants + 1;
            }

            /* pass an id that doesn't exist (reserved id) in tree to get depth
             *
             * sticky peek pairs set to
             * { NULL, depth }
            */
            size_t getDepth (void) {
               peekSet (RESERVED_0);
               return peekLevel();
            }

            /* sticky peek pairs set to
             * { nodeB, levelB }    -> if valid
             * { NULL, depth }      -> otherwise
            */
            bool swap (size_t idA, size_t idB) {
                peekSet (idA);
                s_Node* nodeA = peekNode();

                peekSet (idB);
                s_Node* nodeB = peekNode();

                // invalid ids
                if (nodeA == NULL || nodeB == NULL)
                    return false;

                if (idA == idB) 
                    return true;

                /* Updates that need to be made when you swap nodes
                 * 1 : parent's child vector
                 * 2 : child's parent pointer
                 * 3 : NOI's parent pointer
                 * 4 : NOI's child vector
                 * 5 : descendants count
                 * 6 : update saved variables : root node update if changed. In case of peek vars, you can keep it as it is
                 * even though some params might be different after the swap operation
                */

                // (1) 
                s_Node* parentNodeA = nodeA-> parent;
                s_Node* parentNodeB = nodeB-> parent;

                // swap child in parent's child vector (order of the child in the vector is retained)
                typedef typename std::vector <s_Node*>::iterator iter;
                iter itA, itB;

                if (parentNodeA != NULL)
                    itA = std::find (parentNodeA-> child.begin(), parentNodeA-> child.end(), nodeA);

                if (parentNodeB != NULL)
                    itB = std::find (parentNodeB-> child.begin(), parentNodeB-> child.end(), nodeB);

                /* 3 cases :
                 * (1) both parents exist
                 * (2) parent A exists and parent B doesn't
                 * (3) parent B exists and parent A doesn't
                 * The case where both parents doesn't exist (possible when we pass both inputs as id for root node)which
                 * will becaught at the start
                */
                if (parentNodeA != NULL && parentNodeB != NULL) {
                    s_Node* tempNode = *itA;
                    *itA = *itB;
                    *itB = tempNode;
                }
                else if (parentNodeA != NULL)
                    *itA = nodeB;
                else
                    *itB = nodeA;

                // (2) 
                for (auto const& child : nodeA-> child)
                    child-> parent = nodeB;

                for (auto const& child : nodeB-> child)
                    child-> parent = nodeA;

                // (3)
                nodeB-> parent = parentNodeA;
                nodeA-> parent = parentNodeB;

                // (4) 
                std::vector <s_Node*> tempVec = nodeA-> child;
                nodeA-> child = nodeB-> child;
                nodeB-> child = tempVec;

                // (5) 
                size_t tempVar = nodeA-> numDescendants;
                nodeA-> numDescendants = nodeB-> numDescendants;
                nodeB-> numDescendants = tempVar;

                // (6) 
                m_rootNode = (nodeA-> parent == NULL) ? nodeA :
                             (nodeB-> parent == NULL) ? nodeB :
                             m_rootNode;
                return true;
            }

            /* sticky peek pairs set to
             * { nodeB, levelB }    -> if valid
             * { NULL, depth }      -> otherwise
            */
            std::vector <size_t> getPath (size_t idA, size_t idB) {
                peekSet (idA);
                s_Node* nodeA = peekNode();
                size_t levelA = peekLevel();

                peekSet (idB);
                s_Node* nodeB = peekNode();
                size_t levelB = peekLevel();

                // invalid ids
                if (nodeA == NULL || nodeB == NULL)
                    return {  };

                if (idA == idB) 
                    return { idA };

                /* if A and B are on the same level : bubble up till we reach the same parent node;
                 * else, bubble up the lower level node till they are at the same level, and then bubble both of them up
                 * till we reach the same parent node
                */
                s_Node* lowerNode   = nodeB;
                s_Node* higherNode  = nodeA;
                size_t lowerLevel   = levelB;
                size_t higherLevel  = levelA;

                if (levelA > levelB) {
                    lowerNode   = nodeA;
                    higherNode  = nodeB;

                    lowerLevel  = levelA;
                    higherLevel = levelB;
                }

                std::vector <size_t> lowerPath, higherPath;
                // compute path till both are at the same level
                while (lowerLevel != higherLevel) {
                    lowerPath.push_back (lowerNode-> id);  

                    lowerNode = lowerNode-> parent;
                    lowerLevel--;
                }

                // compute path till we reach same parent
                while (lowerNode != higherNode) {
                    lowerPath.push_back (lowerNode-> id);
                    higherPath.push_back (higherNode-> id);

                    lowerNode  = lowerNode-> parent;
                    higherNode = higherNode-> parent;
                }

                // push common parent once
                lowerPath.push_back (lowerNode-> id);

                // reverse higher path and append it to lower path
                std::reverse (higherPath.begin(), higherPath.end());
                for (auto const& id : higherPath) 
                    lowerPath.push_back (id);

                return lowerPath;
            }

            /* carefull when using this method since we are overwriting the root node of our tree and deleting any existing
             * nodes. This method is usually used to create a new tree using a node/tree 'adopted' from another tree
             *
             * sticky peek pairs set to
             * { NULL, 0 }      -> reset pair
            */
            void importTree (s_Node* rootNode) {
                peekSetRoot();
                remove();
                m_rootNode = rootNode;
            }

            /* tree is displayed in the following format
             * tree :
             *      {                                       <L1>
             *          id : ?                              <L2>
             *          node count : ?
             *          peek pair : ?, ?
             *          depth : ?
             *          nodes : 
             *                  {                           <L3>
             *                      node contents           <L4>
             *                  }
             *                  {
             *                      node contents
             *                  }                           <L3>
             *                  ...
             *      }                                       <L1>
            */
            void dump (std::ostream& ost, 
                       void (*lambda) (T*, std::ostream&) = [](T* nodeData, std::ostream& ost) { 
                                                                ost << *nodeData; 
                                                            }) {
                s_Node* node = peekNode();
                std::string peekId = (node == NULL) ? "NULL" : std::to_string (node-> id);
                
                ost << "tree : " << "\n";
                ost << OPEN_L1;

                ost << TAB_L2   << "id : "          << m_instanceId << "\n";
                ost << TAB_L2   << "node count : "  << getSize()    << "\n";
                ost << TAB_L2   << "peek pair : "   << peekId       
                                                    << ", "     
                                                    << peekLevel()  << "\n";
                ost << TAB_L2   << "depth : "       << getDepth()   << "\n";
                ost << TAB_L2   << "nodes : "       << "\n";
            
                // loop through all nodes level-wise
                peekSetRoot();
                while (peekNode() != NULL) {
                    // dump nodes in L4
                    dumpNode (peekNode(), ost, lambda);
                    peekSetNext();
                }

                ost << CLOSE_L1;
            }
    };
}   // namespace Memory
}   // namespace Collections
#endif  // TREE_IMPL_H