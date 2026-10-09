/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a194(Object *object) {
    object->field_07++;
    if (object->field_cd == 0) {
        func_8012a1e4(object);
        return;
    }
    func_80130678(object, 7);
    func_8012a1e4(object);
}
