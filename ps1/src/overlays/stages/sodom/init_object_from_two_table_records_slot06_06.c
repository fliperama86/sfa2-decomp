/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06_06Rece1d8 data_801ee1d8_slot06_06[][2];
extern Slot06_06Rec83d8 data_801f83d8_slot06_06[][2];

void copy_record_and_set_sequence_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst);

void init_object_from_two_table_records_slot06_06(Object *obj) {
    Slot06_06Rece1d8 *s;
    int x;
    int t;

    obj->state_0 = 1;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    x = obj->table_index;
    /* The local holds x * 2: written at its use six instruction slots differ. */
    t = x * 2;
    /* The index is written in words, as x * 8 + x * 2. With data_801ee1d8_slot06_06[x] this compiler builds a function 4 bytes short. The form is compatible with the original's bytes; what the original source wrote is not known. */
    s = (Slot06_06Rece1d8 *)((s32 *)data_801ee1d8_slot06_06 + (x * 8 + t));
    obj->vel_x = s[0].vel_x;
    copy_record_and_set_sequence_slot06_06(obj, &s[0], &data_801f83d8_slot06_06[obj->table_index][0]);
    copy_record_and_set_sequence_slot06_06(obj, &s[1], &data_801f83d8_slot06_06[obj->table_index][1]);
}
