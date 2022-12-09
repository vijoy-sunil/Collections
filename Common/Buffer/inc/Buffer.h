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

// buffer calls available to user, simplifies fn calls
#define BUFFER_INIT(id,                                                                                     \
                    type,                                                                                   \
                    dataType,                                                                               \
                    capacity)                   MemoryCollections::bufferMgr.initBuffer <dataType> (id, type, capacity)

#define BUFFER_PUSH(id, dataType)               GET_BUFFER (id, dataType)
#define BUFFER_POP(id, dataType)                GET_BUFFER (id, dataType).pop()
#define BUFFER_RESET(id, dataType)              GET_BUFFER (id, dataType).reset()
#define BUFFER_PEEK_FIRST(id, dataType)         GET_BUFFER (id, dataType).peekFirst()
#define BUFFER_PEEK_LAST(id, dataType)          GET_BUFFER (id, dataType).peekLast()
#define BUFFER_AVAILABILITY(id, dataType)       GET_BUFFER (id, dataType).availability()

#define BUFFER_CLOSE(id)                        MemoryCollections::bufferMgr.closeBuffer (id)
#define BUFFER_CLOSE_ALL                        MemoryCollections::bufferMgr.closeAllBuffers()

// default sink for buffer dump is set to cout
#define BUFFER_DUMP(id, dataType)               GET_BUFFER (id, dataType).dump (std::cout)
/* use this to dump buffer containing custom data types by passing in a lambda function specifying how to unravel the
 * custom data type
*/
#define BUFFER_DUMP_CUSTOM(id,                                                                             \
                           dataType,                                                                       \
                           lambda)              GET_BUFFER (id, dataType).dump (std::cout, lambda)
#define BUFFER_MGR_DUMP                         MemoryCollections::bufferMgr.dump (std::cout)

// under the hood
#define GET_BUFFER(id, dataType)                dynamic_cast <MemoryCollections::Buffer <dataType> &>       \
                                                (*MemoryCollections::bufferMgr.getBuffer (id))                                    
#endif  // BUFFER_H