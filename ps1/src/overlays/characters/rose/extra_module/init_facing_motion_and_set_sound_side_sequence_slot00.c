/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void set_sequence_by_sound_side_slot00(Object *obj, int index);

void init_facing_motion_and_set_sound_side_sequence_slot00(Object *obj) {
    obj->countdown = 0x10;
    obj->vel_y = 0x60;
    obj->accel_y = 0x60;
    if (obj->facing_side == 0) {
        obj->vel_x = 0xfff40000;
        obj->accel_x = 0xc000;
        obj->pos_x = obj->pos_x + 0x60;
    } else {
        obj->vel_x = 0xc0000;
        obj->accel_x = 0xffff4000;
        obj->pos_x = obj->pos_x - 0x60;
    }
    set_sequence_by_sound_side_slot00(obj, 0xc);
}

void init_motion_and_start_sequence_9_slot00(Object *obj) {
    if (obj->facing_side == 0) {
        obj->vel_x = 0xffff0000;
        obj->pos_x = obj->pos_x + 0x38;
    } else {
        obj->vel_x = 0x10000;
        obj->pos_x = obj->pos_x - 0x38;
    }
    obj->vel_y = 0x20000;
    obj->accel_x = 0;
    obj->accel_y = -0x2000;
    obj->pos_y = obj->pos_y - 0x49;
    set_sequence_by_sound_side_slot00(obj, 9);
}

void set_sequence_by_sound_side_index_b_slot00(Object *obj) {
    set_sequence_by_sound_side_slot00(obj, 0xb);
}

void set_sequence_by_sound_side_slot00(Object *obj, int index) {
    SequenceStep **table;
    if (obj->sound_side == 0) {
        table = data_1f8000ac;
    } else {
        table = data_1f80015c;
    }
    set_sequence_from_table_5(obj, (s16)index, table);
}
