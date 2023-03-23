#pragma once
#include "pb_common_header.h"

bool pb_create_string_encode_callback(pb_callback_t* field);
bool pb_create_string_decode_callback(pb_callback_t* field);

bool pb_destroy_string_encode_callback(pb_callback_t* field);
bool pb_destroy_string_decode_callback(pb_callback_t* field);

bool pb_create_string_encode_callback_and_set(pb_callback_t* field, std::string value);
std::string pb_destroy_string_decode_callback_and_get(pb_callback_t* field);

bool pb_set_string_for_encode_callback(pb_callback_t* field, std::string value);
std::string pb_get_string_for_decode_callback(pb_callback_t* field);