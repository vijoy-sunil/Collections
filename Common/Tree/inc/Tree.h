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
#ifndef TREE_H
#define TREE_H

#include "TreeMgr.h"

/* types of methods                             tree update                 pre-run
 * (1) mgr                                      n/a                         n/a
 * (2) set                                      no                          n/a
 * (3) peek                                     no                          set
 * (4) update                                   yes                         set
 * (5) stand-alone                              yes                         no
 * (6) utils                                    no                          no
*/

// 'mgr' methods
#define TREE_INIT(id, dataType)                 Memory::treeMgr.initTree <dataType> (id)
#define GET_TREE(id, dataType)                  dynamic_cast <Memory::Tree <dataType> *>                        \
                                                (Memory::treeMgr.getInstance (id))  
#define TREE_CLOSE(id)                          Memory::treeMgr.closeInstance (id)
#define TREE_CLOSE_ALL                          Memory::treeMgr.closeAllInstances()
#define TREE_MGR_DUMP                           Memory::treeMgr.dump (std::cout)

// 'set' methods
#define TREE_PEEK_SET(id)                       peekSet (id)
#define TREE_PEEK_SET_ROOT                      peekSetRoot()
#define TREE_PEEK_SET_NEXT                      peekSetNext()

// 'peek' methods
#define TREE_PEEK_NODE                          peekNode()
#define TREE_PEEK_LEVEL                         peekLevel()
#define TREE_PEEK_CHILD_COUNT                   peekChildCount()
#define TREE_PEEK_IS_END                        peekIsEnd()

// 'update' methods
#define TREE_ADD_CHILD(id, data)                addChild (id, data)
#define TREE_ADD_NULL_CHILD                     addNullChild()
#define TREE_APPEND(node)                       appendTree (node)
#define TREE_ADD_PARENT(id, data)               addParent (id, data)
#define TREE_REMOVE_NODE                        removeNode()
#define TREE_ADOPT_NODE                         removeNode (true)
#define TREE_REMOVE                             remove()
#define TREE_ADOPT                              remove (true)

// 'stand-alone' methods
#define TREE_ADD_ROOT(id, data)                 addRoot (id, data)
#define TREE_SWAP(idA, idB)                     swap (idA, idB)
#define TREE_IMPORT(node)                       importTree (node)
#define TREE_RESET                              importTree (NULL)

// 'utils' methods
#define TREE_SIZE                               getSize()
#define TREE_DEPTH                              getDepth()
#define TREE_PATH(idA, idB)                     getPath (idA, idB)
#define TREE_TAILS                              getTails()
#define TREE_DUMP                               dump (std::cout)
#define TREE_DUMP_CUSTOM(lambda)                dump (std::cout, lambda)                                  
#endif  // TREE_H