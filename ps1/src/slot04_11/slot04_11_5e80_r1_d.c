/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7be0_slot04_11[];

void func_80131468(Object *obj);
void func_80131638(Object *obj);
void func_801b6ca4_slot04_11(Object *obj);
void func_801b6cf4_slot04_11(Object *obj);
void func_801b6f28_slot04_11(Object *obj);
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

void func_801b66ac_slot04_11(Object *obj) {
    func_801312b8(obj);
}

void func_801b66cc_slot04_11(Object *obj) {
    func_80131468(obj);
}

void func_801b66ec_slot04_11(Object *obj) {
    func_80131638(obj);
}

void func_801b670c_slot04_11(Object *obj) {
    ref_other.p = obj->other;
    data_801c7be0_slot04_11[obj->field_04](obj);
}

void func_801b675c_slot04_11(Object *obj) {
    Object *o;

    obj->field_01 = 1;
    obj->field_98 = data_80172a48;
    obj->field_90 = (void *)0x800fb100;
    obj->field_04++;
    obj->field_9c = data_80173c9c;
    func_801b6ca4_slot04_11(obj);
    o = ref_other.p->other;
    func_801204f4(o, o->side, 0xf);
    ((Slot04bObj *)obj)->field_47 = ref_other.p->field_28c;
    func_801b6cf4_slot04_11(obj);
    func_801b6f28_slot04_11(obj);
}
