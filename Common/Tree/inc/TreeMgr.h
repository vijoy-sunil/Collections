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
#ifndef TREE_MGR_H
#define TREE_MGR_H

#include "TreeImpl.h"

namespace Collections {
namespace Memory {
    class TreeMgr: public Admin::InstanceMgr {
        public:
            template <typename T>
            Tree <T>* initTree (size_t instanceId) {

                if (m_instancePool.find (instanceId) == m_instancePool.end()) {
                    Tree <T>* c_tree = new Tree <T> (instanceId);

                    Admin::NonTemplateBase* c_instance = c_tree;
                    m_instancePool.insert (std::make_pair (instanceId, c_instance));
                    return c_tree;
                }
                // instance id already exists
                else
                    assert (false);
            }
    };
    TreeMgr treeMgr;
}   // namespace Memory
}   // namespace Collections
#endif  // TREE_MGR_H