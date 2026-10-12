/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002874c_slot12[];
extern SequenceStep *data_800287a0_slot12[];
extern SequenceStep *data_800287a4_slot12[];
extern u8 data_8002bfe8_slot12[];
extern u8 data_8002c128_slot12[];
extern u8 data_8002c268_slot12[];
void init_two_prims_with_draw_mode_slot12(Object *obj, Slot12Prim *a, int b, int c);

void init_slot12_variant_sprite_slot12(Object *obj) {
    FrameRecord *rec = table_8016e614;
    u8 t;

    if (game_state.mode_flag_2 == 0) {
        rec = table_8016e5c4;
    }
    obj->field_01 = 0;
    obj->state_0++;
    rec += ((Slot12Obj *)obj)->entry_count;
    switch (obj->variant_index) {
    case 0:
        set_sequence_from_table_5(obj, rec->level, data_8002874c_slot12);
        init_two_prims_with_draw_mode_slot12(obj, (Slot12Prim *)(data_8002bfe8_slot12 + obj->table_index * 64), 0x20, 0x20);
        break;
    case 1:
        set_sequence_from_table_5(obj, 0, data_800287a0_slot12);
        init_two_prims_with_draw_mode_slot12(obj, (Slot12Prim *)(data_8002c128_slot12 + obj->table_index * 64), 0x20, 0x20);
        break;
    case 2:
        set_sequence_from_table_5(obj, 0, data_800287a4_slot12);
        init_two_prims_with_draw_mode_slot12(obj, (Slot12Prim *)(data_8002c268_slot12 + obj->table_index * 64), 0x20, 0x10);
        break;
    }
    obj->pos_x = obj->pos_x + 8;
    obj->pos_y = obj->pos_y + 0x10;
    if ((obj->variant_index == 2) & (obj->table_index == 0)) {
        obj->pos_x = 0x30;
        obj->pos_y = 0x70;
    }
}
