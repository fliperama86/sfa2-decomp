/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
void func_80011a18_slot27(Object *obj);
void func_80011a64_slot27(void);
void func_800139a8_slot27(Object *obj);
void func_80013b74_slot27(Object *obj);

void func_80011aac_slot27(Object *obj) {
    int *state = (int *)data_8019045c;
    int *mask = data_80190464;

    int t = game_state.mode;
    int u;
    int side;

    *state = t;
    u = game_state.field_07 | t;
    *state = u;
    side = ref_other.p->side;
    *mask = side;
    if (((u >> side) & 1) != 0) {
        *mask = 1 << ref_other.p->side;
        func_80011a18_slot27(obj);
        func_80011a64_slot27();
        obj->field_09 = 4;
        obj->field_90 = (void *)0x80038000;
        obj->field_98 = data_80017c28_slot27;
        obj->field_9c = data_8001aa14_slot27;
        obj->field_7a = 0;
        obj->field_7c = 0x1e0;
        obj->field_0d = 0;
        obj->field_01 = 0;
        obj->field_0b = 0;
        obj->field_70 = 0;
        obj->field_73 = 0;
        obj->field_04++;
        if ((game_state.field_07 & *mask) != 0) {
            *state = ref_other.p->kind;
            if (*state == 0x12) {
                *state = 4;
                ref_other.p->kind = *state;
            }
            if (*state == 0x13) {
                *state = 0x11;
                ref_other.p->kind = *state;
            }
            if (*state == 0x14) {
                *state = 2;
                ref_other.p->kind = *state;
            }
            obj->field_03 = *state;
            obj->field_46 = 0x4b0;
            func_800139a8_slot27(obj);
            ref_first.p = ref_other.p;
            ref_other.p = func_8011f32c();
            if (ref_other.p != 0) {
                ref_other.p->field_00 = 1;
                ref_other.p->field_02 = 0x24;
                ref_other.p->other = ref_first.p;
                ref_other.p->field_3c = obj;
                obj->field_3c = ref_other.p;
            }
            ref_other.p = obj->other;
            *state = 0;
            func_80013b74_slot27(obj);
        } else {
            obj->field_05 += 2;
        }
    }
}
