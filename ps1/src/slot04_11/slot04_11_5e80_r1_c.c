/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);

void func_801b6604_slot04_11(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b6628_slot04_11(Object *obj) {
    u16 t = obj->field_3a;
    int d = t & 0x7f;

    if (d != 0) {
        obj->field_3a = t & 0xff80;
        if (obj->field_0b == 0) {
            d = -d;
        }
        obj->pos_x = obj->pos_x + d;
    }
}
