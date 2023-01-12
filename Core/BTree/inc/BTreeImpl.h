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
#ifndef BTREE_IMPL_H
#define BTREE_IMPL_H

#include "../../../Common/Tree/inc/TreeImpl.h"

namespace Collections {
namespace Memory {
    // this is used to index into the child vector of a parent
    typedef enum {
        LEFT_CHILD  = 0,
        RIGHT_CHILD = 1
    }e_child;

    template <typename T>
    class BTree : public Tree <T> {
        private:
            /* In a template that we write, there are two kinds of names (identifiers) that could be used - dependant names
             * and non- dependant names. A dependant name is a name that depends on a template parameter; a non- dependant 
             * name has the same meaning irrespective of what the template parameters are. What a dependant name refers to 
             * could be something different for each different instantiation of the template. As a consequence, C++ 
             * templates are subject to "two-phase name lookup". 
             * 
             * When a template is initially parsed (before any instantiation takes place) the compiler looks up the 
             * non-dependent names. When a particular instantiation of the template takes place, the template parameters 
             * are known by then, and the compiler looks up dependent names. During the first phase, the parser needs to 
             * know if a dependant name is the name of a type or the name of a non-type. 
             * 
             * template <typename T> 
             * T::value_type foo (const T& c, typename T::size_type sz) {
             *      return T::number * sz ;
             * }
             * Here, value_type, size_type are name of a type, and number is the name of a non type
             * 
             * By default, a dependant name is assumed to be the name of a non-type. The typename keyword before a 
             * dependant name disambiguates it to be the name of a type.
            */
            typedef typename Tree <T>::s_Node m_Node;

        public:
            BTree (size_t instanceId) : Tree <T> (instanceId) { }

            m_Node* peekChild (e_child child) {
                m_Node* parentNode = Tree <T>::peekNode();
                // invalid peek id
                if (parentNode == NULL)
                    return NULL;
                else
                    return parentNode-> child[child];
            }
          
            bool addLeft (size_t id, const T& data) {
                Tree <T>::savePeek();
                m_Node* parenttNode = Tree <T>::peekNode();
                // invalid peek id
                if (parenttNode == NULL)
                    return false;

                /* (1) initially, both left and right child are set to NULL
                 *     current node ---- LEFT CHILD -----> [NULL] 
                 *                  |
                 *                  ---- RIGHT CHILD ----> [NODE/NULL]
                 *
                 * (2) addChild() adds the non NULL child at the end of the vector
                 *     current node ---- LEFT CHILD -----> [NULL]
                 *                  |
                 *                  ---- RIGHT CHILD ----> [NODE/NULL]
                 *                  |
                 *                  ---- THIRD CHILD ----> [NODE]
                 * 
                 * (3) move the third child to the first position (left child)
                 *     current node ---- THIRD CHILD ----> [NODE] -----> becomes new left child
                 *                  |
                 *                  ---- RIGHT CHILD ----> [NODE/NULL]
                 *                  |
                 *                  ---- LEFT CHILD -----> [NULL] -----> pop back()
                */
                if (peekChild (LEFT_CHILD) == NULL) {
                    Tree <T>::addChild (id, data);

                    std::reverse (parenttNode-> child.begin(), parenttNode-> child.end());
                    parenttNode-> child.pop_back();

                    // set left and right child of new node to NULL
                    Tree <T>::peekSet (id);
                    Tree <T>::addNullChild();
                    Tree <T>::addNullChild();

                    Tree <T>::restorePeek();
                    return true;                    
                }
                else
                    return false;
            }

            bool addRight (size_t id, const T& data) {
                Tree <T>::savePeek();
                m_Node* parenttNode = Tree <T>::peekNode();
                // invalid peek id
                if (parenttNode == NULL)
                    return false;

                /* (1) initially, both left and right child are set to NULL
                 *     current node ---- LEFT CHILD -----> [NODE/NULL] 
                 *                  |
                 *                  ---- RIGHT CHILD ----> [NULL]
                 * 
                 * (2) pop back from child vector
                 *     current node ---- LEFT CHILD -----> [NODE/NULL]
                 *                  |
                 *                  ---- RIGHT CHILD ----> []
                 * 
                 * (3) use addChild() to add the right child
                 *     current node ---- LEFT CHILD -----> [NODE/NULL]
                 *                  |
                 *                  ---- RIGHT CHILD ----> [NODE]
                */
                if (peekChild (RIGHT_CHILD) == NULL) {
                    parenttNode-> child.pop_back();
                    Tree <T>::addChild (id, data);

                    // set left and right child of new node to NULL
                    Tree <T>::peekSet (id);
                    Tree <T>::addNullChild();
                    Tree <T>::addNullChild();

                    Tree <T>::restorePeek();
                    return true;
                }
                else
                    return false;                
            }

            /* the addTail() method inserts a node at the first available space starting from the peek node. This method
             * can also be used to create a complete binary tree (a tree in which all nodes are completely filled except
             * the last level; in the last level, all the nodes must be as left as possible; in a complete binary tree, 
             * the nodes should be added from the left)
            */
            bool addTail (size_t id, const T& data) {
                m_Node* parentNode = Tree <T>::peekNode();
                // invalid peek id
                if (parentNode == NULL)
                    return false;

                std::queue <m_Node*> pendingQ;
                pendingQ.push (parentNode);

                while (!pendingQ.empty()) {
                    m_Node* currentNode = pendingQ.front();
                    pendingQ.pop();
                    
                    // set peek to current node
                    Tree <T>::peekSet (currentNode-> id);

                    if (addLeft (id, data) == false)
                        pendingQ.push (peekChild (LEFT_CHILD));
                    else
                        break;

                    if (addRight (id, data) == false)
                        pendingQ.push (peekChild (RIGHT_CHILD));
                    else
                        break;
                }

                /* restore peek to the parent (instead of the last set peek), note that we are not using the save and 
                 * restore methods since the calls to addLeft() or addRight() would overwrite the saved peek with the 
                 * current node (we should be careful when nesting methods with save-restore statements)
                */
                Tree <T>::peekSet (parentNode-> id);
                return true;
            }

            /* sticky peek pair set to
             * { node, level }  -> at new parent node, if valid id
            */
            bool addParent (size_t id, const T& data) {
                if (Tree <T>::addParent (id, data) == true) {
                    // peek is at the newly added parent, you can add NULL child now
                    Tree <T>::addNullChild();                   
                    return true;
                }
                
                // invalid peek id
                else
                    return false;
            }

            /* (1) find the node we want to delete
             * (2) then, find the deepest node in the binary tree 
             * (3) swap the deepest node with the 'node to be deleted'
             * (4) add NULL child to the parent of 'node to be deleted'
             * (5) then delete (or adopt) the node using remove()
             * 
             * returns either of the 3 pairs
             * { NULL, false }  -> invalid id/empty tree
             * { node, true }   -> adopted node, valid id
             * { NULL, true }   -> remove complete, valid id
             * 
             * sticky peek pair set to
             * { NULL, 0 }      -> reset peek, if valid id
            */
            std::pair <m_Node*, bool> removeNode (bool adopt = false) {
                // (1)
                m_Node* nodeToDelete = Tree <T>::peekNode();
                
                // (2)
                m_Node* deepestNode  = getDeepest();
                
                // invalid nodes
                if (nodeToDelete == NULL || deepestNode == NULL)
                    return { NULL, false };

                // (3)
                Tree <T>::swap (nodeToDelete-> id, deepestNode-> id);

                // (4) 
                m_Node* parentNode = nodeToDelete-> parent;
                if (parentNode != NULL) {
                    Tree <T>::peekSet (parentNode-> id);
                    Tree <T>::addNullChild();
                }

                // (5) here we use the remove() method to grab the node and it's two NULL children
                Tree <T>::peekSet (nodeToDelete-> id);
                return Tree <T>::remove (adopt);
            }

            /* returns either of the 3 pairs
             * { NULL, false }  -> invalid id
             * { node, true }   -> adopted node/tree, valid id
             * { NULL, true }   -> remove complete, valid id
             * 
             * sticky peek pair set to
             * { NULL, 0 }      -> reset peek, if valid id
            */
            std::pair <m_Node*, bool> remove (bool adopt = false) {
                m_Node* nodeToDelete = Tree <T>::peekNode();
                // invalid peek id
                if (nodeToDelete == NULL)
                    return { NULL, false };

                // add NULL child in place of deleted node
                m_Node* parentNode = nodeToDelete-> parent;
                if (parentNode != NULL) {
                    Tree <T>::peekSet (parentNode-> id);
                    Tree <T>::addNullChild();
                }

                Tree <T>::peekSet (nodeToDelete-> id);
                return Tree <T>::remove (adopt);
            }

            void addRoot (size_t id, const T& data) {
                Tree <T>::savePeek();
                /* if first node in the tree
                 *                          {root}
                 *                          |
                 *                  -----------------
                 *                  |               |
                 *                  {NULL}          {NULL}
                */
                if (Tree <T>::m_rootNode == NULL) {
                    Tree <T>::addRoot (id, data);

                    // set left and right child to NULL
                    Tree <T>::peekSetRoot();
                    Tree <T>::addNullChild();
                    Tree <T>::addNullChild();
                }

                /* if addRoot() is called on an existing tree
                 *                          {new root}
                 *                          |
                 *                  -----------------
                 *                  |               |
                 *                  {old root}      {NULL}
                 *                  |
                 *          -----------------
                 *          |               |
                 *          {NULL}          {NULL}
                */
                else {
                    Tree <T>::addRoot (id, data);

                    // set right child to NULL
                    Tree <T>::peekSetRoot();
                    Tree <T>::addNullChild();
                }

                Tree <T>::restorePeek();
            }

            m_Node* getDeepest (void) {
                if (Tree <T>::m_rootNode == NULL)
                    return NULL;

                // the last element in the tail vector will be the deepest (right most) node
                else {
                    Tree <T>::savePeek();
                    Tree <T>::peekSet (Tree <T>::getTails().back());

                    m_Node* deepestNode = Tree <T>::peekNode();

                    Tree <T>::restorePeek();
                    return deepestNode;
                }
            }
    };
}   // namespace Memory
}   // namespace Collections
#endif  // BTREE_IMPL_H