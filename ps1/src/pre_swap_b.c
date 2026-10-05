/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984;
extern u16 data_801a6966;


void func_80131854(Object *object) {
    u16 sum = 0;
    u16 i;
    u32 w;
    if (object->field_02 == 0 && data_801a6984 == 0) data_80185ff8 = data_801a6966;
    if (object->field_02 == 1 && data_801a6984 == 0) data_80185ff8 = data_801a6972;
    if (object->field_02 == 0 && data_801a6984 != 0) data_80185ff8 = object->field_c2;
    if (object->field_02 == 1 && data_801a6984 != 0) data_80185ff8 = object->field_c2;
    i = 0;
    w = data_80185ff8;
    table_80185fd8[object->field_02][0] = (w >> 7) & 1;
    table_80185fd8[object->field_02][1] = (w >> 4) & 1;
    table_80185fd8[object->field_02][2] = (w >> 2) & 1;
    table_80185fd8[object->field_02][3] = (w >> 6) & 1;
    table_80185fd8[object->field_02][4] = (w >> 5) & 1;
    table_80185fd8[object->field_02][5] = (w >> 3) & 1;
    table_80185fd8[object->field_02][6] = w & 1;
    table_80185fd8[object->field_02][7] = (w >> 1) & 1;
    do {
        sum += table_80185fd8[object->field_02][i] << table_8016e664[object->field_02][i];
        i++;
    } while (i < 8);
    object->field_130 = sum + (data_80185ff8 & 0xff00);
    object->field_c2 = object->field_130;
    object->field_150 = object->field_130;
}
