# Collections

## Import
<pre>
using namespace Collections;
</pre>

## Directory structure
<pre>
    Collections
    |-- Common
        |-- Buffer
        |-- LibTest                 
    |-- Core          
        |-- List
        |-- Log
        |-- Tree
</pre>

## Namespaces
<pre>
    Collections
    |-- Memory
        |-- <i>Buffer</i>
        |-- <i>List</i>
        |-- <i>Tree</i>
    |-- Quality
        |-- Test
            |-- <i>LibTest</i>
        |-- Log
            |-- <i>Log</i>
</pre>

## Sample code
>*See complete usage in respective sample/ directory*

### Buffer
<pre>
    #include "Common/Buffer/inc/Buffer.h"

    // create a new buffer ('myBuffer' is a pointer to the buffer instance created)
    auto myBuffer = BUFFER_INIT (0,                                 // instance id
                                 Memory::WITH_OVERFLOW,             // circular buffer type        
                                 int,                               // holds integer
                                 capacity);                         // buffer capacity

    // close this buffer using its instance id
    BUFFER_CLOSE (0);
</pre>

>*Buffer can be used as Queue or Stack using available methods*

### LibTest
<pre>
    #include "Common/LibTest/inc/LibTest.h"

    // define a test case with a test id and description
    LIB_TEST_CASE (2, "sample test") {
        .
        .
        .
        // a test case must return either PASS or FAIL
        return Quality::Test::FAIL;
    } 

    // create a test and specify where to dump the test output
    LIB_TEST_INIT (Quality::Test::TO_CONSOLE |                      // dump to console 
                   Quality::Test::TO_FILE);                         // dump to file (./LibTestSaves/libtest_log.txt)

    // run all tests that have been defined
    LIB_TEST_RUN_ALL;

    // or run them one at a time by specifying test id
    LIB_TEST_RUN (2);
</pre>

### List
<pre>
    #include "Core/List/inc/List.h"

    // create a new list ('myList' is a pointer to the list instance created)
    auto myList = LIST_INIT (0,                                     // instance id
                             int);                                  // holds integer

    // close this list using its instance id
    LIST_CLOSE (0);
</pre>

>*List can be used as Queue or Stack using available methods*

### Log
<pre>
    #include "Core/Log/inc/Log.h"

    // create log instance
    auto myLog = LOG_INIT (0,                                       // instance id 
                           Quality::Log::INFO,                      // only log INFO level messages
                           Quality::Log::TO_FILE_IMMEDIATE |        // dump log to file (in ./LogSaves/)
                           Quality::Log::TO_FILE_BUFFER_CIRCULAR |  // dump log to circular buffered file with capacity
                           Quality::Log::TO_CONSOLE,                // dump log to console
                           5);                                      // circular buffered log file capacity

    // log an info level message
    LOG_INFO (myLog) << "Hello World! " 
                     << "This is a test message. " 
                     << 123 
                     << "," 
                     << 10.1010 
                     << std::endl;

    // close this log using its instance id 
    LOG_CLOSE (0);
</pre>

## Tree
<pre>
    #include "Core/Tree/inc/Tree.h"

    // create a new tree ('myTree' is a pointer to the tree instance created)
    auto myTree = TREE_INIT (0,                                     // instance id 
                             int);                                  // holds integer

    // close this tree using its instance id
    TREE_CLOSE (0);
</pre>