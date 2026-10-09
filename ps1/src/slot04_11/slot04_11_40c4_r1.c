/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7644_slot04_11[];

u8 func_801b49e8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);
void func_801b4970_slot04_11(Object *obj);

void func_801b40c4_slot04_11(Object *obj) {
    Config *c;

    func_801b4970_slot04_11(obj);
    func_801b5764_slot04_11(obj);
    if (obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        func_801209c4(obj);
        c = game_state.config;
        if ((c->field_4d | c->field_4e | c->field_04) == 0 && (u8)func_8012f56c(obj) == 0) {
            obj->field_07++;
            func_801307e0(obj, 0x4c);
        } else {
            obj->field_07++;
            func_801307e0(obj, 0x6f);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b418c_slot04_11(Object *o) {
    u16 a;
    u8 t = 1;
    s32 d;

    func_801b4970_slot04_11(o);
    if ((o->field_3a << 16) < 0) {
        a = *(u16 *)&((Slot04bObj *)o)->field_1c0;
        o->field_45 = t;
        o->field_0b ^= 1;
        if (o->field_cd != 0) {
            a = func_801b49e8_slot04_11(o);
        }
        if (a != 0) {
            o->field_0b = t;
            if ((a & 0x40) == 0) {
                o->field_0b = 0;
                if ((a & 8) == 0) {
                    o->field_07 += 3;
                    *(u16 *)&((Slot04bObj *)o)->field_1c0 = 0;
                    ref_other.p = o->other;
                    o->field_4c = (*(s32 *)&ref_other.p->field_10 - *(s32 *)&o->field_10) / 26;
                    o->field_54 = 0;
                    o->field_50 = 0xa0000;
                    o->field_58 = -0x6000;
                    d = (u32)~o->field_4c >> 31;
                    o->field_0b = d;
                    func_801307e0(o, 0x4f);
                    return;
                }
            }
        }
        d = 0x90000;
        *(u16 *)&((Slot04bObj *)o)->field_1c0 = 0;
        o->field_07++;
        if (o->field_0b != 0) {
            d = -0x90000;
        }
        o->field_50 = 0xa0000;
        o->field_4c = d;
        o->field_54 = 0;
        o->field_58 = -0x6000;
        func_801307e0(o, 0x4d);
    } else {
        func_80130efc(o);
    }
}

void func_801b42f0_slot04_11(Object *obj) {
    func_801b4970_slot04_11(obj);
    if (func_801b5764_slot04_11(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->field_07 = 0xa;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x6f);
    } else if (obj->field_70 + 0x48 >= obj->pos_y && (data_801c7644_slot04_11[obj->field_0b] & obj->field_164) != 0) {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_50 = 0;
        obj->field_58 = 0;
        func_801307e0(obj, 0x4e);
    } else {
        func_80130efc(obj);
    }
}
