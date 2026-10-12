/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void draw_scrolling_tilemap_rows_slot27(void);
extern ObjectFn data_80028e68_slot27[];
extern Slot27Recb380 data_8002b380_slot27;

void load_velocities_from_table_slot27(Object *obj) {
    func_8011ffdc(obj);
}

void func_800167b0_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void start_frame_list_set_image_row_slot27(Object *obj, s16 *p) {
    int t;
    data_8002b380_slot27.ptr = p;
    t = p[1];
    data_8002b380_slot27.count = t;
    data_8002b380_slot27.frame = (u16)p[0];
    obj->image_row = (u8)data_8002b380_slot27.frame & 0x1f;
}

void advance_frame_list_set_image_row_slot27(Object *obj) {
    int t;
    data_8002b380_slot27.count -= 1;
    if (data_8002b380_slot27.count == 0) {
        data_8002b380_slot27.ptr += 2;
        if (*(int *)data_8002b380_slot27.ptr < 0) {
            data_8002b380_slot27.ptr = (u16 *)*(int *)data_8002b380_slot27.ptr;
        }
        t = ((s16 *)data_8002b380_slot27.ptr)[1];
        data_8002b380_slot27.count = t;
        data_8002b380_slot27.frame = data_8002b380_slot27.ptr[0];
        obj->image_row = (u8)data_8002b380_slot27.frame & 0x1f;
    }
}

void call_state_0_handler_then_scroll_map_slot27(Object *obj) {
    data_80028e68_slot27[obj->state_0](obj);
    draw_scrolling_tilemap_rows_slot27();
}
