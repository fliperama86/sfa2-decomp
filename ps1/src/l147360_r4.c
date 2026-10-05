/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Object *func_8011f1e0(void);

void func_80148b60(Object *object) {
    int side;
    u16 i;
    Object *o;
    if ((s16)object->field_5c >= 0) {
        if ((func_80151184() & 0xff) == 0x25) side = 2;
        else side = func_80151184() & 1;
        i = 0;
        do {
            o = func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 9;
                o->field_03 = side;
                o->field_0e = object->field_0e;
                o->field_46 = object->kind;
                o->field_3c = object;
                o->field_48 = data_8017ceb4[i];
                o->field_45 = data_8017ceb8[i];
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->field_98 = &data_80172a48;
                o->field_90 = (void *)0x800fb100;
                o->field_9c = &data_80173c9c;
            }
            i++;
        } while (i < 3);
        object->field_29e = 1;
        func_80120554(object, object->side, 0x322);
    }
}
