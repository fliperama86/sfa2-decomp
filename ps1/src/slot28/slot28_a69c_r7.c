/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void integrate_position_and_ground_y_motion_slot28(Object *o);
void update_fall_until_ground_then_bounce_slot28(Object *o);

void init_fall_motion_and_update_bounce_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->timer_reload = 0x20;
    o->vel_x = 0x50000;
    o->vel_y = 0x50000;
    o->accel_y = -0x1000;
    o->health = 3;
    o->state_1++;
    *(u16 *)&o->ground_y = *(u16 *)&o->pos_y;
    o->accel_x = 0;
    o->y_offset = 0;
    update_fall_until_ground_then_bounce_slot28(o);
}

void update_fall_until_ground_then_bounce_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    integrate_position_and_ground_y_motion_slot28(o);
    if (o->pos_y >= o->ground_y) {
        o->state_1++;
        play_sound_at(0, 0, 0x301);
        obj->timer_reload = 2;
        o->vel_x = 0x48000;
        o->accel_x = 0x1000;
        o->vel_y = 0x60000;
        o->accel_y = -0x1000;
        *(u16 *)&o->pos_y = *(u16 *)&o->ground_y;
    }
}

void land_on_ground_and_start_bounce_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    integrate_position_and_ground_y_motion_slot28(o);
    if (o->pos_y >= o->ground_y) {
        obj->timer_reload = 1;
        o->vel_x = 0x30000;
        o->vel_y = 0x30000;
        o->accel_y = -0x1000;
        o->state_1++;
        o->accel_x = 0;
        *(u16 *)&o->pos_y = *(u16 *)&o->pos_y + *(u16 *)&o->ground_y;
    }
}
