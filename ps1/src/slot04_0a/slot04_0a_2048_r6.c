/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f30_slot04_0a(Object *obj);

void func_801b252c_slot04_0a(Object *obj) {
    s16 t;
    u16 u;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((t & 0xff00) == 0x100) {
        obj->field_3a = t & 0xff;
        obj->field_0b = obj->field_158;
    }
    u = obj->field_3a;
    if ((u & 0xff) != 0) {
        obj->field_3a = u & 0xff00;
        func_801b3f30_slot04_0a(obj);
    }
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_14 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801312b8(obj);
    }
}
