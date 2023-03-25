#include "nanopb-extension/pb_repeated_extension.h"

#include <pb_encode.h>
#include <pb_decode.h>

class pb_repeated_argument
{
public:
    pb_repeated_argument():
        stream(nullptr),
        streamPos(0),
        streamDecodeSetupCallback([] (...) {})
    {}

    pb_repeated_array_stream* stream;
    size_t streamPos;
    pb_repeated_extension_setup_decode_callback streamDecodeSetupCallback;
};

static bool pb_repeated_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)(*arg);
    bool status = false;

    for(argument->streamPos = 0; argument->streamPos < argument->stream->GetSize(); argument->streamPos++)
    {
        void* structureBuffer = argument->stream->GetAt(argument->streamPos);

        if(!(pb_encode_tag_for_field(stream, field) &&
            pb_encode_submessage(stream, argument->stream->GetMsgDesc(), structureBuffer)))
        {
            return false;
        }
    }

    return true;
}

static bool pb_submessage_decode_callback(pb_istream_t *stream, const pb_field_t* field, void** arg)
{
    pb_repeated_argument* argument = (pb_repeated_argument*)(*arg);

    void* structureBuffer = argument->stream->AllocateBufferAndInsert();

    argument->streamDecodeSetupCallback(structureBuffer);

    if(!pb_decode(stream, argument->stream->GetMsgDesc(), structureBuffer))
    {
        return false;
    }

    return true;
}

bool pb_create_repeated_encode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;

    field->arg = argument;
    field->funcs.encode = pb_repeated_encode_callback;

    return true;
}

bool pb_create_repeated_decode_callback(pb_callback_t* field, pb_repeated_array_stream* field_stream, pb_repeated_extension_setup_decode_callback field_setup_callback)
{
    pb_repeated_argument* argument = new pb_repeated_argument();

    argument->stream = field_stream;
    argument->streamDecodeSetupCallback = field_setup_callback;

    field->arg = argument;
    field->funcs.decode = pb_submessage_decode_callback;

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