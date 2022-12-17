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
#include "Utils.h"

// log calls available to user, simplifies fn calls
#define LOG_INIT                                Quality::Log::recordMgr.initRecord
#define LOG_INFO(id)                            LOG (id, Quality::Log::INFO)
#define LOG_WARNING(id)                         LOG (id, Quality::Log::WARNING)
#define LOG_ERROR(id)                           LOG (id, Quality::Log::ERROR)
#define LOG_CLOSE(id)                           Quality::Log::recordMgr.closeRecord (id)
#define LOG_CLOSE_ALL                           Quality::Log::recordMgr.closeAllRecords()
#define LOG_MGR_DUMP                            Quality::Log::recordMgr.dump (std::cout)

// conditional call, a lightweight alternative that only acts as a gate based on level
#define IF_LOG(id, level)                       if (Quality::Log::filter (Quality::Log::recordMgr.getRecord (id), level))
#define IF_NOT_LOG(id, level)                   if (!Quality::Log::filter (Quality::Log::recordMgr.getRecord (id), level))

// under the hood
#define LOG_GET_FILE                            __FILE__
#define LOG_GET_FUNCTION                        __FUNCTION__
#define LOG_GET_LINE                            __LINE__

#define LOG_GET_REF(id)                         Quality::Log::recordMgr.getRecord (id)->getReference()                
#define LOG(id, level)                          IF_NOT_LOG (id, level) {;}                                  \
                                                else                                                        \
                                                    LOG_GET_REF (id) <<                                     \
                                                    Quality::Log::getHeader (id,                            \
                                                                            level,                          \
                                                                            LOG_GET_FILE,                   \
                                                                            LOG_GET_FUNCTION,               \
                                                                            LOG_GET_LINE)
#endif  // LOG_H