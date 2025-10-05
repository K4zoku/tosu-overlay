#include "app.h"
#include "args.h"
#include "glib-object.h"
#include "webkit.h"

GtkApplication *app;
GtkWidget *window;

static const char *APPLICATION_ID = "app.tosu.overlay";
static const char *WINDOW_TITLE = "Tosu Overlay";
static const char *WINDOW_EDIT_TITLE = "Tosu Overlay (edit mode)";

static void activate();

static void enable_inputs();
static void disable_inputs();

static void tosu_enable_edit_mode();
static void tosu_disable_edit_mode();

void app_init() {
  app = gtk_application_new(APPLICATION_ID, G_APPLICATION_NON_UNIQUE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  static char *empty[0];
  int status = g_application_run(G_APPLICATION(app), 0, empty);

  if (status != 0) {
    fprintf(stderr, "Failed to run application\n");
  }
}

void app_set_edit_mode(bool edit) {
  if (edit) {
    gtk_window_set_title(GTK_WINDOW(window), WINDOW_EDIT_TITLE);
    enable_inputs();
    tosu_enable_edit_mode();
  } else {
    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    disable_inputs();
    tosu_disable_edit_mode();
  }
}

void app_destroy() {
  g_object_unref(app);
}

static void layer_shell_init() {
  GdkDisplay *display = gdk_display_get_default();
  if (!GDK_IS_WAYLAND_DISPLAY(display)) {
    return;
  }

  gtk_layer_init_for_window(GTK_WINDOW(window));
  gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

  int monitors = gdk_display_get_n_monitors(display);
  if (options.monitor >= monitors) {
    fprintf(stderr, "Invalid monitor %d\n", options.monitor);
  }
  GdkMonitor *monitor = gdk_display_get_monitor(display, options.monitor);
  gtk_layer_set_monitor(GTK_WINDOW(window), monitor);
  static const gboolean anchors[] = {TRUE, FALSE, TRUE, FALSE};
  for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
    gtk_layer_set_anchor(GTK_WINDOW(window), i, anchors[i]);
  }
}

static void enable_inputs() {
  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);
  }
  gdk_window_input_shape_combine_region(gtk_widget_get_window(window), NULL, 0, 0);
}

static void disable_inputs() {
  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);
  }
  gdk_window_input_shape_combine_region(gtk_widget_get_window(window), cairo_region_create(), 0, 0);
}

static void tosu_enable_edit_mode() {
  static GdkRGBA rgba = {.alpha = 0.3};
  webkit_web_view_set_background_color(web_view, &rgba);
  webkit_web_view_evaluate_javascript(web_view, "window.postMessage('editingStarted');", -1, NULL, NULL, NULL, NULL, NULL);
}

static void tosu_disable_edit_mode() {
  static GdkRGBA rgba = {.alpha = 0.0};
  webkit_web_view_set_background_color(web_view, &rgba);
  webkit_web_view_evaluate_javascript(web_view, "window.postMessage('editingEnded');", -1, NULL, NULL, NULL, NULL, NULL);
}

void activate() {
  window = gtk_application_window_new(app);

  layer_shell_init();

  // set basic window properties
  gtk_window_set_default_size(GTK_WINDOW(window), options.width, options.height);
  gtk_window_move(GTK_WINDOW(window), options.x, options.y);
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
  gtk_widget_set_app_paintable(window, TRUE);

  // check if the screen supports transparency
  GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
  if (!gdk_screen_is_composited(screen)) {
    fprintf(stderr, "Your screen does not support transparency.\n");
    fprintf(stderr, "Maybe your compositor isn't running?\n");
    g_object_unref(app);
    exit(2);
  }
  gtk_widget_set_visual(window, gdk_screen_get_rgba_visual(screen));
  gtk_widget_set_size_request(window, options.width, options.height);
  gtk_window_fullscreen_on_monitor(GTK_WINDOW(window), screen, options.monitor);

  webkit_init();
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(web_view));
  gtk_container_set_border_width(GTK_CONTAINER(window), 0);
  gtk_widget_show_all(window);
  app_set_edit_mode(false);
}
