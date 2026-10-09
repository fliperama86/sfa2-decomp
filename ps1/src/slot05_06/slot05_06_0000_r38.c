/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801cb134_slot05_06(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        ((Slot04aObj *)obj)->field_1c3 = 0x10;
    } else {
        func_80130efc(obj);
    }
}

void func_801cb178_slot05_06(Object *obj) {
    func_801cc814_slot05_06(obj);
    func_801cc84c_slot05_06(obj);
    if (((Slot04aObj *)obj)->field_1c3 != 0) {
        if (--((Slot04aObj *)obj)->field_1c3 == 0) goto go;
    }
    if (obj->pos_y > obj->field_70) {
go:
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_80130efc(obj);
    }
}
