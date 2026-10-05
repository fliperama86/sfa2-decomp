/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



Object *func_8011f1e0(void);

void func_80147a4c(Object *object) {
    fns_cbd0[object->field_04](object);
}

void func_80147a8c(Object *object) {
    object->field_09 = 0;
    object->field_04++;
    object->field_a0 = bytes_cbe0[object->field_03 + object->field_48];
    func_80130768(object, 0x13, seqs_c7f8);
}

void func_80147ae4(Object *object) {
    fns_cbe8[object->field_03](object);
}

void func_80147b24(Object *object) {
    fns_cbf4[object->field_05](object);
    func_80148134(object);
    func_80120028(object);
}

void func_80147b80(Object *object) {
    object->field_46 = 0x1d;
    object->field_05++;
    func_80131094(object);
}

void func_80147bb0(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        object->field_05++;
        func_80130768(object, 0x14, seqs_c7f8);
    } else {
        func_80131094(object);
    }
}

void func_80147c0c(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_05 = 0;
        object->field_04++;
    } else {
        func_80131094(object);
    }
}

void func_80147c50(Object *object) {
    fns_cc00[object->field_05](object);
    func_80120028(object);
}

void func_80147ca4(Object *object) {
    object->field_46 = 5;
    object->field_05++;
    func_80131094(object);
    func_80147ce8(object);
}

void func_80147ce8(Object *object) {
    if (object->field_48 != 0) {
        object->field_4c = 0xffc60000;
        object->field_50 = 0x180000;
        object->field_54 = 0x74000;
        object->field_58 = 0xfffd0000;
    } else {
        object->field_4c = 0x3a0000;
        object->field_50 = 0xffe80000;
        object->field_54 = 0xfff8c000;
        object->field_58 = 0x30000;
    }
    func_80148194(object);
}

void func_80147d54(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        object->field_46 = 7;
        object->field_05++;
    }
    func_80131094(object);
}

void func_80147da0(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        object->field_46 = 7;
        object->field_05++;
        func_80147ce8(object);
    } else {
        func_80148194(object);
    }
}

void func_80147df8(Object *object) {
    func_80147da0(object);
}

void func_80147e18(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        if (object->field_45 != 0) {
            func_80147ef0(object);
        } else {
            object->field_05++;
            func_80148134(object);
            func_80130768(object, 0x14, seqs_c7f8);
        }
    } else {
        func_80148194(object);
    }
}

void func_80147eb0(Object *object) {
    if ((s16)object->field_3a < 0) {
        func_80147ef0(object);
    } else {
        func_80131094(object);
    }
}

void func_80147ef0(Object *object) {
    object->field_04++;
    object->field_05 = 0;
}

void func_80147f04(Object *object) {
    fns_cc18[object->field_05](object);
    func_80120028(object);
}

void func_80147f58(Object *object) {
    object->field_46 = 5;
    object->field_05++;
    func_80147f88(object);
}

void func_80147f88(Object *object) {
    func_80148220(object);
    func_80131094(object);
}

void func_80147fb8(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        object->field_46 = 0x17;
        object->field_05++;
    }
    func_80131094(object);
}

void func_80148004(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v < 0) {
        if (object->field_45 != 0) {
            func_80147ef0(object);
        } else {
            object->field_05++;
            func_80130768(object, 0x14, seqs_c7f8);
        }
    } else if (object->field_a0 != 0 || object->field_48 != 0) {
        func_80148220(object);
    } else {
        func_80131094(object);
    }
}

void func_801480b0(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_05 = 0;
        object->field_04++;
    } else {
        func_80147f88(object);
    }
}

void func_801480f4(void) {
    func_8011f240();
}

void func_80148114(void) {
    func_8011f240();
}

void func_80148134(Object *object) {
    Object *parent = object->field_3c;
    object->field_0b = parent->field_0b;
    if (object->field_0b != 0) {
        object->pos_x = parent->pos_x - object->field_5c;
    } else {
        object->pos_x = parent->pos_x + object->field_5c;
    }
    object->pos_y = parent->pos_y - object->field_5e;
}

void func_80148194(Object *object) {
    Object *parent = object->field_3c;
    int a = object->field_4c;
    int b;
    object->field_0b = parent->field_0b;
    b = object->field_50;
    if (object->field_0b != 0) {
        object->pos_x = parent->pos_x + (a >> 16) - object->field_5c;
    } else {
        object->pos_x = parent->pos_x + (a >> 16) + object->field_5c;
    }
    object->pos_y = parent->pos_y - ((b >> 16) + object->field_5e);
    object->field_4c += object->field_54;
    object->field_50 += object->field_58;
}
