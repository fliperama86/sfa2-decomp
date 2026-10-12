/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a018_slot00[];
extern u8 data_8007a13c_slot00;
extern u8 data_8007a140_slot00;

void func_80077aac_slot00(Object *obj) {
    data_8007a13c_slot00 = obj->sound_side;
    obj->sound_side = obj->variant_index_2;
    obj->variant_index_2 = data_8007a13c_slot00;
    data_8007a140_slot00 = obj->spawn_flag;
    obj->spawn_flag = obj->offset_index;
    obj->offset_index = data_8007a140_slot00;
    data_8007a018_slot00[obj->state_0](obj);
    data_8007a13c_slot00 = obj->sound_side;
    obj->sound_side = obj->variant_index_2;
    obj->variant_index_2 = data_8007a13c_slot00;
    data_8007a140_slot00 = obj->spawn_flag;
    obj->spawn_flag = obj->offset_index;
    obj->offset_index = data_8007a140_slot00;
}
