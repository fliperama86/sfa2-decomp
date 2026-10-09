/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ff0_slot04_0a(Object *obj);
void func_801b4ff8_slot04_0a(Object *obj);
void func_80120028(Object *o);

void func_801b4d2c_slot04_0a(Object *obj) {
    obj->field_09 = 4;
    obj->field_81 = 5;
    obj->field_0c = 0xff;
    obj->field_04++;
    obj->field_1c = obj->field_3c->field_1c;
}

void func_801b4d60_slot04_0a(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_46 = 0;
        if (obj->field_50 != ((obj->field_3c->field_04 << 24) | (obj->field_3c->field_05 << 16) | (obj->field_3c->field_06 << 8))) {
            obj->field_04 = 2;
        } else {
            if (obj->field_3a & 0x100) {
                func_801b4ff0_slot04_0a(obj);
            } else if (obj->field_3a & 0x200) {
                func_801b4ff8_slot04_0a(obj);
            }
            func_80120028(obj);
        }
    }
}
