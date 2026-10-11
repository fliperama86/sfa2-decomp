/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3344_slot04_09(Object *obj) {
    Object *p;

    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    p = obj->other;
    if (obj->vel_y > 0) {
        if (p->field_45 != 0) {
            if (obj->field_a3 == 0) {
                if (obj->frame->box_a != 0 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x30) != 0) {
                    obj->field_a2 = 0xff;
                    obj->field_46 = 4;
                    obj->state_3++;
                    if (obj->vel_x >= 0) {
                        obj->vel_x = 0x80000;
                    } else {
                        obj->vel_x = 0xfff80000;
                    }
                    play_sound_at(obj, obj->side ^ 1, 0x31a);
                    play_side_sound_at(obj, obj->side, 5);
                    func_801307e0(obj, 0x21);
                } else {
                    update_sequence(obj);
                }
            } else if (func_8013fe54(obj, -0x20, 0x20, 0, 0x30) != 0) {
                obj->field_a2 = 0xff;
                obj->field_46 = 4;
                obj->state_3++;
                if (obj->vel_x >= 0) {
                    obj->vel_x = 0x80000;
                } else {
                    obj->vel_x = 0xfff80000;
                }
                play_sound_at(obj, obj->side ^ 1, 0x31a);
                play_side_sound_at(obj, obj->side, 5);
                func_801307e0(obj, 0x21);
            } else {
                update_sequence(obj);
            }
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 7;
        obj->vel_x = 0;
        obj->accel_x = 0;
        func_801307e0(obj, 0x23);
    }
}
