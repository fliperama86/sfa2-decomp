/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0bFrame data_801e4c30_slot0b[];
void func_801e1598_slot0b(Slot0bObj *obj, int arg);
void call_step_on_anim_member_slot0b(Slot0bObj *obj);

void start_step_motion_on_anim_end_slot0b(Slot0bObj *obj) {
    if (data_80190468.p->phase_state == 1 || obj->anim.loop < 0) {
        obj->countdown_timer = 0xf;
        obj->fx_step = 0x180000;
        obj->fy_step = 0xfff40000;
        obj->fy_step_accel = 0xc000;
        obj->anim.cur = data_801e4c30_slot0b;
        obj->anim.timer = 1;
        obj->fx_step_accel = 0xfffe8000;
        obj->step_count++;
        obj->anim.loop = obj->anim.cur->loop;
        if (obj->entry_index != 0) {
            obj->fx_step = -obj->fx_step;
            obj->fx_step_accel = -obj->fx_step_accel;
            obj->fy_step = -obj->fy_step;
            obj->fy_step_accel = -obj->fy_step_accel;
        }
        func_801e1598_slot0b(obj, obj->entry_index + 2);
    }
    call_step_on_anim_member_slot0b(obj);
}
