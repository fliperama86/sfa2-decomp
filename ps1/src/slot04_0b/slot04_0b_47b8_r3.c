/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c28_slot04_0b(Object *obj);
void set_sequence_from_other_side_table_slot04_0b(Object *obj, int index);
void set_position_relative_to_ref_other_slot04_0b(Object *obj, int dx, int dy);

void func_801b4acc_slot04_0b(Object *obj) {
    obj->image_row = 0x1f;
    obj->state_1++;
    set_position_relative_to_ref_other_slot04_0b(obj, 0xe, 0xe);
    set_sequence_from_other_side_table_slot04_0b(obj, 0x14);
    func_8011ffdc(obj);
}

void init_arc_motion_when_other_flag_clear_slot04_0b(Object *obj) {
    if (!(ref_other.p->step_flags & 1)) {
        obj->state_1++;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        if (obj->facing_side != 0) {
            obj->health = 0x340;
            obj->y_offset = 0x2c0;
            obj->vel_x = *(s32 *)&obj->pos_x_frac + 0x40000;
            obj->vel_y = *(s32 *)&obj->pos_x_frac + 0x110000;
        } else {
            *(s16 *)&obj->health = -0x340;
            *(s16 *)&obj->y_offset = -0x2c0;
            obj->vel_x = *(s32 *)&obj->pos_x_frac - 0x40000;
            obj->vel_y = *(s32 *)&obj->pos_x_frac - 0x110000;
        }
        obj->ground_y = 0x500;
        obj->base_x = 0x400;
        *(s16 *)&obj->scroll_factor = -0xa0;
        *(s16 *)&obj->countdown = -0xa0;
        obj->accel_x = *(s32 *)&obj->pos_y_frac - 0x340000;
        obj->accel_y = *(s32 *)&obj->pos_y_frac - 0x4d0000;
        func_801b4c28_slot04_0b(obj);
    } else {
        func_8011ffdc(obj);
    }
}
