/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80141e5c(Object *object);

int func_80140cd8(Object *object, int a, int b);
void func_80140fe0(Object *object);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_801410c8(Object *object);
void func_801b51e0_slot04_09(Object *o);

void func_801b5014_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *w;
    Object *e;
    u8 *s;
    int t;
    u16 f;

    w = (Object *)obj->field_330;
    f = w->field_3a;
    e = o->other;
    s = (u8 *)&obj->field_330;
    if (f & 2) {
        w->field_3a = f & 0xfffd;
        o->field_46 = o->field_46 + 0x100;
        if (func_80140cd8(o, 0, 0) != 0) {
            func_80140770(o, 0, 0xd, 0, 0, 1, 1);
            func_801204f4(o, o->side, 0xb);
            func_801b51e0_slot04_09(o);
            return;
        }
    }
    func_80140fe0(o);
    if (((s16)o->field_46 & 0xff00) != 0) {
        if (e->field_15b == 0) {
            func_80140770(o, 0, 5, -1, 0, 1, 1);
            func_801b51e0_slot04_09(o);
            return;
        }
        if ((u8)func_801410c8(o) != 0) {
            func_80140770(o, 0, 0xd, 7, 0, 1, 1);
            func_801204f4(o, o->side, 0xb);
            func_801b51e0_slot04_09(o);
            return;
        }
    }
    e = o->other;
    w = *(Object **)s;
    e->field_0c = s[4];
    e->field_0d = s[5];
    if ((u8)w->field_3a != 0) {
        e->field_0c = 0xff;
        e->field_80 = 1;
        e->field_0d = e->field_0d + 3;
    }
    ((Slot04aObj *)e)->field_16 = ((Slot04aObj *)e)->field_16 + 1;
    t = o->field_46 + 1;
    o->field_46 = (o->field_46 & 0xff00) + (t & 0xff);
    if (o->field_46 & 1) {
        ((Slot04aObj *)e)->field_16 = ((Slot04aObj *)e)->field_16 - 2;
    }
    func_80130efc(o);
    func_80141e5c(o);
}
