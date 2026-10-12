/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void select_min_placement_and_test(Object *a, Object *b, Box32 *box) {
    if (data_80188ed0.in_00 < data_80188ed0.in_04) {
        if (data_80188ed0.in_00 < data_80188ed0.in_08) {
            set_hit_output_from_first_fields_and_spawn(a, b, box);
        } else {
            copy_second_hit_rect_to_out_and_spawn_effects(a, b, box);
        }
    } else if (data_80188ed0.in_08 < data_80188ed0.in_04) {
        copy_second_hit_rect_to_out_and_spawn_effects(a, b, box);
    } else {
        copy_first_hit_rect_to_out_and_spawn_effects(a, b, box);
    }
}

void set_hit_output_from_first_fields_and_spawn(Object *a, Object *b, Box32 *box) {
    data_80188ed0.out_3c = data_80188ed0.f_0c;
    data_80188ed0.out_40 = data_80188ed0.f_24;
    data_80188ed0.out_44 = data_80188ed0.f_10;
    data_80188ed0.out_48 = data_80188ed0.f_28;
    spawn_hit_effects_and_play_sounds(a, b, box);
}

void copy_first_hit_rect_to_out_and_spawn_effects(Object *a, Object *b, Box32 *box) {
    data_80188ed0.out_3c = data_80188ed0.f_14;
    data_80188ed0.out_40 = data_80188ed0.f_2c;
    data_80188ed0.out_44 = data_80188ed0.f_18;
    data_80188ed0.out_48 = data_80188ed0.f_30;
    spawn_hit_effects_and_play_sounds(a, b, box);
}

void copy_second_hit_rect_to_out_and_spawn_effects(Object *a, Object *b, Box32 *box) {
    data_80188ed0.out_3c = data_80188ed0.f_1c;
    data_80188ed0.out_40 = data_80188ed0.f_34;
    data_80188ed0.out_44 = data_80188ed0.f_20;
    data_80188ed0.out_48 = data_80188ed0.f_38;
    spawn_hit_effects_and_play_sounds(a, b, box);
}
