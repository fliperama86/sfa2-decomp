/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Slot28Rec50ca4 data_80050ca4_slot28[];
extern int data_80051ce4_slot28[];
extern void (*data_8005174c_slot28[])(int *);
void func_80027b18_slot28(Object *o);
void func_80027d08_slot28(void);
void func_80027adc_slot28(Object *o);
void func_8002799c_slot28(int *p);
void func_80027748_slot28(int *p);
void func_8012818c(void);

void func_800275c4_slot28(Object *obj) {
    obj->pos_x = (u8)func_80151184() + 0x40;
    obj->pos_y = 0x10;
}

void func_80027600_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    int i = func_80151184() & 7;
    obj->field_03 = 0;
    obj->field_50 = data_80050ca4_slot28[i].field_00;
    obj->field_58 = data_80050ca4_slot28[i].field_04;
    obj->field_0b = func_80151184() & 1;
}

void func_8002766c_slot28(Object *obj) {
    data_8005174c_slot28[data_8018f5a0->field_50](data_80051ce4_slot28);
}

void func_800276b8_slot28(int *p) {
    GameState *g = &game_state;
    data_8018f5a0->field_50++;
    p[1] = 0;
    p[0] = 0x8000;
    func_80027b18_slot28((Object *)g);
    func_80027d08_slot28();
    func_80027adc_slot28((Object *)g);
    func_8002799c_slot28(p);
    func_80027748_slot28(p);
    data_80190568 = 1;
    func_8012818c();
}
