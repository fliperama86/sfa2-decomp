/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80026a94_slot27[];
extern s16 data_80026a9c_slot27[];
extern s16 data_80026a9e_slot27[];
void load_palette_anim_from_table_index_slot27(Object *obj);
void set_side_bit_in_pass_index_slot27(Object *obj);

void start_selection_slide_or_advance_state_1_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    if (screen_state->field_54 == 0) {
        int *t = scratch_word_a;
        Object *p;
        obj->countdown = 0x258;
        obj->state_2 = obj->state_2 + 1;
        screen_state->field_54 = 0xff;
        *t = 1;
        game_state.selection = *(u8 *)t;
        if (game_state.table_index != 3) {
            *t = 2;
        }
        game_state.table_index_3 = data_80026a94_slot27[scratch_word_a[0]];
        game_state.table_index_3 = (s8)data_8016e68c;
        if (obj->active_state != 0) {
            p = (Object *)((ObjectWords *)obj)->word_54;
            *t = -2;
            scratch_word_b[0] = 0;
            ref_first.p = p;
            if (ref_other.p->field_d8 == 0) {
                p = (Object *)((ObjectWords *)obj)->word_58;
                *t = 2;
                scratch_word_b[0] = 1;
                ref_first.p = p;
            }
            obj->sound_side = scratch_word_b[0];
            ref_first.p->state_1 = ref_first.p->state_1 + 1;
            ref_first.p->table_index = ref_first.p->table_index + 2;
            obj->vel_x = -obj->vel_x;
            obj->y_offset = *(u16 *)t;
            *t = 6;
            *t = ref_other.p->field_d8 + 6;
            load_palette_anim_from_table_index_slot27(obj);
        } else {
            obj->sound_side = 0;
            obj->state_2 = obj->state_2 + 2;
            ref_second.p = ref_other.p;
            ref_other.p = (Object *)pop_block_from_pool();
            if (ref_other.p != 0) {
                ref_other.p->active_count = ref_other.p->active_count + 1;
                ref_other.p->field_02 = 0x22;
                ref_other.p->table_index = 2;
                ref_other.p->other = ref_second.p;
                ref_other.p->parent = obj;
                ((ObjectWords *)obj)->word_54 = (s32)ref_other.p;
                ref_other.p->field_62 = 0;
            }
            p = obj->other;
            *t = 8;
            ref_other.p = p;
            load_palette_anim_from_table_index_slot27(obj);
            *t = ref_other.p->side * 2;
            scratch_word_b[0] = data_80026a9c_slot27[scratch_word_a[0]];
            scratch_word_c[0] = data_80026a9e_slot27[scratch_word_a[0]];
            *(s32 *)&obj->vel_x = scratch_word_b[0];
            obj->vel_y = scratch_word_c[0];
            scratch_word_b[0] = scratch_word_b[0] << 3;
            scratch_word_c[0] = scratch_word_c[0] - scratch_word_b[0];
            obj->pos_x = *(u16 *)scratch_word_c;
            obj->pos_y = 0x3c;
        }
    } else {
        obj->state_2 = 0;
        obj->state_1 = obj->state_1 + 1;
        scratch_word_a[0] = 0;
        set_side_bit_in_pass_index_slot27(obj);
    }
}
