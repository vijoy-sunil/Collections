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
#ifndef LIST_H
#define LIST_H

#include "ListMgr.h"

// list mgr methods
#define LIST_INIT(id, dataType)                 Memory::listMgr.initList <dataType> (id)
#define GET_LIST(id, dataType)                  dynamic_cast <Memory::List <dataType> *>                        \
                                                (Memory::listMgr.getInstance (id))  
#define LIST_CLOSE(id)                          Memory::listMgr.closeInstance (id)
#define LIST_CLOSE_ALL                          Memory::listMgr.closeAllInstances()
#define LIST_MGR_DUMP                           Memory::listMgr.dump (std::cout)

// 'set' operations
#define LIST_PEEK_SET(id)                       peekSet (id)
#define LIST_PEEK_SET_HEAD                      peekSetHead()
#define LIST_PEEK_SET_TAIL                      peekSetTail()
#define LIST_PEEK_SET_NEXT                      peekSetNext()
#define LIST_PEEK_SET_PREVIOUS                  peekSetPrevious()

// 'execute' operations (these need to be executed after a 'set' operation)
#define LIST_PEEK_CURRENT                       peekCurrent()
#define LIST_PEEK_HEAD                          peekHead()
#define LIST_PEEK_TAIL                          peekTail()

#define LIST_ADD_HEAD(data)                     addHead (data)
#define LIST_ADD_TAIL(data)                     addTail (data)
#define LIST_ADD_AFTER(data)                    addAfter (data)
#define LIST_ADD_BEFORE(data)                   addBefore (data)

#define LIST_REMOVE                             remove()
#define LIST_REMOVE_HEAD                        removeHead()
#define LIST_REMOVE_TAIL                        removeTail()

// utils
#define LIST_REVERSE                            reverse()
#define LIST_SWAP(idA,idB)                      swap (idA, idB)
#define LIST_RESET                              reset()
#define LIST_SIZE                               getSize()
#define LIST_DUMP                               dump (std::cout)
#define LIST_DUMP_CUSTOM(lambda)                dump (std::cout, lambda)                                  
#endif  // LIST_H