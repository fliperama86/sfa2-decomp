/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01State55e94 data_80055e94_slot01;

void spawn_linked_object_by_variant_slot01(Object *obj) {
    Object *p = (Object *)pop_block_from_pool();
    int t;
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x95;
        p->sound_side = 0;
        p->table_index = 0x81;
        t = obj->variant_index;
        p->facing_side = t;
        if (t != 0) {
            p->table_index = 0x86;
        }
        p->vel_x = 0;
        p->vel_y = 0;
        p->parent = obj;
        obj->linked_object = (u32)p;
    }
}

void set_record_cursor_and_spawn_y_slot01(Object *obj, Slot01Rec152ec *a1) {
    u16 v;
    int c;
    data_80055e94_slot01.cursor = a1;
    c = a1->count;
    data_80055e94_slot01.count = c;
    v = a1->value;
    data_80055e94_slot01.value = v;
    obj->spawn_y = (v & 0x1f) + 0x1e0;
}

void advance_record_list_and_set_spawn_y_slot01(Object *obj) {
    int t = (u16)data_80055e94_slot01.count - 1;
    data_80055e94_slot01.count = t;
    if ((s16)t == 0) {
        u16 v;
        int c;
        data_80055e94_slot01.cursor++;
        if (*(int *)data_80055e94_slot01.cursor < 0) {
            data_80055e94_slot01.cursor = (Slot01Rec152ec *)*(int *)data_80055e94_slot01.cursor;
        }
        c = data_80055e94_slot01.cursor->count;
        data_80055e94_slot01.count = c;
        v = data_80055e94_slot01.cursor->value;
        data_80055e94_slot01.value = v;
        obj->spawn_y = (v & 0x1f) + 0x1e0;
    }
}
