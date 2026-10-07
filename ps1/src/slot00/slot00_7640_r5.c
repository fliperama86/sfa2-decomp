/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a018_slot00[];
extern u8 data_8007a13c_slot00;
extern u8 data_8007a140_slot00;

void func_80077aac_slot00(Object *obj) {
    data_8007a13c_slot00 = obj->field_66;
    obj->field_66 = obj->field_a0;
    obj->field_a0 = data_8007a13c_slot00;
    data_8007a140_slot00 = obj->field_44;
    obj->field_44 = obj->field_a1;
    obj->field_a1 = data_8007a140_slot00;
    data_8007a018_slot00[obj->field_04](obj);
    data_8007a13c_slot00 = obj->field_66;
    obj->field_66 = obj->field_a0;
    obj->field_a0 = data_8007a13c_slot00;
    data_8007a140_slot00 = obj->field_44;
    obj->field_44 = obj->field_a1;
    obj->field_a1 = data_8007a140_slot00;
}
