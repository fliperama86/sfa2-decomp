/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801a2bc4[];
extern u16 data_801c79b4_slot04_14[];
extern u16 data_801c7c74_slot04_14[];

void func_801b8354_slot04_14(Object *obj);
void func_801b8430_slot04_14(Object *obj, u8 arg);
void func_801b84b8_slot04_14(Object *obj);
void func_8011ffdc(Object *o);

void func_801b8354_slot04_14(Object *obj) {
    obj->field_04 = 2;
}

void func_801b8360_slot04_14(Object *obj) {
    int t;

    if (game_state.field_64 != 0) {
        obj->field_54 = obj->field_54 - 1;
        if (obj->field_54 == 0 && (s16)obj->field_46 != 0) {
            func_801b84b8_slot04_14(obj);
            t = obj->field_46 - 1;
            obj->field_46 = t;
            if ((s16)t != 0) {
                obj->field_54 = 6;
            } else {
                obj->field_46 = 0xfff;
            }
        }
        func_80131094(obj);
        func_8011ffdc(obj);
    } else {
        obj->field_04++;
    }
}

void func_801b8410_slot04_14(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b8430_slot04_14(Object *obj, u8 arg) {
    s16 i;

    for (i = 1; i < 16; i++) {
        if (arg == 0) {
            data_801a2bc4[i] = data_801c79b4_slot04_14[i];
        } else {
            data_801a2bc4[i] = data_801c7c74_slot04_14[i];
        }
    }
    func_80137220(0, 6);
}

void func_801b84b8_slot04_14(Object *obj) {
    s16 i;
    int row;

    row = obj->field_a0 << 4;
    for (i = 1; i < 16; i++) {
        if (obj->field_03 == 0) {
            data_801a2bc4[i] = data_801c79b4_slot04_14[row + i];
        } else {
            data_801a2bc4[i] = data_801c7c74_slot04_14[row + i];
        }
    }
    func_80137220(0, 6);
    obj->field_a0++;
}
