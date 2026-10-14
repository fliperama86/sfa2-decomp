## P2's life caps drawn in two halves (2026-10-14)

PR 364 made one-to-one HUD tiles sample a mirrored 16-texel side one texel on,
as the console does. The owner, with a picture: the bar was "just worse": the
uneven dithering was still there and the border problems were back. His
picture shows both. P1's bottom-right corner had moved a pixel (the bottom-row
end tile is mirrored, the top-row one is not), and P2's left cap still broke the
checker: that cap has a 15-texel side, which the change left alone, and it was
hidden under the yellow fill in the run that PR 364 was checked with.

What the art allows, from the tile rows: an exact mirror puts every border
where the artwork has it, and no one shift does that and keeps the checker
in phase on both caps. But each cap's inner half is texel for texel the bar's
repeating body (3c's columns 0 to 7 and 3a's columns 8 to 15 equal the body
tile's columns 8 to 15 and 0 to 7).

The change: `port_video_pixel_grid` is an exact mirror again (PR 364's shift
out, its build-script key kept). With `pixel_perfect_ui` on, the bridge draws
the two mirrored caps of P2's frame (texels 192,48 and 160,48 of page 13, at Y
7 on list 14) as two 8x16 halves on the original path: the outer half an exact
mirror, its texels given one lower because PsyZ's `FixupFlipUV` adds one to a
mirrored side, and the inner half unmirrored. The game's records stay.

Evidence, on the branch's tree: `test_ui_layout.py` and `test_video.py` pass;
18 one-line changes of the new rule and its use in `gpu.c`, each alone against
`test_ui_layout.py` under limits, were all noticed (two only after a control
was added for each); the 18 of PR 362 still are. One hidden, silent arcade
fight with this tree's program: no doubled dither column in P2's bar, P2's
right cap's border where the exact mirror puts it, and the top and bottom rows
of all four bar ends aligned. P2's left cap's inner half was under the yellow
fill in every picture of that run; its texels rest on the controls and on the
same mechanism as the right cap. Not run: a window.
