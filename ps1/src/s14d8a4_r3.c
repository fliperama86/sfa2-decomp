/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014de20(Object *object) {
    object->field_15a = (u16)(data_80189464 = (s16)data_80189464 >> 3) >> 1 & 0xf;
    object->field_129 = (data_80189464 = (s16)data_80189464 >> 5) & 0xf;
    object->field_12a = (data_80189464 = (s16)data_80189464 >> 4) & 0xf;
}
