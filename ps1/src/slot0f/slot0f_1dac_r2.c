/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e8568_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;

extern ObjectFn data_800e87dc_slot0f[];
void queue_menu_items_with_enabled_highlight_slot0f(Object *o);
void queue_two_choice_menu_items_slot0f(Object *o);

void queue_item_and_call_handler_by_index_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    add_item_to_table(&data_800e8508_slot0f);
    data_800e87dc_slot0f[(s8)obj->handler_index](o);
}

void call_queue_menu_items_with_enabled_highlight_slot0f(Object *object) {
    queue_menu_items_with_enabled_highlight_slot0f(object);
}

void call_queue_two_choice_menu_items_slot0f(Object *object) {
    queue_two_choice_menu_items_slot0f(object);
}

void queue_slot_marker_and_label_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Object *first;

    first = (Object *)&data_800e8608_slot0f;
    if (obj->field_0b == 0) {
        first = (Object *)&data_800e85f8_slot0f;
    }
    add_item_to_table(first);
    add_item_to_table(&data_800e8568_slot0f);
}
