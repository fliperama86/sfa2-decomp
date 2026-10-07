/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_801b4674_slot04_01(Object *obj) {
    obj->field_44 = 1;
    obj->field_04++;
    if (obj->field_3c->side == 0) {
        func_80130768(obj, obj->field_48, data_1f8000b4);
    } else {
        func_80130768(obj, obj->field_48, data_1f800164);
    }
}

void func_801b46dc_slot04_01(Object *obj) {
    Object *p = obj->field_3c;
    int a;

    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04++;
    }
    a = -0x18;
    if (obj->field_48 != 7) {
        a = 0x14;
    }
    if (obj->field_0b == 0) {
        obj->pos_x = a + p->pos_x;
    } else {
        obj->pos_x = p->pos_x - a;
    }
    func_80131094(obj);
    if ((obj->field_03 + (game_state.field_1d + p->side)) & 1) {
        func_80120028(obj);
    } else {
        obj->field_01 = 0;
    }
}
