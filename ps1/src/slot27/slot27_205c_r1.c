/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec9338 data_80029338_slot27[];
extern int data_8019046c[];
extern int data_80190464[];
extern u16 data_801901ba;
extern s8 data_8016e68e;
void func_800125f4_slot27(void);
void func_80011ea4_slot27(void);
void func_80011d90_slot27(void);
void func_800139a8_slot27(Object *obj);
void func_80013a6c_slot27(void);
void func_80013b74_slot27(Object *obj);
void func_80013cc4_slot27(Object *obj);

void func_8001205c_slot27(Object *obj) {
    u8 *e;
    Object *s0;
    int k;

    func_800125f4_slot27();
    func_80011ea4_slot27();
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    if ((*(s32 *)&game_state.field_354) != obj->field_03) {
        func_801205c4(ref_other.p->side, 0);
        obj->field_72 = 0;
        if ((*(u8 *)&data_801901ba) != 0 && (*(u8 *)&game_state.field_b3) != 0) {
            (*(u8 *)&data_801901ba) = 0;
        }
        obj->field_03 = *(u8 *)data_8019045c;
        func_800139a8_slot27(obj);
        goto done;
    }
    s0 = ref_other.p;
    e = (u8 *)(data_80029338_slot27 + ref_other.p->side);
    func_80011d90_slot27();
    if (e[5] == 0xb4 && s0->kind == 4) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->field_c4;
        (*(s32 *)data_80190464) = ref_other.p->field_c2;
        (*(s32 *)&game_state.field_354) = (*(s32 *)data_80190464) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x12;
        }
    }
    if (data_8016e68e != 0 && e[6] == 0xb4) {
        if (s0->kind != 2) goto l280;
        (*(s32 *)&game_state.field_354) = ~ref_other.p->field_c4;
        (*(s32 *)data_80190464) = ref_other.p->field_c2;
        (*(s32 *)&game_state.field_354) = (*(s32 *)data_80190464) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x14;
        }
    }
    if (s0->kind == 2 && e[4] != 0) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->field_c4;
        (*(s32 *)data_80190464) = ref_other.p->field_c2;
        (*(s32 *)&game_state.field_354) = (*(s32 *)data_80190464) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x14;
            data_8016e68e = -1;
        }
    }
l280:
    if (game_state.field_06 != 0) goto done;
    if (ref_other.p->field_c2 & 0x100) {
        if (game_state.field_b1 & 0x80) {
            obj->field_72 = obj->field_72 + 1;
            if (obj->field_72 >= 0x3c) {
                (*(s32 *)data_8019046c) = ref_other.p->kind;
                if ((*(s32 *)data_8019046c) == 0x12) (*(s32 *)data_8019046c) = 4;
                if ((*(s32 *)data_8019046c) == 0x14) (*(s32 *)data_8019046c) = 2;
                game_state.field_b1 = (*(s32 *)data_8019046c);
                if ((*(s32 *)data_8019046c) == 10) goto setb;
                if ((*(s32 *)data_8019046c) == 11) {
setb:
                    (*(u8 *)&data_801901ba) = (*(s32 *)data_8019046c);
                    (*(u8 *)&game_state.field_b3) = 0;
                }
            }
        }
    } else {
        if ((*(u8 *)&data_801901ba) != 0) {
            (*(u8 *)&game_state.field_b3) = 1;
        }
        obj->field_72 = 0;
        obj->field_70 = 0;
    }
    (*(s32 *)&game_state.field_354) = 0x80;
    (*(s32 *)data_80190464) = 0;
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->field_c4;
        (*(s32 *)data_80190464) = ref_other.p->field_c2;
        (*(s32 *)&game_state.field_354) = (*(s32 *)data_80190464) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) == 0) goto done;
    }
    obj->field_06 = obj->field_06 + 1;
    func_801205c4(ref_other.p->side, 1);
    k = (*(s32 *)&game_state.field_354);
    (*(s32 *)data_8019046c) = 0;
    (*(s32 *)data_80190464) = k;
    (*(s32 *)&game_state.field_354) = ((k & 0x80) >> 6) | ((k & 0x10) >> 4);
    if ((*(s32 *)&game_state.field_354) == 0) {
        (*(s32 *)data_8019046c) = 1;
        (*(s32 *)data_80190464) = ((k & 0x40) >> 5) | ((k & 0x20) >> 5);
        (*(s32 *)&game_state.field_354) = (*(s32 *)data_80190464);
    }
    if ((*(s32 *)&game_state.field_354) >= 3) {
        (*(s32 *)data_8019046c) = (*(s32 *)data_8019046c) + 2;
    }
    ref_other.p->field_d4 = (*(s32 *)data_8019046c);
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    (*(s32 *)data_80190464) = ref_other.p->side;
    (*(s32 *)&game_state.field_354) = ref_other.p->side;
    game_state.mode |= 1 << (*(s32 *)&game_state.field_354);
    game_state.field_71 = (*(s32 *)&game_state.field_354);
    func_80013a6c_slot27();
    if ((*(s32 *)&game_state.field_354) != 0) {
        (*(s32 *)&game_state.field_354) = 0;
        if (ref_other.p->field_d4 == 0) (*(s32 *)&game_state.field_354) = 1;
        ref_other.p->field_d4 = (*(s32 *)&game_state.field_354);
    }
    (*(s32 *)&game_state.field_354) = 1;
    func_80013b74_slot27(obj);
    return;
done:
    func_80013cc4_slot27(obj);
}
