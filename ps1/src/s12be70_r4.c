/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_sequence_c_or_e_by_other_field_flag(Object *object) {
    int index;
    object->field_157 = 0;
    ref_other.p = object->other;
    index = 0xc;
    if (ref_other.p->active_state == 0 && ref_other.p->field_157 != 0) {
        object->field_157++;
        index = 0xe;
    }
    set_sequence_by_side_and_mode_2(object, index);
    if (object->field_6b == 0) func_8012cca8(object);
}

void init_fixed_sequence_c_and_clear_counters(Object *object) {
    object->field_60 = 5;
    object->facing_side = object->saved_facing;
    object->countdown = 0;
    object->field_62 = 10;
    object->field_157 = 0;
    set_sequence_by_side_and_mode_2(object, 0xc);
    if (object->field_6b == 0) func_8012cca8(object);
}

void start_sequence_with_variant_zero_spawns_once(Object *object) {
    if (object->done_flag == 0) {
        object->done_flag++;
        spawn_two_counted_objects_variant_0(object);
        play_sound_at(object, object->side, 0x321);
        store_image_row_for_hit_type(object);
    }
    object->field_15b = 1;
    set_sequence_by_side_and_mode_2(object, 0x13);
}

void update_hit_reaction_type_16_sound_spawn(Object *object) {
    if (object->active_state != 0) {
        object->active_state = 0xff;
        object->pair_value = 0x13;
        object->field_60 = 2;
        object->field_69++;
        start_sequence_with_variant_zero_spawns_once(object);
    } else {
        if (object->done_flag == 0) {
            object->done_flag++;
            spawn_two_counted_objects_variant_0(object);
        }
        play_sound_at(object, object->side, 0x321);
        store_image_row_for_hit_type(object);
        if (object->hit_type == 0x16) set_sequence_from_hit_type_base_and_capped_step(object);
        else set_hit_sequence_by_hit_type_and_move_other(object);
    }
}

void start_sequence_with_variant_two_spawns_once(Object *object) {
    if (object->once_flag_2 == 0) {
        object->once_flag_2++;
        spawn_two_counted_objects_variant_2(object);
        play_sound_at(object, object->side, 0x321);
    }
    object->field_15b = 1;
    store_image_row_for_hit_type(object);
    set_sequence_by_side_and_mode_2(object, 0x13);
}

void update_hit_reaction_type_17_sound_spawn(Object *object) {
    if (object->active_state != 0) {
        object->active_state = 0xff;
        object->pair_value = 0x13;
        object->field_60 = 2;
        object->field_69++;
        start_sequence_with_variant_two_spawns_once(object);
    } else {
        if (object->once_flag_2 == 0) {
            object->once_flag_2++;
            spawn_two_counted_objects_variant_2(object);
            play_sound_at(object, object->side, 0x321);
        }
        object->field_15b = 1;
        if (object->hit_type == 0x17) set_sequence_from_hit_type_base_and_capped_step(object);
        else set_hit_sequence_by_hit_type_and_move_other(object);
    }
}

void start_hit_reaction_with_three_spawns(Object *object) {
    if (object->active_state == 0) {
        set_sequence_from_hit_type_base_and_capped_step(object);
    } else {
        object->pair_value = 0x21;
        object->field_60 = 2;
        spawn_three_objects_variant_4(object);
        object->flag_25a_set = 1;
        set_sequence_by_side_and_mode_2(object, 0x21);
    }
}

void spawn_three_objects_and_set_sequence_x21(Object *object) {
    spawn_three_objects_variant_4(object);
    object->flag_25a_set = 1;
    set_sequence_by_side_and_mode_2(object, 0x21);
}
