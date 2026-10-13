/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void skip_three_script_words_3(void) {
    script_cursor += 3;
}

void skip_two_script_words(void) {
    script_cursor += 2;
}

void skip_one_script_word(void) {
    script_cursor += 1;
}

void noop_empty_function_12(void) {
}
