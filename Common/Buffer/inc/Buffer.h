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
#ifndef BUFFER_H
#define BUFFER_H

#include "BufferMgr.h"

// buffer manager methods
#define BUFFER_INIT(id,                                                                                         \
                    type,                                                                                       \
                    dataType,                                                                                   \
                    capacity)                   Memory::bufferMgr.initBuffer <dataType> (id, type, capacity)
// get buffer instance (pointer to buffer object) from mgr
#define GET_BUFFER(id, dataType)                dynamic_cast <Memory::Buffer <dataType> *>                      \
                                                (Memory::bufferMgr.getInstance (id)) 
#define BUFFER_CLOSE(id)                        Memory::bufferMgr.closeInstance (id)
#define BUFFER_CLOSE_ALL                        Memory::bufferMgr.closeAllInstances()
#define BUFFER_MGR_DUMP                         Memory::bufferMgr.dump (std::cout)  

#define BUFFER_PUSH(data)                       push (data)
#define BUFFER_POP                              pop()
#define BUFFER_FLUSH(stream)                    flush (stream)
#define BUFFER_PEEK_FIRST                       peekFirst()
#define BUFFER_PEEK_LAST                        peekLast()
#define BUFFER_AVAILABILITY                     availability()
#define BUFFER_RESET                            reset()
// default sink for buffer dump is set to cout
#define BUFFER_DUMP                             dump (std::cout)
/* use this to dump buffer containing custom data types by passing in a lambda function specifying how to unravel the
 * custom data type
*/
#define BUFFER_DUMP_CUSTOM(lambda)              dump (std::cout, lambda)
#endif  // BUFFER_H