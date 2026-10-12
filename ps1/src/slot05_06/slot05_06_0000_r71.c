/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_velocities_from_table_by_index_slot05_06(Object *obj);
extern u8 data_801dd600_slot05_06[];
extern s32 data_801dd620_slot05_06[];

void init_timer_depth_and_load_velocities_slot05_06(Object *obj) {
    obj->ot_depth = 1;
    ((Slot04aObj *)obj)->timer = 0x18;
    ((Slot04aObj *)obj)->saved_pos_y -= 0x10;
    load_velocities_from_table_by_index_slot05_06(obj);
}

void set_sequence_from_table_index_by_sound_side_slot05_06(Object *obj) {
    SequenceStep **table;
    u8 a = obj->table_index + 0x11;

    if (obj->sound_side == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    set_sequence_from_table_5(obj, a, table);
}

void add_table_offset_to_position_slot05_06(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    s16 dx = *(u16 *)((u8 *)data_801dd600_slot05_06 + ((obj->table_index << 2) & 0x1fc));
    s16 dy = *(u16 *)((u8 *)data_801dd600_slot05_06 + ((obj->table_index << 2) & 0x1fc) + 2);

    if (obj->facing_side != 0) {
        dx = -dx;
    }
    obj->pos_x += dx;
    obj->pos_y -= dy;
}

void load_velocities_from_table_by_index_slot05_06(Object *obj) {
    s32 *p = (s32 *)((u8 *)data_801dd620_slot05_06 + ((obj->table_index << 4) & 0x3f0));

    obj->vel_x = *p++;
    obj->accel_x = *p++;
    obj->vel_y = p[0];
    obj->accel_y = p[1];
    if (obj->facing_side != 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
}
