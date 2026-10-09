/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80013b9c_slot28(void);
void func_80013b68_slot28(void);
void func_80027d54_slot28(Object *obj);
void func_80027e50_slot28(Object *obj);

void func_80013a20_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    if (obj->field_1c == 0) {
        func_80027d54_slot28((Object *)&game_state);
    } else {
        HudState *h = data_8018f5a0;
        h->field_4e = h->field_4e + 1;
    }
}

void func_80013a7c_slot28(Object *obj) {
    if ((s8)data_8016e685 >= 2) {
        func_80027e50_slot28((Object *)&game_state);
    } else {
        HudState *h = data_8018f5a0;
        h->field_4e = h->field_4e + 1;
    }
}

void func_80013ad8_slot28(Object *o) {
    if (((Slot28Obj *)o)->field_f0 == 0) {
        HudState *h;
        ((Slot28Obj *)o)->field_19 = 0;
        h = data_8018f5a0;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
        h->field_50 = 0;
        h->field_52 = 0;
        h->field_54 = 0;
        h->field_56 = 0;
        ((Slot28Obj *)o)->field_2c = 0;
        ((Slot28Obj *)o)->field_86 = 1;
        if (((u8 *)&o->field_90)[1] != 0) {
            ((Slot28Obj *)o)->field_86 = 0;
        }
        ((Slot28Obj *)o)->field_86 = ((Slot28Obj *)o)->field_86 + 0x80;
        func_80013b68_slot28();
    }
}

void func_80013b68_slot28(void) {
    func_80013b9c_slot28();
    game_state.field_44 = 0xff;
    func_8011eae4();
}
