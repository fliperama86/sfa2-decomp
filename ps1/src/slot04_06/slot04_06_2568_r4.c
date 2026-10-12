/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5410_slot04_06[];

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);
void func_801b4874_slot04_06(Object *obj, u16 *p);

void func_801b2850_slot04_06(Object *obj) {
    s16 t;
    GameState *g = &game_state;

    func_801b4850_slot04_06(obj);
    if (obj->pos_y < obj->ground_y) {
        func_801b4818_slot04_06(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_80146960(obj);
        if ((s8)func_8014693c(obj) != 0) {
            t = 0x80;
            func_801b4874_slot04_06(obj, (u16 *)&t);
            ref_other.p->pos_x += t;
            ref_other.p->pos_y -= 0x10;
            if ((s8)func_8014693c(obj) != 0) {
                t = 0x40;
                func_801b4874_slot04_06(obj, (u16 *)&t);
                ref_other.p->pos_x += t;
                ref_other.p->pos_y -= 0x10;
            }
        }
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        ((Slot04aObj *)obj)->field_1c2 = 0xc;
        if (obj->direction_index != 4) {
            ((Slot04aObj *)obj)->field_1c2 = 0x1c;
        }
        g->state_param = 0x1c;
        if ((u8)func_80140cd8(obj, *(s16 *)(data_801c5410_slot04_06 + (obj->direction_index & 0xfe)), 0) != 0 && obj->direction_index != 4) {
            g->spawn_kind = 4;
            func_80147000(obj);
            play_sound_at(obj, obj->side, 0x319);
            ((Slot04aObj *)obj)->field_1c2 = 0x30;
            g->state_param = 0x20;
            func_801307e0(obj, 0x25);
        } else {
            play_sound_at(obj, obj->side, 0x319);
            func_801307e0(obj, 0x25);
        }
    }
}
