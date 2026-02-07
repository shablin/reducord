#include "../flags.h"
#include <malloc.h>
#include <string.h>

bool flags_mk_handler(flag_handler_t* result) {
    if (!result) return false;

    char** names = (char**)malloc(sizeof(char**) * FLAG_CNT);
    if (!names) return false;
    memset(names, NULL, sizeof(char**) * FLAG_CNT);

    flag_callback_t* callbacks = (flag_callback_t*)malloc(sizeof(flag_callback_t*) * FLAG_CNT);
    if (!callbacks) return false;
    memset(names, NULL, sizeof(flag_callback_t**) * FLAG_CNT);

    result->names = names;
    result->callbacks = callbacks;

    return true;
}

bool flags_add_flag(flag_handler_t* handler, option_flag_t flag, char* name, flag_callback_t callback) {
    if (!handler) return false;

    if (flag >= FLAG_CNT || flag < 0) return false;

    handler->names[flag] = name;
    handler->callbacks[flag] = callback;

    return true;
}

void flags_free_handler(flag_handler_t* handler, bool free_ptr) {
    free(handler->names);
    free(handler->callbacks);
    if (free_ptr) free(handler);
}

void flags_execute(flag_handler_t* handler, int argc, char** argv) {
    bool has_executed[FLAG_CNT] = {0};
    for (int ai = 1; ai < argc; ai++) {
        for (int fi = 0; fi < FLAG_CNT; fi++) {
            if (strcmp(argv[ai], handler->names[fi]) == 0) {
                if (!has_executed[fi]) {
                    handler->callbacks[fi]();
                    has_executed[fi] = true;
                }
                break;
            }
        }
    }
}