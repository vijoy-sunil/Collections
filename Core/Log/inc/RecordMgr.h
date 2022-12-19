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
#ifndef RECORD_MGR_H
#define RECORD_MGR_H

#include "Record.h"

namespace Collections {
namespace Quality {
namespace Log {
    class RecordMgr: public Admin::InstanceMgr {
        public:
            void initRecord (size_t instanceId, 
                             e_level level, 
                             e_sink sink,
                             size_t bufferCapacity = 0,
                             std::string format = ".txt") {
            
                // add record object to pool
                if (m_instancePool.find (instanceId) == m_instancePool.end()) {
                    Admin::NonTemplateBase* c_record = new Record (instanceId, 
                                                                   level, 
                                                                   sink, 
                                                                   bufferCapacity,
                                                                   format);

                    m_instancePool.insert (std::make_pair (instanceId, c_record));
                }

                // instance id already exists
                else
                    assert (false);
            }

            void closeRecord (size_t instanceId) {
                if (m_instancePool.find (instanceId) != m_instancePool.end()) { 
                    Record* c_record = static_cast <Record*> (m_instancePool[instanceId]);

                    // flush buffered sink
                    if (c_record-> getSink() & TO_FILE_BUFFER_CIRCULAR)
                        c_record-> flushBufferToFile();

                    delete c_record;
                    // remove from map, so you are able to reuse the instance id
                    m_instancePool.erase (instanceId);       
                }
                // closing a record instance that doesn't exist, do nothing
                else
                    ;
            }

            void closeAllRecords (void) {
                for (auto const& [key, val] : m_instancePool) {
                    Record* c_record = static_cast <Record*> (val);
                    // flush buffered sink
                    if (c_record-> getSink() & TO_FILE_BUFFER_CIRCULAR)
                        c_record-> flushBufferToFile();

                    delete m_instancePool[key];
                }
                m_instancePool.clear();
            }
    };
    // single instance
    RecordMgr recordMgr;
}   // namespace Log
}   // namespace Quality
}   // namespace Collections
#endif  // RECORD_MGR_H