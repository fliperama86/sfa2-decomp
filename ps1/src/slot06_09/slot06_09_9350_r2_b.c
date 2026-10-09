/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eca14_slot06_09[];
extern SequenceStep *data_801ec940_slot06_09[];
extern s16 data_801ec954_slot06_09[];
extern s16 data_801ec956_slot06_09[];
extern u16 data_801ec958_slot06_09[];

void func_801e9cc0_slot06_09(Object *obj);

void func_801e9ca0_slot06_09(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9cc0_slot06_09(Object *obj) {
    int i = (u8)(obj->field_03 * 32 + ((Slot06Obj *)obj)->field_4c);
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];

    obj->field_54 = data_801ec954_slot06_09[i];
    obj->field_58 = data_801ec956_slot06_09[i];
    obj->field_46 = data_801ec958_slot06_09[i];
}

void func_801e9d2c_slot06_09(Object *obj) {
    data_801eca14_slot06_09[obj->field_04](obj);
}

void func_801e9d6c_slot06_09(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_4c = obj->pos_x;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_04++;
    obj->field_54 = 0xf0 - obj->pos_y;
    func_80130700(obj, data_801ec940_slot06_09[0]);
}

void func_801e9dd4_slot06_09(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;

    if (game_state.field_65 == 0) {
        o->pos_y = 0xf0 - (obj->field_54 + ((Slot06Layer *)data_801aa544)->field_3a);
        func_80131094(o);
    }
    if (((Slot06Layer *)data_801aa544)->field_36 >= 0x3a0) {
        o->field_04++;
    }
    func_80120028(o);
}

void func_801e9e58_slot06_09(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
