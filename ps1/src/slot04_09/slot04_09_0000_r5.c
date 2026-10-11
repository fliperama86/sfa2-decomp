/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6210_slot04_09(Object *obj);
void func_801b5ab4_slot04_09(Object *obj);

void func_801b0568_slot04_09(Object *obj) {
    u16 t = obj->step_flags;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        func_801b6210_slot04_09(obj);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 3) {
        obj->step_flags = u & 0xff00;
        func_80130678(obj, 0x32);
    }
    update_sequence(obj);
}

void func_801b05f4_slot04_09(Object *obj) {
    u16 t;
    u16 u;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        play_side_sound_at(obj, obj->side, 0xf);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        func_801b5ab4_slot04_09(obj);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 3) {
        obj->step_flags = u & 0xff00;
        if (obj->field_0b != 0) {
            obj->pos_x += 8;
        } else {
            obj->pos_x -= 8;
        }
    }
}
