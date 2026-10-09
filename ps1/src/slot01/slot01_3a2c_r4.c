/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern int data_80055f3c_slot01;
extern s16 data_80055e9a_slot01;
Block172 *func_8011f1e0(void);
void func_80012020_slot01(Object *obj);

void func_80013da4_slot01(Object *obj) {
    Object *p;
    if (obj->field_60 != 0) {
        func_80131094(obj);
        func_8011ffdc(obj);
    } else if (obj->field_05 != 0) {
        if (game_state.field_04 != 0) {
            obj->field_04++;
        }
        func_8011ffdc(obj);
    } else if (data_80055e9a_slot01 < 0) {
        obj->field_05++;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x30;
            p->field_01 = 1;
            p->field_0c = 0;
            p->field_0d = 0;
            p->field_60 = 1;
            p->field_7a = 0x20;
            p->field_7c = 0x1e8;
            p->field_98 = data_80019330_slot01;
            p->field_90 = (void *)0x80059000;
            p->field_9c = data_800195b8_slot01;
            p->field_03 = *(u8 *)&data_80055f3c_slot01;
        }
    } else {
        func_80012020_slot01(obj);
    }
}
