/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0840_slot04_0a[])(Object *);
void func_80137b64(Object *object);
void func_80137be0(Object *object);
void func_80137cc0(Object *object);
void func_80137dc8(Object *object);
void func_80137f00(Object *object);

void func_801b4160_slot04_0a(Object *obj) {
    data_801c0840_slot04_0a[obj->field_05](obj);
}

void func_801b41a0_slot04_0a(Object *obj) {
    func_80137b64(obj);
}

void func_801b41c0_slot04_0a(Object *obj) {
    func_80137be0(obj);
}

void func_801b41e0_slot04_0a(Object *o) {
    func_80137cc0(o);
}

void func_801b4200_slot04_0a(Object *obj) {
    func_80137dc8(obj);
}

void func_801b4220_slot04_0a(Object *obj) {
    func_80137f00(obj);
}
