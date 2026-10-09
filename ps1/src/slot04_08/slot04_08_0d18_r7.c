/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c3e94_slot04_08;

void func_801b151c_slot04_08(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->field_07++;
        obj->field_46 = 0;
        func_80120554(obj, obj->side, 0x324);
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 8;
            p->field_03 = 1;
            p->field_3c = obj;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y - 0x6c;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_0d = obj->field_0d;
            p->field_08 = 0x20;
            p->field_66 = obj->field_66;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 8;
                p->field_03 = 0;
                p->field_3c = obj;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                p->field_7a = obj->field_7a;
                data_801c3e94_slot04_08 = p;
                p->field_7c = obj->field_7c;
                p->field_0d = obj->field_0d;
                p->field_08 = 0x20;
                p->field_66 = obj->field_66;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
            }
        }
    }
    func_80130efc(obj);
}
