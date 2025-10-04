#pragma once

#ifndef ARGS_HEADER_H
#define ARGS_HEADER_H

#include <argp.h>
#include <stdbool.h>

/**
 * Options used by the program
 * @ingroup args
 */
struct arguments {
  char *tosu_url; ///< The base URL of the Tosu server
  int monitor; ///< The monitor number to display on
  bool fullscreen; ///< Display the window in fullscreen
  int width; ///< The width of the window
  int height; ///< The height of the window
  int x; ///< The x position of the window
  int y; ///< The y position of the window
  bool verbose; ///< Verbose output
  bool version; ///< Print version and exit
  bool edit; ///< Edit mode signal
};

extern struct argp argp;

extern struct arguments options;

#endif
