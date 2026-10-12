/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005198c_slot28;
void scroll_two_objects_x_with_wrap_slot28(Object *obj);

void func_80018dec_slot28(Object *obj) {
    scroll_two_objects_x_with_wrap_slot28(obj);
    if ((s16)data_8005198c_slot28.p->step_flags < 0) {
        data_8018f5a0->pass_index += 1;
        set_menu_command_by_code(1, 0x602);
        start_fade_mode_1801_and_wait();
    }
}
