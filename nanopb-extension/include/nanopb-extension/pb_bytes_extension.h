#pragma once
#include "pb_common_header.h"
#include <vector>

typedef std::vector<unsigned char> pb_bytes_type;

bool pb_create_bytes_encode_callback(pb_callback_t* callback);
bool pb_create_bytes_decode_callback(pb_callback_t* callback);

bool pb_destroy_bytes_encode_callback(pb_callback_t* callback);
bool pb_destroy_bytes_decode_callback(pb_callback_t* callback);

bool pb_set_bytes_for_encode_callback(pb_callback_t* callback, pb_bytes_type value);
pb_bytes_type pb_get_bytes_for_decode_callback(pb_callback_t* callback);

bool pb_create_bytes_encode_callback_and_set(pb_callback_t* callback, pb_bytes_type value);
pb_bytes_type pb_destroy_bytes_decode_callback_and_get(pb_callback_t* callback);