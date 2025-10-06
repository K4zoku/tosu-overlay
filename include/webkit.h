#pragma once

#ifndef WEBKIT_HEADER_H
#define WEBKIT_HEADER_H

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION == 3
#include <webkit2/webkit2.h>
#else
#include <webkit/webkit.h>
#endif

extern WebKitWebView *web_view;

/**
 * Initialize the webkit web view
 * @ingroup webkit
 */
void webkit_init();

#endif
