/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a194(Object *object) {
    object->state_3++;
    if (object->field_cd == 0) {
        func_8012a1e4(object);
        return;
    }
    set_sequence_by_side(object, 7);
    func_8012a1e4(object);
}
