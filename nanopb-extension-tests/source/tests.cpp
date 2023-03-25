#include "tests.h"
#include "utils.h"
#include "test.pb.h"

#include <pb_encode.h>
#include <pb_decode.h>

#include <nanopb-extension/pb_string_extension.h>
#include <nanopb-extension/pb_submessage_extension.h>
#include <nanopb-extension/pb_repeated_extension.h>

#include <string>

static std::string_view smTestString = "Test String!";
static std::string_view smSubString  = "Sub String!";

bool tests::TestStringNormal()
{
    TestStruct testStruct = { 0 };
    pb_byte_t testStructData[128] = { 0 };

    if(!pb_create_string_encode_callback_and_set(&testStruct.testString, smTestString.data()))
    {
        return false;
    }

    pb_ostream_t outputStream = pb_ostream_from_buffer(testStructData, sizeof(testStructData));

    if(!pb_encode(&outputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    if(!pb_destroy_string_encode_callback(&testStruct.testString))
    {
        return false;
    }

    std::cout << "TestStringNormal encoded: " << utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) << std::endl;

    if(utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) != "0a0c5465737420537472696e6721")
    {
        return false;
    }

    if(!pb_create_string_decode_callback(&testStruct.testString))
    {
        return false;
    }

    pb_istream_t inputStream = pb_istream_from_buffer(testStructData, outputStream.bytes_written);
    
    if(!pb_decode(&inputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    std::cout << "TestStruct.testString = \"" << pb_destroy_string_decode_callback_and_get(&testStruct.testString) << "\"" << std::endl;

    return true;
}

bool tests::TestSubmessageNormal()
{
    TestStruct testStruct = { 0 };
    pb_byte_t testStructData[128] = { 0 };

    pb_create_submessage_encode_callback(&testStruct.subStruct, sizeof(SubStruct), SubStruct_fields, 
    [] (void* _buffer) {
        // Initialization Callback
        SubStruct* buffer = reinterpret_cast<SubStruct*>(_buffer);

        buffer->v0 = 0xdead;
        buffer->v1 = 0xbeef;

        pb_create_string_encode_callback_and_set(&buffer->v2, smSubString.data());
    }, 
    [] (void* _buffer) 
    {
        // Deallocation Callback
        SubStruct* buffer = reinterpret_cast<SubStruct*>(_buffer);
        pb_destroy_string_encode_callback(&buffer->v2);
    });

    pb_create_string_encode_callback_and_set(&testStruct.testString, smTestString.data());

    pb_ostream_t outputStream = pb_ostream_from_buffer(testStructData, sizeof(testStructData));

    if(!pb_encode(&outputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    pb_destroy_string_encode_callback(&testStruct.testString);
    pb_destroy_submessage_encode_callback(&testStruct.subStruct);

    std::cout << "TestSubmessageNormal encoded: " << utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) << std::endl;

    if(utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) != "0a0c5465737420537472696e6721121508adbd0310effd021a0b53756220537472696e6721")
    {
        return false;
    }

    memset(&testStruct, 0, sizeof(testStruct));

    pb_create_submessage_decode_callback(&testStruct.subStruct, sizeof(SubStruct), SubStruct_fields, 
    [] (void* _buffer) {
        SubStruct* buffer = reinterpret_cast<SubStruct*>(_buffer);
        pb_create_string_decode_callback(&buffer->v2);
    },
    [] (void* _buffer) {
        SubStruct* buffer = reinterpret_cast<SubStruct*>(_buffer);

        std::cout << "testStruct.subStruct.v0 = " << std::hex << buffer->v0 << std::endl;
        std::cout << "testStruct.subStruct.v1 = " << std::hex << buffer->v1 << std::endl;
        std::cout << "testStruct.subStruct.v2 = " << std::hex << pb_destroy_string_decode_callback_and_get(&buffer->v2) << std::endl;
    });

    pb_istream_t inputStream = pb_istream_from_buffer(testStructData, outputStream.bytes_written);
    
    if(!pb_decode(&inputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    pb_destroy_submessage_decode_callback(&testStruct.subStruct);

    return true;
}

bool tests::TestRepeatedNormal()
{
    TestStruct testStruct = { 0 };
    pb_byte_t testStructData[512] = { 0 };
    std::vector<SubStruct*> arrayVector = {  };

    for(int i = 0; i < 10; i++)
    {
        SubStruct* s = new SubStruct();
        
        s->v0 = i * i;
        s->v1 = i;

        pb_create_string_encode_callback_and_set(&s->v2, std::to_string(i) + std::string("_string"));

        arrayVector.push_back(s);
    }

    pb_create_repeated_encode_callback(&testStruct.repeatedSubStruct, pb_create_repeated_array_stream(&arrayVector, SubStruct_fields));
    pb_ostream_t outputStream = pb_ostream_from_buffer(testStructData, sizeof(testStructData));
    if(!pb_encode(&outputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    pb_iterate_repeated_callback(&testStruct.repeatedSubStruct, [] (size_t bufferIndex, void* bufferData) {
        SubStruct* s = (SubStruct*)bufferData;

        // s is the pointer in the std::vector<SubStruct*> arrayVector

        pb_destroy_string_encode_callback(&s->v2);
        delete s;
    });

    pb_destroy_repeated_encode_callback(&testStruct.repeatedSubStruct);

    std::cout << "TestRepeatedNormal encoded: " << utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) << std::endl;

    if(utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) != "1a0e080010001a08305f737472696e671a0e080110011a08315f737472696e671a0e080410021a08325f737472696e671a0e080910031a08335f737472696e671a0e081010041a08345f737472696e671a0e081910051a08355f737472696e671a0e082410061a08365f737472696e671a0e083110071a08375f737472696e671a0e084010081a08385f737472696e671a0e085110091a08395f737472696e67")
    {
        return false;
    }


    // DECODE
    arrayVector.clear();
    memset(&testStruct, 0, sizeof(testStruct));

    pb_create_repeated_decode_callback(&testStruct.repeatedSubStruct, pb_create_repeated_array_stream(&arrayVector, SubStruct_fields), [] (void* _structureBuffer) {
        SubStruct* s = (SubStruct*)_structureBuffer;

        s->v0 = 0;
        s->v1 = 0;
        
        pb_create_string_decode_callback(&s->v2);
    });

    pb_istream_t inputStream = pb_istream_from_buffer(testStructData, outputStream.bytes_written);
    
    if(!pb_decode(&inputStream, TestStruct_fields, &testStruct))
    {
        return false;
    }

    pb_iterate_repeated_callback(&testStruct.repeatedSubStruct, [] (size_t bufferIndex, void* bufferData) {
        SubStruct* s = (SubStruct*)bufferData;
        
        std::cout << "[" << bufferIndex << "]" << std::endl;
        std::cout << " v0 = " << s->v0 << std::endl;
        std::cout << " v1 = " << s->v1 << std::endl;
        std::cout << " v2 = " << pb_get_string_for_decode_callback(&s->v2) << std::endl;

        pb_destroy_string_decode_callback(&s->v2);
        delete s;
    });

    pb_destroy_repeated_decode_callback(&testStruct.repeatedSubStruct);

    return true;
}