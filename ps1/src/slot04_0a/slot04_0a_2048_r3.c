/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2158_slot04_0a(Object *obj) {
    Object *n;

    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801204f4(obj, obj->side, 0xa);
        func_801204f4(obj, obj->side, 0xe);
        n = func_8011f0e8(obj);
        if (n != 0) {
            n->field_00 = 1;
            n->field_02 = 0xa;
            n->field_66 = obj->field_66;
            n->field_65 = obj->field_65;
            n->field_ac = obj->field_12a;
            n->field_ad = 0;
            n->field_0e = obj->field_0e;
            n->field_0b = obj->field_0b;
            n->field_0c = obj->field_0c;
            n->field_0d = obj->field_0d;
            n->field_26 = obj->field_26;
            *(u16 *)&n->pos_x = *(u16 *)&obj->pos_x;
            *(u16 *)&n->pos_y = *(u16 *)&obj->pos_y;
            n->field_7a = 0x60;
            n->field_5c = 0;
            n->field_3c = obj;
            n->field_7c = 0x1e0;
            n->field_90 = obj->field_90;
            n->field_98 = obj->field_98;
            n->field_9c = obj->field_9c;
            obj->field_14c = (s32)n;
            obj->field_240++;
        }
    }
    func_80130efc(obj);
}
