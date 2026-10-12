/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011ffdc(Object *o) {
    int m = (s16)box_margin[0];
    if (o->pos_x - m + 64 > 512) {
        o->field_01 = 0;
    } else {
        func_80120028(o);
    }
}

void func_80120028(Object *o) {
    o->field_01 = 1;
    if (o->field_08 == 8) {
        func_80120060(o);
    }
}

void func_80120060(Object *o) {
    push_object_by_ot_depth(o);
}

void push_object_by_ot_depth(Object *o) {
    *stack_801ad354-- = o;
    count_8018f59c++;
    if (o->ot_depth == 0) {
        push_object_stack_a(o);
    } else {
        push_object_stack_b(o);
    }
}

void push_object_stack_a(Object *o) {
    *stack_8018db00-- = o;
    count_80190100++;
}

void push_object_stack_b(Object *o) {
    *stack_8018db04-- = o;
    count_80190104++;
}

void init_sound_system(void) {
    char *p;
    func_8016507c();
    p = data_801953d0;
    func_801650bc(p, 4, 16);
    func_8016529c(1);
    func_8014f408(3, 0);
    ptr_80197ee8[0] = p - 0x3f80;
    ptr_80197ee8[1] = p - 0x3160;
    ptr_80197ee8[2] = p - 0x2340;
    ptr_80197ee8[3] = p - 0x1720;
}

void func_801201e8(void) {
    func_801656cc();
    func_80165d34(0x7f, 0x7f);
    func_80164ec0(data_80190a44[4], 0x7f, 0x7f);
    func_80164ec0(data_80190a44[5], 0x7f, 0x7f);
    func_80164ec0(data_80190a44[6], 0x7f, 0x7f);
    func_80164ec0(data_80190a44[7], 0x7f, 0x7f);
    func_8016a730(0, 0, 1);
    func_8016904c(0, 0x50, 0x50);
    func_80120374(1);
}
