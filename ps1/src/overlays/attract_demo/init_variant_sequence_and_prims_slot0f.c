/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_800efd30_slot0f[];
extern SequenceStep *data_800efd84_slot0f[];
extern SequenceStep *data_800efd88_slot0f[];
extern Slot12Prim data_800f77b4_slot0f[];
extern Slot12Prim data_800f78f4_slot0f[];
extern Slot12Prim data_800f7a34_slot0f[];
void init_two_prims_with_draw_mode_slot0f(Object *obj, Slot12Prim *a, int b, int c);

void init_variant_sequence_and_prims_slot0f(Object *obj) {
    FrameRecord *rec = table_8016e614;

    if (game_state.mode_flag_2 == 0) {
        rec = table_8016e5c4;
    }
    obj->field_01 = 0;
    obj->state_0++;
    rec += ((Slot0fObj *)obj)->record_offset;
    switch (obj->variant_index) {
    case 0:
        set_sequence_from_table_5(obj, rec->level, data_800efd30_slot0f);
        init_two_prims_with_draw_mode_slot0f(obj, (Slot12Prim *)((u8 *)data_800f77b4_slot0f + obj->table_index * 64), 0x20, 0x20);
        break;
    case 1:
        set_sequence_from_table_5(obj, 0, data_800efd84_slot0f);
        init_two_prims_with_draw_mode_slot0f(obj, (Slot12Prim *)((u8 *)data_800f78f4_slot0f + obj->table_index * 64), 0x20, 0x20);
        break;
    case 2:
        set_sequence_from_table_5(obj, 0, data_800efd88_slot0f);
        init_two_prims_with_draw_mode_slot0f(obj, (Slot12Prim *)((u8 *)data_800f7a34_slot0f + obj->table_index * 64), 0x20, 0x10);
        break;
    }
    obj->pos_x = obj->pos_x + 8;
    obj->pos_y = obj->pos_y + 0x10;
    if (obj->variant_index == 2 && obj->table_index == 0) {
        obj->pos_x = 0x30;
        obj->pos_y = 0x70;
    }
}
