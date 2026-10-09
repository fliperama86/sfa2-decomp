/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9ad4_slot06_08(Object *obj);
void func_801e9b28_slot06_08(Object *obj, int arg);
void func_8011fc00(Object *object);

extern ObjectFn data_801ea6c0_slot06_08[];
extern ObjectFn data_801ea6d0_slot06_08[];
extern SeqRec *data_801ea6b8_slot06_08[];

void func_801e98d8_slot06_08(Object *obj) {
    data_801ea6c0_slot06_08[obj->field_04](obj);
}

void func_801e9918_slot06_08(Object *obj) {
    obj->field_01 = 0;
    obj->field_04++;
    func_801e9ad4_slot06_08(obj);
    func_801e9b28_slot06_08(obj, 0);
}

void func_801e995c_slot06_08(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            data_801ea6d0_slot06_08[obj->field_05](obj);
        }
    }
}

void func_801e99bc_slot06_08(Object *obj) {
    int v;
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_05++;
        v = func_80151184() & 0xf;
        obj->field_46 = (v & 0xf) ? v : v + 8;
        func_801e9b28_slot06_08(obj, 1);
    } else {
        func_8011fc00(obj);
    }
}

void func_801e9a40_slot06_08(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 == 0) {
            obj->field_05 = 0;
            func_801e9ad4_slot06_08(obj);
            func_801e9b28_slot06_08(obj, 0);
            return;
        }
    }
    func_8011fc00(obj);
}

void func_801e9ab4_slot06_08(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9ad4_slot06_08(Object *obj) {
    int a = (func_80151184() & 3) << 8;
    int b = func_80151184() & 0xff;
    s16 c = (b & 0xff) ? b : b + 8;
    obj->field_46 = c + a;
}

void func_801e9b28_slot06_08(Object *obj, int arg) {
    func_8011fc54(obj, data_801ea6b8_slot06_08, (u8)arg);
}
