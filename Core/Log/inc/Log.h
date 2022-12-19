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
#ifndef LOG_H
#define LOG_H

#include "RecordMgr.h"

// mgr methods
#define LOG_INIT                                Quality::Log::recordMgr.initRecord
#define GET_LOG(id)                             static_cast <Quality::Log::Record*>                                 \
                                                (Quality::Log::recordMgr.getInstance (id))
// override close methods from instance mgr
#define LOG_CLOSE(id)                           Quality::Log::recordMgr.closeRecord (id)
#define LOG_CLOSE_ALL                           Quality::Log::recordMgr.closeAllRecords()
/* Normally, the dump method in the manager contains only the instance ids that are currently active, but we can use a
 * lambda function to provide more information about each instance. For example, here we can dump the level and the sink
 * type of each instance in the format: instanceId-LX-SX (-L is the level, -S is the sink)
*/
#define LOG_MGR_DUMP                            auto lambda = [](Admin::NonTemplateBase* val, std::ostream& ost) {  \
                                                Quality::Log::Record* c_record =                                    \
                                                static_cast <Quality::Log::Record*> (val);                          \
                                                ost << "-L"                                                         \
                                                    << c_record-> getLevel()                                        \
                                                    << "-S"                                                         \
                                                    << c_record-> getSink();                                        \
                                                };                                                                  \
                                                Quality::Log::recordMgr.dump (std::cout, lambda)

// logging methods
#define LOG_INFO(c_record)                      LOG (c_record, Quality::Log::INFO)
#define LOG_WARNING(c_record)                   LOG (c_record, Quality::Log::WARNING)
#define LOG_ERROR(c_record)                     LOG (c_record, Quality::Log::ERROR)

// under the hood   
#define LOG_GET_FILE                            __FILE__
#define LOG_GET_FUNCTION                        __FUNCTION__
#define LOG_GET_LINE                            __LINE__
#define LOG(c_record, level)                    if (! (c_record-> filterLevel (level))) { ; }                       \
                                                else                                                                \
                                                    c_record-> getReference() <<                                    \
                                                    c_record-> getHeader (level,                                    \
                                                                          LOG_GET_FILE,                             \
                                                                          LOG_GET_FUNCTION,                         \
                                                                          LOG_GET_LINE) 
#endif  // LOG_H