#include "webkit.h"
#include "args.h"

WebKitWebView *web_view;

gboolean context_menu_disabled(__attribute__((unused)) WebKitWebView *web_view,
                               __attribute__((unused))
                               WebKitContextMenu *context_menu,
                               __attribute__((unused)) GdkEvent *event,
                               __attribute__((unused))
                               WebKitHitTestResult *hit_test_result,
                               __attribute__((unused)) gpointer user_data) {
  return TRUE;
}

static GString *build_tosu_ingame_url() {
  int len = strlen(options.tosu_url);
  bool ending_slash = options.tosu_url[len - 1] == '/';
  GString *url = g_string_new(options.tosu_url);
  if (!ending_slash) {
    g_string_append_c(url, '/');
  }
  g_string_append(url, "api/ingame");
  return url;
}

void webkit_init() {
  WebKitWebContext *context = webkit_web_context_get_default();
  webkit_web_context_set_cache_model(context,
                                     WEBKIT_CACHE_MODEL_DOCUMENT_VIEWER);

  WebKitSecurityManager *security_manager =
      webkit_web_context_get_security_manager(context);
  webkit_security_manager_register_uri_scheme_as_secure(security_manager, "ws");
  webkit_security_manager_register_uri_scheme_as_secure(security_manager,
                                                        "http");

  web_view = WEBKIT_WEB_VIEW(
      g_object_new(WEBKIT_TYPE_WEB_VIEW, "web-context", context, NULL));
  g_signal_connect(web_view, "context-menu", G_CALLBACK(context_menu_disabled),
                   NULL);
  WebKitSettings *settings = webkit_settings_new();
  if (options.verbose) {
    webkit_settings_set_enable_write_console_messages_to_stdout(settings, true);
  }

  GString *tosu_overlay_url = build_tosu_ingame_url();
  fprintf(stdout, "Connecting to %s\n", tosu_overlay_url->str);
  webkit_web_view_load_uri(web_view, tosu_overlay_url->str);
  g_string_free(tosu_overlay_url, TRUE);
}