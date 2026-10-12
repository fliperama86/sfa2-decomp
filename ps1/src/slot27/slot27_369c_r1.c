/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800138bc_slot27(void);
void func_80013b74_slot27(Object *obj);
void func_80013cc4_slot27(Object *obj);

#define SEL (*(int *)&game_state.field_354)

void func_8001369c_slot27(Object *obj) {
    int v;
    s16 t46;

    func_800138bc_slot27();
    if (SEL != 0) {
        game_state.table_index_3 = (s8)data_8016e68c;
    } else {
        game_state.table_index_3 = 0;
    }
    {
        int *s = (int *)data_8019045c;
        u8 *f = &game_state.selection;

        v = *s & 1;
        *s = v;
        if (*f != v) {
            *f = *(u8 *)s;
            if (game_state.table_index != 3) {
                *s = v + 1;
            }
            func_801205c4(game_state.other_object->side, 0);
        }
    }
    if (game_state.busy_flag != 0) {
        goto tail;
    }
    t46 = obj->countdown; t46--;
    obj->countdown = t46;
    if (t46 != 0) {
        int *s = (int *)data_8019045c;

        *s = ~game_state.other_object->prev_input_bits;
        *s = (game_state.other_object->input_bits & *s) & 0xf0;
        if (*s == 0) {
            goto tail;
        }
    }
    {
        int *t;

        obj->state_2 = obj->state_2 + 1;
        ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
        if (obj->sound_side != 0) {
            ref_first.p = (Object *)((ObjectWords *)obj)->word_58;
        }
        ref_first.p->field_6b = 0xff;
        t = (int *)data_8019045c;
        *t = game_state.selection;
        func_801205c4(game_state.other_object->side, 1);
        *t = 0xa;
        *t = obj->sound_side + 0xa;
        func_80013b74_slot27(obj);
        return;
    }
tail:
    func_80013cc4_slot27(obj);
}
