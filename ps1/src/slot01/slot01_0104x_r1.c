/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002ceb0_slot01;
extern ObjectRef data_80055f44_slot01;
extern int data_80055f3c_slot01;
extern u8 data_80015174_slot01[];

void init_slot01_screen_and_spawn_objects_slot01(void) {
    u16 pal[80] = {
        0x0000, 0x5840, 0x6040, 0x68c0, 0x7900, 0x7980, 0x79c0, 0x7a04, 0x7a46, 0x7a8a, 0x7acc, 0x7b10, 0x7b54, 0x7b96, 0x7bda, 0x7bde,
        0x0000, 0x2816, 0x3058, 0x3098, 0x38da, 0x411c, 0x495e, 0x51de, 0x5a1e, 0x625e, 0x6a9e, 0x72de, 0x731e, 0x735e, 0x7b9e, 0x7bde,
        0x0000, 0x1980, 0x19c2, 0x2202, 0x2244, 0x2a86, 0x32c8, 0x330a, 0x3b4c, 0x438e, 0x4bd0, 0x63d4, 0x6bd6, 0x73d8, 0x7bda, 0x7bde,
        0x0000, 0x015a, 0x019c, 0x01dc, 0x021e, 0x025e, 0x029e, 0x1ade, 0x231e, 0x2b5e, 0x339e, 0x3bde, 0x53de, 0x63de, 0x6bde, 0x7bde,
        0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000};
    Rect rect;
    Object *s2;
    int s1;
    Object *p;

    s2 = game_state.focus_object;
    data_80055f3c_slot01 = game_state.kind_index;
    data_801ac6a8[0] = 0;
    data_801aa5ea[0] = 0;
    game_state.kind_index = data_801abf08;
    s1 = s2->kind_copy;
    data_80190568 = 1;
    init_object_pools();
    if (data_801abf08 != 0x12) {
        func_801285e0();
    }
    init_sprite_tables();
    s2->kind_copy = s1;
    rect.x = 0x70;
    rect.y = 0x1f0;
    rect.w = 0x10;
    rect.h = 2;
    func_80157fc4(&rect, data_80015174_slot01);
    func_80157d9c(0);
    if (data_80197f10 >= 2) {
        rect.x = 0x70;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0xb;
        func_80157fc4(&rect, (u8 *)0x80070d48);
        func_80157d9c(0);
        rect.x = 0x40;
        rect.y = 0x1f0;
        rect.w = 0x10;
        rect.h = 5;
        func_80157fc4(&rect, (u8 *)pal);
        func_80157d9c(0);
        p = (Object *)pop_block_from_pool();
        if (p) {
            p->active_count = 1;
            p->field_02 = 0x95;
            p->table_index = 0;
            p->variant_index = s2->team_id;
            p->parent = s2;
        }
        data_8002ceb0_slot01 = (Object *)pop_block_from_pool();
        if (data_8002ceb0_slot01) {
            data_8002ceb0_slot01->active_count = 1;
            data_8002ceb0_slot01->field_02 = 0x1b;
            data_8002ceb0_slot01->table_index = 0;
            data_8002ceb0_slot01->variant_index = s2->team_id;
            data_8002ceb0_slot01->parent = s2;
            data_8002ceb0_slot01->spawn_x = 0x70;
            data_8002ceb0_slot01->spawn_y = 0x1e0;
            data_80055f44_slot01.p = data_8002ceb0_slot01;
        }
        data_8018f5a0->substep_index++;
        game_state.timer_or_bits = 0x30;
        game_state.field_4f = 0;
        game_state.status_flags = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
        game_state.field_09 = 1;
        game_state.state_2c_flag = 0xff;
        game_state.field_65 = 1;
        func_80151020(0x605);
        func_80125dc0(0, 0x20, 0xd, 0);
        func_80125dc0(1, 0x20, 0xd, 0);
        func_80125dc0(2, 0x20, 0xd, 0);
        func_80125dc0(3, 0x20, 0xd, 0);
        queue_image_upload_job(0, 6);
        queue_image_upload_job(1, 0);
        queue_image_upload_job(2, 1);
        queue_image_upload_job(3, 2);
    }
}
