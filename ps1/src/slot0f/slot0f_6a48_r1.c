/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_800f7c94_slot0f[4];
extern s16 *data_800eff60_slot0f[];
extern u16 data_800eff70_slot0f[];
extern u8 data_800f7cd4_slot0f[];
void func_800e6aac_slot0f(Object *obj, HudSlot *c, s16 i);
void func_800e826c_slot0f(u8 *buf, unsigned int n, int pos, int flags);
void func_800e6b8c_slot0f(char *src, char *dst);

void func_800e6a48_slot0f(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        func_800e6aac_slot0f(obj, &data_800f7c94_slot0f[i], i);
    }
}

void func_800e6aac_slot0f(Object *o, HudSlot *c, s16 i) {
    Slot0fObj *obj = (Slot0fObj *)o;
    char buf[0x10];
    char *dst;
    s16 *src = data_800eff60_slot0f[i];

    c->field_00 = 0;
    c->field_08 = 8;
    c->field_09 = 8;
    c->field_0a = 0;
    c->field_0b = 0x18;
    func_800e826c_slot0f((u8 *)buf, *src, 2, 1);
    dst = (char *)data_800f7cd4_slot0f + i * 8;
    func_800e6b8c_slot0f(buf, dst);
    c->field_0c = (int)dst;
    c->field_04 = (u16)obj->pos_x + data_800eff70_slot0f[i];
    c->field_06 = obj->field_16 + 8;
}
