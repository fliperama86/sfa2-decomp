/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e6bd8_slot0f(Object *obj);
void func_800e6874_slot0f(Object *obj);
void func_800e6a48_slot0f(Object *obj);
void func_800e6db4_slot0f(Object *obj);

void func_800e675c_slot0f(Object *obj) {
    int t;

    obj->pos_x = 0;
    obj->pos_y = 0;
    obj->pos_x = 0x58;
    obj->pos_y = 0x58;
    obj->field_01 = 0;
    obj->ot_depth = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->field_0c = 0;
    obj->y_offset = 10;
    obj->base_x = 0;
    obj->state_0++;
    obj->variant_index = table_8016e5c4[game_state.left_rank_index].level;
    ((ObjectWords *)obj)->word_50 = (s32)game_state.cursor;
    game_state.row_ptr = (Row *)((u8 *)data_80190468.p + 0x21e);
    ((ObjectWords *)obj)->word_54 = (s32)game_state.row_ptr;
    t = ((Object *)((u8 *)data_80190468.p + ((Slot0fObj *)data_80190468.p)->object_offset))->accum_count & 0x3f;
    /* health is stored twice. The first store goes through a plain pointer:
       written as a member store, this compiler moves the load of the second
       value above it. What the original source had here is not known. */
    *(u16 *)((u8 *)obj + 0x5c) = t;
    t = *(u16 *)&game_state.ring_b[0x3f] & 0x3f;
    obj->health = t;
    func_800e6bd8_slot0f(obj);
    game_state.field_2bf = 0;
    func_800e6874_slot0f(obj);
    func_800e6a48_slot0f(obj);
    func_800e6db4_slot0f(obj);
}
