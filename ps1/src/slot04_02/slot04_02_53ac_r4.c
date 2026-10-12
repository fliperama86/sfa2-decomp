/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b591c_slot04_02(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        ((Slot04aObj *)obj)->field_27d = 0;
        obj->hit_count_3 = 0;
        obj->countdown = obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = *(u8 *)&obj->meter;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b59ac_slot04_02(Object *obj) {
    Object *p;
    s16 t;
    u16 u;
    obj->hit_count_3 = obj->hit_count_3 + 1;
    update_sequence(obj);
    u = obj->countdown - 1;
    obj->countdown = u;
    if ((u8)u == 0) {
        if (obj->active_state != 0) {
            func_801b631c_slot04_02(obj, 1, 0, 3, 1);
        } else {
            p = obj->parent;
            if (p->field_02 == 9) {
                p->state_0 = 2;
                p->state_1 = 0;
                p->state_2 = 0;
                p->state_3 = 0;
            }
        }
        func_80142c70(obj);
    }
    if ((obj->pressed_bits & 0xff00) != 0) {
        if (((Slot04aObj *)obj)->field_27d == 0) {
            ((Slot04aObj *)obj)->field_27d = obj->hit_count_3;
        }
    } else {
        t = obj->countdown;
        if ((t & 0xff00) != 0) {
            obj->countdown = t - 0x100;
        }
        func_80142fe8(obj);
    }
}
