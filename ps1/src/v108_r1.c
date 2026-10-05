/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 func_80130184(Object *object);

/* Exact. The cd == 0 arm carries its own copy of the call (the compiler merged only the jal). */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: not exact, same size, 7 slots. The constant 1 now matches (field_159 = 1
 * is stored next to the kind test). Only the delay slots and targets of the early-out
 * branches differ: the original sends the kind, field_163 and field_260 tests to the
 * move before the call (nop slots) and fills the slot of the field_5c test with it. */
void func_8012d7e4(Object *object) {
    if (object->field_cd == 0) {
        func_801308c4(object, 0x14);
        return;
    }
    if ((object->kind != 6 && object->kind != 0xf) ||
        object->field_163 != 0 || object->field_260 != 0 || (s16)object->field_5c < 0 ||
        ((game_state.config->field_4d | game_state.config->field_4e) | game_state.config->field_04) != 0 ||
        func_8012f56c(object) || !func_8014a170(object, data_8017d898)) {
        func_801308c4(object, 0x14);
    } else {
        object->field_04 = 1;
        object->field_06 = 7;
        object->field_12a = 2;
        object->field_15a = 6;
        object->field_05 = 0;
        object->field_07 = 0;
        object->field_258 = 0;
        object->field_259 = 0;
        object->field_25a = 0;
        object->field_159 = 1;
        if (object->kind == 0xf) {
            object->field_159 = 0;
        }
    }
}
