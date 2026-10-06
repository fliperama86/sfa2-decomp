/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8007a028_slot00[];
void func_80077c5c_slot00(Object *obj, u8 *rec);
void func_80077cd8_slot00(Object *obj, s16 *out);

void func_80077b90_slot00(Object *obj) {
    u8 *rec = (u8 *)obj + 0x40;
    obj->field_0c = 0xff;
    obj->field_0d = 2;
    obj->field_09 = 4;
    obj->field_44 = 1;
    obj->field_1e = 3;
    obj->field_04++;
    func_80077c5c_slot00(obj, rec);
    func_80077cd8_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x50;
    func_80077c5c_slot00(obj, rec);
    func_80077cd8_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x60;
    func_80077c5c_slot00(obj, rec);
    func_80077cd8_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x20;
    func_80077c5c_slot00(obj, rec);
    func_80077cd8_slot00(obj, (s16 *)rec);
}

void func_80077c5c_slot00(Object *obj, u8 *rec) {
    func_80130768(obj, data_8007a028_slot00[func_80151184() & 0xf], seqs_8017c7f8);
    *(SequenceStep **)rec = obj->sequence;
    *(u16 *)(rec + 4) = obj->field_38;
    *(u16 *)(rec + 6) = obj->field_3a;
}
