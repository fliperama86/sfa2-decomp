/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801b4a7c_slot04_06(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->field_3b & 0x80) {
        game_state_second.field_4b = game_state.field_4b | (1 << obj->side);
    }
    if (((Slot04aObj *)obj)->field_3a != 0) {
        ((Slot04aObj *)obj)->field_3a = 0;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_03 = 6;
            p->field_3c = obj;
            p->field_1c = obj->field_1c;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_0d = obj->field_0d;
            p->field_02 = 0x11;
            p->field_08 = 0x20;
            p->field_90 = obj->field_90;
            p->field_66 = obj->field_66;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
    func_80130efc(obj);
}
