/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

/* buttons is not set on every path: when data_801a6984 is not zero and the
   second condition does not hold, the code tests the register as it is.
   Reading the unset local is undefined behaviour in C. It reproduces the
   original's instructions with this compiler and is not a defined
   implementation: a port has to give buttons a value on that path. A
   fourth parameter in place of the local was tried and gives other code. */
/* The call of set_slot_random_timer passes a fourth argument although the callee takes three: the original has entry in the fourth argument register at the call. Written with three arguments, this function differs from the original in 7 instruction slots. */
void start_slot_stage_from_single_direction(Object *object, u8 index, u8 arg) {
    u16 entry;
    u8 t0;
    int buttons;
    if (data_801a6984[0] == 0) {
        if (object->side == 0) {
            buttons = data_801a6966 & 0xf000;
        } else {
            buttons = data_801a6972 & 0xf000;
        }
    }
    if (game_state.versus_flag != 0 && data_801a6984[0] != 0 && game_state.mode == object->field_02 + 1) {
        buttons = object->input_bits & 0xf000;
    }
    if ((u16)buttons == 0) {
        reset_slot_stage_clear_result(object, (u8)index, (u8)arg);
    } else {
        if ((u16)buttons == 0x1000) {
            t0 = 0;
        } else if ((u16)buttons == 0x8000) {
            t0 = 1;
        } else if ((u16)buttons == 0x4000) {
            t0 = 2;
        } else if ((u16)buttons == 0x2000) {
            t0 = 3;
        } else {
            reset_slot_stage_clear_result(object, (u8)index, (u8)arg);
            return;
        }
        object->slots[(u8)index].stage = object->slots[(u8)index].stage + 1;
        entry = table_8017a8cc[(u8)arg * 7];
        object->slots[(u8)index].entry_index = t0;
        object->slots[(u8)index].repeat_count = entry;
        ((void (*)(Object *, u8, u8, u16))set_slot_random_timer)(object, (u8)index, (u8)arg, entry);
    }
}
