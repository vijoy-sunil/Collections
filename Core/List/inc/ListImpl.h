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
#ifndef LIST_IMPL_H
#define LIST_IMPL_H

#include <iostream>

// dump formatting
#define LIST_DUMP_LINE_LENGTH               50
#define LIST_DUMP_LINE_STYLE                '~'
#define LIST_DUMP_LINE_BREAK                std::string (LIST_DUMP_LINE_LENGTH, LIST_DUMP_LINE_STYLE) << std::endl

namespace Collections {
namespace Memory {
    class ListBase {
        public:
            virtual ~ListBase() = 0;
    };
    inline ListBase::~ListBase() {} 

    template <typename T>
    class List: public ListBase {
        private:
            // node definition
            typedef struct Node {
                size_t id;
                Node* next;
                Node* previous;
                T data;
            }s_Node;

            size_t m_instanceId;
            size_t m_numNodes;
            size_t m_nextAvailableId;

            s_Node* m_headNode;
            s_Node* m_tailNode;
            s_Node* m_peekNode;
            
            s_Node* createNode (const T& data) {
                s_Node* newNode = new s_Node;
                m_numNodes++;

                newNode-> id = m_nextAvailableId;
                // this is used to set the id of newly created nodes (decoupled with num nodes)
                m_nextAvailableId++;

                newNode-> next = NULL;
                newNode-> previous = NULL;
                newNode-> data = data;
                return newNode;
            }

            s_Node* getNode (size_t id) {
                // if id is not found (invalid), this method returns NULL

                // set head node as start point of search
                s_Node* currentNode  = m_headNode;
                while (currentNode != NULL) {
                    // id found
                    if (currentNode-> id == id)
                        break;
                
                    currentNode = currentNode-> next;
                }
                return currentNode;
            }

            // used in list dump function, the nodes are displated in format: [ data -id -prevId -nextId ]
            void dumpNode (s_Node* currentNode, 
                           std::ostream& ost, 
                           void (*lambda) (T*, std::ostream&)) {

                if (currentNode != NULL) {
                    // use the lambda function to unravel node data
                    lambda (&(currentNode-> data), ost);
                    ost << " -I" << currentNode-> id;

                    if (currentNode-> previous != NULL)
                        ost << " -P" << currentNode-> previous-> id;
                    if (currentNode-> next != NULL)
                        ost << " -N" << currentNode-> next-> id;
                }
            }

        public:
            List (size_t instanceId) {
                m_instanceId = instanceId;
                m_numNodes = 0;
                m_nextAvailableId = 0;

                m_headNode = NULL;
                m_tailNode = NULL;
                m_peekNode = NULL;
            }

            ~List (void) {
                // destroy all nodes
                reset();
            }

            inline void peekSet (size_t id) {
                m_peekNode = getNode (id);
            }

            inline void peekSetHead (void) {
                m_peekNode = m_headNode;
            }

            inline void peekSetTail (void) {
                m_peekNode = m_tailNode;
            }

            void peekSetNext (void) {
                if (m_peekNode == NULL)
                   return;

                m_peekNode = m_peekNode-> next;
            }

            void peekSetPrevious (void) {
                if (m_peekNode == NULL)
                    return;

                m_peekNode = m_peekNode-> previous;
            }

            inline s_Node* peekCurrent (void) {
                return m_peekNode;
            }

            s_Node* peekHead (void) {
                peekSetHead();
                return peekCurrent();
            }

            s_Node* peekTail (void) {
                peekSetTail();
                return peekCurrent();
            }

            void addHead (const T& data) {
                s_Node* newNode = createNode (data);
                // create link
                if (m_headNode != NULL) {
                    newNode-> next = m_headNode;
                    m_headNode-> previous = newNode;
                }

                // if new node is the only node in the list
                else
                    m_tailNode = newNode; 

                // update head node
                m_headNode = newNode;
            }

            void addTail (const T& data) {
                s_Node* newNode = createNode (data);
                // create link
                if (m_headNode != NULL) {
                    m_tailNode-> next = newNode;
                    newNode-> previous = m_tailNode;
                }

                // if new node is the only node in the list
                else
                    m_headNode = newNode;

                // update tail node
                m_tailNode = newNode;
            }

            bool addAfter (const T& data) {
                s_Node* currentNode = peekCurrent();
                // id not found
                if (currentNode == NULL)
                    return false;

                // if NOI is the tail node
                if (currentNode == m_tailNode)
                    addTail (data);    

                else {
                    s_Node* newNode = createNode (data);
                    // save link
                    s_Node* nextNode = currentNode-> next;

                    // create link
                    currentNode-> next = newNode;
                    newNode-> previous = currentNode;

                    newNode-> next = nextNode;
                    nextNode-> previous = newNode;
                }       
                return true;     
            }

            bool addBefore (const T& data) {
                s_Node* currentNode = peekCurrent();
                // id not found
                if (currentNode == NULL)
                    return false;

                // if NOI is the head node
                if (currentNode == m_headNode)
                    addHead (data);

                else {
                    s_Node* newNode = createNode (data);
                    s_Node* previousNode = currentNode-> previous;

                    // create link
                    previousNode-> next = newNode;
                    newNode-> previous = previousNode;

                    newNode-> next = currentNode;
                    currentNode-> previous = newNode;
                }
                return true;
            }

            bool remove (void) {
                s_Node* currentNode = peekCurrent();
                // id not found
                if (currentNode == NULL)
                    return false;

                // if NOI is tail node
                if (currentNode == m_tailNode)
                    m_tailNode = currentNode-> previous;
                else
                    currentNode-> next-> previous = currentNode-> previous;

                // if NOI is head node
                if (currentNode == m_headNode)
                    m_headNode = currentNode-> next;
                else
                    currentNode-> previous-> next = currentNode-> next;

                // remove node
                delete currentNode;
                // set peek position to NULL since we have removed the node
                m_peekNode = NULL;

                m_numNodes--;
                return true;
            }
            
            bool removeHead (void) {
                // set peek position to head
                peekSetHead();
                return remove();
            }

            bool removeTail (void) {
                // set peek position to tail
                peekSetTail();
                return remove();
            }

            void reverse (void) {
                s_Node* currentNode = m_headNode;
                // if list is empty
                if (currentNode == NULL)
                    return;

                // if there is only one node
                if (m_numNodes == 1)
                    return;

                // swap next and previous pointers
                while (currentNode != NULL) {
                    s_Node* tempNode = currentNode-> next;
                    currentNode-> next = currentNode-> previous;
                    currentNode-> previous = tempNode;

                    // move to next node
                    currentNode = currentNode-> previous;
                }

                // swap head node and tail node
                s_Node* tempNode = m_headNode;
                m_headNode = m_tailNode;
                m_tailNode = tempNode;
            }

            bool swap (size_t idA, size_t idB) {
                // no need to swap if both ids are same
                if (idA == idB)
                    return true;

                // get nodes
                s_Node* nodeA = getNode (idA);
                s_Node* nodeB = getNode (idB);

                // id not found
                if (nodeA == NULL || nodeB == NULL)
                    return false;

                // https://stackoverflow.com/questions/20095529/swapping-nodes-in-double-linked-list
                
                // swap A and B if B comes before A and they are adjacent
                if (nodeB-> next == nodeA) {
                    s_Node* tempNode = nodeB;
                    nodeB = nodeA;
                    nodeA = tempNode;
                }

                s_Node* swapperVector[4];
                swapperVector[0] = nodeA-> previous;
                swapperVector[1] = nodeB-> previous;
                swapperVector[2] = nodeA-> next;
                swapperVector[3] = nodeB-> next;

                // if A and B are adjacent { 0, 1, 2, 3 } -> { 2, 0, 3, 1 }
                if (nodeA-> next == nodeB) {
                    nodeA-> previous = swapperVector[2];
                    nodeB-> previous = swapperVector[0];
                    nodeA-> next = swapperVector[3];
                    nodeB-> next = swapperVector[1];
                }
                // else { 0, 1, 2, 3 } -> { 1, 0, 3, 2 }
                else {
                    nodeA-> previous = swapperVector[1];
                    nodeB-> previous = swapperVector[0];
                    nodeA-> next = swapperVector[3];
                    nodeB-> next = swapperVector[2];
                }
                // set head and tail node links
                m_headNode = (nodeA-> previous == NULL) ? nodeA :
                             (nodeB-> previous == NULL) ? nodeB : 
                             m_headNode;
                m_tailNode = (nodeA-> next == NULL) ? nodeA :
                             (nodeB-> next == NULL) ? nodeB : 
                             m_tailNode;

                // refresh outer pointers 
                if (nodeA-> previous != NULL)
                    nodeA-> previous-> next = nodeA;

                if (nodeA-> next != NULL)
                    nodeA-> next-> previous = nodeA;

                if (nodeB-> previous != NULL)
                    nodeB-> previous-> next = nodeB;

                if (nodeB-> next != NULL)
                    nodeB-> next-> previous = nodeB;

                return true;
            }

            void reset (void) {
                s_Node* currentNode = m_headNode;
                while (currentNode != NULL) {
                    s_Node* nextNode = currentNode-> next;
                    delete currentNode;

                    currentNode = nextNode;
                }

                // reset stats
                m_numNodes = 0;
                m_nextAvailableId = 0;

                m_headNode = NULL;
                m_tailNode = NULL;
                m_peekNode = NULL;
            }

            inline size_t getSize (void) {
                return m_numNodes;
            }

            void dump (std::ostream& ost, 
                       void (*lambda) (T*, std::ostream&) = [](T* nodeData, std::ostream& ost) { 
                                                                ost << *nodeData; 
                                                            }) {
                ost << LIST_DUMP_LINE_BREAK;
                ost << "LIST DUMP" 
                    << "\n"; 
                ost << LIST_DUMP_LINE_BREAK;

                ost << "CONTENTS: "
                    << "\t";
                
                s_Node* currentNode = m_headNode;
                while (currentNode != NULL) {
                    ost << "[ ";
                    dumpNode (currentNode, ost, lambda);
                    ost << " ] ";

                    currentNode = currentNode-> next;
                }
                ost << "\n";

                ost << "ID/TYPE: "
                    << "\t"
                    << "[ "
                    << m_instanceId << "/" << typeid (T).name()
                    << " ]"
                    << "\n";

                ost << "NUM NODES: "
                    << "\t"
                    << "[ "
                    << m_numNodes
                    << " ]"
                    << "\n";

                ost << "LAST USED ID: "
                    << "\t"
                    << "[ ";
                if (m_nextAvailableId != 0)
                    ost << m_nextAvailableId - 1;
                ost << " ]"
                    << "\n";

                ost << "HEAD NODE: "
                    << "\t"
                    << "[ ";
                dumpNode (m_headNode, ost, lambda);
                ost << " ]"
                    << "\n";

                ost << "TAIL NODE: "
                    << "\t"
                    << "[ ";
                dumpNode (m_tailNode, ost, lambda);
                ost << " ]"
                    << "\n";

                ost << "LAST PEEKED: "
                    << "\t"
                    << "[ ";
                dumpNode (m_peekNode, ost, lambda);
                ost << " ]"
                    << "\n";                                      
                
                ost << LIST_DUMP_LINE_BREAK;
            }
    };
}   // namespace Memory
}   // namespace Collections
#endif  // LIST_IMPL_H