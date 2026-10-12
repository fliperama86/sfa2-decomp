/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


u8 skip_three_script_words(Object *unused) {
    skip_three_script_words_3();
    return 1;
}

u8 skip_three_script_words_2(Object *unused) {
    skip_one_script_word();
    return 1;
}
