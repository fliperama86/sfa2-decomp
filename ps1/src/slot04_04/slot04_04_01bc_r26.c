/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c4304_slot04_04[];

void func_801b2188_slot04_04(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 10);
    func_80138ae8(&game_state, obj);
    if (obj->facing_side != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x + 0x18;
    } else {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x - 0x18;
    }
    obj->vel_x = data_801c4304_slot04_04[obj->direction_index * 2];
    obj->accel_x = data_801c4304_slot04_04[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c4304_slot04_04[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c4304_slot04_04[obj->direction_index * 2 + 3];
    play_side_sound_at(obj, obj->side, 7);
    if (obj->field_49 != 0) {
        a = (obj->direction_index >> 1) + 0x4c;
    } else {
        a = obj->direction_index + 0x30;
    }
    func_801307e0(obj, a);
}

void func_801b22c8_slot04_04(Object *obj) {
    if ((obj->step_flags & 0x80) == 0) {
        obj->active_state = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}
