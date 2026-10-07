/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e698[];

void func_801b23a8_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    rec->field_00 = 0;
    rec->field_03 = obj->kind;
    rec->field_04 = 0;
    rec->field_06 = data_8016e698[obj->side];
    rec->field_07 = 0;
    rec->field_08 = data_8016e698[obj->side + 2];
    rec->field_09 = 0;
    rec->field_0a = data_8016e698[4];
    rec->field_0b = 0;
    rec->field_0c = data_8016e698[5];
    rec->field_0d = 0;
    rec->field_0e = 0;
    rec->field_0f = 0;
    rec->field_10 = 0;
    rec->field_11 = 0;
    rec->field_12 = 0;
    rec->field_13 = 0;
    rec->field_14 = 0;
}
