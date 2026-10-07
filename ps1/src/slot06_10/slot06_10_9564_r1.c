/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea998_slot06_10[];
extern void (*data_801ea9a8_slot06_10[])(Object *, Object *);
extern ObjectFn data_801ea9b4_slot06_10[];
extern void (*data_801ea9c4_slot06_10[])(Object *, Object *);
extern SequenceStep *data_801ebbc4_slot06_10[];
extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void func_801e9da8_slot06_10(Object *obj, Object *other);

void func_801e9564_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 1;
    } else {
        func_80131094(obj);
    }
}

void func_801e95a0_slot06_10(Object *obj, Object *other) {
    func_80131094(obj);
}

void func_801e95c0_slot06_10(Object *obj, int unused, u8 index) {
    if (game_state.field_47 != 0) {
        obj->field_05 = 3;
        func_80130700(obj, data_801ebbc4_slot06_10[index]);
    }
}

void func_801e960c_slot06_10(Object *obj, Object *p) {
    u16 x = p->pos_x;

    if (obj->field_03 != 0) {
        if ((s16)obj->pos_x < (s16)x) {
            return;
        }
    } else {
        if ((s16)x < (s16)obj->pos_x) {
            return;
        }
    }
    obj->field_05 = 1;
    func_80130700(obj, data_801ebbc4_slot06_10[1]);
}

void func_801e9678_slot06_10(Object *obj, Object *p) {
    u16 x = p->pos_x;

    if (obj->field_03 != 0) {
        if ((s16)x < (s16)obj->pos_x) {
            return;
        }
    } else {
        if ((s16)obj->pos_x < (s16)x) {
            return;
        }
    }
    obj->field_05 = 0;
    obj->field_46 = 0;
    func_80130700(obj, data_801ebbc4_slot06_10[2]);
}

void func_801e96e8_slot06_10(Object *obj, Object *other) {
    obj->field_46 = (s16)obj->field_46 + 1;
    if ((s16)obj->field_46 >= 0x258) {
        obj->field_46 = 0;
        obj->field_05 = 2;
        func_80130700(obj, data_801ebbc4_slot06_10[3]);
    }
}

void func_801e973c_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e975c_slot06_10(Object *obj) {
    data_801ea998_slot06_10[obj->field_04](obj);
}

void func_801e979c_slot06_10(Object *obj) {
    u16 y;

    obj->field_0f = 1;
    obj->field_04 = 1;
    obj->field_76 = 0x340;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    y = obj->pos_y;
    obj->field_05 = 0;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->pos_y = y + 0x70;
    func_80130700(obj, data_801ebbc4_slot06_10[5]);
}

void func_801e980c_slot06_10(Object *obj) {
    Object *p;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        p = &player_left;
        if (obj->field_03 != 0) {
            p = p + 1;
        }
        data_801ea9a8_slot06_10[obj->field_05](obj, p);
    }
    func_8011ffdc(obj);
}

void func_801e9890_slot06_10(Object *obj, Object *p) {
    u16 x;

    if ((s16)obj->field_3a & 0x8000) {
        x = p->pos_x;
        if (obj->field_03 != 0) {
            if ((s16)x < 0x300) {
                return;
            }
        } else {
            if ((s16)x >= 0x201) {
                return;
            }
        }
        obj->field_05 = 1;
        func_80130700(obj, data_801ebbc4_slot06_10[6]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9918_slot06_10(Object *obj, Object *p) {
    u16 x;

    if (obj->field_3a & 0x800) {
        x = p->pos_x;
        if (obj->field_03 != 0) {
            if (obj->pos_x <= (s16)x) {
                obj->field_05 = 2;
                func_80130700(obj, data_801ebbc4_slot06_10[7]);
            } else if ((s16)x < 0x301) {
                obj->field_05 = 0;
                func_80130700(obj, data_801ebbc4_slot06_10[5]);
            }
        } else {
            if (obj->pos_x >= (s16)x) {
                obj->field_05 = 2;
                func_80130700(obj, data_801ebbc4_slot06_10[7]);
            } else if ((s16)x >= 0x200) {
                obj->field_05 = 0;
                func_80130700(obj, data_801ebbc4_slot06_10[5]);
            }
        }
    } else {
        func_80131094(obj);
    }
}

void func_801e99e0_slot06_10(Object *obj, Object *p) {
    u16 x;

    if ((s16)obj->field_3a & 0x8000) {
        x = p->pos_x;
        if (obj->field_03 != 0) {
            if ((s16)obj->pos_x < (s16)x) {
                return;
            }
        } else {
            if ((s16)x < (s16)obj->pos_x) {
                return;
            }
        }
        obj->field_05 = 1;
        func_80130700(obj, data_801ebbc4_slot06_10[8]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9a70_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9a90_slot06_10(Object *obj) {
    data_801ea9b4_slot06_10[obj->field_04](obj);
}

void func_801e9ad0_slot06_10(Object *obj) {
    u16 y;

    obj->field_0f = 1;
    obj->field_04 = 1;
    obj->field_76 = 0x340;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    y = obj->pos_y;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->pos_y = y + 0x70;
    func_80130700(obj, data_801ebbc4_slot06_10[9]);
}

void func_801e9b3c_slot06_10(Object *obj) {
    Object *p;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        p = &player_left;
        if (obj->field_0b != 0) {
            p = p + 1;
        }
        data_801ea9c4_slot06_10[obj->field_05](obj, p);
    }
    func_8011ffdc(obj);
}

void func_801e9bc0_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e9da8_slot06_10(obj, other);
        if (data_801f67f0_slot06_10 < data_801f67f4_slot06_10) {
            obj->field_05 = obj->field_05 + 1;
            func_80130700(obj, data_801ebbc4_slot06_10[10]);
            return;
        }
    }
    func_80131094(obj);
}
