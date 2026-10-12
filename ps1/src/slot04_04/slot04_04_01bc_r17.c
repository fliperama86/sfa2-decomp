/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4210_slot04_04[];
void func_801b17c4_slot04_04(Object *obj);
void func_801b1a54_slot04_04(Object *obj);

void func_801b16d0_slot04_04(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->input_matched_flag = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index;
    func_801307e0(obj, a);
}

void func_801b1744_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->countdown = 0xf;
        obj->state_3++;
        if (obj->mode_flag_2 == 0) {
            func_801b17c4_slot04_04(obj);
        } else {
            obj->countdown = data_801c4210_slot04_04[obj->input_dir_second];
            func_801b1a54_slot04_04(obj);
        }
    }
}
