/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_800f7c94_slot0f[4];
extern s16 *data_800eff60_slot0f[];
extern u16 data_800eff70_slot0f[];
extern u8 data_800f7cd4_slot0f[];
void init_hud_slot_two_digit_text_slot0f(Object *obj, HudSlot *c, s16 i);
void format_decimal_padded_slot0f(u8 *buf, unsigned int n, int pos, int flags);
void copy_string_with_prefix_and_terminator_slot0f(char *src, char *dst);

void func_800e6a48_slot0f(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        init_hud_slot_two_digit_text_slot0f(obj, &data_800f7c94_slot0f[i], i);
    }
}

void init_hud_slot_two_digit_text_slot0f(Object *o, HudSlot *c, s16 i) {
    Slot0fObj *obj = (Slot0fObj *)o;
    char buf[0x10];
    char *dst;
    s16 *src = data_800eff60_slot0f[i];

    c->slot_type = 0;
    c->init_size_a = 8;
    c->init_size_b = 8;
    c->field_0a = 0;
    c->init_const_byte = 0x18;
    format_decimal_padded_slot0f((u8 *)buf, *src, 2, 1);
    dst = (char *)data_800f7cd4_slot0f + i * 8;
    copy_string_with_prefix_and_terminator_slot0f(buf, dst);
    c->data_ptr = (int)dst;
    c->pos_x = (u16)obj->pos_x + data_800eff70_slot0f[i];
    c->pos_y = obj->base_y + 8;
}
