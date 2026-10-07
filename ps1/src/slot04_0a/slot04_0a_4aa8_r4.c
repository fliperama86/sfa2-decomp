/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The two locals are read without having been set: the original stores two saved registers that it never assigns. */
void func_801b4e38_slot04_0a(Object *obj) {
    Object *p[3];
    u8 a;
    int b;
    Object *t;

    if (data_80197f10 >= 3) {
        func_801460ec((u32 *)p);
        t = p[0];
        t->field_00 = 1;
        t->field_02 = 0x15;
        t->field_03 = 0;
        t->field_0e = obj->field_0e;
        t->field_48 = a;
        t->field_4c = b;
        t->field_46 = 8;
        t->field_20 = 8;
        t->field_3c = obj;
        t->field_0d = obj->field_0d + 3;
        t->field_44 = 1;
        t->field_50 = 0x1000800;
        t->field_7a = obj->field_7a;
        t->field_7c = obj->field_7c;
        t->field_08 = 0x20;
        t->field_66 = obj->field_66;
        t = p[1];
        t->field_00 = 1;
        t->field_02 = 0x15;
        t->field_03 = 1;
        t->field_0e = obj->field_0e;
        t->field_48 = a;
        t->field_4c = b;
        t->field_46 = 0x10;
        t->field_20 = 0x10;
        t->field_3c = obj;
        t->field_0d = obj->field_0d + 4;
        t->field_0d = obj->field_0d;
        t->field_44 = 1;
        t->field_50 = 0x1000800;
        t->field_7a = obj->field_7a;
        t->field_7c = obj->field_7c;
        t->field_08 = 0x20;
        t->field_66 = obj->field_66;
        t = p[2];
        t->field_00 = 1;
        t->field_02 = 0x15;
        t->field_03 = 2;
        t->field_0e = obj->field_0e;
        t->field_48 = a;
        t->field_4c = b;
        t->field_46 = 0x18;
        t->field_20 = 0x18;
        t->field_3c = obj;
        t->field_0d = obj->field_0d + 4;
        t->field_0d = obj->field_0d;
        t->field_44 = 1;
        t->field_50 = 0x1000800;
        t->field_7a = obj->field_7a;
        t->field_7c = obj->field_7c;
        t->field_08 = 0x20;
        t->field_66 = obj->field_66;
    }
}
