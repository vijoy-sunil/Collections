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

// dump formatting
#define LOG_DUMP_LINE_LENGTH                50
#define LOG_DUMP_LINE_STYLE                 '.'
#define LOG_DUMP_LINE_BREAK                 std::string (LOG_DUMP_LINE_LENGTH, LOG_DUMP_LINE_STYLE) << std::endl

namespace Collections {
namespace Quality {
namespace Log {
    class RecordMgr {
        private:
            // container for records, allows us to use multiple logs in one project
            std::map <size_t, Record*> m_recordPool;

        public:
            void initRecord (size_t instanceId, 
                             e_level level, 
                             e_sink sink,
                             size_t bufferCapacity = 0,
                             std::string format = ".txt") {
            
                // add record object to pool
                if (m_recordPool.find (instanceId) == m_recordPool.end()) {
                    Record *c_record = new Record (instanceId, 
                                                   level, 
                                                   sink, 
                                                   bufferCapacity,
                                                   format);

                    m_recordPool.insert (std::make_pair (instanceId, c_record));
                }

                // instance id already exists
                else
                    assert (false);
            }

            Record* getRecord (size_t instanceId) {
                Record* c_record;
                if (m_recordPool.find (instanceId) != m_recordPool.end())
                    c_record = m_recordPool[instanceId];
                
                // invalid instance id
                else  
                    assert (false);

                return c_record;
            }

            void closeRecord (size_t instanceId) {
                if (m_recordPool.find (instanceId) != m_recordPool.end()) { 
                    // flush buffered sink
                    if (m_recordPool[instanceId]->getSink() & TO_FILE_BUFFER_CIRCULAR)
                        m_recordPool[instanceId]->flushBufferToFile();

                    delete m_recordPool[instanceId];
                    // remove from map, so you are able to reuse the instance id
                    m_recordPool.erase (instanceId);       
                }
                // closing a record instance that doesn't exist, do nothing
                else
                    ;
            }

            void closeAllRecords (void) {
                for (auto const& [key, val] : m_recordPool) {
                    // flush buffered sink
                    if (val->getSink() & TO_FILE_BUFFER_CIRCULAR)
                        val->flushBufferToFile();

                    delete m_recordPool[key];
                }
                m_recordPool.clear();
            }

            // X-LX-SX is the format in which the isntance info is dumped (-L is the level, -S is the sink)
            void dump (std::ostream& ost) {
                ost << LOG_DUMP_LINE_BREAK;
                ost << "LOG MGR DUMP" 
                    << "\n"; 
                ost << LOG_DUMP_LINE_BREAK;

                ost << "INSTANCES: "
                    << "\t";
                
                for (auto const& [key, value] : m_recordPool) {
                    ost << "[ " << key
                        << "-L" 
                        << value->getLevel()
                        << "-S"
                        << value->getSink()
                        << " ] ";
                }
                ost << "\n";
                ost << LOG_DUMP_LINE_BREAK;    
            }
    };
    // single instance
    RecordMgr recordMgr;
}   // namespace Log
}   // namespace Quality
}   // namespace Collections
#endif  // RECORD_MGR_H