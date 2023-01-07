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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// output dirs
const std::string LOG_SAVE_DIR      =   "./LogSaves";
const std::string LIBTEST_SAVE_DIR  =   "./LibTestSaves";

// dump formatting
#define TAB_L1                          "\t"
#define TAB_L2                          TAB_L1 "\t"
#define TAB_L3                          TAB_L2 "\t"
#define TAB_L4                          TAB_L3 "\t"
#define TAB_L5                          TAB_L4 "\t"
#define TAB_L6                          TAB_L5 "\t"

#define OPEN_L1                         TAB_L1 << "{" << "\n"
#define OPEN_L3                         TAB_L3 << "{" << "\n"
#define OPEN_L5                         TAB_L5 << "{" << "\n"

#define CLOSE_L1                        TAB_L1 << "}" << "\n"
#define CLOSE_L3                        TAB_L3 << "}" << "\n"
#define CLOSE_L5                        TAB_L5 << "}" << "\n"

/* reserved id use cases (these ids are not to be used by user as instance ids)
 * 
 * RESERVED_0                           used by TREE to capture depth of the tree
 * RESERVED_1                           used by TREE as dummy id in swap operation
 * RESERVED_2                           used as offset instance id for the buffer used in LOG. An offset instance id is an
 *                                      instance id obtained by adding the offset to its parent instance id
*/
#define RESERVED_0                      SIZE_MAX
#define RESERVED_1                      SIZE_MAX - 1
#define RESERVED_2                      99
#endif  // CONSTANTS_H