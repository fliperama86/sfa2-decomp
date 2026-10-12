/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

/* What it does (inferred, not an original name): a per-frame update of a
   character state that has another object (field_40, "other") in view. It
   first runs func_801b2858_slot04_09 (the move). Then, when step_flags is
   negative as a halfword: it sets active_state to 1, vel_y to 0x90000 and
   accel_y to -0x6000, adds 1 to state_3, and sets a velocity (vel_x,
   accel_x): when field_a3 is 0 a fixed one whose sign follows facing_side;
   when field_a3 is not 0 a velocity toward other, vel_x = ((other's
   pos_x - pos_x) >> 4) << 16 and accel_x = 0. It starts sequence 0x33 in
   both cases. When step_flags is not negative: if active_flag is not 0 and
   other's hit_type is not 0xff it sets field_a3 to 0x80 and clears
   active_flag; if other's pos_x - pos_x + 0x24 is below 0x48 it clears
   vel_x and accel_x; then it hands over to update_sequence. */
void func_801b36f8_slot04_09(Object *obj) {
    Object *p;
    int d;

    func_801b2858_slot04_09(obj);
    if ((s16)obj->step_flags & 0x8000) {
        obj->active_state = 1;
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->field_a3 == 0) {
            if (obj->facing_side != 0) {
                obj->vel_x = 0xe0000;
                obj->accel_x = -0x8000;
            } else {
                obj->vel_x = 0xfff20000;
                obj->accel_x = 0x8000;
            }
            func_801307e0(obj, 0x33);
        } else {
            p = obj->other;
            d = p->pos_x - obj->pos_x;
            /* The value is shifted in d before the store: with the expression written in the store this function differs from the original in 5 instruction slots (the load of other then reads through the argument register). */
            d = (d >> 4) << 16;
            obj->vel_x = d;
            obj->accel_x = 0;
            func_801307e0(obj, 0x33);
        }
    } else {
        p = obj->other;
        if (obj->active_flag != 0) {
            if (p->hit_type != 0xff) {
                obj->field_a3 = 0x80;
                obj->active_flag = 0;
            }
            /* The original loads the pointer again here: without this statement this function differs from the original in 10 instruction slots. */
            p = obj->other;
        }
        if (p->pos_x - obj->pos_x + 0x24 < 0x48) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
        update_sequence(obj);
    }
}
