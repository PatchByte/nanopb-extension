#pragma once
#include "pb_common_header.h"
#include <functional>

typedef std::function<void(void*)> pb_submessage_extension_callback;

bool pb_create_submessage_encode_callback(pb_callback_t* field, pb_size_t submessage_size, const pb_msgdesc_t* submessage_desc, pb_submessage_extension_callback submessage_initialize_callback, pb_submessage_extension_callback submessage_deallocate_callback);
bool pb_create_submessage_decode_callback(pb_callback_t* field, pb_size_t submessage_size, const pb_msgdesc_t* submessage_desc, pb_submessage_extension_callback submessage_initialize_callback, pb_submessage_extension_callback submessage_deallocate_callback);

bool pb_destroy_submessage_encode_callback(pb_callback_t* field);
bool pb_destroy_submessage_decode_callback(pb_callback_t* field);

void* pb_get_submessage_buffer(pb_callback_t* field);