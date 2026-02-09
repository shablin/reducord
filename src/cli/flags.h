#ifndef __FLAGS_H__
#define __FLAGS_H__

#include <stdbool.h>

typedef enum {
    FLAG_HELP,
    FLAG_CLEAN_CACHE,
    FLAG_CLEAN_LOGS,
    FLAG_CLEAN_VERSIONS,
    FLAG_HIGH_PRIORITY,
    FLAG_CNT
} option_flag_t;

typedef void (*flag_callback_t)(void);
typedef void (*unk_flag_callback_t)(char*);

typedef struct {
    char** names;
    flag_callback_t* callbacks;
} flag_handler_t;

/*
    Allocates resources for `flag_handler_t`.
*/
bool flags_mk_handler(flag_handler_t* result);

/*
    Adds specific flag (also name, callback) to `flag_handler_t`,
    so the flag will be handeled, if specified by user.
*/
bool flags_add_flag(flag_handler_t* handler, option_flag_t flag, char* name, flag_callback_t callback);

/*
    Frees resources, allocated by `flags_mk_handler`.
    Frees memory, allocated for `handler` itself, if `free_ptr` is `true`.
*/
void flags_free_handler(flag_handler_t* handler, bool free_ptr);

/*
    Scans cmd arguments for flags specified via `flags_add_flag`, and executes
    corresponding callbacks, if found any.
*/
void flags_execute(flag_handler_t* handler, int argc, char** argv);

/*
    Same as `flags_execute`, but calls `unk_cb(flag)`, if unknown flag is found.
    Behaves as `flags_execute`, if `unk_cb` is `NULL`.
*/
void flags_execute_unk(flag_handler_t* handler, int argc, char** argv, unk_flag_callback_t unk_cb);

#ifdef IMPL_FLAGS
#include "./impl/flags_impl.c"
#endif

#endif