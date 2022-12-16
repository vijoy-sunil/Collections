#ifndef HEAP_MGR_HPP
#define HEAP_MGR_HPP

#include "../Logger/Logger.hpp"
#include "../../Common/ErrorCode.h"
#include <stdint.h>
#include <chrono>
#include <ctime>
#include <map>

namespace HeapMgr_ns {
    typedef enum {
        C_STYLE,
        CPP_STYLE
    }allocationType_t;
}

class HeapMgr {
    private:
        HeapMgr_ns::allocationType_t allocationType;
        
        typedef struct {
            std::time_t timeOfRequest;
            uint8_t* functionName;
            size_t requestedBytes;
            size_t totalBytes;
            bool isFreed;
        }requestPacket_t;

        std::map<uint8_t*, requestPacket_t> logEntry;
        Logger loggerHeapMgr;

    public:
        HeapMgr(HeapMgr_ns::allocationType_t _allocationType, 
                Logger_ns::logType_t _logType, const std::string fileName = "");
        ~HeapMgr(void);

        uint8_t* requestMemory(size_t numBytes, bool clearMemory = false);
        errorCode_t freeMemory(uint8_t* ptr);
        errorCode_t logRequest(requestPacket_t requestPacket);
};

#endif