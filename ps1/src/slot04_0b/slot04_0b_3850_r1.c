/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c284c_slot04_0b[];
extern u8 data_801c2eb8_slot04_0b[];
extern s32 data_801c2ebc_slot04_0b[];
extern u8 data_801c2ee0_slot04_0b[];

void func_801b39f0_slot04_0b(Object *o, Object *unused);
void func_801b3e88_slot04_0b(Object *o);

/* The call of func_801b39f0_slot04_0b passes one argument although the function takes two: the original does not set the second argument register before it. Written with a second parameter passed on, this function is 12 bytes longer than the original. */
void func_801b3850_slot04_0b(Object *o, Object *second) {
    s32 vx;
    s32 dx;
    int hi;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    o->field_a0 = 0xff;
    o->field_04++;
    o->field_09 = 0;
    ref_other.p = o->field_3c;
    o->field_49 = ref_other.p->field_49;
    o->field_1c = ref_other.p->field_1c;
    ((Slot04aObj *)o)->field_6c = data_801c284c_slot04_0b;
    if (o->field_ac >= 0xc) {
        o->field_5c = data_801c2eb8_slot04_0b[ref_other.p->field_12a >> 1];
        func_801b3e88_slot04_0b(o);
    }
    o->field_45 = 0;
    o->field_50 = 0;
    if (o->field_ac == 4) {
        o->field_0d++;
    }
    vx = data_801c2ebc_slot04_0b[o->field_ac >> 1];
    hi = o->field_af & 0xfe;
    dx = *(s16 *)(data_801c2ee0_slot04_0b + hi);
    o->pos_y = o->pos_y - *(u16 *)(data_801c2ee0_slot04_0b + hi + 2);
    if (o->field_0b == 0) {
        vx = -vx;
        dx = -dx;
    }
    o->field_4c = vx;
    o->pos_x = o->pos_x + dx;
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)o)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)o)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(o, o->field_ac >> 1);
    ((void (*)(Object *))func_801b39f0_slot04_0b)(o);
}
