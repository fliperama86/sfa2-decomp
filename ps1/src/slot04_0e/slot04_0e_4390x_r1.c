/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80120554(Object *o, int b, unsigned c);
void func_80146478(Object *object, u8 a, int dx, int dy);

void func_801b4390_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s16 a;
    obj->field_47 = 0x20;
    o->field_12c = o->field_12c + 1;
    p = o->other;
    a = p->pos_x;
    a -= o->pos_x;
    if (o->field_0b != 0) {
        a = -a;
    }
    func_80146478(o, 6, a, 0);
    func_80120554(p, p->side, 0x306);
}
