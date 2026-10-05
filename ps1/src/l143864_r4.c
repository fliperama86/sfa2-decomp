/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801461c4(Object *object) {
    u8 facing;
    int hi;

    object->field_01 = 1;
    object->field_44 = 1;
    object->field_98 = &data_80172a48;
    object->field_04 = object->field_04 + 1;
    object->field_90 = (void *)0x800fb100;
    object->field_09 = 0;
    object->field_9c = &data_80173c9c;
    object->pos_x = object->pos_x + table_8017c9f0[func_80151184() & 0xf];
    object->pos_y = object->pos_y - table_8017c9f0[func_80151184() & 0xf];
    func_80146450(object);
    object->field_0b = object->field_3c->field_0b;
    facing = object->field_03;
    if ((facing & 0x80) == 0) {
        hi = object->field_48 != 0;
        func_80130768(object, table_8017ca10[(hi << 3) + (facing & 7)], table_8017c7f8);
    } else {
        object->field_09 = 8;
        func_80146394(object);
        func_80130768(object, table_8017ca20[facing & 7], table_8017c7f8);
    }
}
