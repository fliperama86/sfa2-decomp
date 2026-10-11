/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b3ecc_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 9);
    *(s32 *)&obj->vel_x = 0;
    *(s32 *)&obj->accel_x = 0;
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x8000) != 0) {
        obj->field_0b = 1;
    }
    obj->field_46 = 0xc00;
    func_801307e0(obj, 0x1b);
}

void func_801b3f5c_slot04_07(Object *obj) {
    int t = obj->field_46 - 0x100;
    obj->field_46 = t;
    if ((s16)t == 0) {
        obj->state_3++;
    }
}

void func_801b3f90_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->field_70) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        game_state.field_63 = 0x40;
        play_side_sound_at(obj, obj->side, 8);
        func_80140cd8(obj, 0xe, 0);
        play_sound_at(obj, obj->side, 0x319);
        func_801307e0(obj, 0x1c);
    }
}
