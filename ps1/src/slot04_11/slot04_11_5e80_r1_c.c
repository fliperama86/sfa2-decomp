/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_and_tables_from_direction(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);

void func_801b6604_slot04_11(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj) {
    u16 t = obj->step_flags;
    int d = t & 0x7f;

    if (d != 0) {
        obj->step_flags = t & 0xff80;
        if (obj->facing_side == 0) {
            d = -d;
        }
        obj->pos_x = obj->pos_x + d;
    }
}
