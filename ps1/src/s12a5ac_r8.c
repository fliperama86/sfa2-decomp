/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: RESIDUAL func_8012bbac: original keeps the two "field_07 = 0" arms apart (no cross-jump merge); built merges them (-8 bytes). 3 source shapes tried. */



void func_8012bc24(Object *object) {
    ObjectFn fn;
    func_80129be0(object);
    object->field_254 = 0;
    if (object->field_02 == 0) {
        if (object->field_cd == 0) {
            fn = scratch_fn_e_left;
        } else {
            fn = scratch_call_left;
        }
    } else {
        if (object->field_cd == 0) {
            fn = scratch_fn_e_right;
        } else {
            fn = scratch_call_right;
        }
    }
    fn(object);
}

void func_8012bcc0(Object *object) {
    handler_table_19ec[object->field_07](object);
}
