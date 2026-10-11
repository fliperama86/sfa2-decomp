/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3550_slot04_0b(Object *obj) {
    Object *o = obj->other;
    int a;

    obj->field_a2 = 0;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_a2 = 1;
        obj->step_flags = obj->step_flags & 0xff00;
        func_80146478(obj, 1, -0x54, 0x55);
        a = 0;
        if (obj->field_a1 == 0) {
            obj->field_a1++;
            a = 9;
        }
        if (func_80140cd8(obj, a, 0) != 0) {
            obj->state_3 = 4;
            play_sound_at(obj, o->side, 0x340);
            play_side_sound_at(obj, obj->side, 0xb);
            func_801307e0(obj, 0x19);
        }
        obj->field_46 = obj->field_46 + 1;
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 != 0) {
        if (o->field_15b != 0) {
            if ((u8)func_801410c8(obj) == 0) {
                goto check;
            }
            func_80146478(obj, 1, -0x54, 0x55);
        }
        play_sound_at(obj, o->side, 0x30d);
        play_side_sound_at(obj, obj->side, 0xb);
        obj->state_3 = 3;
        func_801307e0(obj, 0x19);
    }
check:
    if (obj->field_a2 != 0) {
        play_sound_at(obj, o->side, 0x309);
        play_side_sound_at(obj, obj->side, 0xa);
    }
    update_sequence(obj);
}
