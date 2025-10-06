#pragma once

#ifndef GTK_HEADER_H
#define GTK_HEADER_H

#include <stdbool.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

#if GTK_MAJOR_VERSION == 3

#include <gtk-layer-shell.h>

#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#endif

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#else

#include <gtk4-layer-shell.h>

#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/wayland/gdkwayland.h>
#endif

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#endif

extern GtkApplication *app;
extern GtkWidget *window;

/**
 * Initialize the application
 * @ingroup app
 */
void app_init();

/**
 * Set the application edit mode
 *
 * @param edit true if enable edit mode, false otherwise
 *
 * @ingroup app
 */
void app_set_edit_mode(bool edit);

/**
 * Destroy the application, free memory
 * @ingroup app
 */
void app_destroy();

#endif
