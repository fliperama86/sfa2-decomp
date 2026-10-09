/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013d8c_slot28(Object *obj, SeqRec *rec);

void func_80013d5c_slot28(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    func_80013d8c_slot28(object, *p);
}
