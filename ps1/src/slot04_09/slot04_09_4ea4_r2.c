/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b51e0_slot04_09(Object *o);

void update_hold_check_damage_and_gauge_slot04_09(Object *o, Object *unused) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *w;
    Object *e;
    u8 *s;
    int t;
    u16 f;

    w = (Object *)obj->linked_object;
    f = w->step_flags;
    e = o->other;
    s = (u8 *)&obj->linked_object;
    if (f & 2) {
        w->step_flags = f & 0xfffd;
        o->countdown = o->countdown + 0x100;
        if (apply_damage_to_other_return_defeated(o, 0, 0) != 0) {
            set_other_hit_reaction_state_0(o, 0, 0xd, 0, 0, 1, 1);
            play_side_sound_at(o, o->side, 0xb);
            func_801b51e0_slot04_09(o);
            return;
        }
    }
    update_sequence_by_pressed_bytes(o);
    if (((s16)o->countdown & 0xff00) != 0) {
        if (e->field_15b == 0) {
            set_other_hit_reaction_state_0(o, 0, 5, -1, 0, 1, 1);
            func_801b51e0_slot04_09(o);
            return;
        }
        if ((u8)drain_gauge_by_other_input(o) != 0) {
            set_other_hit_reaction_state_0(o, 0, 0xd, 7, 0, 1, 1);
            play_side_sound_at(o, o->side, 0xb);
            func_801b51e0_slot04_09(o);
            return;
        }
    }
    e = o->other;
    w = *(Object **)s;
    e->field_0c = s[4];
    e->image_row = s[5];
    if ((u8)w->step_flags != 0) {
        e->field_0c = 0xff;
        e->sequence_changed_flag = 1;
        e->image_row = e->image_row + 3;
    }
    ((Slot04aObj *)e)->field_16 = ((Slot04aObj *)e)->field_16 + 1;
    t = o->countdown + 1;
    o->countdown = (o->countdown & 0xff00) + (t & 0xff);
    if (o->countdown & 1) {
        ((Slot04aObj *)e)->field_16 = ((Slot04aObj *)e)->field_16 - 2;
    }
    update_sequence(o);
    set_position_from_other_box(o);
}
