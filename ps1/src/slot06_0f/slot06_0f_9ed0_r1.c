/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
extern ObjectFn data_801eefc4_slot06_0f[];
extern ObjectFn data_801eefd4_slot06_0f[];
extern ObjectFn data_801eefe4_slot06_0f[];
extern ObjectFn data_801eefec_slot06_0f[];
extern ObjectFn data_801eeff4_slot06_0f[];

Block172 *func_8011f1e0(void);
void func_801ea1a8_slot06_0f(Object *obj);
void func_801ea704_slot06_0f(Object *obj);
void func_801ea734_slot06_0f(Object *obj);
int func_801ea77c_slot06_0f(Object *obj);
void func_801ea7b4_slot06_0f(Object *obj);
int func_801ea7e4_slot06_0f(Object *obj);
void func_801ea828_slot06_0f(Object *obj);
int func_801ea85c_slot06_0f(Object *obj);
void func_801ea898_slot06_0f(Object *obj);
int func_801ea8c8_slot06_0f(Object *obj);
void func_801ea8e0_slot06_0f(Object *obj);
void func_801ea91c_slot06_0f(Object *obj, s16 idx);

void func_801e9ed0_slot06_0f(void) {
    Object *s;
    Object *n;
    Object *right;
    int k;
    data_80190468.p = (Object *)&game_state;
    if (((GameState *)data_80190468.p)->field_40 == 0xf && ((GameState *)data_80190468.p)->field_80 == 0) {
        s = &player_left;
        right = s + 1;
        k = player_left.kind == 0xf;
        if (player_right.kind == ((GameState *)data_80190468.p)->field_40) {
            k |= 2;
        }
        if (k != 1) {
            if (k == 0) {
                return;
            }
            if (k == 2 || (k == 3 && player_right.field_d7 != 0)) {
                s = right;
            }
        }
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            n->field_00 = 1;
            n->field_02 = 0x46;
            n->field_3c = s;
            n->field_66 = s->side;
            n->field_09 = 0xe;
            n->field_1c = s->field_1c;
            n->field_7a = 0x60;
            n->field_7c = 0x1e0;
            n->field_0d = s->field_0d;
            n->field_90 = s->field_90;
            n->field_98 = s->field_98;
            n->field_9c = s->field_9c;
        }
    }
}

void func_801ea010_slot06_0f(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_801eefc4_slot06_0f[obj->field_04](obj);
}

void func_801ea060_slot06_0f(Object *obj) {
    s16 t;
    game_state.field_358 = obj->field_3c;
    if (game_state.field_358->field_01 != 0) {
        obj->field_04 = obj->field_04 + 1;
        obj->field_0c = game_state.field_358->field_0c;
        obj->field_0e = game_state.field_358->field_0e;
        obj->field_09 = 0xe;
        obj->field_1c = game_state.field_358->field_1c;
        obj->pos_x = game_state.field_358->pos_x;
        obj->pos_y = game_state.field_358->pos_y - 6;
        t = 0x30;
        if (game_state.field_358->field_0b == 0) {
            t = -0x30;
        }
        obj->pos_x += t;
        obj->field_03 = game_state.field_358->kind;
        obj->field_0f = 1;
        obj->field_46 = 1;
        obj->field_0b = 0;
        obj->field_48 = 0;
        func_801ea91c_slot06_0f(obj, 0);
        func_801ea1a8_slot06_0f(obj);
    }
}

void func_801ea1a8_slot06_0f(Object *obj) {
    GameState *g = (GameState *)data_80190468.p;
    if (g->field_6a == 0) {
        game_state.field_358 = obj->field_3c;
        if (obj->field_03 == game_state.field_358->kind) {
            if (g->field_65 == 0) {
                data_801eefd4_slot06_0f[obj->field_05](obj);
            }
            if (game_state.field_226 == 0) {
                func_8011ffdc(obj);
            } else {
                obj->field_01 = 0;
            }
            return;
        }
    }
    obj->field_04 = 2;
}

void func_801ea278_slot06_0f(Object *obj) {
    data_801eefe4_slot06_0f[obj->field_06](obj);
}

void func_801ea2b8_slot06_0f(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    func_801ea734_slot06_0f(obj);
}

void func_801ea2e4_slot06_0f(Object *obj) {
    if (func_801ea8c8_slot06_0f(obj)) {
        func_801ea8e0_slot06_0f(obj);
    } else if (func_801ea85c_slot06_0f(obj)) {
        func_801ea898_slot06_0f(obj);
    } else if (func_801ea77c_slot06_0f(obj)) {
        func_801ea7b4_slot06_0f(obj);
    } else if (func_801ea7e4_slot06_0f(obj)) {
        func_801ea828_slot06_0f(obj);
    } else {
        func_801ea734_slot06_0f(obj);
        func_80131094(obj);
    }
}

void func_801ea394_slot06_0f(Object *obj) {
    data_801eefec_slot06_0f[obj->field_06](obj);
}

void func_801ea3d4_slot06_0f(Object *obj) {
    int v = obj->field_46 - 1;
    obj->field_46 = v;
    if ((s16)v < 0) {
        obj->field_0b = 0;
        obj->field_06++;
        func_801ea91c_slot06_0f(obj, 2);
    } else {
        func_80131094(obj);
    }
}

void func_801ea42c_slot06_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801ea704_slot06_0f(obj);
    } else {
        if (func_801ea8c8_slot06_0f(obj)) {
            func_801ea8e0_slot06_0f(obj);
        }
        if (func_801ea77c_slot06_0f(obj) == 0 && func_801ea85c_slot06_0f(obj) == 0 && func_801ea7e4_slot06_0f(obj) != 0) {
            func_801ea828_slot06_0f(obj);
        } else {
            func_80131094(obj);
        }
    }
}

void func_801ea4d0_slot06_0f(Object *obj) {
    data_801eeff4_slot06_0f[obj->field_06](obj);
}

void func_801ea510_slot06_0f(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_0b = 0;
        obj->field_06++;
        func_801ea91c_slot06_0f(obj, 3);
    }
    func_80131094(obj);
}

void func_801ea56c_slot06_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801ea704_slot06_0f(obj);
    } else if (func_801ea8c8_slot06_0f(obj)) {
        func_801ea8e0_slot06_0f(obj);
    } else if (func_801ea7e4_slot06_0f(obj) == 0 && func_801ea85c_slot06_0f(obj) == 0 && func_801ea77c_slot06_0f(obj) != 0) {
        func_801ea7b4_slot06_0f(obj);
    } else {
        func_80131094(obj);
    }
}
