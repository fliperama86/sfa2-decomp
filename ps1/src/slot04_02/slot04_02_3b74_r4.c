/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b625c_slot04_02(Object *obj);
void func_801b4180_slot04_02(Object *obj);

void func_801b4050_slot04_02(Object *obj) {
    Object *p = obj->other;

    func_80126130();
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        play_sound_at(p, p->side, 0x306);
        func_8013788c(obj);
        func_80126244();
        func_801b4180_slot04_02(obj);
    } else if (func_801b625c_slot04_02(obj) != 0) {
        if (obj->mode_flag_2 == 0) {
            obj->sound_index = 0;
            obj->field_bf = 0xff;
            func_80155d4c(4, (s8)obj->side);
            func_80155eac(4, obj->side);
        }
        p->round_count++;
    }
}

void func_801b4134_slot04_02(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) != 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}
