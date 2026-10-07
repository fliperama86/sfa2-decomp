/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b08e0_slot04_07(Object *obj) {
    s16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    } else if ((t & 0xff) == 0) {
        /* The listing stores pos_x to itself. Written as the facing-dependent offset with zero in both arms; inferred from the allocation, not known from the original. */
        if (obj->field_0b == 0) {
            obj->pos_x -= 0;
        } else {
            obj->pos_x += 0;
        }
        obj->field_3a = obj->field_3a & 0xff00;
    }
}
