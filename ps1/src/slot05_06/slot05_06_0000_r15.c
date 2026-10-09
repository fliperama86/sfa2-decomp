/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801dd288_slot05_06[];
extern u8 data_801ad398;
extern ObjectFn data_801dd2b4_slot05_06[];
extern ObjectFn data_801dd2e4_slot05_06[];

void func_801c92e0_slot05_06(Object *obj) {
    data_801ad398 = data_801dd288_slot05_06[obj->field_15a](obj);
}

int func_801c9328_slot05_06(Object *obj) {
    obj->field_12a = 2;
    return 1;
}

int func_801c9338_slot05_06(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801c934c_slot05_06(Object *obj) {
    return 1;
}

int func_801c9354_slot05_06(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801c9368_slot05_06(Object *obj) {
    if (*(u32 *)&obj->field_04 == 0x1010101) {
        if (obj->field_45 == 0) {
            if (obj->field_163 == 0) {
                if (*(s16 *)&obj->field_5c >= 0) goto yes;
            }
        }
    }
    return 0;
yes:
    return 1;
}

int func_801c93b8_slot05_06(Object *obj) {
    return obj->field_177 != 0;
}

int func_801c93c4_slot05_06(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

void func_801c93d8_slot05_06(Object *obj) {
    data_801dd2b4_slot05_06[obj->field_15a](obj);
}

void func_801c9418_slot05_06(Object *obj) {
    data_801dd2e4_slot05_06[obj->field_07](obj);
}
