## Experimental flags in shared settings (2026-10-12)

The owner requires every experimental feature toggle to be represented in the
settings file and suggests an [experimental] section. Use that section for
independent opt-in switches, including pixel-perfect UI sampling and the arcade
canvas. Defaults are off; enabling UI sampling does not enable the wider canvas.

The settings file is the shared configuration for startup and the future UI.
Do not require an experiment-specific executable, launcher or directory tree
just to select a feature. Any command-line override must address the same
setting, not create a separate experimental control mechanism.

This records the requirement, not completed integration: the current private
settings parser prototype is not wired into the normal port or staged build.
