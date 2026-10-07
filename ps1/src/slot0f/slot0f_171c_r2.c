/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_800e0d30_slot0f(void);
void func_800e2348_slot0f(Object *o);
void func_800e2328_slot0f(Object *o);

void func_800e186c_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    o->field_01 += 1;
    o->field_0d = func_800e0d30_slot0f();
    o->field_09 = 1;
    obj->field_0b = 0;
    func_800e2348_slot0f(o);
}

void func_800e18bc_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b;
    func_800e2348_slot0f(o);
    b = o->field_10;
    if (b & 0x860) {
        if (b & 0x820) {
            if (obj->field_0b == 2) {
                func_800e2328_slot0f(o);
            } else {
                obj->field_0c = 0;
                o->field_01 += 1;
                o->field_04 += 1;
            }
            func_80120554(0, 0, 0x205);
        } else {
            func_800e2328_slot0f(o);
            func_80120554(0, 0, 0x202);
        }
    }
}
