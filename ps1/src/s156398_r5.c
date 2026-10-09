/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011a55c(Object *o);

void func_80157174(Object *dest, Object *object) {
    int i;

    if (object->field_120 == 1) {
        if (object->side == 0) {
            dest->field_21e = *(u16 *)&dest->field_220;
            for (i = 0; i < 0x40; i++) {
                ((u8 *)dest + i)[0x15e] = ((u8 *)dest + i)[0x19e];
            }
        } else {
            dest->field_21e = dest->field_222;
            for (i = 0; i < 0x40; i++) {
                ((u8 *)dest + i)[0x15e] = ((u8 *)dest + i)[0x1de];
            }
        }
    }
}

void func_801571f4(Block172 *block) {
    Object *object = (Object *)block;
    table_8018191c[object->field_04](object);
}

void func_80157234(Object *object) {
    object->field_09 = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->field_04++;
    func_8011a55c(object);
}
