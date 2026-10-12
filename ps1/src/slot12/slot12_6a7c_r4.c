/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 *data_80028a18_slot12[];
extern u16 data_80028a28_slot12[];
extern u8 data_8002c508_slot12[];
void func_800161ec_slot12(u8 *buf, unsigned int n, int pos, int flags);
void func_80016d98_slot12(char *src, char *dst);

void func_80016cb4_slot12(Object *o, HudSlot *c, s16 i) {
    Slot12Obj *obj = (Slot12Obj *)o;
    char buf[0x10];
    char *dst;
    s16 *src = data_80028a18_slot12[i];

    c->slot_type = 0xe;
    c->init_size_a = 8;
    c->init_size_b = 8;
    c->field_0a = 0;
    c->init_const_byte = 0x18;
    func_800161ec_slot12((u8 *)buf, *src, 2, 1);
    dst = (char *)data_8002c508_slot12 + i * 8;
    func_80016d98_slot12(buf, dst);
    c->data_ptr = (int)dst;
    c->pos_x = obj->base_x + data_80028a28_slot12[i];
    c->pos_y = obj->base_y + 8;
}

void func_80016d98_slot12(char *src, char *dst) {
    if (*src != 0) {
        *dst++ = 0x78;
        do {
            *dst++ = *src++;
        } while (*src != 0);
    }
    *dst++ = 0x40;
    *dst = 0x40;
    dst[1] = 0;
}
