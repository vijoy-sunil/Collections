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
#ifndef LIST_MGR_H
#define LIST_MGR_H

#include "ListImpl.h"
#include <cassert>
#include <map>

namespace Collections {
namespace Memory {
    class ListMgr {
        private:
            std::map <size_t, ListBase*> m_listPool;

        public:
            template <typename T>
            void initList (size_t instanceId) {

                // create and add list object to pool
                if (m_listPool.find (instanceId) == m_listPool.end()) {
                    ListBase* c_list = new List <T> (instanceId);
                    m_listPool.insert (std::make_pair (instanceId, c_list));
                }
                // instance id already exists
                else
                    assert (false);
            }

            ListBase* getList (size_t instanceId) {
                if (m_listPool.find (instanceId) != m_listPool.end())
                    return m_listPool[instanceId];
                // invalid instance id
                else
                    assert (false);
            }

            void closeList (size_t instanceId) {
                if (m_listPool.find (instanceId) != m_listPool.end()) {
                    delete m_listPool[instanceId];
                    // remove from map, so you are able to reuse the instance id
                    m_listPool.erase (instanceId);       
                }
                // closing a list instance that doesn't exist, do nothing
                else
                    ;
            }

            void closeAllLists (void) {
                for (auto const& [key, val] : m_listPool)
                    delete m_listPool[key];

                // clear all entries in pool
                m_listPool.clear();
            }   

            void dump (std::ostream& ost) {
                ost << LIST_DUMP_LINE_BREAK;
                ost << "LIST MGR DUMP" 
                    << "\n"; 
                ost << LIST_DUMP_LINE_BREAK;

                ost << "INSTANCES: "
                    << "\t";
                for (auto const& [key, val] : m_listPool)
                    ost << "[ " << key << " ] ";

                ost << "\n";
                ost << LIST_DUMP_LINE_BREAK;
            }
    };
    ListMgr listMgr;
}   // namespace Memory
}   // namespace Collections
#endif  // LIST_MGR_H