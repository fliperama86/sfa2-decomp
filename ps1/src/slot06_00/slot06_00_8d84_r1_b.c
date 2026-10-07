/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06_00Rec3010 data_801f3010_slot06_00[];

void func_801e8f74_slot06_00(Object *obj) {
    Slot06_00Rec3010 *r = &data_801f3010_slot06_00[obj->field_03];
    u32 m = 0x3ffffff;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        r->field_00 = (r->field_00 + r->field_10) & m;
        r->field_08 = (r->field_08 + r->field_14) & m;
    }
    if (((game_state.field_32 + game_state.field_24) & 1) != 0) {
        *(u32 *)&obj->field_10 = r->field_08;
        *(u32 *)&obj->field_14 = r->field_0c;
        obj->sequence = r->field_1c;
    } else {
        *(u32 *)&obj->field_10 = r->field_00;
        *(u32 *)&obj->field_14 = r->field_04;
        obj->sequence = r->field_18;
    }
    func_8011ffdc(obj);
}

void func_801e9060_slot06_00(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
