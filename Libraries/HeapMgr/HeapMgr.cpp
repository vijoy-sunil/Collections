#include "HeapMgr.hpp"

using namespace Logger_ns;
using namespace HeapMgr_ns;

HeapMgr::HeapMgr(allocationType_t _allocationType, 
                 logType_t _logType, const std::string fileName): 
                 loggerHeapMgr(_logType, fileName) {
    allocationType = _allocationType;
    /* set up log file
    */
   loggerHeapMgr.addLog(INFO, __func__, "allocationType:", allocationType);
}

HeapMgr::~HeapMgr(void) {

}

uint8_t* HeapMgr::requestMemory(size_t numBytes, bool clearMemory) {
    if(allocationType == C_STYLE) {
        uint8_t* ptr = (uint8_t*)malloc(sizeof(uint8_t) * numBytes);
        if(ptr != NULL) {
            
        }

        return ptr;
    }
    else {
        
    }
}

errorCode_t HeapMgr::freeMemory(uint8_t* ptr) {

}

errorCode_t HeapMgr::logRequest(requestPacket_t requestPacket) {

}
