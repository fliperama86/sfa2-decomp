/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ce9f0_slot04_17[];
extern ObjectFn data_801cea00_slot04_17[];
extern ObjectFn data_801cea08_slot04_17[];
extern s16 data_801cea10_slot04_17[];
extern ObjectFn data_801cea18_slot04_17[];
extern ObjectFn data_801cea24_slot04_17[];
extern ObjectRef data_80190458;
extern ObjectRef data_80190414;

void func_8011f38c(Object *o);
void func_80130dc0(Object *obj);
void func_80131468(Object *obj);
void func_801b63fc(Object *obj);

void func_801b5a34_slot04_17(Object *obj);
void func_801b7684_slot04_17(Object *obj);
void func_801b76a8_slot04_17(Object *obj);

void func_801b6c24_slot04_17(Object *obj);
void func_801b6cbc_slot04_17(Object *obj);
void func_801b6d14_slot04_17(Object *obj);
void func_801b6d98_slot04_17(Object *obj);
void func_801b6ec8_slot04_17(Object *obj);
void func_801b7088_slot04_17(Object *obj);
void func_801b70ac_slot04_17(Object *obj);
void func_801b70cc_slot04_17(Object *obj);
void func_801b70ec_slot04_17(Object *obj);

void func_801b6b24_slot04_17(Object *obj) {
    ref_other.p = obj->field_3c;
    data_80190458.p = ref_other.p->field_3c;
    data_80190414.p = obj->other;
    data_801ce9f0_slot04_17[obj->field_04](obj);
}

void func_801b6ba0_slot04_17(Object *obj) {
    obj->field_01 = 1;
    obj->field_0d = 2;
    obj->field_0c = 0;
    obj->field_20 = 0;
    obj->field_24 = 0;
    obj->field_09 = 0;
    obj->field_04++;
    obj->field_1c = ref_other.p->field_1c;
    func_801b6cbc_slot04_17(obj);
    func_80130768(obj, 1, seqs_8017c7f8);
    func_801b6c24_slot04_17(obj);
}

void func_801b6c24_slot04_17(Object *obj) {
    func_801b6cbc_slot04_17(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    func_80131094(obj);
}

void func_801b6c74_slot04_17(Object *o) {
    Object **p = (Object **)ref_other.p->field_3c;
    int i;

    i = 4;
    do {
        if (p == (Object **)o) {
            *(u32 *)o = 0;
            break;
        }
        p++;
    } while (--i != 0);
    func_8011f38c(o);
}

void func_801b6cbc_slot04_17(Object *obj) {
    obj->pos_x = (u16)ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y + 0x10;
    if (obj->field_03 != 0) {
        obj->pos_x = obj->pos_x - 0xc;
        obj->pos_y = obj->pos_y - 6;
    }
}

void func_801b6d14_slot04_17(Object *obj) {
    if (obj->field_219 != 0) {
        obj->kind = 0x13;
        func_801b5a34_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b70ec_slot04_17(obj);
    } else if (obj->field_128 != 0) {
        func_801b6ec8_slot04_17(obj);
    } else {
        func_801b6d98_slot04_17(obj);
    }
}

void func_801b6d98_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea00_slot04_17[obj->field_07](obj);
}

void func_801b6dd8_slot04_17(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 0 || obj->field_218 == 0 || (u8)func_8013f8c4(obj, -0x17, 0x14) == 0) {
        func_801b7088_slot04_17(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}

void func_801b6e64_slot04_17(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b70ac_slot04_17(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b6ec8_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801cea08_slot04_17[obj->field_07](obj);
}

void func_801b6f0c_slot04_17(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    func_801b7088_slot04_17(obj);
}

void func_801b6f3c_slot04_17(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b70cc_slot04_17(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b6fa0_slot04_17(Object *object) {
    int t;

    if (object->field_219 != 0) {
        object->kind = 0x13;
        func_801b5a34_slot04_17(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->field_211 & 1) {
        object->field_129 = 2;
    }
    object->field_07 = 3;
    object->field_128 = 4;
    object->field_159 = 1;
    func_80141f28(object, *(s16 *) ((char *) data_801cea10_slot04_17 + (object->field_12a & 0xfe)));
    t = 0xc;
    if (object->field_48 != 0) {
        t = 0x12;
    }
    if (object->field_129 != 0) {
        t += 3;
    }
    func_801307e0(object, (object->field_12a >> 1) + t);
}

void func_801b7088_slot04_17(Object *object) {
    object->field_159 = 1;
    func_80130dc0(object);
}

void func_801b70ac_slot04_17(Object *obj) {
    func_801312b8(obj);
}

void func_801b70cc_slot04_17(Object *obj) {
    func_80131468(obj);
}

void func_801b70ec_slot04_17(Object *obj) {
    if (obj->field_219 == 0) {
        obj->kind = 0x11;
        func_801b5a34_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b6d14_slot04_17(obj);
    } else {
        data_801cea18_slot04_17[obj->field_128 >> 1](obj);
    }
}

void func_801b7174_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea24_slot04_17[obj->field_07](obj);
}

void func_801b71b4_slot04_17(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x17, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b7684_slot04_17(obj);
        func_801b76a8_slot04_17(obj);
    }
}
