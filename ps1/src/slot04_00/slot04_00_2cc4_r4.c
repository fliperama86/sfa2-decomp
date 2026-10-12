/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern BoxTables data_801c0148_slot04_00[];

void set_box_tables_from_health_index_slot04_00(Object *obj);
void call_handler_by_state_1_slot04_00(Object *obj);

void init_spawned_object_from_parent_slot04_00(Object *obj) {
    u8 a[6] = { 8, 9, 10, 0xff, 0xff, 0xff };
    int b[6] = { 0x30000, 0x3c000, 0x48000, 0x30000, 0x3c000, 0x48000 };
    int v;
    int x;

    obj->state_0 = obj->state_0 + 1;
    obj->ot_depth = 0;
    ref_other.p = obj->parent;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    obj->box_tables = data_801c0148_slot04_00;
    if (obj->vel_table_index >= 6) {
        set_box_tables_from_health_index_slot04_00(obj);
    }
    obj->countdown_2 = 5;
    obj->active_state = 0;
    obj->vel_y = 0;
    if (obj->vel_table_index == 4) {
        obj->image_row = obj->image_row + 1;
    }
    obj->variant_index_2 = a[obj->vel_table_index >> 1];
    v = b[obj->vel_table_index >> 1];
    x = 0x59;
    if (obj->facing_side == 0) {
        v = -v;
        x = -0x59;
    }
    obj->vel_x = v;
    obj->pos_x = obj->pos_x + x;
    obj->pos_y = obj->pos_y - 0x36;
    if (ref_other.p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, obj->vel_table_index >> 1);
    call_handler_by_state_1_slot04_00(obj);
}
