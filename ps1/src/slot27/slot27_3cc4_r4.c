/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80027bc4_slot27[];
extern SequenceStep *data_80027c18_slot27[];
extern void func_80014298_slot27(Object *obj);

void func_80014120_slot27(Object *obj) {
    Object *other = obj->other;

    u8 k = obj->field_48;
    u8 kind;
    int side;

    if (k != obj->field_03) {
        obj->field_03 = k;
    }
    kind = other->kind;
    if (kind != obj->field_48) {
        obj->field_48 = kind;
        func_80014298_slot27(obj);
        func_80130768(obj, obj->field_48, data_80027bc4_slot27);
    }
    side = other->side;
    if (((game_state.mode >> side) & 1) != 0) {
        obj->field_05 = obj->field_05 + 1;
        obj->field_45 = other->field_d4;
        func_80014298_slot27(obj);
        func_80130768(obj, obj->field_48, data_80027c18_slot27);
    }
    func_80131094(obj);
}

void func_800141f0_slot27(Object *obj) {
    u8 d = obj->other->field_d4;

    if (obj->field_45 != d) {
        obj->field_45 = d;
        func_80014298_slot27(obj);
    }
    if ((obj->field_3a & 0x80) != 0) {
        obj->field_46 = 0;
    }
    if ((s16)obj->field_46 != 0) {
        func_80131094(obj);
    }
}

void func_80014268_slot27(Object *unused) {
}
