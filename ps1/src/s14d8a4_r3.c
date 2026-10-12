/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014de20(Object *object) {
    object->field_15a = (u16)(data_80189464 = (s16)data_80189464 >> 3) >> 1 & 0xf;
    object->input_dir_second = (data_80189464 = (s16)data_80189464 >> 5) & 0xf;
    object->direction_index = (data_80189464 = (s16)data_80189464 >> 4) & 0xf;
}
