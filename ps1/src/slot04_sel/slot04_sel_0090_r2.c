/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e694[];
extern u8 data_8016e696;

void func_801b01e4_slot04_sel(Object *obj, Slot04SelRec *rec) {
    rec->field_00 = 0;
    rec->field_03 = obj->kind;
    rec->field_04 = 0;
    rec->field_08 = data_8016e694[obj->side];
    rec->field_09 = 0;
    rec->field_0a = data_8016e696;
    rec->field_0b = 0;
    rec->field_12 = 0;
    rec->field_13 = 0;
    rec->field_14 = 0;
    rec->field_15 = 0;
    rec->field_16 = 0;
    rec->field_17 = 0;
    rec->field_18 = 0;
}
