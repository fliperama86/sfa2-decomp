/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Same form as func_8012a194: the early return has its own call of
   func_8012a3f4, and the compiler merges only the call instruction. */
void func_8012a3a4(Object *object) {
    object->field_07++;
    if (object->field_cd == 0) {
        func_8012a3f4(object);
        return;
    }
    func_80130678(object, 4);
    func_8012a3f4(object);
}
