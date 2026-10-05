/* Reconstruction. Names/roles inferred, not original symbols. */
#include "object.h"

extern SequenceStep **sequences_left, **sequences_right;
extern FrameRecord *frames_left, *frames_right;
extern Metrics metrics_left[], metrics_right[];
extern s16 scratch_a, scratch_b, scratch_c;
int box_delta(const Box6 *boxes, u8 index);
int wide_delta(const Object *object, s16 index);

void build_metrics(Object *object) {
    SequenceStep **sequences;
    SequenceStep *step;
    FrameRecord *frame;
    Metrics *out;
    s16 slot, elapsed;
    if (object->side == 0) {
        object->frames = frames_left;
        sequences = sequences_left;
    } else {
        object->frames = frames_right;
        sequences = sequences_right;
    }
    for (slot = 11; slot != -1; --slot) {
        elapsed = 0;
        step = sequences[slot];
        /* The frame stays fixed while the sequence advances. */
        frame = &object->frames[step->frame_index];
        for (;;) {
            if (frame->active) {
                if (object->side == 0) out = &metrics_left[slot];
                else out = &metrics_right[slot];
                out->elapsed = elapsed;
                out->wide_delta = wide_delta(object, elapsed);
                scratch_a = 0;
                if (frame->box_a) scratch_a = box_delta(object->boxes_a, frame->box_a);
                scratch_b = 0;
                if (frame->box_b) scratch_b = box_delta(object->boxes_b, frame->box_b);
                scratch_c = 0;
                if (frame->box_c) scratch_c = box_delta(object->boxes_c, frame->box_c);
                elapsed = scratch_a;
                if (elapsed < scratch_b) elapsed = scratch_b;
                if (elapsed < scratch_c) elapsed = scratch_c;
                out->maximum = elapsed;
                break;
            }
            elapsed += step->duration;
            if (step->flags < 0) break;
            ++step;
        }
    }
}

int box_delta(const Box6 *boxes, u8 index) {
    const Box6 *box = &boxes[index];
    return -(int)box->origin + (int)box->extent;
}
int wide_delta(const Object *object, s16 index) {
    const Box32 *box = object->wide_boxes;
    box += (u8)index;
    return -(int)box->origin + (int)box->endpoint;
}
