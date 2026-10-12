/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Pair data_80015ddc_slot01[];
extern u8 data_80055ea0_slot01[];
extern u8 data_80055ea4_slot01;
void init_primitive_table_and_load_images_slot01(Object *o);
void draw_tile_map_window_slot01(Object *o);

void set_position_from_parent_table_entry_slot01(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *p = obj->parent;
    obj->pos_x = data_80015ddc_slot01[p->field_02].first + box_margin[0];
    *(u16 *)&obj->pos_y = data_80015ddc_slot01[p->field_02].second;
}

void call_handler_by_mode_byte_0_or_1_slot01(void) {
    u8 v = data_80055ea4_slot01;
    u8 *p = data_80055ea0_slot01;
    if (v == 0) {
        init_primitive_table_and_load_images_slot01((Object *)p);
    } else if (v == 1) {
        draw_tile_map_window_slot01((Object *)p);
    }
}
