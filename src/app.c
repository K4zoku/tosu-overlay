#include "app.h"
#include "args.h"
#include "gtk/gtk.h"
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

  GListModel * monitors = gdk_display_get_monitors(display);
  if ((guint) options.monitor >= g_list_model_get_n_items(monitors)) {
    fprintf(stderr, "Invalid monitor %d\n", options.monitor);
  }
  GdkMonitor *monitor = g_list_model_get_item(monitors, options.monitor);
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
  gdk_surface_set_input_region(gtk_native_get_surface(GTK_NATIVE(window)), NULL);
}

static void disable_inputs() {
  if (gtk_layer_is_layer_window(GTK_WINDOW(window))) {
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);
  }
  gdk_surface_set_input_region(gtk_native_get_surface(GTK_NATIVE(window)), cairo_region_create());
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
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
  gtk_widget_set_size_request(window, options.width, options.height);

  GtkCssProvider *css_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_string(css_provider, "window { background-color: transparent; }");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref(css_provider);

  webkit_init();
  g_object_set(window, "child", web_view, NULL);
  gtk_window_present(GTK_WINDOW(window));
  app_set_edit_mode(false);
}
