/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_8007a0ac_slot00[];
void func_80078458_slot00(Object *obj, int mode);

void func_800780f4_slot00(Object *obj) {
    obj->field_a4 = 2;
    obj->field_a5 = 0;
    obj->vel_y = data_8007a0ac_slot00[obj->table_index * 2];
    obj->accel_y = data_8007a0ac_slot00[obj->table_index * 2 + 2];
    if (obj->facing_side != 0) {
        obj->vel_x = data_8007a0ac_slot00[obj->table_index * 2 - 1];
        obj->accel_x = data_8007a0ac_slot00[obj->table_index * 2 + 1];
    } else {
        obj->vel_x = -data_8007a0ac_slot00[obj->table_index * 2 - 1];
        obj->accel_x = -data_8007a0ac_slot00[obj->table_index * 2 + 1];
    }
    func_80078458_slot00(obj, 2);
}
