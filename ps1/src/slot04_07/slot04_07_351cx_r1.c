/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b351c_slot04_07(Object *obj) {
    int t;
    int u;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        t = (u8)obj->countdown;
        u = obj->state_3;
        obj->hit_count_3 = 0;
        ((Slot04aObj *)obj)->field_27d = 0;
        u++;
        obj->state_3 = u;
        obj->countdown = t | 0x1400;
        t = ((Slot04aObj *)obj)->base_offset + 0x1e;
        if (obj->field_4b != 0) {
            t = 0x48;
        }
        obj->cursor_timer_a = t;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}
