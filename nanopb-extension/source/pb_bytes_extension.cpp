#include "nanopb-extension/pb_bytes_extension.h"

#include <pb_encode.h>
#include <pb_decode.h>

class pb_bytes_argument
{
public:
    pb_bytes_argument():
        value({})
    {}

    pb_bytes_type value;
};

static bool pb_bytes_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    pb_bytes_argument* argument = (pb_bytes_argument*)(*arg);

    return  pb_encode_tag_for_field(stream, field) &&
            pb_encode_string(stream, argument->value.data(), argument->value.size());
}

static bool pb_bytes_decode_callback(pb_istream_t *stream, const pb_field_t* field, void** arg)
{
    pb_bytes_argument* argument = (pb_bytes_argument*)(*arg);

    argument->value.clear();
    size_t len = stream->bytes_left;
    uint8_t* buf = new uint8_t[len + 1];

    memset(buf, 0, len + 1);

    if(pb_read(stream, buf, len) == false)
    {
        delete[] buf;
        return false;
    }

    for(size_t i = 0; i < len; i++)
    {
        argument->value.push_back(buf[i]);
    }

    delete[] buf;
    return true;
}


bool pb_create_bytes_encode_callback(pb_callback_t* callback)
{
    pb_bytes_argument* argument = new pb_bytes_argument();

    callback->arg = argument;
    callback->funcs.encode = &pb_bytes_encode_callback;

    return true;
}

bool pb_create_bytes_decode_callback(pb_callback_t* callback)
{
    pb_bytes_argument* argument = new pb_bytes_argument();

    callback->arg = argument;
    callback->funcs.decode = &pb_bytes_decode_callback;

    return true;
}

static inline bool pb_destroy_bytes_callback(pb_callback_t* callback)
{
    pb_bytes_argument* argument = (pb_bytes_argument*)callback->arg;
    delete argument;
    callback->arg = nullptr;
    return true;
}

bool pb_destroy_bytes_encode_callback(pb_callback_t* callback)
{
    return pb_destroy_bytes_callback(callback);
}

bool pb_destroy_bytes_decode_callback(pb_callback_t* callback)
{
    return pb_destroy_bytes_callback(callback);
}

bool pb_set_bytes_for_encode_callback(pb_callback_t* callback, pb_bytes_type value)
{
    pb_bytes_argument* argument = (pb_bytes_argument*)callback->arg;
    argument->value = value;
    return true;
}

pb_bytes_type pb_get_bytes_for_decode_callback(pb_callback_t* callback)
{
    pb_bytes_argument* argument = (pb_bytes_argument*)callback->arg;
    return argument->value;
}

bool pb_create_bytes_encode_callback_and_set(pb_callback_t* callback, pb_bytes_type value)
{
    return  pb_create_bytes_encode_callback(callback) &&
            pb_set_bytes_for_encode_callback(callback, value);
}

pb_bytes_type pb_destroy_bytes_decode_callback_and_get(pb_callback_t* callback)
{
    auto value = pb_get_bytes_for_decode_callback(callback);
    pb_destroy_bytes_decode_callback(callback);
    return value;
}