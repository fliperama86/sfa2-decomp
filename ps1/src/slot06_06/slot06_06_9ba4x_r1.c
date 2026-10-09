/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06_06Rece1d8 data_801ee1d8_slot06_06[][2];
extern Slot06_06Rec83d8 data_801f83d8_slot06_06[][2];

void func_801e9c58_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst);

void func_801e9ba4_slot06_06(Object *obj) {
    Slot06_06Rece1d8 *s;
    int x;
    int t;

    obj->field_04 = 1;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    x = obj->field_03;
    /* The local holds x * 2: written at its use six instruction slots differ. */
    t = x * 2;
    /* The index is written in words, as x * 8 + x * 2. With data_801ee1d8_slot06_06[x] this compiler builds a function 4 bytes short. The form is compatible with the original's bytes; what the original source wrote is not known. */
    s = (Slot06_06Rece1d8 *)((s32 *)data_801ee1d8_slot06_06 + (x * 8 + t));
    obj->field_4c = s[0].field_10;
    func_801e9c58_slot06_06(obj, &s[0], &data_801f83d8_slot06_06[obj->field_03][0]);
    func_801e9c58_slot06_06(obj, &s[1], &data_801f83d8_slot06_06[obj->field_03][1]);
}
