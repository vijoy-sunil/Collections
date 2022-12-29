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
#ifndef LIB_TEST_MGR_H
#define LIB_TEST_MGR_H

#include "../../../Admin/Constants.h"
#include <iostream>
#include <map>
#include <cassert>
#include <fstream>
#include <vector>
#include <chrono>
#include <filesystem>

// https://en.cppreference.com/w/cpp/chrono/duration
#define ELAPSED_TIME_CAST               std::chrono::milliseconds
#define ELAPSED_TIME_UNIT               "ms"

// dumb formatting
#define DUMP_LINE_LENGTH                100
#define DUMP_LINE_STYLE                 '-'
#define DUMP_LINE_BREAK                 std::string (DUMP_LINE_LENGTH, DUMP_LINE_STYLE) << std::endl

namespace Collections {
namespace Quality {
namespace Test {
    // test result sink types
    typedef enum {
        TO_CONSOLE = 1,
        TO_FILE = 2
    }e_sink;

    // test status
    typedef enum {
        PASS = 0,
        FAIL = 1,
        PENDING = 2
    }e_status;

    // define bit operators for enum
    inline e_sink operator | (e_sink a, e_sink b) {
        return static_cast <e_sink> (static_cast <int> (a) | static_cast <int> (b));
    }
   
    class LibTestMgr {
        private:
            size_t m_numTests;
            e_sink m_sink;

            // keep track of stats
            std::vector <size_t> m_passedTests,
                                 m_failedTests, 
                                 m_pendingTests;
            
            std::ofstream m_saveFile;
            std::string m_saveFileName = LIBTEST_SAVE_DIR + "/libtest_log";

            // map to store fn pointers to test cases
            std::map <size_t, e_status (*) (void)> m_tests;
            
            // map to store desc and test status
            std::map <size_t, std::pair <std::string, e_status> > m_summary;
            
            std::string statusToString (e_status status) {
                std::string result;
                switch (status) {
                    case PASS:
                        result = "PASSED";
                        break;

                    case FAIL:
                        result = "FAILED";
                        break;

                    case PENDING:
                        result = "PENDING";
                        break;
                    
                    default:
                        result = "UNDF";
                        break;
                }
                return result;
            }

            void dumpSummary (std::ostream& ost, 
                              ELAPSED_TIME_CAST::rep elapsed) {
                ost << DUMP_LINE_BREAK;
                ost << "TEST SUMMARY" 
                    << "\n"; 
                ost << DUMP_LINE_BREAK;

                ost << "TOTAL TESTS: " 
                    << "\t"
                    << "[ " << m_numTests << " ]"
                    << "\n"; 

                ost << "ELAPSED TIME: "
                    << "\t"
                    << "[ " << elapsed << " " << ELAPSED_TIME_UNIT << " ]"
                    << "\n"; 

                ost << "PASSED: "
                    << "\t"
                    << "[ " << m_passedTests.size() << "/" << m_numTests << " ]"
                    << "\n"; 

                ost << "\t\t"
                    << "[ ";
                for (auto const& id : m_passedTests)
                    ost << id << " ";
                ost << "]"
                    << "\n"; 

                ost << "FAILED: "
                    << "\t"
                    << "[ " << m_failedTests.size() << "/" << m_numTests << " ]"
                    << "\n"; 

                ost << "\t\t"
                    << "[ ";
                for (auto const& id : m_failedTests)
                    ost << id << " ";
                ost << "]"
                    << "\n"; 

                ost << "PENDING: "
                    << "\t"
                    << "[ " << m_pendingTests.size() << "/" << m_numTests << " ]"
                    << "\n"; 

                ost << "\t\t"
                    << "[ ";
                for (auto const& id : m_pendingTests)
                    ost << id << " ";
                ost << "]"
                    << "\n";  

                ost << DUMP_LINE_BREAK;
            }

            void dumpTestResult (size_t testId, 
                                 std::ostream& ost,
                                 ELAPSED_TIME_CAST::rep elapsed) {
                ost << DUMP_LINE_BREAK;
                
                ost << "ID: "
                    << "\t\t"
                    << "[ " << testId << " ]"
                    << "\n"; 

                ost << "DESCRIPTION: "
                    << "\t"
                    << "[ " << m_summary[testId].first << " ]"
                    << "\n"; 
                
                ost << "STATUS: "
                    << "\t"
                    << "[ " << statusToString (m_summary[testId].second) << " ]"
                    << "\n"; 

                ost << "ELAPSED TIME: "
                    << "\t"
                    << "[ " << elapsed << " " << ELAPSED_TIME_UNIT << " ]"
                    << "\n";
                
                ost << DUMP_LINE_BREAK; 
            }

            inline bool ifTestIdExist (size_t testId) {
                return m_summary.find (testId) != m_summary.end();
            }

        public:
            LibTestMgr (void) {}

            ~LibTestMgr (void) {
                if (m_sink & TO_FILE)
                    m_saveFile.close();
            }

            void initLibTestMgr (e_sink sink,
                                 std::string format = ".txt") {
                m_sink = sink;

                // append format to file name
                m_saveFileName += format;
                // create dir
                std::filesystem::create_directories(LIBTEST_SAVE_DIR);
                // open file, overwrite on every test run
                if (m_sink & TO_FILE) { 
                    m_saveFile.open (m_saveFileName, 
                                     std::ios_base::out);

                    if (!m_saveFile.is_open())
                        assert (false);
                }
            }

            void registerTest (e_status (*testCase) (void),
                               size_t testId, 
                               std::string desc) {
                m_numTests++;

                // save function pointer
                m_tests[testId] = testCase;

                // save test description and set test status to PENDING
                m_summary[testId].first = desc;
                m_summary[testId].second = PENDING;

                m_pendingTests.push_back (testId);
            }

            void runTest (size_t testId) {
                // assert on invalid test id
                if (!ifTestIdExist (testId))
                    assert (false);
                
                // run test only if it is in pending state
                if (m_summary[testId].second != PENDING)
                    return;

                // compute elapsed time using steday clock
                auto begin = std::chrono::steady_clock::now();

                // execute test and save result
                m_summary[testId].second = m_tests[testId]();

                auto end = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast <ELAPSED_TIME_CAST> (end - begin);

                // update stats
                m_summary[testId].second == PASS ? 
                                            m_passedTests.push_back (testId) : 
                                            m_failedTests.push_back (testId);

                m_pendingTests.erase (std::remove (m_pendingTests.begin(), 
                                                   m_pendingTests.end(), 
                                                   testId),
                                      m_pendingTests.end());

                if (m_sink & TO_CONSOLE)
                    dumpTestResult (testId, std::cout, elapsed.count());
                if (m_sink & TO_FILE)
                    dumpTestResult (testId, m_saveFile, elapsed.count());
            }

            void runAllTests (void) {
                /* there will be significant overhead when running all tests due to individual dumps and stats update per 
                 * test
                */
                auto begin = std::chrono::steady_clock::now();
                // run all tests
                for (const auto& [key, val] : m_tests)
                    runTest (key);

                auto end = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast <ELAPSED_TIME_CAST> (end - begin);

                if (m_sink & TO_CONSOLE)
                    dumpSummary (std::cout, elapsed.count());
                if (m_sink & TO_FILE)
                    dumpSummary (m_saveFile, elapsed.count());
            }
    };
    // single instance
    LibTestMgr libTestMgr;
}   // namespace Test
}   // namespace Quality
}   // namespace Collections
#endif  // LIB_TEST_MGR_H