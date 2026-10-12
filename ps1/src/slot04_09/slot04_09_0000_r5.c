/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_one_moving_child_opposite_facing_slot04_09(Object *obj);
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
        spawn_one_moving_child_opposite_facing_slot04_09(obj);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 3) {
        obj->step_flags = u & 0xff00;
        set_sequence_by_side(obj, 0x32);
    }
    update_sequence(obj);
}

void update_sequence_sound_spawn_and_nudge_x_slot04_09(Object *obj) {
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
        if (obj->facing_side != 0) {
            obj->pos_x += 8;
        } else {
            obj->pos_x -= 8;
        }
    }
}
