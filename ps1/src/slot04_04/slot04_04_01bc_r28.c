/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c4344_slot04_04[];

void func_801b2468_slot04_04(Object *obj) {
    u16 a;

    obj->state_3++;
    obj->field_46 = *(u8 *)&obj->field_46 + 0x400;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    obj->vel_x = data_801c4344_slot04_04[obj->field_12a * 2];
    obj->accel_x = data_801c4344_slot04_04[obj->field_12a * 2 + 1];
    obj->vel_y = data_801c4344_slot04_04[obj->field_12a * 2 + 2];
    obj->accel_y = data_801c4344_slot04_04[obj->field_12a * 2 + 3];
    a = (obj->field_12a >> 1) + 0x50;
    func_801307e0(obj, a);
}

void func_801b2548_slot04_04(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->state_3++;
        obj->field_46 = *(u8 *)&obj->field_46 + 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -6, 0x4a);
    }
}
