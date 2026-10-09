/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_800240d0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Slot28Obj *b;
    obj->field_47--;
    if (obj->field_47 == 0) {
        obj->field_47 = 8;
        obj->field_05 += 1;
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 0;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 1;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 2;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 3;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 4;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x79;
            b->field_03 = 5;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            b->field_6c = obj->field_6c;
        }
    }
    obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}
