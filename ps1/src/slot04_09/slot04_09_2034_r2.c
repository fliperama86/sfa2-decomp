/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b2194_slot04_09(Object *obj) {
    if (obj->field_a2 != 0 && obj->other->field_45 == 0) {
        obj->field_a2 = 0;
        game_state.field_63 = 0x18;
    }
    func_801b2858_slot04_09(obj);
    if (obj->field_50 < 0) {
        obj->field_07++;
        obj->field_0b ^= 1;
        func_801307e0(obj, 0x23);
    }
}

void func_801b2224_slot04_09(Object *obj) {
    u16 t;

    if (obj->field_a2 != 0 && obj->other->field_45 == 0) {
        obj->field_a2 = 0;
        game_state.field_63 = 0x18;
    }
    func_801b2858_slot04_09(obj);
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        t = *(u16 *)&obj->field_70;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->pos_y = t;
        func_801209c4(obj);
        func_801307e0(obj, 0x24);
    }
}
