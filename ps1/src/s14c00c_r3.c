/* Reconstruction. Names/roles inferred, not original symbols.
 * Unresolved: the 20e/20f split in read_script_word_and_dispatch/9f4/a7c/b1c. The original does
 * sll 16 / sra 24 (and a move) where this compiles to srl 8. Calls without an
 * argument (nop in the delay slot) are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void call_script_switch_by_arg(Object *object) {
    if (object->script_arg == 0) {
        enter_script_mode_1_and_read_opcode(object);
    } else if (object->script_arg == 2) {
        read_script_word_or_restore_script(object);
    } else if (object->script_arg == 4) {
        func_8014cba4(object);
    }
}
