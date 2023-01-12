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
#ifndef BTREE_H
#define BTREE_H

#include "BTreeMgr.h"
// more info on the different types of methods can be found in Tree.h

// 'mgr' methods
#define BTREE_INIT(id, dataType)                Memory::bTreeMgr.initBTree <dataType> (id)
#define GET_BTREE(id, dataType)                 dynamic_cast <Memory::BTree <dataType> *>                       \
                                                (Memory::bTreeMgr.getInstance (id))  
#define BTREE_CLOSE(id)                         Memory::bTreeMgr.closeInstance (id)
#define BTREE_CLOSE_ALL                         Memory::bTreeMgr.closeAllInstances()
#define BTREE_MGR_DUMP                          Memory::bTreeMgr.dump (std::cout)

// 'set' methods
#define BTREE_PEEK_SET(id)                      peekSet (id)
#define BTREE_PEEK_SET_ROOT                     peekSetRoot()
#define BTREE_PEEK_SET_NEXT                     peekSetNext()

// 'peek' methods
#define BTREE_PEEK_NODE                         peekNode()
#define BTREE_PEEK_LEVEL                        peekLevel()
#define BTREE_PEEK_CHILD_COUNT                  peekChildCount()
#define BTREE_PEEK_IS_END                       peekIsEnd()
#define BTREE_PEEK_LEFT                         peekChild (static_cast <Memory::e_child> (0))
#define BTREE_PEEK_RIGHT                        peekChild (static_cast <Memory::e_child> (1))

// 'update' methods
#define BTREE_ADD_LEFT(id, data)                addLeft (id, data)
#define BTREE_ADD_RIGHT(id, data)               addRight (id, data)
#define BTREE_ADD_TAIL(id, data)                addTail (id, data)
#define BTREE_ADD_PARENT(id, data)              addParent (id, data)
#define BTREE_REMOVE_NODE                       removeNode()
#define BTREE_ADOPT_NODE                        removeNode (true)
#define BTREE_REMOVE                            remove()
#define BTREE_ADOPT                             remove (true)

// 'stand-alone' methods
#define BTREE_ADD_ROOT(id, data)                addRoot (id, data)
#define BTREE_SWAP(idA, idB)                    swap (idA, idB)
#define BTREE_IMPORT(node)                      importTree (node)
#define BTREE_RESET                             importTree (NULL)

// 'utils' methods
#define BTREE_SIZE                              getSize()
#define BTREE_DEPTH                             getDepth()
#define BTREE_DEEPEST                           getDeepest()
#define BTREE_PATH(idA, idB)                    getPath (idA, idB)
#define BTREE_TAILS                             getTails()
#define BTREE_DUMP                              dump (std::cout)
#define BTREE_DUMP_CUSTOM(lambda)               dump (std::cout, lambda)                                  
#endif  // BTREE_H