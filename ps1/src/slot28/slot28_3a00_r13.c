/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015ffc_slot28(Object *obj, SequenceStep *step);

void func_80015fcc_slot28(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    func_80015ffc_slot28(object, (SequenceStep *)*p);
}
