/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80146888(Object *object);

void func_801b4298_slot04_08(Object *obj) {
    if ((u8)func_80146888(obj)) {
        if (obj->field_0b != 0) {
            ref_other.p->pos_x -= 0x20;
        } else {
            ref_other.p->pos_x += 0x20;
        }
    }
}

void func_801b4310_slot04_08(Object *obj) {
    int z = 0;
    int i;
    u8 *p;
    u8 c;
    u8 d;

    p = (u8 *)obj->slots;
    for (c = z, i = 0x5f; i >= 0; i--) {
        *p++ = c;
    }
    p = &((Slot04aObj *)obj)->field_184;
    for (d = z, i = 0xf; i >= 0; i--) {
        *p++ = d;
    }
}

void func_801b4354_slot04_08(Object *obj) {
    s16 t = obj->field_3a;
    if ((t & 0x8000) == 0 && (t & 0xff00) != 0 && (t & 0xff00) == 0x300 && game_state.field_48 == obj->field_ce) {
        obj->field_3a = t & 0xff;
        func_801204f4(obj, obj->side, 0x11);
    }
}
