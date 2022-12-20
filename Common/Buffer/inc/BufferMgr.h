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
#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "BufferImpl.h"

namespace Collections {
namespace Memory {
    class BufferMgr: public Admin::InstanceMgr {
        public:
            template <typename T>
            Buffer <T>* initBuffer (size_t instanceId, 
                                    e_type type, 
                                    size_t capacity) {

                // create and add buffer object to pool
                if (m_instancePool.find (instanceId) == m_instancePool.end()) {
                    Buffer <T>* c_buffer = new Buffer <T> (instanceId, type, capacity);

                    // upcasting
                    Admin::NonTemplateBase* c_instance = c_buffer;
                    m_instancePool.insert (std::make_pair (instanceId, c_instance));

                    return c_buffer;
                }
                // instance id already exists
                else
                    assert (false);
            }
    };
    BufferMgr bufferMgr;
}   // namespace Memory
}   // namespace Collections
#endif  // BUFFER_MGR_H