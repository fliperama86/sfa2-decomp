/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_jump_velocity_by_table_index_slot28(Object *obj);

void update_vertical_bounce_motion_slot28(Object *obj) {
    int sum;
    ((Slot28Obj *)obj)->pos_y -= obj->vel_y;
    sum = obj->vel_y + obj->accel_y;
    obj->vel_y = sum;
    if (((Slot28Obj *)obj)->timer_reload != 0) {
        if (sum >= 0) {
            ((Slot28Obj *)obj)->timer_reload = 0;
            set_jump_velocity_by_table_index_slot28(obj);
        }
    } else if (sum < 0) {
        ((Slot28Obj *)obj)->timer_reload = 0xff;
        set_jump_velocity_by_table_index_slot28(obj);
        obj->vel_y = -obj->vel_y;
        obj->accel_y = -obj->accel_y;
    }
}

void set_jump_velocity_by_table_index_slot28(Object *obj) {
    if (obj->table_index == 4) {
        obj->vel_y = 0x14000;
    } else {
        obj->vel_y = 0x12000;
    }
    obj->accel_y = -0x1000;
}
