/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e68_slot04_0a(Object *object);
void func_801b3f30_slot04_0a(Object *object);

void func_801b3620_slot04_0a(Object *obj) {
    Object *o;
    u8 b;
    u16 t = obj->field_3a;
    if (t & 0xff) {
        obj->field_3a = t & 0xff00;
        func_801b3e68_slot04_0a(obj);
    }
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        o = obj->other;
        b = o->field_0b;
        obj->field_0b = b;
        if (b == 0) {
            obj->pos_x = o->pos_x + 0x28;
        } else {
            obj->pos_x = o->pos_x - 0x28;
        }
        func_801204f4(obj, obj->side, 0xf);
        func_801307e0(obj, 0x4a);
    }
}

void func_801b36dc_slot04_0a(Object *obj) {
    u16 t = obj->field_3a;
    if (t & 0xff) {
        obj->field_3a = t & 0xff00;
        func_801b3f30_slot04_0a(obj);
    }
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}
