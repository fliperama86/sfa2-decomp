/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079938_slot2b[];
extern s16 data_8007ef24_slot2b;

void func_80077afc_slot2b(Object *obj) {
    data_80079938_slot2b[obj->field_05](obj);
}
