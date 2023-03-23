#include "nanopb-extension/pb_string_extension.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include <functional>

enum class pb_string_callback_proto_mode
{
    INVALID,
    ENCODE,
    DECODE
};

enum class pb_string_callback_value_mode
{
    INVALID,
    SET,
    CALLBACK
};

class pb_string_callback_argument
{
public:
    pb_string_callback_argument():
        encodeCallback([] () -> std::string { return ""; }),
        decodeCallback([] (...) -> void {}),
        value(""),
        valueMode(pb_string_callback_value_mode::INVALID),
        protoMode(pb_string_callback_proto_mode::INVALID)
    {}

    std::string GetValueForEncode()
    {
        switch (valueMode)
        {
        case pb_string_callback_value_mode::SET: return value;
        case pb_string_callback_value_mode::CALLBACK: return encodeCallback();
        default: return "";
        }
    }

    void SetValueForDecode(std::string Value)
    {
        switch (valueMode)
        {
        case pb_string_callback_value_mode::SET: { value = Value; }
        case pb_string_callback_value_mode::CALLBACK: { decodeCallback(Value); }
        default: return;
        }
    }

    std::function<std::string()> encodeCallback;
    std::function<void(std::string)> decodeCallback;
    std::string value;
    pb_string_callback_value_mode valueMode;
    pb_string_callback_proto_mode protoMode;
};

static bool pb_string_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    pb_string_callback_argument* argument = (pb_string_callback_argument*)(*arg);
    std::string value = argument->GetValueForEncode();

    if(argument->protoMode != pb_string_callback_proto_mode::ENCODE)
    {
        return false;
    }

    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_string(stream, (const pb_byte_t*)value.c_str(), value.length());
}

static bool pb_string_decode_callback(pb_istream_t* stream, const pb_field_t* field, void** arg)
{
    pb_string_callback_argument* argument = (pb_string_callback_argument*)(*arg);

    if(argument->protoMode != pb_string_callback_proto_mode::DECODE)
    {
        return false;
    }

    size_t len = stream->bytes_left;
    uint8_t* buf = new uint8_t[len + 1];

    memset(buf, 0, len + 1);

    if(pb_read(stream, buf, len) == false)
    {
        delete[] buf;
        return false;
    }

    argument->SetValueForDecode(reinterpret_cast<char*>(buf));

    delete[] buf;
    return true;
}

bool pb_create_string_encode_callback(pb_callback_t* field)
{

    pb_string_callback_argument* argument = new pb_string_callback_argument();

    argument->protoMode = pb_string_callback_proto_mode::ENCODE;
    
    field->arg = argument;
    field->funcs.encode = &pb_string_encode_callback;

    return true;

}

bool pb_create_string_decode_callback(pb_callback_t* field)
{
    pb_string_callback_argument* argument = new pb_string_callback_argument();

    argument->protoMode = pb_string_callback_proto_mode::DECODE;
    argument->valueMode = pb_string_callback_value_mode::SET;
    
    field->arg = argument;
    field->funcs.decode = &pb_string_decode_callback;

    return true;
}

bool pb_destroy_string_encode_callback(pb_callback_t* field)
{
    if(field->arg == nullptr)
    {
        return false;
    }

    pb_string_callback_argument* argument = (pb_string_callback_argument*)field->arg;
    delete argument;
    field->arg = nullptr;

    return true;
}

bool pb_destroy_string_decode_callback(pb_callback_t* field)
{
    if(field->arg == nullptr)
    {
        return false;
    }

    pb_string_callback_argument* argument = (pb_string_callback_argument*)field->arg;
    delete argument;
    field->arg = nullptr;

    return true;
}

bool pb_create_string_encode_callback_and_set(pb_callback_t* field, std::string value)
{
    return  pb_create_string_encode_callback(field) &&
            pb_set_string_for_encode_callback(field, value);
}

std::string pb_destroy_string_decode_callback_and_get(pb_callback_t* field)
{
    std::string v = pb_get_string_for_decode_callback(field);
    pb_destroy_string_decode_callback(field);

    return v;
}

bool pb_set_string_for_encode_callback(pb_callback_t* field, std::string value)
{
    if(field->arg == nullptr)
    {
        return false;
    }

    pb_string_callback_argument* argument = (pb_string_callback_argument*)field->arg;

    argument->valueMode = pb_string_callback_value_mode::SET;
    argument->value = value;

    return true;
}

std::string pb_get_string_for_decode_callback(pb_callback_t* field)
{
    if(field->arg == nullptr)
    {
        return "";
    }

    pb_string_callback_argument* argument = (pb_string_callback_argument*)field->arg;

    return argument->value;
}