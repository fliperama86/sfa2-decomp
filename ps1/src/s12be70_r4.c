/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012c89c(Object *object) {
    int index;
    object->field_157 = 0;
    ref_other.p = object->other;
    index = 0xc;
    if (ref_other.p->field_45 == 0 && ref_other.p->field_157 != 0) {
        object->field_157++;
        index = 0xe;
    }
    func_801308c4(object, index);
    if (object->field_6b == 0) func_8012cca8(object);
}

void func_8012c92c(Object *object) {
    object->field_60 = 5;
    object->field_0b = object->field_158;
    object->field_46 = 0;
    object->field_62 = 10;
    object->field_157 = 0;
    func_801308c4(object, 0xc);
    if (object->field_6b == 0) func_8012cca8(object);
}

void func_8012c990(Object *object) {
    if (object->field_259 == 0) {
        object->field_259++;
        func_80152b40(object);
        func_80120554(object, object->side, 0x321);
        func_801376b8(object);
    }
    object->field_15b = 1;
    func_801308c4(object, 0x13);
}

void func_8012ca04(Object *object) {
    if (object->field_45 != 0) {
        object->field_45 = 0xff;
        object->field_68 = 0x13;
        object->field_60 = 2;
        object->field_69++;
        func_8012c990(object);
    } else {
        if (object->field_259 == 0) {
            object->field_259++;
            func_80152b40(object);
        }
        func_80120554(object, object->side, 0x321);
        func_801376b8(object);
        if (object->field_61 == 0x16) func_8012c72c(object);
        else func_8012c5cc(object);
    }
}

void func_8012cac8(Object *object) {
    if (object->field_258 == 0) {
        object->field_258++;
        func_80152c34(object);
        func_80120554(object, object->side, 0x321);
    }
    object->field_15b = 1;
    func_801376b8(object);
    func_801308c4(object, 0x13);
}

void func_8012cb3c(Object *object) {
    if (object->field_45 != 0) {
        object->field_45 = 0xff;
        object->field_68 = 0x13;
        object->field_60 = 2;
        object->field_69++;
        func_8012cac8(object);
    } else {
        if (object->field_258 == 0) {
            object->field_258++;
            func_80152c34(object);
            func_80120554(object, object->side, 0x321);
        }
        object->field_15b = 1;
        if (object->field_61 == 0x17) func_8012c72c(object);
        else func_8012c5cc(object);
    }
}

void func_8012cc00(Object *object) {
    if (object->field_45 == 0) {
        func_8012c72c(object);
    } else {
        object->field_68 = 0x21;
        object->field_60 = 2;
        func_80152d28(object);
        object->field_25a = 1;
        func_801308c4(object, 0x21);
    }
}

void func_8012cc6c(Object *object) {
    func_80152d28(object);
    object->field_25a = 1;
    func_801308c4(object, 0x21);
}
