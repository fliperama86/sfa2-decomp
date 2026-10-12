/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801c0168_slot04_00[];
extern u32 data_801c0314_slot04_00[];
extern u32 data_801c052c_slot04_00[];

void unlink_from_parent_and_release_object_slot04_00(Object *obj) {
    ref_other.p = obj->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags & 0x80) {
        ref_other.p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void set_box_tables_from_health_index_slot04_00(Object *obj) {
    if (obj->vel_table_index == 6) {
        game_state.cursor = data_801c0168_slot04_00;
    } else if (obj->vel_table_index == 8) {
        game_state.cursor = data_801c0314_slot04_00;
    } else {
        game_state.cursor = data_801c052c_slot04_00;
    }
    game_state.cursor = (u32 *)game_state.cursor[(s16)obj->health];
    obj->box_tables = (BoxTables *)game_state.cursor;
}
