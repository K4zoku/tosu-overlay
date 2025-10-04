#pragma once

#ifndef GTK_HEADER_H
#define GTK_HEADER_H

#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>
#include <gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <stdbool.h>

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
