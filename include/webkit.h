#pragma once

#ifndef WEBKIT_HEADER_H
#define WEBKIT_HEADER_H

#include <webkit2/webkit2.h>

extern WebKitWebView *web_view;

/**
 * Initialize the webkit web view
 * @ingroup webkit
 */
void webkit_init();

#endif
