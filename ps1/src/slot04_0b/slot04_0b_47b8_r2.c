/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4db8_slot04_0b(Object *obj, int index);
void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy);

void func_801b4928_slot04_0b(Object *obj) {
    if (!(ref_other.p->field_3a & 1)) {
        obj->field_05++;
        if (obj->field_0b != 0) {
            obj->field_4c = 0xfff80000;
        } else {
            obj->field_4c = 0x80000;
        }
        obj->field_50 = 0x20000;
        obj->field_54 = 0;
        obj->field_58 = -0x4000;
        func_801b4e0c_slot04_0b(obj, -0x44, 0x38);
        func_801b4db8_slot04_0b(obj, 0x18);
    }
    func_8011ffdc(obj);
}

void func_801b49c0_slot04_0b(Object *obj) {
    GameState *g = &game_state;
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_4c += obj->field_54;
    obj->field_50 += obj->field_58;
    if (obj->field_50 < 0) {
        if (!(obj->pos_y < ref_other.p->field_70)) {
            obj->field_04++;
        }
    }
    func_8011ff74(obj);
    if (g->field_1d & 1) {
        func_8011ffdc(obj);
    }
}
