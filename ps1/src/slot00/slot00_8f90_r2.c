/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

void func_800790e4_slot00(Object *o) {
    Slot00Obj *t = (Slot00Obj *)o->field_3c;
    if (o->field_03 == 0) {
        if (o == t->field_30) {
            t->field_30 = 0;
        }
    } else {
        if (o == t->field_34) {
            t->field_34 = 0;
        }
    }
    func_8011f38c(o);
}
