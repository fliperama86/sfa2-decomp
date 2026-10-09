/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013d8c_slot28(Object *obj, SeqRec *rec);
void func_80013dc4_slot28(Object *obj, SeqRec *rec);

void func_80013d8c_slot28(Object *obj, SeqRec *rec) {
    int w;
    int t;
    obj->sequence = (SequenceStep *)rec;
    t = rec->header;
    w = t;
    t = w >> 16;
    obj->field_38 = t;
    obj->field_3a = w;
    func_80013dc4_slot28(obj, rec);
}
