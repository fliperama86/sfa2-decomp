/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c60e0_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

u8 func_80140cd8(Object *object, int a, int b);
void func_80146960(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b4958_slot04_0e(Object *obj);
void func_801b49c8_slot04_0e(Object *obj);

void func_801b4410_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    obj->field_47 += 0xff;
    if ((obj->field_47 & 0x80) != 0) {
        obj->field_27b = 0;
        o->field_12c = o->field_12c + 1;
        func_801307e0(o, 0x44);
    }
}

void func_801b4458_slot04_0e(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_4c = 0x90000;
        obj->field_50 = 0x50000;
        obj->field_54 = 0xffff4000;
        obj->field_58 = -0x8000;
        obj->field_12c = obj->field_12c + 1;
    }
    func_80130efc(obj);
}

void func_801b44bc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    func_801b4958_slot04_0e(o);
    if (o->field_50 >= 0) {
        func_80130efc(o);
    } else {
        obj->field_47 = 6;
        o->field_4c = 0;
        o->field_54 = 0;
        o->field_50 = 0;
        o->field_58 = 0xffff4000;
        o->field_12c = o->field_12c + 1;
        func_801204f4(o, o->side, 0x14);
    }
}

void func_801b453c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p = o->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[o->side];

    obj->field_47 += 0xff;
    if ((obj->field_47 & 0x80) != 0) {
        func_801b4958_slot04_0e(o);
        o->field_12c = o->field_12c + 1;
        r->field_04 = 0;
        r->field_0c = 0x8000;
        r->field_08 = 0;
        r->field_00 = (*(s32 *)&o->field_10 - *(s32 *)&p->field_10) >> 4;
        func_80130efc(o);
    }
}

void func_801b45e8_slot04_0e(Object *obj) {
    func_801b49c8_slot04_0e(obj);
    func_801b4958_slot04_0e(obj);
    if (obj->field_70 < obj->pos_y) {
        obj->field_38 = 1;
        obj->field_12c = obj->field_12c + 1;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->pos_y = obj->field_70;
        func_801204f4(obj, obj->side, 0x12);
        func_801204f4(obj, obj->side, 6);
    }
    func_80130efc(obj);
}

void func_801b4678_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];

    func_801b49c8_slot04_0e(obj);
    func_80130efc(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_12c = obj->field_12c + 1;
        game_state.field_63 = 0x18;
        *(u8 *)&obj->field_3a = 0;
        r->field_04 = 0;
        r->field_00 = 0;
        r->field_08 = 0;
        r->field_0c = 0;
        r->field_04 = 0xfff80000;
        r->field_0c = -0x8000;
        func_80140cd8(obj, data_801c60e0_slot04_0e[obj->field_12a], data_801c60e0_slot04_0e[obj->field_12a + 1]);
        if (((Slot04bObj *)p)->field_5c < 0) {
            game_state.field_6b = 0;
            func_80147000(obj);
        }
        func_801307e0(obj, 0x45);
    }
}

void func_801b4784_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];
    Object *q;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 2) {
        *(u8 *)&obj->field_3a = 1;
        func_801204f4(obj, obj->side, 0x13);
    }
    if (*(u8 *)&obj->field_3a == 0) {
        func_801b49c8_slot04_0e(obj);
        if (p->field_70 < p->pos_y) {
            obj->field_07 = obj->field_07 + 1;
            game_state.field_63 = 0x18;
            func_80146960(obj);
            obj->field_12c = 0;
            p->field_261 = 0;
            p->field_14 = 0;
            p->field_15b = 1;
            p->pos_y = p->field_70;
            func_80140770(obj, 4, 0xa, -0x200, 0, 0, 0);
            if (((Slot04bObj *)p)->field_5c < 0) {
                obj->field_167 = (obj->field_12a >> 1) + 0xc;
            }
            q = r->field_14;
            if (q != 0) {
                q->field_48 = 0x57;
                q->field_05 = q->field_05 + 1;
            }
        }
    }
}

void func_801b48d4_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b4914_slot04_0e(Object *o) {
    *(s32 *)&o->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    *(s32 *)&o->field_14 -= o->field_50;
    o->field_50 += o->field_58;
}

void func_801b4958_slot04_0e(Object *obj) {
    s32 a = obj->field_4c;

    if (a != 0) {
        if (obj->field_0b == 0) {
            a = -a;
        }
        *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_4c = 0;
        }
    }
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}

void func_801b49c8_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];

    *(s32 *)&p->field_10 += r->field_00;
    r->field_00 += r->field_08;
    *(s32 *)&p->field_14 -= r->field_04;
    r->field_04 += r->field_0c;
}
