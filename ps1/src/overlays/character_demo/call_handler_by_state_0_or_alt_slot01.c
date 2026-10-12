/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_80015408_slot01[];
extern s16 data_80015418_slot01[];
extern u8 data_8002ceb8_slot01[];
void call_handler_by_state_0_2_slot01(Object *obj);
void init_effect_motion_and_load_image_slot01(Object *obj);
void init_sprite_set_textured_quads_slot01(Object *obj, u8 *a, int b, int c, int d);

void call_handler_by_state_0_or_alt_slot01(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    if (obj->table_index & 0x80) {
        call_handler_by_state_0_2_slot01(obj);
    } else {
        data_80015408_slot01[obj->state_0](obj);
    }
}

void init_scaled_sprite_object_slot01(Object *obj) {
    int a;
    int b;
    obj->field_0e = 0;
    obj->ot_depth = 0;
    obj->table_index_2 = 0;
    obj->field_20 = 0xfff0;
    obj->y_scale = 0xfff0;
    obj->state_0++;
    init_effect_motion_and_load_image_slot01(obj);
    ref_other.p = obj->parent;
    a = (u16)func_8015bd0c(0, 0, data_80015418_slot01[0], data_80015418_slot01[1]);
    b = (u16)func_8015bdd4(data_80015418_slot01[2], data_80015418_slot01[3]);
    init_sprite_set_textured_quads_slot01(obj, data_8002ceb8_slot01, a, b, ref_other.p->side);
}
