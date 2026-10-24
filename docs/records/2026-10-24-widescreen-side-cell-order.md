## Widescreen: the side cells drawn in the game's order within an entry (2026-10-24)

The owner saw Gouki's haze, along the bottom of the stage, cover only the
384 of the picture with widescreen on. Its layer (the 32-pixel grid of
`background_layers[2]`, moving at 1.37 of the camera's speed) repeats four
haze tiles over the whole map row, and its side cells were made and queued
every frame (seen with a scratch print of the map and the queue in a
scripted training fight against Gouki). They were covered: the floor's
skewed quads share the haze's entry of the ordering table, and the bridge
sent an entry's queued packets in the order queued, so the floor's side
cells, made after the haze's, were drawn over them. The game's list has the
opposite order (a layer linked later is drawn first).

`port_gpu_queue_owner` now names the pool of the layer that the next queued
packets belong to, and the walk sends them just before the first packet of
that pool in the entry, after those of layers linked later, or at the
entry's end; packets of no owner still go at the entry's tag. The side cells
are owned by their layer's pool. `port/tools/test_ui_layout.py` checks the
order (two layers with packets in one entry, one without, one packet of no
owner, an owner at the list's end), `port/tools/test_wide_stage.py` that
every copy's side cells carry its pool. Seen in the game afterwards: the
haze over the whole width at both ends of the stage.
