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
#ifndef BTREE_MGR_H
#define BTREE_MGR_H

#include "BTreeImpl.h"

namespace Collections {
namespace Memory {
    class BTreeMgr: public Admin::InstanceMgr {
        public:
            template <typename T>
            BTree <T>* initBTree (size_t instanceId) {

                if (m_instancePool.find (instanceId) == m_instancePool.end()) {
                    BTree <T>* c_bTree = new BTree <T> (instanceId);

                    Admin::NonTemplateBase* c_instance = c_bTree;
                    m_instancePool.insert (std::make_pair (instanceId, c_instance));
                    return c_bTree;
                }
                // instance id already exists
                else
                    assert (false);
            }
    };
    BTreeMgr bTreeMgr;
}   // namespace Memory
}   // namespace Collections
#endif  // BTREE_MGR_H