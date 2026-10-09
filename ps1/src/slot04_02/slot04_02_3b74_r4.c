/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80126130(void);
void func_80126244(void);
void func_80155d4c(int idx, int side);
void func_80155eac(int idx, int side);
void func_8013788c(Object *object);
u8 func_801b625c_slot04_02(Object *obj);
void func_801b4180_slot04_02(Object *obj);

void func_801b4050_slot04_02(Object *obj) {
    Object *p = obj->other;

    func_80126130();
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((obj->field_46 & 0xff) == 0) {
        obj->field_07++;
        func_80120554(p, p->side, 0x306);
        func_8013788c(obj);
        func_80126244();
        func_801b4180_slot04_02(obj);
    } else if (func_801b625c_slot04_02(obj) != 0) {
        if (obj->field_cd == 0) {
            obj->field_be = 0;
            obj->field_bf = 0xff;
            func_80155d4c(4, (s8)obj->side);
            func_80155eac(4, obj->side);
        }
        p->field_6a++;
    }
}

void func_801b4134_slot04_02(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) != 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}
