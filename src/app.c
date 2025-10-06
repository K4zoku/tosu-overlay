#include "app.h"
#include "args.h"
#include "gtk/gtk.h"
#include "webkit.h"

GtkApplication *app;
GtkWidget *window;

static const char *APPLICATION_ID = "app.tosu.overlay";
static const char *WINDOW_TITLE = "Tosu Overlay";
static const char *WINDOW_EDIT_TITLE = "Tosu Overlay (edit mode)";

static void activate(GtkApplication *app, gpointer user_data);

static void enable_inputs();
static void disable_inputs();

static void tosu_enable_edit_mode();
static void tosu_disable_edit_mode();

void app_init() {
  app = gtk_application_new(APPLICATION_ID, G_APPLICATION_REPLACE);
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
  if (!(options.layer_shell_enabled && GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default()) && gtk_layer_is_supported())) {
    return;
  }

  gtk_layer_init_for_window(GTK_WINDOW(window));
  gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

  static const gboolean anchors[] = {TRUE, FALSE, TRUE, FALSE};
  for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
    gtk_layer_set_anchor(GTK_WINDOW(window), i, anchors[i]);
  }
}

static void enable_inputs() {
  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);
  }
#if GTK_MAJOR_VERSION == 3
  gdk_window_input_shape_combine_region(gtk_widget_get_window(window), NULL, 0, 0);
#else
  gdk_surface_set_input_region(gtk_native_get_surface(GTK_NATIVE(window)), NULL);
#endif
}

static void disable_inputs() {
  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);
  }
#if GTK_MAJOR_VERSION == 3
  gdk_window_input_shape_combine_region(gtk_widget_get_window(window), cairo_region_create(), 0, 0);
#else
  gdk_surface_set_input_region(gtk_native_get_surface(GTK_NATIVE(window)), cairo_region_create());
#endif
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

void activate(GtkApplication *app, __attribute__((unused)) gpointer user_data) {
  window = gtk_application_window_new(app);

  layer_shell_init();
  GdkDisplay *display = gdk_display_get_default();
  guint width, height;
#if GTK_MAJOR_VERSION == 3
  guint monitors = gdk_display_get_n_monitors(display);
  if ((guint)options.monitor >= monitors) {
    fprintf(stderr, "'%d' is not a valid monitor\n", options.monitor);
    fprintf(stderr, "Valid monitors: \n");
    for (guint i = 0; i < monitors; i++) {
      GdkMonitor *monitor = gdk_display_get_monitor(display, i);
      fprintf(stderr, "  Monitor %d: %s\n", i, gdk_monitor_get_model(monitor));
      g_object_unref(monitor);
    }
    fprintf(stderr, "\n");
    exit(1);
  }
  GdkMonitor *monitor = gdk_display_get_monitor(display, options.monitor);
  GdkRectangle geometry;
  gdk_monitor_get_workarea(monitor, &geometry);
  width = geometry.width;
  height = geometry.height;
  GdkScreen *screen = gdk_screen_get_default();
  gtk_window_fullscreen_on_monitor(GTK_WINDOW(window), screen, options.monitor);
#else
  GListModel *monitors = gdk_display_get_monitors(display);
  if ((guint)options.monitor >= g_list_model_get_n_items(monitors)) {
    fprintf(stderr, "'%d' is not a valid monitor\n", options.monitor);
    fprintf(stderr, "Valid monitors: \n");
    for (guint i = 0; i < g_list_model_get_n_items(monitors); i++) {
      GdkMonitor *monitor = g_list_model_get_item(monitors, i);
      fprintf(stderr, "  Monitor %d: %s\n", i, gdk_monitor_get_model(monitor));
    }
    fprintf(stderr, "\n");
    exit(1);
  }
  GdkMonitor *monitor = g_list_model_get_item(monitors, options.monitor);
  GdkRectangle geometry;
  gdk_monitor_get_geometry(monitor, &geometry);
  width = geometry.width;
  height = geometry.height;
  gtk_window_fullscreen_on_monitor(GTK_WINDOW(window), monitor);
  g_object_unref(monitor);
#endif
  // set basic window properties
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_monitor(GTK_WINDOW(window), monitor);
  }

  // transparent background
  GtkCssProvider *css_provider = gtk_css_provider_new();
#if GTK_MAJOR_VERSION == 3
  gtk_css_provider_load_from_data(css_provider, "window { background-color: transparent; }", -1, NULL);
  GtkStyleContext *style_context = gtk_widget_get_style_context(GTK_WIDGET(window));
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
#else
  gtk_css_provider_load_from_string(css_provider, "window.background { background-color: transparent; }");
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
#endif
  webkit_init();
  guint overlay_width, overlay_height;
  overlay_width = options.width > 0 ? options.width : width;
  overlay_height = options.height > 0 ? options.height : height;
#if GTK_MAJOR_VERSION == 3
  if (GDK_IS_X11_DISPLAY(display)) {
    // check if the screen supports transparency
    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
    if (!gdk_screen_is_composited(screen)) {
      fprintf(stderr, "Your screen does not support transparency.\n");
      fprintf(stderr, "Maybe your compositor isn't running?\n");
      g_object_unref(app);
      exit(2);
    }
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    gtk_widget_set_visual(window, visual);
  }
#endif

  GtkWidget *fixed_container = gtk_fixed_new();
#if GTK_MAJOR_VERSION == 3
  gtk_widget_set_app_paintable(window, TRUE);
  gtk_widget_set_app_paintable(fixed_container, TRUE);
#endif
  gtk_widget_set_size_request(GTK_WIDGET(web_view), overlay_width, overlay_height);
  gtk_fixed_put(GTK_FIXED(fixed_container), GTK_WIDGET(web_view), options.x, options.y);

#if GTK_MAJOR_VERSION == 3
  gtk_container_add(GTK_CONTAINER(window), fixed_container);
  gtk_widget_show_all(window);
#else
  gtk_window_set_child(GTK_WINDOW(window), fixed_container);
  gtk_window_present(GTK_WINDOW(window));
#endif

  if (GDK_IS_X11_DISPLAY(display)) {
    fprintf(stderr, "Setting some EWMH for X11...\n");
#if GTK_MAJOR_VERSION == 3
    GdkWindow *gdk_window = gtk_widget_get_window(GTK_WIDGET(window));
    gdk_window_set_skip_pager_hint(gdk_window, TRUE);
    gdk_window_set_skip_taskbar_hint(gdk_window, TRUE);
#else
    GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    // TODO: use xcb ewmh to set these things
    gdk_x11_surface_set_skip_pager_hint(surface, TRUE);
    gdk_x11_surface_set_skip_taskbar_hint(surface, TRUE);
#pragma clang diagnostic pop
#endif
  }
#if GTK_MAJOR_VERSION == 3
  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
#endif

  app_set_edit_mode(false);
}
