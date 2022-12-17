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

#define LIST_INIT(id, dataType)                 Memory::listMgr.initList <dataType> (id)

// 'set' operations
#define LIST_PEEK_SET(id, dataType)             GET_LIST (id, dataType).peekSet
#define LIST_PEEK_SET_HEAD(id, dataType)        GET_LIST (id, dataType).peekSetHead()
#define LIST_PEEK_SET_TAIL(id, dataType)        GET_LIST (id, dataType).peekSetTail()
#define LIST_PEEK_SET_NEXT(id, dataType)        GET_LIST (id, dataType).peekSetNext()
#define LIST_PEEK_SET_PREVIOUS(id, dataType)    GET_LIST (id, dataType).peekSetPrevious()

// 'execute' operations
#define LIST_PEEK_CURRENT(id, dataType)         GET_LIST (id, dataType).peekCurrent()
#define LIST_PEEK_HEAD(id, dataType)            GET_LIST (id, dataType).peekHead()
#define LIST_PEEK_TAIL(id, dataType)            GET_LIST (id, dataType).peekTail()

#define LIST_ADD_HEAD(id, dataType)             GET_LIST (id, dataType).addHead
#define LIST_ADD_TAIL(id, dataType)             GET_LIST (id, dataType).addTail
#define LIST_ADD_AFTER(id, dataType)            GET_LIST (id, dataType).addAfter
#define LIST_ADD_BEFORE(id, dataType)           GET_LIST (id, dataType).addBefore

#define LIST_REMOVE(id, dataType)               GET_LIST (id, dataType).remove()
#define LIST_REMOVE_HEAD(id, dataType)          GET_LIST (id, dataType).removeHead()
#define LIST_REMOVE_TAIL(id, dataType)          GET_LIST (id, dataType).removeTail()

// utils
#define LIST_REVERSE(id, dataType)              GET_LIST (id, dataType).reverse()
#define LIST_SWAP(id, dataType)                 GET_LIST (id, dataType).swap
#define LIST_RESET(id, dataType)                GET_LIST (id, dataType).reset()
#define LIST_SIZE(id, dataType)                 GET_LIST (id, dataType).getSize()

#define LIST_CLOSE(id)                          Memory::listMgr.closeList (id)
#define LIST_CLOSE_ALL                          Memory::listMgr.closeAllLists()

#define LIST_DUMP(id, dataType)                 GET_LIST (id, dataType).dump (std::cout)
#define LIST_DUMP_CUSTOM(id,                                                                                \
                         dataType,                                                                          \
                         lambda)                GET_LIST (id, dataType).dump (std::cout, lambda)

#define LIST_MGR_DUMP                           Memory::listMgr.dump (std::cout)

// under the hood
#define GET_LIST(id, dataType)                  dynamic_cast <Memory::List <dataType> &>       \
                                                (*Memory::listMgr.getList (id))                                    
#endif  // LIST_H