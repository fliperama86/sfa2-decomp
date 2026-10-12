/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80028864_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void spawn_slot27_selection_row_objects_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *a;
    Object *b;
    int i;
    u8 *base;
    int j;
    int m;
    int n;
    int v9;
    u16 t12;

    obj->state_0 = 2;
    for (m = 0; m < 0x160; m++) {
        data_801a2824[m] = ((u16 *)0x800edc00)[m];
        data_801a2824[m + 0xa00] = ((u16 *)0x800edc00)[m];
    }
    queue_image_upload_job(0, 0);
    i = 0;
    base = (u8 *)&game_state.sub_index;
    if (*(s16 *)base >= 0) {
    do {
        a = (Object *)pop_block_from_pool();
        if (a != 0) {
            a->field_02 = 0xaa;
            a->active_count = 1;
            a->field_01 = 1;
            a->table_index = 0x80;
            v9 = *(u8 *)&game_state.sub_index - i + 3;
            a->state_0 = 1;
            a->countdown = 0x1f;
            a->ot_depth = v9;
            t12 = ((Slot27Obj *)obj)->angle;
            a->pos_y = 0xbc;
            a->health = i << 5;
            a->step_flags = 0x14;
            a->data_ptr_a = data_80017c28_slot27;
            a->data_ptr_b = data_8001aa14_slot27;
            a->spawn_y = 0x1e0;
            a->field_90 = (void *)0x80038000;
            a->spawn_x = 0;
            a->image_row = 0x16;
            ((Slot27Obj *)a)->angle = t12;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->field_02 = 0xaa;
            b->active_count = 1;
            b->field_01 = 1;
            b->table_index = 0x80;
            b->ot_depth = a->ot_depth - 1;
            b->state_0 = 1;
            b->countdown = a->countdown;
            ((Slot27Obj *)b)->angle = ((Slot27Obj *)a)->angle + 2;
            b->pos_y = *(u16 *)&a->pos_y - 2;
            b->health = a->health;
            b->step_flags = (&game_state.byte_table)[i + 1];
            b->field_90 = (void *)0x80038000;
            b->data_ptr_a = data_80017c28_slot27;
            b->data_ptr_b = data_8001aa14_slot27;
            b->spawn_y = 0x1e0;
            b->spawn_x = 0;
            b->image_row = 0;
            if (i != 7) {
                if (i == *(s16 *)&game_state.sub_index) {
                    b->image_row = (&game_state.byte_table)[i + 1] + 2;
                } else {
                    for (m = 0; m < 0x10; m++) {
                        data_801a2824[(&game_state.byte_table)[i + 1] * 0x10 + m] = *(u16 *)((u8 *)0x800ee000 + (&game_state.byte_table)[i + 1] * 0x20 + m * 2);
                        data_801a3c24[(&game_state.byte_table)[i + 1] * 0x10 + m] = *(u16 *)((u8 *)0x800ee000 + (&game_state.byte_table)[i + 1] * 0x20 + m * 2);
                    }
                    queue_image_upload_job(0, 0);
                    b->image_row = (&game_state.byte_table)[i + 1] + 2;
                    a->image_row = 0x15;
                    a->step_flags = 0x15;
                }
            } else {
                b->image_row = (&game_state.byte_table)[8] + 2;
            }
            set_sequence_from_table_5(a, (s16)a->step_flags, data_80028864_slot27);
            set_sequence_from_table_5(b, (&game_state.byte_table)[i + 1], data_80028864_slot27);
        }
        i++;
    } while (i <= *(s16 *)&game_state.sub_index);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        b->active_count = 1;
        b->field_01 = 1;
        b->field_02 = 0xaa;
        b->table_index = 0x80;
        b->ot_depth = 2;
        b->state_0 = 1;
        b->countdown = a->countdown;
        ((Slot27Obj *)b)->angle = ((Slot27Obj *)a)->angle + 2;
        b->pos_y = *(u16 *)&a->pos_y - 2;
        b->health = a->health;
        b->field_90 = (void *)0x80038000;
        b->data_ptr_a = data_80017c28_slot27;
        b->data_ptr_b = data_8001aa14_slot27;
        b->spawn_y = 0x1e0;
        b->spawn_x = 0;
        b->image_row = 0x16;
        set_sequence_from_table_5(b, 0x16, data_80028864_slot27);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        b->active_count = 1;
        b->field_01 = 1;
        b->field_02 = 0xaa;
        b->table_index = 0x80;
        b->ot_depth = 2;
        b->state_0 = 1;
        b->countdown = a->countdown;
        ((Slot27Obj *)b)->angle = ((Slot27Obj *)a)->angle + 2;
        b->pos_y = *(u16 *)&a->pos_y - 2;
        b->health = a->health;
        b->field_90 = (void *)0x80038000;
        b->data_ptr_a = data_80017c28_slot27;
        b->data_ptr_b = data_8001aa14_slot27;
        b->spawn_y = 0x1e0;
        b->spawn_x = 0;
        b->image_row = 0x17;
        set_sequence_from_table_5(b, (&game_state.byte_table)[i] + 0x17, data_80028864_slot27);
    }

    j = i;
    for (; j < 8; j++) {
        a = (Object *)pop_block_from_pool();
        if (a != 0) {
            a->field_02 = 0xaa;
            a->active_count = 1;
            a->field_01 = 1;
            a->table_index = 0x80;
            v9 = j - *(u8 *)&game_state.sub_index + 3;
            a->state_0 = 1;
            a->countdown = 0x1f;
            a->ot_depth = v9;
            t12 = ((Slot27Obj *)obj)->angle;
            a->pos_y = 0xbc;
            a->health = j << 5;
            a->step_flags = 0x14;
            a->field_90 = (void *)0x80038000;
            a->data_ptr_a = data_80017c28_slot27;
            a->data_ptr_b = data_8001aa14_slot27;
            a->spawn_y = 0x1e0;
            a->spawn_x = 0;
            a->image_row = 0x16;
            ((Slot27Obj *)a)->angle = t12;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->field_02 = 0xaa;
            b->active_count = 1;
            b->field_01 = 1;
            b->table_index = 0x80;
            b->ot_depth = a->ot_depth;
            b->state_0 = 1;
            b->countdown = a->countdown;
            ((Slot27Obj *)b)->angle = ((Slot27Obj *)a)->angle + 2;
            b->pos_y = *(u16 *)&a->pos_y - 2;
            b->health = a->health;
            b->step_flags = (&game_state.byte_table)[j + 1];
            b->field_90 = (void *)0x80038000;
            b->data_ptr_a = data_80017c28_slot27;
            b->data_ptr_b = data_8001aa14_slot27;
            b->spawn_x = 0;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            if (((game_state.bit_mask >> (&game_state.byte_table)[j + 1]) & 1) && j < 7) {
                for (n = 0; n < 0x10; n++) {
                    data_801a2824[(&game_state.byte_table)[j + 1] * 0x10 + n] = *(u16 *)((u8 *)0x800ee000 + (&game_state.byte_table)[j + 1] * 0x20 + n * 2);
                    data_801a2824[(&game_state.byte_table)[j + 1] * 0x10 + n + 0xa00] = *(u16 *)((u8 *)0x800ee000 + (&game_state.byte_table)[j + 1] * 0x20 + n * 2);
                }
                queue_image_upload_job(0, 0);
                set_sequence_from_table_5(b, (&game_state.byte_table)[j + 1], data_80028864_slot27);
                b->image_row = (&game_state.byte_table)[j + 1] + 2;
                a->image_row = 0x15;
                a->step_flags = 0x15;
            } else {
                for (n = 0; n < 0x10; n++) {
                    data_801a2824[(&game_state.byte_table)[j + 1] * 0x10 + n] = *(u16 *)((u8 *)0x800edc00 + (&game_state.byte_table)[j + 1] * 0x20 + n * 2);
                    data_801a2824[(&game_state.byte_table)[j + 1] * 0x10 + n + 0xa00] = *(u16 *)((u8 *)0x800edc00 + (&game_state.byte_table)[j + 1] * 0x20 + n * 2);
                }
                queue_image_upload_job(0, 0);
                set_sequence_from_table_5(b, 0x13, data_80028864_slot27);
                b->image_row = 0x15;
            }
            set_sequence_from_table_5(a, (s16)a->step_flags, data_80028864_slot27);
            b->data_ptr_a = data_80017c28_slot27;
            b->field_90 = (void *)0x80038000;
            b->data_ptr_b = data_8001aa14_slot27;
        }
    }
}
