/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Every path ends in its own call of func_8012df44. The compiler merges only
   the call instruction, which is what the original shows: the argument
   register is reloaded on the paths that come after the first call and left
   alone on the path that comes straight from the entry. */
void func_8012dee4(Object *object) {
    u8 v = object->field_164;
    object->field_247 = 0;
    if (v == 0) {
        func_8012df44(object);
        return;
    }
    if (func_8012eff8(object) == 0) {
        func_8012df44(object);
        return;
    }
    object->field_07++;
    func_8012df44(object);
}
