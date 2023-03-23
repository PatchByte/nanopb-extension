#include "utils.h"

#include <sstream>
#include <iomanip>

std::string utils::buffer_to_hex_string(void* _dataBuffer, size_t dataBufferSize)
{
    unsigned char* dataBuffer = (unsigned char*)_dataBuffer;

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < dataBufferSize; ++i)
    {
        ss << std::setw(2) << static_cast<unsigned>(dataBuffer[i]);
    }
    return ss.str();
}