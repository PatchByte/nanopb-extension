/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_TEST_PB_H_INCLUDED
#define PB_TEST_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _SubStruct {
    int32_t v0;
    int64_t v1;
    pb_callback_t v2;
} SubStruct;

typedef struct _TestStruct {
    pb_callback_t testString;
    pb_callback_t subStruct;
    pb_callback_t repeatedSubStruct;
} TestStruct;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define SubStruct_init_default                   {0, 0, {{NULL}, NULL}}
#define TestStruct_init_default                  {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define SubStruct_init_zero                      {0, 0, {{NULL}, NULL}}
#define TestStruct_init_zero                     {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define SubStruct_v0_tag                         1
#define SubStruct_v1_tag                         2
#define SubStruct_v2_tag                         3
#define TestStruct_testString_tag                1
#define TestStruct_subStruct_tag                 2
#define TestStruct_repeatedSubStruct_tag         3

/* Struct field encoding specification for nanopb */
#define SubStruct_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, INT32,    v0,                1) \
X(a, STATIC,   REQUIRED, INT64,    v1,                2) \
X(a, CALLBACK, OPTIONAL, STRING,   v2,                3)
#define SubStruct_CALLBACK pb_default_field_callback
#define SubStruct_DEFAULT NULL

#define TestStruct_FIELDLIST(X, a) \
X(a, CALLBACK, OPTIONAL, STRING,   testString,        1) \
X(a, CALLBACK, OPTIONAL, MESSAGE,  subStruct,         2) \
X(a, CALLBACK, REPEATED, MESSAGE,  repeatedSubStruct,   3)
#define TestStruct_CALLBACK pb_default_field_callback
#define TestStruct_DEFAULT NULL
#define TestStruct_subStruct_MSGTYPE SubStruct
#define TestStruct_repeatedSubStruct_MSGTYPE SubStruct

extern const pb_msgdesc_t SubStruct_msg;
extern const pb_msgdesc_t TestStruct_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SubStruct_fields &SubStruct_msg
#define TestStruct_fields &TestStruct_msg

/* Maximum encoded size of messages (where known) */
/* SubStruct_size depends on runtime parameters */
/* TestStruct_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
