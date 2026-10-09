/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e68c;
void func_801205c4(int a, unsigned c);
void func_800138bc_slot27(void);
void func_80013b74_slot27(Object *obj);
void func_80013cc4_slot27(Object *obj);

#define SEL (*(int *)&game_state.field_354)

void func_8001369c_slot27(Object *obj) {
    int v;
    s16 t46;

    func_800138bc_slot27();
    if (SEL != 0) {
        game_state.field_56 = (s8)data_8016e68c;
    } else {
        game_state.field_56 = 0;
    }
    {
        int *s = (int *)data_8019045c;
        u8 *f = &game_state.field_2e;

        v = *s & 1;
        *s = v;
        if (*f != v) {
            *f = *(u8 *)s;
            if (game_state.field_14 != 3) {
                *s = v + 1;
            }
            func_801205c4(game_state.field_358->side, 0);
        }
    }
    if (game_state.field_06 != 0) {
        goto tail;
    }
    t46 = obj->field_46; t46--;
    obj->field_46 = t46;
    if (t46 != 0) {
        int *s = (int *)data_8019045c;

        *s = ~game_state.field_358->field_c4;
        *s = (game_state.field_358->field_c2 & *s) & 0xf0;
        if (*s == 0) {
            goto tail;
        }
    }
    {
        int *t;

        obj->field_06 = obj->field_06 + 1;
        ref_first.p = (Object *)obj->field_54;
        if (obj->field_66 != 0) {
            ref_first.p = (Object *)obj->field_58;
        }
        ref_first.p->field_6b = 0xff;
        t = (int *)data_8019045c;
        *t = game_state.field_2e;
        func_801205c4(game_state.field_358->side, 1);
        *t = 0xa;
        *t = obj->field_66 + 0xa;
        func_80013b74_slot27(obj);
        return;
    }
tail:
    func_80013cc4_slot27(obj);
}
