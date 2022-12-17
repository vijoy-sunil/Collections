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
#ifndef LIB_TEST_H
#define LIB_TEST_H

#include "LibTestMgr.h"
#include "LambdaMgr.h"

#define LIB_TEST_INIT                       Quality::Test::libTestMgr.initLibTestMgr  
#define LIB_TEST_CASE(id, desc)             MAKE_FN (id);                                       \
                                            namespace Collections {                             \
                                            namespace Quality {                                 \
                                            namespace Test {                                    \
                                            auto LAMBDA_BASE (id) = []() {                      \
                                                libTestMgr.registerTest (FN_BASE (id),          \
                                                                    id,                         \
                                                                    desc);                      \
                                            };                                                  \
                                            LambdaMgr l_ ## id (LAMBDA_BASE (id));              \
                                            }                                                   \
                                            }                                                   \
                                            }                                                   \
                                            MAKE_FN (id)
        
#define LIB_TEST_RUN(id)                    Quality::Test::libTestMgr.runTest (id)
#define LIB_TEST_RUN_ALL                    Quality::Test::libTestMgr.runAllTests()

/* The ## provides a way to concatenate actual arguments during macro expansion. If a parameter in the replacement text is 
 * adjacent to a ##, the parameter is replaced by the actual argument, the ## and surrounding white space are removed, and 
 * the result is re-scanned.
*/
#define LAMBDA_BASE(id)                     lambda_ ## id
#define FN_BASE(id)                         test_ ## id
#define MAKE_FN(id)                         Quality::Test::e_status FN_BASE (id) (void)
#endif  // LIB_TEST_H