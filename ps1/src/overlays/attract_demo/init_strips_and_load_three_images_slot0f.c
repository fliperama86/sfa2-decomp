/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Strip1c data_800f0264_slot0f[2][0x200];
extern int data_800f7268_slot0f;
void update_blink_counter_and_queue_entries_slot0f(int a, int b);

void init_strips_and_load_three_images_slot0f(void) {
    Rect rect;
    u8 *src;
    int i;
    int j;
    Chan *c = &background_layers[1];
    store_all_slot_image_rects_and_queue_jobs();
    c->field_1e = 0x1800;
    c->map_width = 0x400;
    c->map_height = 0x100;
    c->ot_index = 0x1b;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x200; j++) {
            Strip1c *s = &data_800f0264_slot0f[i][j];
            init_draw_mode_prim_3((Tx *)s);
            s->color_r = 0x80;
            s->color_g = 0x80;
            s->color_b = 0x80;
        }
    }
    data_800f7268_slot0f = 1;
    update_blink_counter_and_queue_entries_slot0f(0, 0);
    src = (u8 *)0x800c7000;
    rect.x = 0x180;
    rect.y = 0;
    rect.w = 0x40;
    rect.h = 0x100;
    for (i = 0; i < 3; i++) {
        func_80157fc4(&rect, src);
        rect.x += 0x40;
        src += 0x8000;
    }
}
