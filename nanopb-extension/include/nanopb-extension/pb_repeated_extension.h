#pragma once
#include "pb_common_header.h"
#include <functional>
#include <vector>

class pb_repeated_array_stream
{
public:
    pb_repeated_array_stream():
        arrayBufferPointer(nullptr)
    {}

    ~pb_repeated_array_stream()
    {
    }
    
    template<typename T>
    pb_repeated_array_stream* OpenStream(std::vector<T*>* stream_vector, const pb_msgdesc_t* stream_data_type_desc)
    {
        arrayBufferPointer = stream_vector;
        arrayDataTypeMsgDesc = stream_data_type_desc;

        cbGetArraySize = [&] () -> size_t {
            std::vector<T*>* vec = (std::vector<T*>*)arrayBufferPointer;
            return vec->size();
        };

        cbGetArrayAt = [&] (size_t pos) -> void* {
            std::vector<T*>* vec = (std::vector<T*>*)arrayBufferPointer;
            return vec->at(pos);
        };

        cbAllocateBufferAndInsert = [&] () -> void* {
            std::vector<T*>* vec = (std::vector<T*>*)arrayBufferPointer;
            T* c = new T();
            vec->push_back(c);
            return c;
        };

        return this;
    }

    size_t GetSize()
    {
        return cbGetArraySize();
    }

    void* GetAt(size_t i)
    {
        return cbGetArrayAt(i);
    }

    const pb_msgdesc_t* GetMsgDesc() { return arrayDataTypeMsgDesc; }
    void* AllocateBufferAndInsert() { return cbAllocateBufferAndInsert(); }
private:
    void* arrayBufferPointer;
    const pb_msgdesc_t* arrayDataTypeMsgDesc;

    std::function<size_t()> cbGetArraySize;
    std::function<void*(size_t i)> cbGetArrayAt;
    std::function<void*()> cbAllocateBufferAndInsert;
};

template<typename T>
inline pb_repeated_array_stream* pb_create_repeated_array_stream(std::vector<T*>* vec, const pb_msgdesc_t* desc)
{
    return (new pb_repeated_array_stream())->OpenStream(vec, desc);
}

inline bool pb_destroy_repeated_array_stream(pb_repeated_array_stream* arrayStream)
{
    delete arrayStream;
    return true;
}

typedef std::function<void(void* buffer)> pb_repeated_extension_setup_decode_callback;
typedef std::function<void(size_t indexNumber, void* indexBuffer)> pb_repeated_extension_iterator_callback;

bool pb_create_repeated_encode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream);
bool pb_create_repeated_decode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream, pb_repeated_extension_setup_decode_callback field_setup_callback);

bool pb_iterate_repeated_callback(pb_callback_t* field, pb_repeated_extension_iterator_callback field_iteration_callback);

bool pb_destroy_repeated_encode_callback(pb_callback_t* field, bool deallocate_array_stream = true);
bool pb_destroy_repeated_decode_callback(pb_callback_t* field, bool deallocate_array_stream = true);