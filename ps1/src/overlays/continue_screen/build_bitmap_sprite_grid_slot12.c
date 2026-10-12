/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 *data_8002b62c_slot12;
extern u16 *data_8002b624_slot12;
extern u16 data_800230a0_slot12[];
extern u32 data_80022eb0_slot12[];
void begin_primitive_buffer_with_draw_mode_slot12(void);
void add_sprite_or_finish_sprite_buffer_slot12(Object *obj, u8 a, s16 x, s16 y);

void build_bitmap_sprite_grid_slot12(Object *obj, int a) {
    Object *o = game_state.object;
    u16 fp;
    s16 y;
    int x0;
    int x;
    u16 code;
    int m;
    s8 i;
    s8 j;

    data_8002b62c_slot12 += 8;

    if ((s16)o->field_b2 == 9) {
        a = 0;
    }
    a &= 0x3f;
    fp = data_800230a0_slot12[a];
    a = (s16)o->countdown_timer;
    if ((s16)o->field_b2 == 9) {
        if (a >= 0x20) {
            a = 0x20;
        }
    }
    a &= 0x7c;
    data_8002b624_slot12 = (u16 *)&data_80022eb0_slot12[a];
    x0 = -0x38;
    begin_primitive_buffer_with_draw_mode_slot12();
    y = (s16)fp * 3;
    for (i = 7; i >= 0; i--) {
        x = x0;
        a = *--data_8002b62c_slot12;
        code = *--data_8002b624_slot12;
        for (j = 7; j >= 0; j--) {
            m = 1 << j;
            if (a & m) {
                add_sprite_or_finish_sprite_buffer_slot12(obj, code & 0xff, x, y);
                obj->health++;
            }
            x += 0xc;
        }
        y -= fp;
        x0 += 2;
    }
    add_sprite_or_finish_sprite_buffer_slot12(obj, 0xff, 0, 0);
}
