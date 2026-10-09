/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ee14c_slot06_11[];
extern SequenceStep *data_801ee160_slot06_11[];
extern ObjectFn data_801ee170_slot06_11[];
extern ObjectFn data_801ee180_slot06_11[];
extern Slot06_11Rece188 data_801ee188_slot06_11[];
extern u8 data_801ee288_slot06_11[];
extern u16 data_801ee290_slot06_11[];
extern ObjectFn data_801ee2f0_slot06_11[];
extern ObjectFn data_801ee300_slot06_11[];
extern ObjectFn data_801ee314_slot06_11[];
extern Slot06_11Rec0028 *data_801f6200_slot06_11;
extern s16 data_801f6204_slot06_11;
extern s16 data_801f6208_slot06_11;

void func_801e97e8_slot06_11(Object *obj, Object *other, Slot06_11Rec0028 *rec);
void func_801e99ac_slot06_11(Object *obj, int arg);
void func_801e9a64_slot06_11(Object *obj, int arg);

void func_801e92c4_slot06_11(Object *obj) {
    data_801ee170_slot06_11[obj->field_04](obj);
}

void func_801e9304_slot06_11(Object *obj) {
    obj->field_04 = 1;
    obj->field_0a = 1;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_54 = 0x6d26cadb;
    func_80130700(obj, data_801ee160_slot06_11[obj->field_03]);
}

void func_801e9364_slot06_11(Object *obj) {
    data_801ee180_slot06_11[obj->field_03](obj);
}

void func_801e93a4_slot06_11(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_80131094(obj);
    }
    func_80120028(obj);
}

void func_801e93f0_slot06_11(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    if ((game_state.field_65 | game_state.field_74) == 0) {
        s16 t = obj->field_46;
        int x;
        int y;
        u8 dir;

        obj->field_46 = t - 1;
        dir = data_801ee288_slot06_11[(u8)t >> 5];
        x = data_801ee188_slot06_11[t & 0x1f].field_00;
        y = data_801ee188_slot06_11[t & 0x1f].field_04;
        if (dir & 1) {
            x = -x;
        }
        if (dir & 2) {
            y = -y;
        }
        *(u32 *)&obj->field_10 = x + *(u32 *)&obj->field_10;
        *(u32 *)&obj->field_14 = *(u32 *)&obj->field_14 - y;
        func_80131094(obj);
        x = obj->field_54;
        y = x << 31;
        x >>= 1;
        y |= x;
        obj->field_54 = y;
        if ((y & 1) == 0) {
            return;
        }
    }
    func_80120028(obj);
}

void func_801e94dc_slot06_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e94fc_slot06_11(Object *obj) {
    data_801ee2f0_slot06_11[obj->field_04](obj);
}

void func_801e953c_slot06_11(Object *obj) {
    u8 t;

    obj->field_81 = 4;
    t = obj->field_04;
    obj->field_0c = 0;
    obj->field_04 = t + 1;
    data_801ee300_slot06_11[obj->field_03](obj);
}

void func_801e9590_slot06_11(Object *obj) {
    obj->pos_y = 0xc8;
    obj->field_0f = 0;
    game_state.field_30c = (Pair *)&player_left;
    data_801f6200_slot06_11 = (Slot06_11Rec0028 *)((u8 *)obj + 0x5c);
    obj->field_61 = 0;
    data_801f6200_slot06_11->field_06 = 0;
    data_801f6200_slot06_11->field_00 = *(u32 *)&((Object *)game_state.field_30c)->field_10;
    data_801f6200_slot06_11->field_04 = ((Object *)game_state.field_30c)->field_45;
    game_state.field_30c = (Pair *)(&player_left + 1);
    data_801f6200_slot06_11 = (Slot06_11Rec0028 *)((u8 *)obj + 0x28);
    *((u8 *)obj + 0x2d) = 0;
    data_801f6200_slot06_11->field_06 = 0;
    data_801f6200_slot06_11->field_00 = *(u32 *)&((Object *)game_state.field_30c)->field_10;
    data_801f6200_slot06_11->field_04 = ((Object *)game_state.field_30c)->field_45;
}

void func_801e965c_slot06_11(Object *obj) {
    obj->field_0e = 8;
    obj->field_09 = 0x1d;
    obj->field_0b = 0;
    obj->field_0f = 1;
    func_80130768(obj, obj->field_45, data_801ee14c_slot06_11);
}

void func_801e96a0_slot06_11(Object *obj) {
    obj->field_0e = 8;
    obj->field_09 = 8;
    obj->field_0f = 1;
    game_state.field_358 = obj->field_3c;
    obj->pos_x = game_state.field_358->pos_x;
    func_80130768(obj, (s16)(obj->field_45 + obj->field_03), data_801ee14c_slot06_11);
}

void func_801e9704_slot06_11(Object *obj) {
    data_801ee314_slot06_11[obj->field_03](obj);
}

void func_801e9744_slot06_11(Object *obj) {
    Object *p = &player_left;

    game_state.field_30c = (Pair *)p;
    func_801e97e8_slot06_11(obj, p, (Slot06_11Rec0028 *)((u8 *)obj + 0x5c));
    game_state.field_30c = (Pair *)(p + 1);
    func_801e97e8_slot06_11(obj, p + 1, (Slot06_11Rec0028 *)((u8 *)obj + 0x28));
    obj->pos_x = ((Slot06Layer *)data_801aa5d4)->field_12;
    if (game_state.field_74 == 0) {
        obj->pos_y = 0xf8 - (obj->pos_y ^ 1);
    }
}

void func_801e97e8_slot06_11(Object *obj, Object *other, Slot06_11Rec0028 *rec) {
    other->field_293 = 0;
    data_801f6204_slot06_11 = other->pos_x - 0x100;
    data_801f6208_slot06_11 = data_801f6204_slot06_11;
    data_801f6204_slot06_11 = data_801f6204_slot06_11 >> 5;
    data_801f6204_slot06_11 = data_801ee290_slot06_11[data_801f6204_slot06_11 * 2];
    data_801f6208_slot06_11 = data_801f6208_slot06_11 & 0x1f;
    data_801f6204_slot06_11 = data_801f6204_slot06_11 >> data_801f6208_slot06_11;
    if (data_801f6204_slot06_11 != 0) {
        other->field_293 = 0xff;
        if (*(u16 *)&other->field_04 != 0x301) {
            u8 e = other->field_45;
            u8 r = rec->field_04;

            if ((e & r) == 0) {
                if ((e | r) != 0) {
                    rec->field_05 = 0;
                    rec->field_06 = 0;
                    func_801e9a64_slot06_11(other, 1);
                    func_801e99ac_slot06_11(other, 1);
                } else if (*(u32 *)&other->field_10 == rec->field_00) {
                    rec->field_05 -= 1;
                    if ((rec->field_05 & 0x80) != 0) {
                        rec->field_05 = 0x2a;
                        rec->field_06 = 0;
                        func_801e99ac_slot06_11(other, 0);
                    }
                } else {
                    rec->field_06 -= 1;
                    if ((rec->field_06 & 0x80) != 0) {
                        rec->field_05 = 8;
                        rec->field_06 = 0xb;
                        if (*(u16 *)&other->field_04 == 0x101) {
                            func_801e9a64_slot06_11(other, 2);
                        } else {
                            func_801e9a64_slot06_11(other, 0);
                        }
                        func_801e99ac_slot06_11(other, 1);
                        rec->field_05 = 9;
                        rec->field_06 = 0xb;
                    }
                }
            }
        }
    }
    rec->field_00 = *(u32 *)&other->field_10;
    rec->field_04 = other->field_45;
}
