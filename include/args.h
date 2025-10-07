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
  int width; ///< The width of the overlay
  int height; ///< The height of the overlay
  int x; ///< The x position of the overlay
  int y; ///< The y position of the overlay
  bool verbose; ///< Verbose output
  bool version; ///< Print version and exit
  bool layer_shell_disabled; ///< Enable layer shell
  bool edit; ///< Edit mode signal
  bool visible; ///< Visibility signal
};

extern struct argp argp;

extern struct arguments options;

#endif
