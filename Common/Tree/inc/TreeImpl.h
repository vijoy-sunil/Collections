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

            inline void savePeek (void) {
                m_savePeekNode = peekNode();
            }

            void restorePeek (void) {
                if (m_savePeekNode != NULL)
                    peekSet (m_savePeekNode-> id);
            }

            inline void resetPeek (void) {
                m_stickyPeek = { NULL, 0 };
            }

        private:
            size_t m_instanceId;
            std::pair <s_Node*, size_t> m_stickyPeek;
            
            // save and restore peek using this
            s_Node* m_savePeekNode;

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
            
            /* returns either of the following
             * { false { NULL, true } }     -> update level count
             * { false { node, false } }    -> we have a node (could be a NULL node)
             * { true { NULL, false } }     -> end of tree
            */
            std::pair <bool, std::pair <s_Node*, bool>> stepperNext (void) {
                if (!m_stepperQueue.empty()) {
                    if (m_nodesInLevel == 0) {
                        m_nodesInLevel = m_stepperQueue.size();
                        // not end of tree, no node, update level count
                        return { false, { NULL, true } };
                    }
                    else
                        m_nodesInLevel--;

                    // get next node in queue
                    s_Node* currentNode = m_stepperQueue.front();
                    m_stepperQueue.pop();

                    if (currentNode != NULL) {
                        for (auto const& child : currentNode-> child)
                            m_stepperQueue.push (child);
                    }

                    // not end of tree, current node, don't update level count
                    return { false, { currentNode, false } };
                }

                // reached end of tree
                return { true, { NULL, false } };
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

                /* Here, ret could be either of the following
                 * { false { root, false } }    -> we have root node
                 * { true { NULL, false } }     -> root doesn't exist (we need to set level to 0)
                */
                std::pair <bool, std::pair <s_Node*, bool>> ret = stepperNext(); 
                size_t level = (ret.first == true) ? 0 : 1;

                // loop till end of tree
                while (ret.first == false) {
                    // update level count
                    if (ret.second.second == true)
                        level++;

                    // we have a node in ret
                    else {
                        s_Node* currentNode = ret.second.first;
                        if (currentNode != NULL && currentNode-> id == id)
                            return { currentNode, level };
                    }

                    /* Here, ret could be either of the following
                     * { false { NULL, true } }     -> update level count
                     * { false { node, false } }    -> we have a node (could be a NULL node)
                     * { true { NULL, false } }     -> end of tree
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

                // the dumpNode() can also show details for a NULL node
                std::string nodeId          = (node == NULL)            ? "NULL" : std::to_string (node-> id);
                std::string numDescendants  = (node == NULL)            ? "0"    : std::to_string (node-> numDescendants);
                std::string parentId        = (node == NULL)            ? "NULL" : 
                                              (node-> parent == NULL)   ? "NULL" : std::to_string (node-> parent-> id);

                ost << OPEN_L3;
                ost << TAB_L4   << "id : "                  << nodeId                           << "\n";
                ost << TAB_L4   << "descendants count : "   << numDescendants                   << "\n";
                ost << TAB_L4   << "parent id : "           << parentId                         << "\n";
                ost << TAB_L4   << "child count : "         << peekChildCount()                 << "\n";
                ost << TAB_L4   << "child id : "            << "\n";
                
                ost << OPEN_L5;
                if (node != NULL) {
                    for (auto const& child : node-> child) {
                        if (child != NULL)
                ost << TAB_L6   << child-> id               << "\n";
                        else
                ost << TAB_L6   << "NULL"                   << "\n";
                    }
                }
                ost << CLOSE_L5;

                if (node != NULL) {
                ost << TAB_L4   << "data : ";               lambda (& (node-> data), ost);  ost << "\n";    
                }
                else
                ost << TAB_L4   << "data : "                << "NULL"                           << "\n";
                
                ost << TAB_L4   << "level : "               << peekLevel()                      << "\n";
                ost << CLOSE_L3;
            }

        public:
            Tree (size_t instanceId) {
                m_instanceId   = instanceId;
                m_rootNode     = NULL;
                
                resetPeek();
                m_savePeekNode = NULL;
                m_nodesInLevel = 0;
            }

            ~Tree (void) {
                // destroy all nodes
                peekSetRoot();
                remove();
            }

            /* sticky peek pair set to
             * { node, level }  -> if valid id
             * { NULL, depth }  -> otherwise
            */
            inline void peekSet (size_t id) {
                m_stickyPeek = getNode (id);
            }

            /* sticky peek pair set to
             * { NULL, 0 }      -> if root doesn't exist
             * { root, 1 }      -> otherwise
            */
            void peekSetRoot (void) {
                // force reset queue
                stepperReset();
                stepperStart();

                /* instead of setting peek node to root node directly, we use the stepper to set it for us (same as in the
                 * peekSet method). This allows us to use peekSetNext method afterwards
                 *
                 * Here, stepperNext() could return either of the following
                 * { false { root, false } }    -> we have root node
                 * { true { NULL, false } }     -> root doesn't exist (we need to set level to 0)
                */
                s_Node* rootNode = stepperNext().second.first;
                if (rootNode == NULL)
                    resetPeek();
                else
                    m_stickyPeek = { rootNode, 1 };
            }

            /* this is an efficient way to traverse the tree (vs getNode method) since there is no need to start the
             * traversal every time from the root. Here, we use the stepper method instead (saves the last position into
             * the stepper queue)
             * 
             * sticky peek pair set to
             * { NULL, 0 }      -> end of tree reached
             * { node, level}   -> otherwise (node could be a NULL child)
             *
             * if the tree has been updated, calling peekSetNext() may result in wrong result since the stepper queue may
             * not reflect the changes made during the tree update. (updates that effect this method involves any change
             * in the child vector or the node itself, or any other method that changes the peek position will also effect
             * this method). To combat this, either set peek again to parent or to the root 
            */
            void peekSetNext (void) {
                /* ret could be either of the following:
                 * { false { NULL, true } }     -> update level count
                 * { false { node, false } }    -> we have a node (could be a NULL node)
                 * { true { NULL, false } }     -> end of tree
                */
                std::pair <bool, std::pair <s_Node*, bool>> ret = stepperNext();  

                if (ret.second.second == true) {
                    m_stickyPeek.second++;
                    // move 1 more step to get past the level indicator
                    ret = stepperNext();
                }

                // end of tree
                if (ret.first == true)
                    resetPeek();
                // we have a node (could be a NULL node)
                else 
                    m_stickyPeek.first = ret.second.first;
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

            // { NULL, 0 } indicates end of tree
            inline bool peekIsEnd (void) {
                return (m_stickyPeek.first  == NULL && 
                        m_stickyPeek.second == 0) ? true : false;
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

            /* a NULL child only has its level associated with it
             *      {                                     
             *          id : NULL                               
             *          descendants count : 0
             *          parent id : NULL
             *          child count : 0
             *          child id :  
             *                  {                             
             *                  }                       
             *          data : NULL
             *          level : [valid level]   // <----------- NULL node's level
             *      }  
            */
            bool addNullChild (void) {
                s_Node* parentNode = peekNode();
                // peek node id not found
                if (parentNode == NULL)
                    return false;

                parentNode-> child.push_back (NULL);
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
             * { node, level }  -> at new parent node, if valid id
            */
            bool addParent (size_t id, const T& data) {
                s_Node* childNode = peekNode();
                // peek node id not found
                if (childNode == NULL)
                    return false;

                if (childNode == m_rootNode) {
                    addRoot (id, data);
                    // set peek to newly added parent (new root) for uniformity
                    peekSetRoot();
                    return true;
                }

                /* adding a parent to childNode is done through the following steps
                 * (1) add new node (acts as parent to adopted node/tree) using addChild() (note that the new node will
                 *     be added to end of the parent's child vector)
                 * (2) save the position of childNode in parent's child vector
                 * (3) change position of newly added node from back of the vector to saved position from (2)
                 * (4) remove (with adopt) the childNode (and its descendants)
                 * (5) append the adopted node/tree from (4) to the newly added node
                */
                s_Node* parentNode = childNode-> parent;

                // (1) using addChild() takes care of all internal updates
                peekSet (parentNode-> id);
                addChild (id, data);  

                // (2)
                typename std::vector <s_Node*>::iterator iter;
                iter = std::find (parentNode-> child.begin(), 
                                  parentNode-> child.end(), 
                                  childNode);

                // (3)
                s_Node* newNode = parentNode-> child.back();
                parentNode-> child.pop_back();
                parentNode-> child.insert (iter, newNode);

                // (4) 
                peekSet (childNode-> id);
                s_Node* rootNode = remove (true).first;

                // (5)
                peekSet (id);
                return appendTree (rootNode);        
            }

            /* this is different from the remove() method, as in removeNode() removes/adopts only a single node. When you
             * remove a single node all its children are added as children to NOI's parent. For this reason, we cannot 
             * use this method on the root node, because when you remove the root node its children becomes orphaned and
             * can't be added as children to the root's parent (NULL)
             * 
             * returns either of the 3 pairs
             * { NULL, false }  -> invalid id/root node
             * { node, true }   -> adopted node, valid id
             * { NULL, true }   -> remove complete, valid id
             * 
             * sticky peek pair set to
             * { NULL, 0 }      -> reset peek, if valid id
            */
            std::pair <s_Node*, bool> removeNode (bool adopt = false) {
                s_Node* currentNode = peekNode();
                // peek id not found, or if peek node is root node
                if (currentNode == NULL || currentNode == m_rootNode)
                    return { NULL, false };

                s_Node* parentNode = currentNode-> parent;
                
                // save position of currentNode in parent's child vector
                typename std::vector <s_Node*>::iterator iter;
                iter = std::find (parentNode-> child.begin(),
                                  parentNode-> child.end(),
                                  currentNode);

                // update descendants of all parents upto root node
                s_Node* tempNode = parentNode;
                while (tempNode != NULL) {
                    tempNode-> numDescendants -= 1;
                    tempNode = tempNode-> parent;
                }

                // add child of currentNode as parentNode's children
                parentNode-> child.insert (iter, currentNode-> child.begin(),
                                                 currentNode-> child.end());

                // remove current node from parent's child vector
                parentNode-> child.erase (std::remove (parentNode-> child.begin(), 
                                                       parentNode-> child.end(), 
                                                       currentNode),
                                          parentNode-> child.end());

                // set parent of new children
                for (auto const& child : currentNode-> child) {
                    if (child != NULL)
                        child-> parent = parentNode;
                }

                // clear currentNode stats
                currentNode-> numDescendants = 0;
                currentNode-> parent = NULL;
                currentNode-> child.clear();

                resetPeek();

                if (adopt == true) 
                    return { currentNode, true };
                else {
                    delete currentNode;
                    return { NULL, true };
                }
            }

            /* return orphaned node/tree if adopt = true, else delete all orphaned nodes and return NULL. Since we also
             * return NULL if the id is invalid, we need a boolean to differentiate between the two NULL return cases
             * 
             * returns either of the 3 pairs
             * { NULL, false }  -> invalid id
             * { node, true }   -> adopted node/tree, valid id
             * { NULL, true }   -> remove complete, valid id
             * 
             * sticky peek pair set to
             * { NULL, 0 }      -> reset peek, if valid id
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
                    resetPeek();
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
            
                        for (auto const& child : currentNode-> child) {
                            if (child != NULL)
                                pendingQ.push (child);
                        }

                        delete currentNode;
                    }

                    resetPeek();
                    return { NULL, true };
                }
            }

            inline size_t getSize (void) {
                return (m_rootNode == NULL) ? 0 : m_rootNode-> numDescendants + 1;
            }

            // pass an id that doesn't exist (reserved id) in tree to get depth
            size_t getDepth (void) {
                savePeek();

                peekSet (RESERVED_0);
                size_t depth = peekLevel();

                restorePeek();
                return depth;
            }

            bool swap (size_t idA, size_t idB) {
                savePeek();

                peekSet (idA);
                s_Node* nodeA = peekNode();

                peekSet (idB);
                s_Node* nodeB = peekNode();

                // invalid ids
                if (nodeA == NULL || nodeB == NULL) {
                    restorePeek();
                    return false;
                }

                if (idA == idB) {
                    restorePeek();
                    return true;
                }

                /* there are 2 special cases where our swap operation fails
                 * (1) when nodeA is the immediate parent of nodeB
                 * (2) and vice-versa
                 * 
                 * To get around this we insert a reserved node between the two nodes, which will be removed after the
                 * swap operation is complete
                */
                if (nodeB-> parent == nodeA) {
                    peekSet (nodeB-> id);
                    addParent (RESERVED_1, m_rootNode-> data);
                }
                
                if (nodeA-> parent == nodeB) {
                    peekSet (nodeA-> id);
                    addParent (RESERVED_1, m_rootNode-> data);
                }

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
                for (auto const& child : nodeA-> child) {
                    if (child != NULL)
                        child-> parent = nodeB;
                }

                for (auto const& child : nodeB-> child) {
                    if (child != NULL)
                        child-> parent = nodeA;
                }

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

                // remove reserved node
                peekSet (RESERVED_1);
                removeNode();

                restorePeek();
                return true;
            }

            std::vector <size_t> getPath (size_t idA, size_t idB) {
                savePeek();

                peekSet (idA);
                s_Node* nodeA = peekNode();
                size_t levelA = peekLevel();

                peekSet (idB);
                s_Node* nodeB = peekNode();
                size_t levelB = peekLevel();

                // invalid ids
                if (nodeA == NULL || nodeB == NULL) {
                    restorePeek();
                    return {  };
                }

                if (idA == idB) {
                    restorePeek();
                    return { idA };
                }

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

                restorePeek();
                return lowerPath;
            }

            std::vector <size_t> getTails (void) {
                if (m_rootNode == NULL)
                    return { };

                savePeek();

                std::vector <size_t> tails;
                peekSetRoot();
                while (!peekIsEnd()) {
                    // return all node ids with child count/descendant count = 0
                    if (peekNode() != NULL && peekChildCount() == 0)
                        tails.push_back (peekNode()-> id);

                    peekSetNext();
                }

                restorePeek();
                return tails;
            }

            /* carefull when using this method since we are overwriting the root node of our tree and deleting any existing
             * nodes. This method is usually used to create a new tree using a node/tree 'adopted' from another tree
             *
             * sticky peek pair set to
             * { NULL, 0 }      -> reset peek
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
            
                savePeek();
                // loop through all nodes level-wise
                peekSetRoot();
                while (!peekIsEnd()) {
                    // dump nodes in L4
                    dumpNode (peekNode(), ost, lambda);
                    peekSetNext();
                }

                ost << CLOSE_L1;
                restorePeek();
            }
    };
}   // namespace Memory
}   // namespace Collections
#endif  // TREE_IMPL_H