/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80157380(Object *obj) {
    Object *p[3];
    /* s3 gets no value in this function: the three stores of it below write what the register holds on entry, as the original's instructions do. Nothing is claimed here about what that is. func_80142c70 has the same local. */
    s16 s3;

    if (data_80197f10 >= 3) {
        if (obj->field_49 == 0) {
            func_801460ec((u32 *)p);
            p[0]->field_00 = 1;
            p[0]->field_02 = 0xb;
            p[0]->field_03 = 1;
            p[0]->field_0e = obj->field_0e;
            p[0]->field_48 = obj->side;
            p[0]->field_46 = 5;
            p[0]->field_20 = 5;
            p[0]->vel_x = s3;
            p[0]->field_0d = obj->field_0d;
            p[0]->field_3c = obj;
            p[0]->field_81 = 5;
            p[0]->field_7a = 0x60;
            p[0]->field_7c = 0x1e0;
            p[1]->field_00 = 1;
            p[1]->field_02 = 0xb;
            p[1]->field_03 = 2;
            p[1]->field_0e = obj->field_0e;
            p[1]->field_48 = obj->side;
            p[1]->field_46 = 10;
            p[1]->field_20 = 10;
            p[1]->vel_x = s3;
            p[1]->field_0d = obj->field_0d + 3;
            p[1]->field_3c = obj;
            p[1]->field_81 = 5;
            p[1]->field_7a = 0x60;
            p[1]->field_7c = 0x1e0;
            p[2]->field_00 = 1;
            p[2]->field_02 = 0xb;
            p[2]->field_03 = 3;
            p[2]->field_0e = obj->field_0e;
            p[2]->field_48 = obj->side;
            p[2]->field_46 = 0xe;
            p[2]->field_20 = 0xe;
            p[2]->vel_x = s3;
            p[2]->field_0d = obj->field_0d + 4;
            p[2]->field_3c = obj;
            p[2]->field_81 = 5;
            p[2]->field_7a = 0x60;
            p[2]->field_7c = 0x1e0;
        }
    }
}
