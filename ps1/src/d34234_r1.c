/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern u8 data_8019016c;
extern s16 data_80188d04;
extern u8 data_8019808e;
extern u8 data_80198422;
extern u32 data_801ac86c;
extern u32 data_801ac874;
extern u32 data_801ac87c;
extern u32 data_801ac880;
extern u32 data_801ac884;
void call_kind_table_for_two_effects(void);
void call_function_at_data_pointer(void);

/*
 * What it does (inferred, not original names): queues the pieces of a
 * screen for drawing. It clears data_80188d04 and returns at once when
 * data_8019016c is not 0, data_801a6938 is 0, the first byte of
 * data_801ac6a8 is 0 or data_8018db10 is not 0. Otherwise it hands
 * records of data_801ac6a8, of the tables and of the strips (all picked by
 * the buffer selector sel) to func_8015bf34 together with three pointers
 * into the block at data_801987c8, calls func_801347b4 seven times with
 * the words data_801ac86c to data_801ac884, and ends with queue_player_status_text_items.
 */
void add_selector_hud_prims_and_strips(void) {
    /* Only the low 16 bits of the selector count, as a signed number. */
    s16 sel = data_801a27d0;
    /* data_801987c8 is only added to here, never read through. */
    int part20 = (int)data_801987c8 + 0x20;
    int part38 = (int)data_801987c8 + 0x38;
    int part3c = (int)data_801987c8 + 0x3c;
    u8 *base = data_801ac6a8;
    Strip1c *p1;
    int i;

    data_80188d04 = 0;
    if (data_8019016c != 0)
        return;
    if (data_801a6938 == 0)
        return;
    if (base[0] == 0)
        return;
    if (data_8018db10 != 0)
        return;

    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0xc4]);
    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0x124]);
    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0xe4]);
    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0x144]);
    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0x104]);
    func_8015bf34(part20, (Cmd *)&base[sel * 16 + 0x164]);
    func_8015bf34(part38, (Cmd *)&base[sel * 16 + 0x4]);
    func_8015bf34(part38, (Cmd *)&base[sel * 16 + 0x24]);
    /* i = 0 stands here, before the calls; a for loop or a later
       i = 0 changes the code (see the tail loops). */
    i = 0;
    func_801347b4(0, data_801ac86c, 0xea, 0x20, 0xe, 0xd);
    func_801347b4(1, data_801ac874, 0x1a, 0x20, 0xe, 0xd);
    func_801347b4(2, data_801ac870, 0xd8, 0xe2, 8, 0);
    func_801347b4(3, data_801ac878, 0x18, 0xe2, 8, 0);
    func_801347b4(4, data_801ac87c, 0x88, -0x11, 0x11, 0x10);
    func_801347b4(5, data_801ac880, 0xd9, -0x11, 0x11, 0xb);
    func_801347b4(6, data_801ac884, 0x89, 1, 0x11, 0x19);
    func_8015bf34(part3c, (Cmd *)&base[sel * 16 + 0x44]);
    func_8015bf34(part3c, (Cmd *)&base[sel * 16 + 0x64]);

    call_kind_table_for_two_effects();
    call_function_at_data_pointer();
    func_8015bf34(part38, (Cmd *)&data_80188d6c[sel]);
    func_8015bf34(part38, (Cmd *)data_80188d6c[sel].unknown_1c);
    p1 = strips[sel];
    while (i < data_8019808e) {
        func_8015bf34(part38, (Cmd *)p1);
        i++;
        p1++;
    }
    p1 = data_80190014[sel];
    i = 0;
    while (i < data_80198422) {
        func_8015bf34(part38, (Cmd *)p1);
        i++;
        p1++;
    }
    if (data_80198098 != 0) {
        func_8015bf34(part38, (Cmd *)strips2[sel]);
        func_8015bf34(part38, (Cmd *)&strips2[sel][1]);
    }
    if (data_8019842c != 0) {
        func_8015bf34(part38, (Cmd *)data_80188e4c[sel]);
        func_8015bf34(part38, (Cmd *)&data_80188e4c[sel][1]);
    }
    queue_player_status_text_items();
}
