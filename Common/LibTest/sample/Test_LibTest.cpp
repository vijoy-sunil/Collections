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

#include "../inc/LibTest.h"
using namespace Collections;

int main (void) {
    // init the test
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE | Quality::Test::TO_FILE);

    // you can either run individual tests by specifying test ids
    LIB_TEST_RUN (2);
    LIB_TEST_RUN (4);
    LIB_TEST_RUN (6);

    // or run all the tests
    LIB_TEST_RUN_ALL;

    /* a test is only run ONCE even if you run it individually (using test id) AND collectively (using run all method) at
     * the same time
    */
    return 0;
}

// define a test case by specifying test id and test description
LIB_TEST_CASE (2, "sample pass test") {
    // a test case must return either PASS or FAIL
    return (1 + 1 == 2) ? Quality::Test::PASS : Quality::Test::FAIL;
} 

LIB_TEST_CASE (4, "sample fail test") {
    return (2 * 1024 == 0) ? Quality::Test::PASS : Quality::Test::FAIL;
} 

LIB_TEST_CASE (6, "sample fail test") {
    return (10 / 5 == 0) ? Quality::Test::PASS : Quality::Test::FAIL;
} 