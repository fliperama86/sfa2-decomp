/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b3c24_slot04_09(Object *obj) {
    if (obj->field_a2 != 0 && obj->other->field_45 == 0) {
        obj->field_a2 = 0;
        game_state.field_63 = 0x18;
    }
    func_801b2858_slot04_09(obj);
    if (obj->vel_y < 0) {
        obj->state_3++;
        obj->field_0b ^= 1;
        func_801307e0(obj, 0x23);
    }
}

void func_801b3cb4_slot04_09(Object *obj) {
    u16 t;

    if (obj->field_a2 != 0 && obj->other->field_45 == 0) {
        obj->field_a2 = 0;
        game_state.field_63 = 0x18;
    }
    func_801b2858_slot04_09(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        t = *(u16 *)&obj->ground_y;
        obj->field_45 = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = t;
        func_801209c4(obj);
        func_801307e0(obj, 0x24);
    }
}
