/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c6e70_slot04_10[];

void func_801b090c_slot04_10(Object *obj);
void func_801b0c38_slot04_10(Object *obj);
void func_801b0c8c_slot04_10(Object *obj);
void func_801b0ce8_slot04_10(Object *obj);
void func_8011f38c(Object *o);
void func_80130dc0(Object *obj);
void func_80131468(Object *obj);
void func_801b67f8_slot04_10(Object *obj);
void func_801b6850_slot04_10(Object *obj);
void func_801b6858_slot04_10(Object *obj, s16 idx);
void func_801b68e0_slot04_10(Object *obj);
void func_801b6920_slot04_10(Object *obj);
void func_801b6968_slot04_10(Object *obj);
void func_801b69f0_slot04_10(Object *obj);
void func_801b6a0c_slot04_10(Object *obj);
void func_801b6aa0_slot04_10(Object *obj);
void func_801b6ae8_slot04_10(Object *obj);
void func_801b6b70_slot04_10(Object *obj);
void func_801b6bd4_slot04_10(Object *obj);
void func_801b6c20_slot04_10(Object *obj);
void func_801b6cb8_slot04_10(Object *obj);

void func_801b6708_slot04_10(Object *obj) {
    obj->field_01 = 0;
    if (ref_other.p->kind != obj->field_03) {
        func_801b67f8_slot04_10(obj);
    } else if (ref_other.p->frame->field_09 == 0) {
        func_801b6850_slot04_10(obj);
    } else {
        obj->pos_x = ref_other.p->pos_x;
        obj->pos_y = ref_other.p->pos_y;
        obj->field_0b = ref_other.p->field_0b;
        obj->field_01 = 1;
        if (ref_other.p->frame->field_09 != obj->field_48) {
            func_801b6858_slot04_10(obj, ref_other.p->frame->field_09);
        } else {
            func_80131094(obj);
        }
    }
}

void func_801b67f8_slot04_10(Object *obj) {
    obj->field_04++;
}

void func_801b680c_slot04_10(Object *o) {
    Object *p = ref_other.p;

    if (p->field_28 != (s32)o) {
        func_8011f38c(o);
    } else {
        p->field_28 = 0;
    }
}

void func_801b6850_slot04_10(Object *obj) {
    obj->field_48 = 0;
}

void func_801b6858_slot04_10(Object *obj, s16 idx) {
    obj->field_48 = idx;
    func_80130768(obj, idx, data_801c6e70_slot04_10);
}

void func_801b6888_slot04_10(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b090c_slot04_10(obj);
    } else if (obj->field_128 != 0) {
        func_801b6bd4_slot04_10(obj);
    } else {
        func_801b68e0_slot04_10(obj);
    }
}

void func_801b68e0_slot04_10(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b6aa0_slot04_10(obj);
    } else {
        func_801b6920_slot04_10(obj);
    }
}

void func_801b6920_slot04_10(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b6968_slot04_10(obj);
    } else if (obj->field_07 == 1) {
        func_801b6a0c_slot04_10(obj);
    }
}

void func_801b6968_slot04_10(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x26, 0x14) != 0) {
        func_801b69f0_slot04_10(obj);
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b69f0_slot04_10(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 2;
    obj->field_06 = 0;
    obj->field_07 = 0;
}

void func_801b6a0c_slot04_10(Object *obj) {
    s16 t = obj->field_3a;
    s16 n;

    if (t < 0) {
        func_801312b8(obj);
    } else {
        if (t & 0x100) {
            n = 3;
            if (obj->field_0b == 0) {
                n = -3;
            }
            obj->pos_x = n + obj->pos_x;
        }
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b6aa0_slot04_10(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b6ae8_slot04_10(obj);
    } else if (obj->field_07 == 1) {
        func_801b6b70_slot04_10(obj);
    }
}

void func_801b6ae8_slot04_10(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x24, 0x14) != 0) {
        func_801b69f0_slot04_10(obj);
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b6b70_slot04_10(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b6bd4_slot04_10(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_07 == 0) {
        func_801b6c20_slot04_10(obj);
    } else if (obj->field_07 == 1) {
        func_801b6cb8_slot04_10(obj);
    }
}

void func_801b6c20_slot04_10(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_129 == 0 && obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x26, 0x14) != 0) {
        func_801b69f0_slot04_10(obj);
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b6cb8_slot04_10(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b6d1c_slot04_10(Object *obj) {
    s16 n;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_219 != 0) {
        if (obj->field_129 != 0) {
            func_801b0c8c_slot04_10(obj);
        } else if (obj->field_48 == 0) {
            func_801b0ce8_slot04_10(obj);
        } else {
            func_801b0c38_slot04_10(obj);
        }
    } else {
        n = 0xc;
        if (obj->field_48 != 0) {
            n = 0x12;
        }
        if (obj->field_129 != 0) {
            n += 3;
        }
        n += obj->field_12a >> 1;
        func_801307e0(obj, n);
    }
}
