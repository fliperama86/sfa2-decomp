/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80027bc4_slot27[];
extern SequenceStep *data_80027c18_slot27[];
extern void func_80014298_slot27(Object *obj);

void func_80014120_slot27(Object *obj) {
    Object *other = obj->other;

    u8 k = obj->variant_index;
    u8 kind;
    int side;

    if (k != obj->table_index) {
        obj->table_index = k;
    }
    kind = other->kind;
    if (kind != obj->variant_index) {
        obj->variant_index = kind;
        func_80014298_slot27(obj);
        func_80130768(obj, obj->variant_index, data_80027bc4_slot27);
    }
    side = other->side;
    if (((game_state.mode >> side) & 1) != 0) {
        obj->state_1 = obj->state_1 + 1;
        obj->active_state = other->field_d4;
        func_80014298_slot27(obj);
        func_80130768(obj, obj->variant_index, data_80027c18_slot27);
    }
    advance_sequence_checked(obj);
}

void func_800141f0_slot27(Object *obj) {
    u8 d = obj->other->field_d4;

    if (obj->active_state != d) {
        obj->active_state = d;
        func_80014298_slot27(obj);
    }
    if ((obj->step_flags & 0x80) != 0) {
        obj->countdown = 0;
    }
    if ((s16)obj->countdown != 0) {
        advance_sequence_checked(obj);
    }
}

void func_80014268_slot27(Object *unused) {
}
