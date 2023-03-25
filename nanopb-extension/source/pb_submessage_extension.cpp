#include "nanopb-extension/pb_submessage_extension.h"

#include <pb_encode.h>
#include <pb_decode.h>

enum class pb_submessage_extension_proto_mode : unsigned char
{
    INVALID,
    ENCODE,
    DECODE
};

class pb_submessage_extension_argument
{
public:
    pb_submessage_extension_argument():
        submessage_size(0),
        submessage_desc(nullptr),
        submessage_allocated_buffer(nullptr),
        has_submessage_buffer_been_allocated(false),
        submessage_initialize_callback([] (...) {}),
        submessage_deallocate_callback([] (...) {}),
        submessage_proto_mode(pb_submessage_extension_proto_mode::INVALID)
    {}

    ~pb_submessage_extension_argument()
    {
        Deallocate();
    }

    bool PreAllocate()
    {
        if(has_submessage_buffer_been_allocated)
        {
            return false;
        }

        submessage_allocated_buffer = malloc(submessage_size);
        has_submessage_buffer_been_allocated = true;
        memset(submessage_allocated_buffer, 0, submessage_size);

        return true;
    }

    bool Deallocate()
    {
        if(!has_submessage_buffer_been_allocated)
        {
            return false;
        }

        submessage_deallocate_callback(submessage_allocated_buffer);

        free(submessage_allocated_buffer);
        submessage_allocated_buffer = nullptr;
        has_submessage_buffer_been_allocated = false;

        return true;
    }

    pb_size_t submessage_size;
    const pb_msgdesc_t* submessage_desc;
    pb_submessage_extension_callback submessage_initialize_callback;
    pb_submessage_extension_callback submessage_deallocate_callback;
    pb_submessage_extension_proto_mode submessage_proto_mode;

    // this will be allocated by the buffer once required
    bool has_submessage_buffer_been_allocated;
    void* submessage_allocated_buffer;
};

static bool pb_submessage_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    pb_submessage_extension_argument* argument = (pb_submessage_extension_argument*)(*arg);

    if(argument->submessage_proto_mode != pb_submessage_extension_proto_mode::ENCODE)
    {
        return false;
    }

    argument->submessage_initialize_callback(argument->submessage_allocated_buffer);

    return  pb_encode_tag_for_field(stream, field) &&
            pb_encode_submessage(stream, argument->submessage_desc, argument->submessage_allocated_buffer);
}

static bool pb_submessage_decode_callback(pb_istream_t *stream, const pb_field_t* field, void** arg)
{
    pb_submessage_extension_argument* argument = (pb_submessage_extension_argument*)(*arg);

    if(argument->submessage_proto_mode != pb_submessage_extension_proto_mode::DECODE)
    {
        return false;
    }

    argument->submessage_initialize_callback(argument->submessage_allocated_buffer);

    return pb_decode(stream, argument->submessage_desc, argument->submessage_allocated_buffer);
}

bool pb_create_submessage_encode_callback(pb_callback_t* field, pb_size_t submessage_size, const pb_msgdesc_t* submessage_desc, pb_submessage_extension_callback submessage_initialize_callback, pb_submessage_extension_callback submessage_deallocate_callback)
{
    pb_submessage_extension_argument* argument = new pb_submessage_extension_argument();

    argument->submessage_size = submessage_size;
    argument->submessage_desc = submessage_desc;
    argument->submessage_proto_mode = pb_submessage_extension_proto_mode::ENCODE;
    argument->submessage_initialize_callback = submessage_initialize_callback;
    argument->submessage_deallocate_callback = submessage_deallocate_callback;
    if(!argument->PreAllocate())
    {
        delete argument;
        return false;
    }

    field->arg = argument;
    field->funcs.encode = &pb_submessage_encode_callback;

    return true;
}

bool pb_create_submessage_decode_callback(pb_callback_t* field, pb_size_t submessage_size, const pb_msgdesc_t* submessage_desc, pb_submessage_extension_callback submessage_initialize_callback, pb_submessage_extension_callback submessage_deallocate_callback)
{
    pb_submessage_extension_argument* argument = new pb_submessage_extension_argument();

    argument->submessage_size = submessage_size;
    argument->submessage_desc = submessage_desc;
    argument->submessage_proto_mode = pb_submessage_extension_proto_mode::DECODE;
    argument->submessage_initialize_callback = submessage_initialize_callback;
    argument->submessage_deallocate_callback = submessage_deallocate_callback;
    if(!argument->PreAllocate())
    {
        delete argument;
        return false;
    }

    field->arg = argument;
    field->funcs.decode = &pb_submessage_decode_callback;

    return true;
}

bool pb_destroy_submessage_encode_callback(pb_callback_t* field)
{
    pb_submessage_extension_argument* argument = (pb_submessage_extension_argument*)(field->arg);

    argument->Deallocate();
    delete argument;
    field->arg = nullptr;

    return true;
}

bool pb_destroy_submessage_decode_callback(pb_callback_t* field)
{
    pb_submessage_extension_argument* argument = (pb_submessage_extension_argument*)(field->arg);

    argument->Deallocate();
    delete argument;
    field->arg = nullptr;

    return true;
}

void* pb_get_submessage_buffer(pb_callback_t* field)
{
    pb_submessage_extension_argument* argument = (pb_submessage_extension_argument*)(field->arg);
    return argument->submessage_allocated_buffer;
}