/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_8002c3a8_slot12[4];
void init_entry_primitive_pairs_slot12(Object *obj);
void init_four_prim_pairs_slot12(Object *obj);
void func_80016c50_slot12(Object *obj);
void count_record_bytes_into_four_counters_slot12(Object *obj);

void init_rank_list_object_slot12(Object *obj) {
    s16 t;

    obj->ot_depth = 4;
    obj->field_01 = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->pos_x = 0;
    obj->pos_y = 0;
    obj->field_0c = 0;
    obj->state_0 = obj->state_0 + 1;
    data_8002c3a8_slot12[0].value = 0;
    data_8002c3a8_slot12[1].value = 0;
    data_8002c3a8_slot12[2].value = 0;
    data_8002c3a8_slot12[3].value = 0;
    obj->pos_x = 0xe8;
    obj->pos_y = 0x58;
    obj->y_offset = 0xa;
    obj->base_x = 0;
    obj->variant_index = table_8016e5c4[game_state.left_rank_index].level;
    if (game_state.field_2bf == 0) {
        ((ObjectWords *)obj)->word_50 = (int)game_state.config + 0x19e;
        obj->accel_x = *(s16 *)((u8 *)game_state.config + 0x220);
    } else {
        ((ObjectWords *)obj)->word_50 = (int)game_state.config + 0x1de;
        obj->accel_x = *(s16 *)((u8 *)game_state.config + 0x222);
    }
    t = obj->accel_x;
    obj->health = t;
    if (t >= 0) {
        obj->health = t & 0x3f;
        count_record_bytes_into_four_counters_slot12(obj);
        game_state.field_2bf = 0;
        obj->ground_y = 0xf;
        init_four_prim_pairs_slot12(obj);
        func_80016c50_slot12(obj);
        init_entry_primitive_pairs_slot12(obj);
    } else {
        game_state.field_2bf = 0;
        obj->vel_x = 0xb0;
        init_four_prim_pairs_slot12(obj);
        func_80016c50_slot12(obj);
    }
}
