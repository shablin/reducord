/**
 * @file flags.h
 * @brief Provides an interface for handling command-line flags in the Reducord CLI application.
 * 
 * @author The Reducord Authors
 * @date 2026-03-15
 */


#ifndef __FLAGS_H__
#define __FLAGS_H__

#include <stdbool.h>

typedef enum {
	FLAG_HELP,              //< Displays help information about the CLI application and its available flags.
	FLAG_CLEAN_CACHE,       //< Deletes the Discord cache, including IndexDB, GPU Cache, and shaders.
	FLAG_CLEAN_LOGS,        //< Deletes the Discord log files.
	FLAG_CLEAN_VERSIONS,    //< Deletes the Discord version files, which can free up space by removing old versions of Discord that are no longer needed.
	FLAG_HIGH_PRIORITY,     //< Sets the Discord process priority to high, which can improve performance during optimization tasks.
	FLAG_CNT                //< Number of flags.
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