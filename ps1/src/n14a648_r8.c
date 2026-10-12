/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 skip_script_words_by_mode(Object *unused) {
    if (data_80189468 == 0 || data_80189468 == 2) {
        skip_two_script_words();
        return 1;
    } else if (data_80189468 == 4 || data_80189468 == 6) {
        noop_empty_function_12();
    }
    return 1;
}
