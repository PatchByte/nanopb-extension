#include "nanopb-extension/pb_repeated_extension.h"

#include <pb_encode.h>
#include <pb_decode.h>

enum class pb_repeated_argument_value_mode : unsigned char
{
    INVALID,
    SUBMESSAGE,
    CUSTOM
};

class pb_repeated_argument
{
public:
    pb_repeated_argument():
        stream(nullptr),
        streamDecodeSetupCallback([] (...) {}),
        streamValueMode(pb_repeated_argument_value_mode::INVALID),
        streamDecodePos(0)
    {}

    pb_repeated_array_stream* stream;
    pb_repeated_extension_setup_decode_callback streamDecodeSetupCallback;
    pb_repeated_argument_value_mode streamValueMode;

    size_t streamDecodePos;

    // Callback for the repeated field
    // Custom CB or Submessage CB

    pb_repeated_extension_iterator_for_encode_callback streamCustomEncodeCB;
    pb_repeated_extension_iterator_for_decode_callback streamCustomDecodeCB;
};

static bool pb_repeated_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)(*arg);
    bool status = false;

    for(size_t i = 0; i < argument->stream->GetSize(); i++)
    {
        void* structureBuffer = argument->stream->GetAt(i);

        if(!pb_encode_tag_for_field(stream, field)) { return false; }

        switch (argument->streamValueMode)
        {
            case pb_repeated_argument_value_mode::SUBMESSAGE:
                if(!pb_encode_submessage(stream, argument->stream->GetMsgDesc(), structureBuffer)) return false;
                continue;
            case pb_repeated_argument_value_mode::CUSTOM:
                if(!argument->streamCustomEncodeCB(stream, field, i, structureBuffer)) return false;
                continue;
            default:
                return false;
        }
    }

    return true;
}

static bool pb_repeated_decode_callback(pb_istream_t *stream, const pb_field_t* field, void** arg)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)(*arg);

    void* structureBuffer = argument->stream->AllocateBufferAndInsert();
    argument->streamDecodeSetupCallback(structureBuffer);
    argument->streamDecodePos++;

    switch (argument->streamValueMode)
    {
        case pb_repeated_argument_value_mode::SUBMESSAGE:
            if(!pb_decode(stream, argument->stream->GetMsgDesc(), structureBuffer))
            {
                return false;
            }
            return true;
        case pb_repeated_argument_value_mode::CUSTOM:
            return argument->streamCustomDecodeCB(stream, field, argument->streamDecodePos, structureBuffer);
        default:
            return false;
    }
}

bool pb_create_submessage_repeated_encode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;
    argument->streamValueMode = pb_repeated_argument_value_mode::SUBMESSAGE;

    field->arg = argument;
    field->funcs.encode = pb_repeated_encode_callback;

    return true;
}

bool pb_create_submessage_repeated_decode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream, pb_repeated_extension_setup_decode_callback field_setup_callback)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;
    argument->streamDecodeSetupCallback = field_setup_callback;
    argument->streamValueMode = pb_repeated_argument_value_mode::SUBMESSAGE;

    field->arg = argument;
    field->funcs.decode = pb_repeated_decode_callback;

    return true;
}

bool pb_create_custom_repeated_encode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream, pb_repeated_extension_iterator_for_encode_callback field_iterator_encode_callback)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;
    argument->streamValueMode = pb_repeated_argument_value_mode::CUSTOM;
    argument->streamCustomEncodeCB = field_iterator_encode_callback;

    field->arg = argument;
    field->funcs.encode = pb_repeated_encode_callback;

    return true;
}

bool pb_create_custom_repeated_decode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream, pb_repeated_extension_iterator_for_decode_callback field_iterator_decode_callback)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;
    argument->streamCustomDecodeCB = field_iterator_decode_callback;
    argument->streamValueMode = pb_repeated_argument_value_mode::CUSTOM;

    field->arg = argument;
    field->funcs.decode = pb_repeated_decode_callback;

    return true;
}

bool pb_iterate_repeated_callback(pb_callback_t* field, pb_repeated_extension_iterator_callback field_iteration_callback)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)field->arg;
    
    for(pb_size_t i = 0; i < argument->stream->GetSize(); i++)
    {
        field_iteration_callback(i, argument->stream->GetAt(i));
    }

    return true;
}

bool pb_destroy_repeated_encode_callback(pb_callback_t* field, bool deallocate_array_stream)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)field->arg;

    if(deallocate_array_stream)
    {
        pb_destroy_repeated_array_stream(argument->stream);
    }

    delete argument;
    field->arg = nullptr;

    return true;
}

bool pb_destroy_repeated_decode_callback(pb_callback_t* field, bool deallocate_array_stream)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)field->arg;

    if(deallocate_array_stream)
    {
        pb_destroy_repeated_array_stream(argument->stream);
    }

    delete argument;
    field->arg = nullptr;

    return true;
}