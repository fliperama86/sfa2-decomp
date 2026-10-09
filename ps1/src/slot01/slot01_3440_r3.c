/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019c68_slot01[];
extern u8 data_80019dcc_slot01[];
extern SequenceStep *data_80024330_slot01[];
extern GameState *data_80015a4c_slot01;
extern void (*data_80015a64_slot01[])(Object *obj);
void func_80013a24_slot01(Object *obj);
void func_80013a2c_slot01(Object *obj);
void func_80013ac0_slot01(Object *obj);
void func_8011ffdc(Object *o);

void func_80013734_slot01(Object *obj) {
    obj->field_01 = 1;
    obj->field_0c = 1;
    obj->field_09 = 0x10;
    obj->field_7a = 0x10;
    obj->field_7c = 0x1f0;
    obj->field_98 = data_80019c68_slot01;
    obj->field_9c = data_80019dcc_slot01;
    obj->field_90 = (void *)0x8006d478;
    obj->field_04++;
    if (obj->field_03 == 0) {
        func_80013a2c_slot01(obj);
        func_80013ac0_slot01(obj);
        func_80130768(obj, 0, data_80024330_slot01);
    } else if (obj->field_03 == 1) {
        func_80013a2c_slot01(obj);
        obj->field_04 = 2;
        obj->pos_x = data_80015a4c_slot01->field_d2;
        obj->pos_y = data_80015a4c_slot01->field_d4;
        func_80013a24_slot01(obj);
        func_80013ac0_slot01(obj);
        func_80130768(obj, 1, data_80024330_slot01);
    } else if (obj->field_03 >= 2) {
        obj->field_04 = 4;
        func_80130768(obj, obj->field_03, data_80024330_slot01);
    }
}

/* The call of func_8011ffdc passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 7 instruction slots. */
void func_8001385c_slot01(Object *obj) {
    data_80015a64_slot01[obj->field_05](obj);
    ((void (*)(void))func_8011ffdc)();
}
