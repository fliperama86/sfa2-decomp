/* Intro-only indexed graphics adapter. No original CPU code is executed. */
#ifndef PORT_INTRO_VIDEO_H
#define PORT_INTRO_VIDEO_H
#include <stddef.h>
#include <stdint.h>

#define INTRO_VIDEO_WIDTH 384u
#define INTRO_VIDEO_HEIGHT 224u
#define INTRO_VIDEO_MEMORY 0x40000u
#define INTRO_VIDEO_OBJECTS 896u

/* Tiles are validated, row-major 4-bit pens stored one per byte. For 8-pixel
 * tiles, code*2+half identifies the alternating left/right physical halves.
 * A missing tile is an error, not an invisible replacement. */
typedef const uint8_t *(*intro_video_tile)(void *context, unsigned side,
                                         uint32_t code, unsigned half);
struct intro_video {
    const uint8_t *memory;          /* Big-endian map/palette/rowscroll words. */
    size_t memory_size;
    uint32_t map_base[3];           /* Byte offsets, aligned to 16 KiB. */
    uint32_t palette_base;          /* Four consecutive 1 KiB pages. */
    uint32_t row_base;              /* 1024 big-endian row offsets. */
    uint16_t scroll_x[3], scroll_y[3]; /* Hardware, not world coordinates. */
    uint16_t row_offset;
    uint16_t layer_control, priority_control, video_control;
    const uint8_t *objects;         /* x, y, code, attributes; eight bytes each. */
    size_t object_count;
    uint16_t object_x_offset, object_y_offset;
    intro_video_tile tile;
    void *tile_context;
};

/* Checked before output writes. A missing fetched tile returns -1; output is
 * then incomplete and must not be displayed. Otherwise returns zero. */
int intro_video_render(const struct intro_video *video, uint16_t *rgb555,
                       size_t pixel_count);
uint16_t intro_video_color(uint16_t color);
unsigned intro_video_map_index(unsigned layer, unsigned column, unsigned row);
#endif
