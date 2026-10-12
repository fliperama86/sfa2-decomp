/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_800517b8_slot28[];
extern u32 data_800517d8_slot28[];
extern Slot28Rec51e94 data_80051e94_slot28;
void init_four_textured_screen_quads_slot28(Object *obj);
void link_two_quads_into_ordering_table_slot28(Object *obj);
void func_80027f78_slot28(Object *obj);

void load_image_by_index_and_start_countdown_slot28(Object *obj) {
    HudState *h;
    load_image_from_stream(data_800517b8_slot28[(s8)data_8016e685]);
    h = data_8018f5a0;
    h->countdown_timer = 0x12c;
    h->step_index_2 = h->step_index_2 + 1;
    init_four_textured_screen_quads_slot28(obj);
    start_fade_mode_c01_and_wait();
}

void func_80027f0c_slot28(Object *obj) {
    HudState *h;
    int t;
    func_80027f78_slot28(obj);
    link_two_quads_into_ordering_table_slot28(obj);
    h = data_8018f5a0;
    t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->substep_index = h->substep_index + 1;
    }
}

void func_80027f78_slot28(Object *obj) {
    int t = (s8)data_8016e685;
    int idx;
    if (t >= 5) {
        idx = (t - 5) * 2;
        if (*(u8 *)&obj->field_1c == 0) {
            idx += 1;
        }
        data_80051e94_slot28.field_00 = 0;
        data_80051e94_slot28.init_size_a = 0x10;
        data_80051e94_slot28.init_size_b = 0x10;
        data_80051e94_slot28.init_param = 2;
        data_80051e94_slot28.entry_byte = 0x12;
        data_80051e94_slot28.pos_x = 0x60;
        data_80051e94_slot28.pos_y = 0x20;
        data_80051e94_slot28.data_word = data_800517d8_slot28[idx];
        add_item_to_table(&data_80051e94_slot28);
    }
}
