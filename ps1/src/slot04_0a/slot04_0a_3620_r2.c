/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f58_slot04_0a(Object *object);

void func_801b37fc_slot04_0a(Object *obj) {
    int a = 0;
    int b;
    if ((s16)obj->step_flags & 0xff00) {
        b = 0x59;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -0x1d;
            b = 0x56;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3f58_slot04_0a(obj);
    }
    update_sequence(obj);
}

void func_801b3894_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 a = 0x48;
    o->countdown = (s16)o->countdown - 0x100;
    if ((o->countdown & 0xff00) == 0) {
        o->state_3++;
        o->hit_count_3 = 0;
        obj->field_27d = 0;
        o->countdown = (u8)o->countdown | 0x1400;
        if (o->field_4b == 0) {
            a = obj->base_offset + 0x1e;
        }
        o->cursor_timer_a = a;
        o->field_180 = 0;
    }
    func_80142fe8(o);
    update_sequence(o);
}
