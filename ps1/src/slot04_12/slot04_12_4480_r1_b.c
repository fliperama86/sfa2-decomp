/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
void func_8011ffdc(Object *o);
extern ObjectFn data_801c4ef8_slot04_12[];

void set_sequence_and_tables_from_direction(Object *object);
void func_801b4ba8_slot04_12(Object *obj);
int apply_velocity_and_accel_return_vel_y_slot04_12(Object *obj);

void func_801b4ba8_slot04_12(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

int apply_velocity_and_accel_return_vel_y_slot04_12(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y;
}

void call_handler_by_state_0_4_slot04_12(Object *obj) {
    data_801c4ef8_slot04_12[obj->state_0](obj);
}

void init_projectile_velocity_and_sequence_slot04_12(Object *obj) {
    Object *p;
    SequenceStep *s;

    obj->spawn_flag = 1;
    p = obj->parent;
    obj->state_0++;
    obj->active_state = 0;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x10000;
    } else {
        obj->vel_x = 0xffff0000;
    }
    if (obj->table_index == 0) {
        if (obj->facing_side != 0) {
            obj->pos_x = obj->pos_x + 0x80;
        } else {
            obj->pos_x = obj->pos_x - 0x80;
        }
        ((ObjectWords *)obj)->word_58 = obj->pos_x;
        s = seqs_8017c7f8[15];
    } else {
        ((ObjectWords *)obj)->word_58 = obj->pos_x;
        if (p->side == 0) {
            s = *data_1f8000b4;
        } else {
            s = *data_1f800164;
        }
    }
    set_sequence_entry(obj, s);
}

void move_by_vel_x_until_offset_then_advance_slot04_12(Object *obj) {
    if (game_state.field_65 == 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        if ((u32)(obj->pos_x - obj->accel_y + 0x20) >= 0x40) {
            obj->state_0++;
        }
    }
    advance_sequence_checked(obj);
    set_state_0_if_x_outside_margin_narrow(obj);
    func_8011ffdc(obj);
}

void release_object_record_2_slot04_12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
