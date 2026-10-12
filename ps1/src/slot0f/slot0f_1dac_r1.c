/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e8518_slot0f[];

extern ObjectFn data_800e87cc_slot0f[];

void update_message_wait_return_to_start_slot0f(Object *obj) {
    s8 t;

    if (obj->pos_x_frac & 0x860) {
        obj->field_0f = 1;
    }
    t = obj->field_0f;
    t--;
    obj->field_0f = t;
    if (t == 0) {
        obj->active_count = 1;
        obj->field_01 = 0;
        obj->ot_depth = 0;
        obj->state_0 = 0;
    }
}

void clear_loop_flag_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    obj->loop_flag = 0;
}

void call_handler_by_handler_index_b_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    data_800e87cc_slot0f[(s8)obj->handler_index_b](o);
}

void queue_three_items_with_highlight_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    int i;

    add_item_to_table(&data_800e8508_slot0f);
    for (i = 0; i < 3; i++) {
        Slot0fRec8508 *rec = &data_800e8518_slot0f[i];

        if (obj->field_0a == i) {
            rec->field_0b = 0x10;
        } else {
            rec->field_0b = 0x1a;
        }
        add_item_to_table(rec);
    }
}
