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
#ifndef INSTANCE_MGR_H
#define INSTANCE_MGR_H

#include <iostream>
#include <map>
#include <cassert>
#include "Constants.h"

namespace Collections {
namespace Admin {
    /* A template which can generate a family of types, such as  Buffer <int> or List <double>, all these variants are 
     * not related such that the one is somehow derived from the other or such. So you have to establish some relation 
     * between all these generated types in order store them in a single container (map). One way is to use a common non
     * template base class
     */
    class NonTemplateBase {
        public:
            /* the destructor is virtual for the base class because if you did not have a virtual destructor and through 
             * the pointer to base class when you call destructor you end up calling base class destructor. In this case 
             * you want polymorphism to work on your destructor as well, e.g. through calling destructor on your base class
             * you want to end up calling destructor of your most derived class not JUST your base class.
            */
            virtual ~NonTemplateBase() = 0;
    };
    /* pure virtual Destructors must be defined, which is against the pure virtual behaviour. The only difference between 
     * Virtual and Pure Virtual Destructor is, that pure virtual destructor will make its Base class Abstract, hence you 
     * cannot create object of that class (hence why we are doing it). We need an implementation here because If you derive
     * anything from base (UPCASTING) and then try to delete or destroy it, base's destructor will eventually be called. 
     * Since it is pure and doesn't have an implementation, will cause compilation error
    */
    inline NonTemplateBase::~NonTemplateBase() {} 

    // all managers will be derived from this class, which allows us to have multiple instances
    class InstanceMgr {
        protected:
            std::map <size_t, NonTemplateBase*> m_instancePool;

        public:
            ~InstanceMgr (void) {
                closeAllInstances();
            }

            NonTemplateBase* getInstance (size_t instanceId) {
                if (m_instancePool.find (instanceId) != m_instancePool.end())
                    return m_instancePool[instanceId];

                // invalid instance id
                else
                    assert (false);
            }

            void closeInstance (size_t instanceId) {
                if (m_instancePool.find (instanceId) != m_instancePool.end()) {
                    delete m_instancePool[instanceId];
                    // remove from map, so you are able to reuse the instance id
                    m_instancePool.erase (instanceId);       
                }
                // closing an instance that doesn't exist, do nothing
                else
                    ;
            }

            void closeAllInstances (void) {
                for (auto const& [key, val] : m_instancePool)
                    delete m_instancePool[key];

                // clear all entries in pool
                m_instancePool.clear();
            }   

            void dump (std::ostream& ost, 
                       void (*lambda) (NonTemplateBase*, std::ostream&) = 
                       [](NonTemplateBase* instance, std::ostream& ost) {
                            // do nothing
                       }) {
                ost << DUMP_LINE_BREAK;
                ost << "INSTANCE MGR DUMP" 
                    << "\n"; 
                ost << DUMP_LINE_BREAK;

                ost << "INSTANCES: "
                    << "\t";
                    
                for (auto const& [key, val] : m_instancePool) {
                    ost << "[ " << key;
                    /* dump more information about each instance ids through lambda, since the mgr itself doesn't have
                     * much information unless we cast it back to original type
                    */
                    lambda (val, ost);
                    ost << " ] ";
                }

                ost << "\n";
                ost << DUMP_LINE_BREAK;
            }
    };
}   // namespace Admin
}   // namespace Collections
#endif  // INSTANCE_MGR_H