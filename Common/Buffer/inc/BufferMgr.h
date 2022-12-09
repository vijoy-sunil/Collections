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
#include <cassert>
#include <map>

namespace Collections {
namespace Memory {
    class BufferMgr {
        private:
            std::map <size_t, BufferBase*> m_bufferPool;

        public:
            template <typename T>
            void initBuffer (size_t instanceId, 
                             e_type type, 
                             size_t capacity) {

                // create and add buffer object to pool
                if (m_bufferPool.find (instanceId) == m_bufferPool.end()) {
                    BufferBase* c_buffer = new Buffer <T> (instanceId, type, capacity);
                    m_bufferPool.insert (std::make_pair (instanceId, c_buffer));
                }
                // instance id already exists
                else
                    assert (false);
            }

            BufferBase* getBuffer (size_t instanceId) {
                if (m_bufferPool.find (instanceId) != m_bufferPool.end())
                    return m_bufferPool[instanceId];
                // invalid instance id
                else
                    assert (false);
            }

            void closeBuffer (size_t instanceId) {
                if (m_bufferPool.find (instanceId) != m_bufferPool.end()) {
                    delete m_bufferPool[instanceId];
                    // remove from map, so you are able to reuse the instance id
                    m_bufferPool.erase (instanceId);       
                }
                // closing a buffer instance that doesn't exist, do nothing
                else
                    ;
            }

            void closeAllBuffers (void) {
                for (auto const& [key, val] : m_bufferPool)
                    delete m_bufferPool[key];

                // clear all entries in pool
                m_bufferPool.clear();
            }   

            void dump (std::ostream& ost) {
                ost << BUFFER_DUMP_LINE_BREAK;
                ost << "BUFFER MGR DUMP" 
                    << "\n"; 
                ost << BUFFER_DUMP_LINE_BREAK;

                ost << "INSTANCES: "
                    << "\t";
                for (auto const& [key, val] : m_bufferPool)
                    ost << "[ " << key << " ] ";

                /* realistically, we would like to dump more information about each instance ids but that would require us 
                 * to cast it back to its original type which is not available within the mgr
                */
                ost << "\n";
                ost << BUFFER_DUMP_LINE_BREAK;
            }
    };
    BufferMgr bufferMgr;
}   // namespace Memory
}   // namespace Collections
#endif  // BUFFER_MGR_H