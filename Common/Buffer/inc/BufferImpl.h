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
#ifndef BUFFER_IMPL_H
#define BUFFER_IMPL_H

#include <iostream>

// dump formatting
#define BUFFER_DUMP_LINE_LENGTH             50
#define BUFFER_DUMP_LINE_STYLE              '~'
#define BUFFER_DUMP_LINE_BREAK              std::string (BUFFER_DUMP_LINE_LENGTH, BUFFER_DUMP_LINE_STYLE) << std::endl

namespace MemoryCollections {
    typedef enum {
        CIRCULAR_NO_OVERWRITE = 1,
        CIRCULAR_OVERWRITE = 2
    }e_type;

    /* Buffer alone is not a type, but a template which can generate a family of types, such as  Buffer <int> and 
     * Buffer<double>. All these variants are not related such that the one is somehow derived from the other or such. So 
     * you have to establish some relation between all  these generated types. One way is to use a common non-template base
     * class
     */
    class BufferBase {
        public:
            /* the destructor is virtual for the base class because if you did not have a virtual destructor and through 
             * the pointer to base class when you call destructor you end up calling base class destructor. In this case 
             * you want polymorphism to work on your destructor as well, e.g. through calling destructor on your base class
             * you want to end up calling destructor of your most derived class not your base class.
            */
            virtual ~BufferBase() = 0;
    };
    /* we need to provide an implementation to the pure virtual destructor of the base class, this is because If you derive
     * anything from base and then try to delete or destroy it, base's destructor will eventually be called. Since it is 
     * pure and doesn't have an implementation, undefined behavior will ensue.
    */
    inline BufferBase::~BufferBase() {} 

    template <typename T>
    class Buffer: public BufferBase {
        private:
            size_t m_instanceId;
            e_type m_type;
            size_t m_capacity;
            size_t m_numItems;

            T* m_buffer;
            // operate then increment
            T* m_head;
            T* m_tail;
            T* m_end;

            inline bool isEmpty (void) {
                return m_numItems == 0;
            } 

            inline bool isFull (void) {
                return m_numItems == m_capacity;
            }

        public:
            Buffer (size_t instanceId, e_type type, size_t capacity) {
                m_instanceId = instanceId;
                m_type = type;
                m_capacity = capacity;
                m_numItems = 0;

                m_buffer = new T[capacity];
                
                m_head = m_buffer;
                m_tail = m_buffer;
                m_end = m_buffer + m_capacity - 1;
            }

            ~Buffer (void) {
                delete[] m_buffer;
            }
            
            // overload << operator for push operation
            Buffer& operator << (const T& data) {
                // always push when in overwrite mode
                if (!isFull() || m_type == CIRCULAR_OVERWRITE) {
                    *m_head = data;
                    m_numItems++;

                    // when head pointer is at the end of the buffer
                    m_head = m_head == m_end ? m_buffer : m_head + 1;

                    /* if num items is greater than capacity, that means we have overflowed over the oldest element, so
                     * we need to update the tail pointer (pointing to the oldest element) and correct num items
                    */
                    if (m_type == CIRCULAR_OVERWRITE && m_numItems > m_capacity) {
                        m_tail = m_tail == m_end ? m_buffer : m_tail + 1;
                        // correct num items
                        m_numItems--;
                    }
                }

                // if push fails due to maximum capacity, do nothing
                else
                    ;

                return *this;
            }

            T* pop (void) {
                T* data = NULL;

                if (!isEmpty()) {
                    data = m_tail;
                    m_numItems--;

                    // when tail pointer is at the end of the buffer
                    m_tail = m_tail == m_end ? m_buffer : m_tail + 1;
                }   
                return data; 
            }
            
            inline T* peekFirst (void) {
                return isEmpty() ? NULL : m_tail;
            }

            inline T* peekLast (void) {
                return isEmpty() ? NULL : 
                /* head pointer will be at the start either when the buffer is empty, or when an item has been inserted at 
                 * the end and wrap around is complete
                */
                m_head == m_buffer ? m_end : (m_head - 1);
            }

            inline size_t availability (void) {
                return m_capacity - m_numItems;
            }

            void reset (void) {
                m_numItems = 0;
                m_head = m_buffer;
                m_tail = m_buffer;
            }

            void dump (std::ostream& ost, 
                       void (*lambda) (T*, std::ostream&) = [](T* readPtr, std::ostream& ost) { ost << *readPtr; }) {
                T* readPtr = m_tail;
                size_t numItems = m_numItems;

                ost << BUFFER_DUMP_LINE_BREAK;
                ost << "BUFFER DUMP" 
                    << "\n"; 
                ost << BUFFER_DUMP_LINE_BREAK;

                ost << "CONTENTS: "
                    << "\t";
                while (numItems != 0) {
                    ost << "[ ";
                    lambda (readPtr, ost);
                    ost << " ] ";

                    numItems--;
                    readPtr = readPtr == m_end ? m_buffer : readPtr + 1;
                }
                ost << "\n";

                ost << "ID/TYPE: "
                    << "\t"
                    << "[ "
                    << m_instanceId << "/" << typeid (T).name()
                    << " ]"
                    << "\n";

                ost << "AVAILABILITY: "
                    << "\t"
                    << "[ " 
                    << availability() << "/" << m_capacity
                    << " ]"
                    << "\n";

                ost << "PEEK: "
                    << "\t\t";
                if (m_numItems != 0) {
                    ost << "[ ";
                    lambda (peekFirst(), ost);
                    ost << "/";
                    lambda (peekLast(), ost);                     
                    ost << " ]" ;
                }
                ost << "\n";
                ost << BUFFER_DUMP_LINE_BREAK;
            }
    };
}   // namespace MemoryCollections
#endif  // BUFFER_IMPL_H