/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern Slot0fRec8508 data_800e8638_slot0f;
extern Slot0fRec8508 data_800e8658_slot0f;
extern Slot0fRec8508 data_800e8668_slot0f;
extern Slot0fRec8508 data_800e8688_slot0f;
extern Slot0fRec8508 data_800e8698_slot0f;
extern ObjectFn data_800e87ec_slot0f[];

void queue_slot_marker_and_mode_message_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Slot0fRec8508 *rec;

    if (obj->field_0b == 0) {
        add_item_to_table(&data_800e85f8_slot0f);
    } else {
        add_item_to_table(&data_800e8608_slot0f);
    }
    switch (obj->mode) {
    case 0:
        rec = &data_800e8638_slot0f;
        break;
    case 1:
        rec = &data_800e8658_slot0f;
        break;
    case 2:
        rec = &data_800e8668_slot0f;
        break;
    case 3:
        rec = &data_800e8698_slot0f;
        break;
    case 4:
        rec = &data_800e8688_slot0f;
        break;
    default:
        return;
    }
    add_item_to_table(rec);
}

void queue_item_and_call_handler_by_index_2_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    add_item_to_table(&data_800e8508_slot0f);
    data_800e87ec_slot0f[(s8)obj->handler_index](o);
}
