/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
Block172 *func_8011f1e0(void);
void func_8011ffdc(Object *obj);

void func_800145f4_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Object *n;
    if (data_8018f5a0->field_4e >= 3 && game_state.mode != 3) {
        int sum = *(u16 *)&o->pos_y + obj->field_50;
        int y;
        *(u16 *)&o->pos_y = sum;
        y = (s16)sum;
        if ((u32)y >= 0x1e1) {
            obj->field_01 = 0;
            obj->field_05 = obj->field_05 + 1;
            n = (Object *)func_8011f1e0();
            if (n != 0) {
                n->field_00 = 1;
                n->field_02 = 0xaa;
                n->field_03 = 0x80;
            }
        }
    }
    func_8011ffdc(o);
}

void func_800146a8_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    if (data_8018f5a0->field_4e >= 3 && game_state.mode != 3) {
        int sum = *(u16 *)&o->pos_y + obj->field_50;
        int y;
        *(u16 *)&o->pos_y = sum;
        y = (s16)sum;
        if ((u32)y >= 0x201) {
            obj->field_05 = obj->field_05 + 1;
        }
    }
    func_8011ffdc(o);
}

void func_80014730_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->field_05 = obj->field_05 + 1;
}
