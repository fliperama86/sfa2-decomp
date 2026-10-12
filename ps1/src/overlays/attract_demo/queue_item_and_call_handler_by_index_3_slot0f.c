/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e85b8_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern Slot0fRec8508 data_800e86b8_slot0f;
extern Slot0fRec8508 data_800e86c8_slot0f;
extern ObjectFn data_800e87fc_slot0f[];

void queue_item_and_call_handler_by_index_3_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    add_item_to_table(&data_800e8508_slot0f);
    data_800e87fc_slot0f[(s8)obj->handler_index](o);
}

void queue_slot_marker_and_label_3_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Object *first;

    first = (Object *)&data_800e8608_slot0f;
    if (obj->field_0b == 0) {
        first = (Object *)&data_800e85f8_slot0f;
    }
    add_item_to_table(first);
    add_item_to_table(&data_800e85b8_slot0f);
}

void queue_slot_marker_and_two_mode_message_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Slot0fRec8508 *rec;

    if (obj->field_0b == 0) {
        add_item_to_table(&data_800e85f8_slot0f);
    } else {
        add_item_to_table(&data_800e8608_slot0f);
    }
    switch (obj->mode) {
    case 0:
        rec = &data_800e86b8_slot0f;
        break;
    case 1:
        rec = &data_800e86c8_slot0f;
        break;
    default:
        return;
    }
    add_item_to_table(rec);
}

void reset_state_index_and_handler_indices_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    obj->state_index = 1;
    obj->field_07 = 0;
    obj->field_06 = 0;
    obj->field_05 = 0;
    obj->handler_index = 0;
    obj->handler_index_b = 0;
}
