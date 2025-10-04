#include <stdlib.h>

#include "args.h"

struct arguments options;

static char doc[] = "Just an overlay for osu!. Powered by Tosu, GTK and Webkit";
static char args_doc[] =
    "[-t tosu-url] [-m monitor] [-w width] [-h height] [-x x] [-y y]";
static struct argp_option argp_options[] = {
    {"tosu-url", 't', "tosu-url", OPTION_ARG_OPTIONAL, "The base URL of the Tosu server.", 0},
    {"monitor", 'm', "monitor", OPTION_ARG_OPTIONAL, "The monitor number to display on.", 0},
    {"fullscreen", 'f', 0, OPTION_ARG_OPTIONAL, "Fullscreen the window.", 0},
    {"verbose", 'V', 0, OPTION_ARG_OPTIONAL, "Verbose output.", 0},
    {"width", 'w', "width", OPTION_ARG_OPTIONAL, "The width of the window.", 0},
    {"height", 'h', "height", OPTION_ARG_OPTIONAL, "The height of the window.", 0},
    {"x", 'x', "x", OPTION_ARG_OPTIONAL, "The x position of the window.", 0},
    {"y", 'y', "y", OPTION_ARG_OPTIONAL, "The y position of the window.", 0},
    {"edit", 'e', "e", OPTION_ARG_OPTIONAL, "Toggle edit mode for running overlay (useful for global hotkeys)", 0},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 't':
    arguments->tosu_url = arg;
    break;
  case 'm':
    arguments->monitor = (int)strtol(arg, NULL, 10);
    break;
  case 'f':
    arguments->fullscreen = true;
    break;
  case 'w':
    arguments->width = (int)strtol(arg, NULL, 10);
    break;
  case 'h':
    arguments->height = (int)strtol(arg, NULL, 10);
    break;
  case 'x':
    arguments->x = (int)strtol(arg, NULL, 10);
    break;
  case 'y':
    arguments->y = (int)strtol(arg, NULL, 10);
    break;
  case 'e':
    arguments->edit = true;
    break;
  case ARGP_KEY_ARG:
    return 0;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct argp argp = {argp_options, parse_opt, args_doc, doc, 0, 0, 0};