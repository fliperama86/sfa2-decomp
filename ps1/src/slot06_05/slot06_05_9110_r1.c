/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ec378_slot06_05[];
extern ObjectFn data_801ec388_slot06_05[];
extern ObjectFn data_801ec398_slot06_05[];
extern void (*data_801ec3a8_slot06_05[])(Object *obj, int arg);
extern SequenceStep *data_801ec344_slot06_05[];



Object *func_8011f32c(void);
int rand(void);
void func_80130700(Object *object, SequenceStep *entry);
void func_8011f38c(Object *o);

void func_801e9110_slot06_05(Object *obj) {
    data_801ec378_slot06_05[obj->field_04](obj);
}

void func_801e9150_slot06_05(Object *obj) {
    Object *c;
    u32 x;
    obj->field_04 = obj->field_04 + 1;
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x18;
        c->field_03 = 0;
        c->field_3c = obj;
        c->field_81 = 4;
        c->field_09 = obj->field_09 + 0xff;
        *(Object **)&obj->field_2c = c;
    }
    x = *(u32 *)&obj->field_10;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_4c = 0x2000;
    obj->field_54 = -0x100;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_58 = 0;
    obj->field_81 = 4;
    obj->field_50 = x;
    func_80130700(obj, data_801ec344_slot06_05[5]);
}

void func_801e920c_slot06_05(Object *obj) {
    int a;
    int b;
    int v;
    if ((game_state.field_65 | game_state.field_74) == 0) {
        *(u32 *)&obj->field_10 += obj->field_4c;
        obj->field_4c += obj->field_54;
        a = obj->field_50;
        b = *(u32 *)&obj->field_10;
        if (a != b) {
            v = -(b < a);
            if (obj->field_58 != v) {
                obj->field_58 = v;
                if (v != 0) {
                    obj->field_4c = -0x2000;
                    obj->field_54 = 0x100;
                } else {
                    obj->field_4c = 0x2000;
                    obj->field_54 = -0x100;
                }
            }
        }
        if (*(s32 *)(data_801aa5d4 + 0x10) < 0x170) {
            return;
        }
        func_80131094(obj);
    }
    func_80120028(obj);
}

void func_801e92d8_slot06_05(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e92f8_slot06_05(Object *obj) {
    data_801ec388_slot06_05[obj->field_04](obj);
}

void func_801e9338_slot06_05(Object *obj) {
    int x = 0x800000;
    obj->field_0a = 1;
    obj->field_4c = 0x8000;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_58 = x;
    obj->field_04 = obj->field_04 + 1;
    obj->field_81 = 4;
    obj->field_50 = *(u32 *)&obj->field_10 - x;
    func_80130700(obj, data_801ec344_slot06_05[obj->field_03]);
}

void func_801e93b0_slot06_05(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        obj->field_50 += obj->field_4c;
        *(u32 *)&obj->field_10 = (obj->field_50 & 0x3ff0000) + obj->field_58;
    }
    func_8011ffdc(obj);
}

void func_801e9410_slot06_05(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9430_slot06_05(Object *obj) {
    data_801ec398_slot06_05[obj->field_04](obj);
}

void func_801e9470_slot06_05(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_01 = 1;
    obj->field_04 = 1;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->field_26 = p->field_26;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    func_80130700(obj, data_801ec344_slot06_05[6]);
}

void func_801e94f8_slot06_05(Object *obj) {
    Object *p = obj->field_3c;
    s16 a;
    s16 b;
    s16 lo;
    s16 hi;
    int x;
    int r;
    if (game_state.field_65 == 0) {
        b = player_left.pos_x;
        a = player_right.pos_x;
        if (b <= a) { lo = b; hi = a; } else { lo = a; hi = b; }
        x = *(u16 *)&obj->pos_x;
        r = 0;
        if ((s16)(x - lo + 0x50) >= 0xa1 && (s16)(x - hi + 0x50) >= 0xa1) {
            x = (s16)x;
            r = 2;
            if ((s16)lo < x) {
                r = !(x < (s16)hi);
            }
        }
        data_801ec3a8_slot06_05[obj->field_05](obj, r);
    }
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
}

void func_801e9618_slot06_05(Object *obj, int arg) {
    int i;
    if ((s16)obj->field_3a & 0x8000) {
        if ((u8)arg != obj->field_05) {
            i = (u8)(arg + 6) << 2;
            obj->field_05 = arg;
        } else {
            if ((rand() & 0x7f) != 0) {
                goto skip;
            }
            i = ((rand() & 1) + 0xb) << 2;
        }
        func_80130700(obj, *(SequenceStep **)((u8 *)data_801ec344_slot06_05 + i));
    } else {
skip:
        func_80131094(obj);
    }
}

void func_801e96c0_slot06_05(Object *obj, int arg) {
    if (((s16)obj->field_3a & 0x8000) && (u8)arg != obj->field_05) {
        obj->field_04 = 1;
        obj->field_07 = 0;
        obj->field_06 = 0;
        obj->field_05 = 0;
        func_80130700(obj, data_801ec344_slot06_05[8]);
    } else {
        func_80131094(obj);
    }
}

void func_801e972c_slot06_05(Object *obj, int arg) {
    if (((s16)obj->field_3a & 0x8000) && (u8)arg != obj->field_05) {
        obj->field_04 = 1;
        obj->field_07 = 0;
        obj->field_06 = 0;
        obj->field_05 = 0;
        func_80130700(obj, data_801ec344_slot06_05[10]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9798_slot06_05(Object *obj) {
    Object *p = obj->field_3c;
    if (p->field_28 == (u32)obj) {
        p->field_28 = 0;
    } else if (*(Object **)&p->field_2c == obj) {
        *(u32 *)&p->field_2c = 0;
    } else if (((Slot06Obj *)p)->field_30 == (s32)obj) {
        ((Slot06Obj *)p)->field_30 = 0;
    } else if (p->field_34 == (u32)obj) {
        p->field_34 = 0;
    }
    func_8011f38c(obj);
}
