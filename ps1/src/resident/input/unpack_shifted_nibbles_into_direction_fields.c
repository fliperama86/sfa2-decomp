/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void unpack_shifted_nibbles_into_direction_fields(Object *object) {
    object->field_15a = (u16)(script_work_value = (s16)script_work_value >> 3) >> 1 & 0xf;
    object->input_dir_second = (script_work_value = (s16)script_work_value >> 5) & 0xf;
    object->direction_index = (script_work_value = (s16)script_work_value >> 4) & 0xf;
}
