/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002ceb0_slot01;
extern ObjectRef data_80055f44_slot01;
extern int data_80055f3c_slot01;
extern u8 data_80015174_slot01[];

void func_80010104_slot01(void) {
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

    s2 = game_state.field_78;
    data_80055f3c_slot01 = game_state.field_40;
    data_801ac6a8[0] = 0;
    data_801aa5ea[0] = 0;
    game_state.field_40 = data_801abf08;
    s1 = s2->field_2ac;
    data_80190568 = 1;
    func_8011eb4c();
    if (data_801abf08 != 0x12) {
        func_801285e0();
    }
    func_8011a744();
    s2->field_2ac = s1;
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
        p = (Object *)func_8011f1e0();
        if (p) {
            p->field_00 = 1;
            p->field_02 = 0x95;
            p->field_03 = 0;
            p->field_48 = s2->field_65;
            p->field_3c = s2;
        }
        data_8002ceb0_slot01 = (Object *)func_8011f1e0();
        if (data_8002ceb0_slot01) {
            data_8002ceb0_slot01->field_00 = 1;
            data_8002ceb0_slot01->field_02 = 0x1b;
            data_8002ceb0_slot01->field_03 = 0;
            data_8002ceb0_slot01->field_48 = s2->field_65;
            data_8002ceb0_slot01->field_3c = s2;
            data_8002ceb0_slot01->field_7a = 0x70;
            data_8002ceb0_slot01->field_7c = 0x1e0;
            data_80055f44_slot01.p = data_8002ceb0_slot01;
        }
        data_8018f5a0->field_4e++;
        game_state.field_c8 = 0x30;
        game_state.field_4f = 0;
        game_state.field_ab = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
        game_state.field_09 = 1;
        game_state.field_2c = 0xff;
        game_state.field_65 = 1;
        func_80151020(0x605);
        func_80125dc0(0, 0x20, 0xd, 0);
        func_80125dc0(1, 0x20, 0xd, 0);
        func_80125dc0(2, 0x20, 0xd, 0);
        func_80125dc0(3, 0x20, 0xd, 0);
        func_80137220(0, 6);
        func_80137220(1, 0);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}
