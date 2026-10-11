/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec51844 data_80051844_slot28[];
void func_80013d8c_slot28(Object *obj, SeqRec *rec);

void func_80013cc0_slot28(Object *obj) {
    int t = obj->step_timer - 1;
    Object *o = obj;
    SeqRec *rec;
    obj->step_timer = t;
    if ((s16)t == 0) {
        rec = (SeqRec *)o->sequence;
        rec = rec + 1;
        if ((s16)o->step_flags < 0) {
            rec = rec - 1;
            func_80013d8c_slot28(obj, rec);
        } else {
            func_80013d8c_slot28(o, rec);
        }
    }
    func_801519b4(data_80051844_slot28);
    if ((obj->step_flags & 1) == 0) {
        func_801519b4(data_80051844_slot28 + 1);
    }
}
