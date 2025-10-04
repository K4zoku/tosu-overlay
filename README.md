# Tosu Overlay Linux

<p align="center">
  <img src="./logo.svg" width="64" height="64">
</p>

> An overlay for osu!. Powered by Tosu, GTK and Webkit.
> Only support Linux, for Windows, please use the official [tosu overlay](https://github.com/tosuapp/tosu).

## ✨ Features

- [x] Transparent window.
- [x] Clickthrough window.
- [x] Overlay edit mode.
- [x] Global hotkey.
- [x] Wayland support.
- [x] X11 support.
- [x] Multi-monitor.
- [ ] Attach overlay to osu! window (didn't find a way).

## 📥 Installation

You can install the package from [AUR](https://aur.archlinux.org/packages/tosu-overlay/)

```sh
yay -S tosu-overlay
```

## 🛠️ Development

### 📚 Dependencies

- [GTK+-3.0](https://gitlab.gnome.org/GNOME/gtk/)
- [Webkit2GTK-4.1](https://webkitgtk.org/)
- [GTKLayerShell](https://github.com/wmww/gtk-layer-shell)

### 🏗️ Building

#### 🐞 Debugging build

```sh
make
```

#### 📦 Release build

```sh
make BUILD_PROFILE=RELEASE
```

### 📥 Install

```sh
sudo make PREFIX=/usr install
```

**Note**: The binary will be installed to `PREFIX/bin/`

### 🪾 Project Structure

```
.make/           # Makefile templates
build/           # Build output directory
distribution/    # Some files needed for distribute the project
  bin/           # Executable output directory
  lib/           # Library output directory
  obj/           # Object file output directory
  test/          # Unit test output directory
docs/            # Documentation files
include/         # Public header files
src/             # Source files
test/            # Unit test files
```
