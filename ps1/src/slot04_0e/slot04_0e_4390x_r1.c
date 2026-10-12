/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_sound_at(Object *o, int b, unsigned c);
void func_80146478(Object *object, u8 a, int dx, int dy);

void func_801b4390_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s16 a;
    obj->field_47 = 0x20;
    o->substate_index = o->substate_index + 1;
    p = o->other;
    a = p->pos_x;
    a -= o->pos_x;
    if (o->facing_side != 0) {
        a = -a;
    }
    func_80146478(o, 6, a, 0);
    play_sound_at(p, p->side, 0x306);
}
