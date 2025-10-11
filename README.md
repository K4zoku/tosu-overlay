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

## Usage

```css
Usage: tosu-overlay [OPTION...]
            [-t tosu-url] [-m monitor-id] [-w width] [-h height] [-x x] [-y y]
            [-efvV]
Just an overlay for osu!. Powered by Tosu, GTK and Webkit

  -e, --ipc-edit             Send an IPC signal to edit the overlay.
  -h, --height[=height]      The height of the overlay.
  -i, --ipc-visible          Send an IPC signal to toggle the overlay.
  -l, --layer-shell-disabled Disable layer shell (wayland only).
  -m, --monitor[=monitor-id] The monitor number to display on.
  -t, --tosu-url[=tosu-url]  The base URL of the Tosu server.
  -V, --verbose              Verbose output.
  -w, --width[=width]        The width of the overlay.
  -x, --x[=x]                The x position of the overlay.
  -y, --y[=y]                The y position of the overlay.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -v, --version              Print version and exit.
```

## ❓ FAQ

### 'error dispatching to Wayland display' on Nvidia GPU

Run tosu-overlay with `__NV_DISABLE_EXPLICIT_SYNC=1` environment variable.

### 🪾 Project Structure

```
.
├─ .make/           # Makefile templates
├─ distribution/    # Some files needed for distribute the project
├─ build/           # Build output directory
│  ├─ bin/          # Executable output directory
│  ├─ lib/          # Library output directory
│  ├─ obj/          # Object file output directory
│  └─ test/         # Unit test output directory
├─ docs/            # Documentation files
├─ include/         # Header files
├─ src/             # Source files
├─ test/            # Unit test files
```
