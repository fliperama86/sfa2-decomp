/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern u8 data_800319b8_slot01[];
extern SequenceStep *data_800212ac_slot01[];
Poly28 *init_poly_ft4_run_with_tpage_clut_slot01(Object *obj, Poly28 *p, int a2, int a3);
void update_or_draw_sprite_set_slot01(Object *obj);

void init_object_copy_parent_position_slot01(Object *obj) {
    Object *r;

    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1ea;
    obj->data_ptr_a = data_80019330_slot01;
    obj->data_ptr_b = data_800195b8_slot01;
    obj->field_90 = (void *)0x80059000;
    obj->facing_side = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->state_0 = obj->state_0 + 1;
    r = obj->parent;
    obj->pos_x = r->pos_x;
    obj->pos_y = r->pos_y;
    if (obj->table_index & 0x80) {
        obj->field_01 = 1;
        obj->field_0c = 1;
        obj->ot_depth = 0;
        set_sequence_from_table_5(obj, obj->table_index - 0x80, data_800212ac_slot01);
    } else {
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->ot_depth = 3;
        set_sequence_from_table_5(obj, 0, (SequenceStep **)((ObjectWords *)obj)->word_58);
        init_poly_ft4_run_with_tpage_clut_slot01(obj, (Poly28 *)data_800319b8_slot01, (u16)func_8015bd0c(0, 0, 0x300, 0), (u16)func_8015bdd4(0x70, 0x1eb));
        init_poly_ft4_run_with_tpage_clut_slot01(obj, (Poly28 *)(data_800319b8_slot01 + 0xf0), (u16)func_8015bd0c(0, 0, 0x300, 0), (u16)func_8015bdd4(0x70, 0x1eb));
        update_or_draw_sprite_set_slot01(obj);
    }
}
