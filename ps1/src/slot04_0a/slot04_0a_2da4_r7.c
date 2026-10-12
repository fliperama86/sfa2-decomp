/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void store_image_rows_for_object_slot04_0a(Object *obj);

void func_801b3528_slot04_0a(Object *obj) {
    Object *o;
    int dx;
    int dy;

    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        o = obj->other;
        dx = obj->pos_x - o->pos_x;
        if (dx < 0) {
            dx = -dx;
        }
        dy = obj->pos_y - o->pos_y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dx >= 0xa1 || dy >= 0x61 || obj->field_241 != 0) {
            store_image_rows_for_object_slot04_0a(obj);
            obj->state_3 = 3;
            obj->other_lock = 8;
            set_sequence_by_side_and_mode(obj, 0x4a);
        } else {
            obj->other_lock = 0xf;
            obj->state_3++;
            play_side_sound_at(obj, obj->side, 0xf);
            set_sequence_by_side_and_mode(obj, 0x49);
        }
    }
}
