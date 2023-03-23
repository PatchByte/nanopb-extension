#include "tests.h"
#include "utils.h"
#include "test.pb.h"

#include <pb_encode.h>
#include <pb_decode.h>
#include <nanopb-extension/pb_string_extension.h>

static std::string_view smTestString = "Test String!";

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

    std::cout << "TestStruct encoded: " << utils::buffer_to_hex_string(testStructData, outputStream.bytes_written) << std::endl;

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