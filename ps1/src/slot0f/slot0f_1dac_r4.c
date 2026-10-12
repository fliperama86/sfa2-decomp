/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e8598_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern Slot0fRec8508 data_800e8648_slot0f;
extern Slot0fRec8508 data_800e8658_slot0f;
extern Slot0fRec8508 data_800e8678_slot0f;
extern Slot0fRec8508 data_800e8688_slot0f;
extern Slot0fRec8508 data_800e86a8_slot0f;

void queue_slot_marker_and_label_2_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Object *first;

    first = (Object *)&data_800e8608_slot0f;
    if (obj->field_0b == 0) {
        first = (Object *)&data_800e85f8_slot0f;
    }
    add_item_to_table(first);
    add_item_to_table(&data_800e8598_slot0f);
}

void queue_slot_marker_and_mode_message_2_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Slot0fRec8508 *rec;

    if (obj->field_0b == 0) {
        add_item_to_table(&data_800e85f8_slot0f);
    } else {
        add_item_to_table(&data_800e8608_slot0f);
    }
    switch (obj->mode) {
    case 0:
        rec = &data_800e8648_slot0f;
        break;
    case 1:
        rec = &data_800e8658_slot0f;
        break;
    case 2:
        rec = &data_800e8678_slot0f;
        break;
    case 3:
        rec = &data_800e86a8_slot0f;
        break;
    case 4:
        rec = &data_800e8688_slot0f;
        break;
    default:
        return;
    }
    add_item_to_table(rec);
}
