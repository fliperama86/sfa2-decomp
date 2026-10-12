/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
extern ObjectFn data_801dd3ac_slot05_06[];

void func_801ca168_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->mode != 0) {
        o->state_3++;
        obj->mode = 0;
    }
    update_sequence(o);
}

void func_801ca1a8_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->status_flags & 0x80) {
        o->other->other_state_code = 5;
        func_801312b8(o);
    } else {
        if (obj->mode != 0) {
            obj->mode = 0;
            o->active_flag_4 = 0;
        }
        if (o->vel_x >= 0) {
            func_801cc814_slot05_06(o);
        }
        func_80142adc(o);
        update_sequence(o);
    }
}

void func_801ca234_slot05_06(Object *obj) {
    data_801dd3ac_slot05_06[obj->state_3](obj);
}

void func_801ca274_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->state_3++;
    obj->countdown_2 = 0x32;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    play_side_sound_at(o, o->side, 7);
    func_801307e0(o, 0x22);
}
