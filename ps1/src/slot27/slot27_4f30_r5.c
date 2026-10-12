/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec9348 data_80029348_slot27[];
extern void (*data_800284fc_slot27[])(Object *);

void scroll_and_link_cell_grid_to_ot_slot27(Object *obj) {
    Cell20 *c = data_80029348_slot27[data_801a27d0].cells;
    PrimTag *ot = (PrimTag *)data_801987c8 + 0x1d;
    int i;
    int j;
    int t = obj->health;

    obj->health = t - 2;
    if ((s16)obj->health < ((Slot27Obj *)obj)->health_threshold) {
        obj->health = t + 0x1e;
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            c->pos_y = obj->health + (i << 5);
            ((PrimTag *)c)->addr = ot->addr;
            ot->addr = (u32)c;
            c++;
        }
    }
    ((PrimTag *)c)->addr = ot->addr;
    ot->addr = (u32)c;
}

void increment_state_0_3_slot27(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_to_pool_table_8_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_5_slot27(Object *obj) {
    data_800284fc_slot27[obj->state_0](obj);
}
