/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e44_slot04_0a(Object *obj) {
    int i;
    u32 z = 0;
    u32 *p = (u32 *)obj->slots;

    for (i = 0xf; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b3e68_slot04_0a(Object *obj) {
    Rect r;
    u8 *p;
    u8 *src;

    src = table_801a27e4 + (obj->field_0d << 5);
    p = table_801a27e4;
    r.x = (obj->side << 5) + 0x110;
    r.y = (obj->field_d4 << 2) + 0x1e8;
    r.w = 0x10;
    r.h = 4;
    func_80158028(&r, src);
    p += 0x1400;
    func_80158028(&r, p + (obj->field_0d << 5));
    func_80137220(0, 6);
    func_801379bc(obj, 2);
}

void func_8013788c(Object *object);

void func_801b3f10_slot04_0a(Object *obj) {
    obj->field_103 = 0;
    func_8013788c(obj);
}

