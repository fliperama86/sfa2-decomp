/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e7a2c_slot0b[];
extern SequenceStep *data_801e4cc0_slot0b[];
int func_8015bdd4(int a, int b);
SlotCell *func_801e101c_slot0b(Object *obj, SlotCell *cell, int arg, int base);
void func_801e1624_slot0b(Slot0bObj *obj);
void func_801e1644_slot0b(Slot0bCursor *c);
void func_801e169c_slot0b(Slot0bCursor *c, Slot0bFrame *f);
void func_801e16d0_slot0b(Slot0bCursor *c);

void func_801e1598_slot0b(Slot0bObj *obj, int arg) {
    u8 index = arg;
    int a;
    func_80130768((Object *)obj, index, data_801e4cc0_slot0b);
    a = func_8015bd0c(0, 0, 0x300, 0) & 0xffff;
    func_801e101c_slot0b((Object *)obj, (SlotCell *)&data_801e7a2c_slot0b[obj->field_03 * 0x50], a, (u16)func_8015bdd4(0x140, 0x1e7));
}

void func_801e1624_slot0b(Slot0bObj *obj) {
    func_801e1644_slot0b(&obj->anim);
}

void func_801e1644_slot0b(Slot0bCursor *c) {
    Slot0bFrame *f;
    c->timer--;
    if (c->timer == 0) {
        f = c->cur + 1;
        if (c->loop < 0) {
            f = c->cur;
        }
        func_801e169c_slot0b(c, f);
    }
}

void func_801e169c_slot0b(Slot0bCursor *c, Slot0bFrame *f) {
    c->cur = f;
    c->timer = f->duration;
    c->loop = c->cur->loop;
    func_801e16d0_slot0b(c);
}
