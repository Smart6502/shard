# shard 

An tiling and floating window manager with quite some features and low memory usage!

- Floating + tiling.
- Fullscreen toggle.
- Window centering.
- Mix of mouse and keyboard workflow.
- Focus with cursor.
- Alt-Tab window focusing.
- All windows die on exit.
- No window borders (yet).
- etc etc etc.


<br>

## Keybinds 

**Programs**

| combo                    | action           | program        |
| ------------------------ | ---------------- | -------------- |
| `MOD4` + `x`             | terminal         | `termite`      |
| `MOD4` + `d`             | dmenu            | `dmenu_run`    |
| `MOD4` + `p`             | screenshot       | `flameshot`    |
| `XF86_AudioLowerVolume`  | volume down      | `amixer`       |
| `XF86_AudioRaiseVolume`  | volume up        | `amixer`       |
| `XF86_AudioMute`         | volume toggle    | `amixer`       |
| `XF86_MonBrightnessUp`   | brightness up    | `brightnessctl`|
| `XF86_MonBrightnessDown` | brightness down  | `brightnessctl`|


## Dependencies

- `xlib` (*usually `libX11`*).


## Installation

1) Copy `config.def.h` to `config.h` and modify it to suit your needs.
2) Run `make` to build `shard`.
3) Copy it to your path or run `make install`.

To use with a DM, put this into /usr/share/xsessions/shard.desktop:
```
[Desktop Entry]
Name=shard
Comment=shard
Exec=shard
Type=Application
```

Or you can use an xinit implementation, such as:
```
sh ~/.fehbg
exec shard
```

