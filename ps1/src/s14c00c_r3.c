/* Reconstruction. Names/roles inferred, not original symbols.
 * Unresolved: the 20e/20f split in func_8014c914/9f4/a7c/b1c. The original does
 * sll 16 / sra 24 (and a move) where this compiles to srl 8. Calls without an
 * argument (nop in the delay slot) are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014c994(Object *object) {
    if (object->field_20f == 0) {
        func_8014c9f4(object);
    } else if (object->field_20f == 2) {
        func_8014ca7c(object);
    } else if (object->field_20f == 4) {
        func_8014cba4(object);
    }
}
