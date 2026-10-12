/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801e1fdc_slot0b[])(Object *);
extern u8 data_801e766c_slot0b[];
void follow_ref_object_and_build_quads_slot0b(Object *obj);
SlotCell *init_list_cells_tpage_clut_slot0b(Object *obj, SlotCell *cell, int arg, int base);

void call_handler_by_state_0_with_parent_slot0b(Object *obj) {
    ref_other.p = obj->parent;
    data_801e1fdc_slot0b[obj->state_0](obj);
}

void init_poly_object_with_tpage_and_clut_slot0b(Object *obj) {
    int s;
    int r;

    obj->facing_side = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->state_0 = obj->state_0 + 1;
    if (obj->table_index == 0) {
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->ot_depth = 3;
        set_sequence_from_table_5(obj, 0, (SequenceStep **)((ObjectWords *)obj)->word_58);
        s = func_8015bd0c(0, 0, 0x280, 0) & 0xffff;
        r = func_8015bdd4(0x140, 0x1e6);
        init_list_cells_tpage_clut_slot0b(obj, (SlotCell *)(data_801e766c_slot0b + obj->variant_index * 0x1e0), s, r & 0xffff);
        follow_ref_object_and_build_quads_slot0b(obj);
    }
}
