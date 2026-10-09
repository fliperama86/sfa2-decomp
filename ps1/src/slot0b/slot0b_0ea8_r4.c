/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef void (*SlotFn)(Slot0bObj *);

extern SlotFn table_801e4ce0_slot0b[];
extern u8 data_801e7a2c_slot0b[];
void func_801e0ea8_slot0b(Object *obj, u8 *p);

void func_801e125c_slot0b(Object *obj) {
    if (data_80190468.p->field_04 == 0) {
        table_801e4ce0_slot0b[obj->field_05]((Slot0bObj *)obj);
        if (data_80190a40 != 1) {
            func_801e0ea8_slot0b(obj, data_801e7a2c_slot0b + obj->field_03 * 0x50 + data_801a27d0 * 0x28);
        }
    } else {
        obj->field_04 = 2;
    }
}
