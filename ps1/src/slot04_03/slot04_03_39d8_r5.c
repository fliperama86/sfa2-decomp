/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c0aa8_slot04_03;
extern SequenceStep **data_801c0aac_slot04_03;

void func_801b3f8c_slot04_03(Object *obj, Object *p);

void func_801b3eec_slot04_03(Object *obj, Object *p) {
    obj->field_04++;
    obj->field_1c = p->field_1c;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0e = p->field_0e;
    obj->field_48 = 0;
    if (p->side == 0) {
        data_801c0aa8_slot04_03 = *(SequenceStep ***)0x1f8000b4;
    } else {
        data_801c0aac_slot04_03 = *(SequenceStep ***)0x1f800164;
    }
    func_801b3f8c_slot04_03(obj, p);
}

void func_801b3f8c_slot04_03(Object *obj, Object *p) {
    obj->field_01 = 0;
    if (p->kind == obj->field_03) {
        if (p->frame->field_09 != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&p->field_10;
            *(s32 *)&obj->field_14 = *(s32 *)&p->field_14;
            obj->field_0b = p->field_0b;
            obj->field_01 = 1;
            if (obj->field_48 != p->frame->field_09) {
                obj->field_48 = p->frame->field_09;
                if (p->side == 0) {
                    func_80130768(obj, obj->field_48, data_801c0aa8_slot04_03);
                } else {
                    func_80130768(obj, obj->field_48, data_801c0aac_slot04_03);
                }
            } else {
                func_80131094(obj);
            }
        } else {
            obj->field_48 = 0;
        }
    } else {
        obj->field_04++;
    }
}
