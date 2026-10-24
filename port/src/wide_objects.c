/* Widescreen, step 4 (port/widescreen.md): the objects of the wide view's sides drawn, with the game's rules unchanged.
 *
 * The game's margin test, clear_or_set_flag_by_pos_x_limit, runs at the end of an object's update: an object more than 448
 * pixels right of the camera's left edge (box_margin[0]) gets its drawn flag (field_01, byte 1) cleared; one inside gets it set
 * and, for mode 8, a place on the frame's collision list. Drawing reads only that flag (draw_all_block172_entries,
 * update_enabled_units_by_handler_index, call_handler_by_mode_for_active_entries, all inside
 * select_buffers_and_run_first_setup); the update goes on either way, so a hidden object's place and pose are right. The test
 * takes nothing out on the left.
 *
 * With widescreen on, the override of the test notes each object that it hid while the object is within the wide view's
 * margin (up to 448 + 2 * PORT_WIDE_EXTRA, the right edge of the view at its stage-end anchor plus the game's own 64); the
 * override of the draw phase sets their flag for the draw phase alone and clears it again after. The collision list, the
 * flag as the game's logic reads it (children read their parent's) and everything else stay as the game makes them.
 * Read from the C on 2026-10-24 (ps1/src/resident/objects/clear_or_set_flag_by_pos_x_limit.c and
 * resident/graphics/select_buffers_and_run_first_setup.c). */
#include "port.h"
#include "settings.h"
#include "video.h"

#include <stdint.h>

#define BOX_MARGIN 0x801aa5e6u   /* box_margin[0]: the camera's left edge as the game draws it (s16) */
#define OBJ_DRAWN  1u            /* Object.field_01 */
#define OBJ_POS_X  0x12u         /* Object.pos_x (s16) */
#define SIDE_MAX   256u

static uint32_t side[SIDE_MAX];   /* the objects the test hid this frame that the wide view shows */
static unsigned nside;
static port_fn test_c, draw_c;
static int said;   /* the one line of a run */

static int wide_on(void)
{
    return port_settings.widescreen && port_settings.arcade_video;
}

static int16_t read16(const unsigned char *p)
{
    return (int16_t)(p[0] | p[1] << 8);
}

/* In place of clear_or_set_flag_by_pos_x_limit(Object *o). */
static void port_o_margin_test(unsigned object)
{
    const unsigned char *margin;
    const unsigned char *o;
    int x;
    if (!test_c) test_c = port_function_impl("clear_or_set_flag_by_pos_x_limit");
    port_call1(test_c, object);
    if (!wide_on() || nside == SIDE_MAX) return;
    o = port_gs((const void *)(uintptr_t)object, OBJ_POS_X + 2);
    margin = port_gs((const void *)(uintptr_t)BOX_MARGIN, 2);
    if (!o || !margin || o[OBJ_DRAWN]) return;   /* drawn: the test let it be */
    x = read16(o + OBJ_POS_X) - read16(margin) + 64;
    if (x <= 512 || x > 512 + 2 * PORT_WIDE_EXTRA) return;   /* hidden for another reason, or past the wide view too */
    if (nside && side[nside - 1] == object) return;
    side[nside++] = object;
}

/* In place of select_buffers_and_run_first_setup(void), the frame's draw phase. */
static void port_o_draw_phase(void)
{
    unsigned i, n = nside;
    if (!draw_c) draw_c = port_function_impl("select_buffers_and_run_first_setup");
    for (i = 0; i < n; i++) {
        unsigned char *o = port_gs((const void *)(uintptr_t)side[i], 2);
        if (o && !o[OBJ_DRAWN]) o[OBJ_DRAWN] = 1;
        else side[i] = 0;   /* drawn already, or not readable: left alone */
    }
    if (n && !said) {
        said = 1;
        printf("wide-video: objects of the wide view's sides drawn (first: %u this frame)\n", n);
        fflush(stdout);
    }
    port_call0(draw_c);
    for (i = 0; i < n; i++) {
        unsigned char *o = side[i] ? port_gs((const void *)(uintptr_t)side[i], 2) : NULL;
        if (o && o[OBJ_DRAWN] == 1) o[OBJ_DRAWN] = 0;
    }
    nside = 0;
}

const struct port_override port_wide_object_overrides[] = {
    { "clear_or_set_flag_by_pos_x_limit", (void *)port_o_margin_test,
      "the game's margin test, and with widescreen on a note of the objects it hid that the wide view's sides show (no rule changes)", NULL },
    { "select_buffers_and_run_first_setup", (void *)port_o_draw_phase,
      "the game's draw phase, with the noted objects' drawn flag set for it alone (widescreen; the collision list and the logic's flag stay)", NULL },
    { NULL, NULL, NULL, NULL }
};
