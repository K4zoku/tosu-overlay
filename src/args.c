#include "args.h"

#include <stdlib.h>

struct arguments options;

static char doc[] = "Just an overlay for osu!. Powered by Tosu, GTK and Webkit";
static char args_doc[] =
    "[-t tosu-url] [-m monitor-id] [-w width] [-h height] [-x x] [-y y] [-efvV]";
static struct argp_option argp_options[] = {
    {"tosu-url", 't', "tosu-url", OPTION_ARG_OPTIONAL, "The base URL of the Tosu server.", 0},
    {"monitor", 'm', "monitor-id", OPTION_ARG_OPTIONAL, "The monitor number to display on.", 0},
    {"verbose", 'V', 0, OPTION_ARG_OPTIONAL, "Verbose output.", 0},
    {"version", 'v', 0, OPTION_ARG_OPTIONAL, "Print version and exit.", 0},
    {"layer-shell-disabled", 'l', 0, OPTION_ARG_OPTIONAL, "Disable layer shell (wayland only).", 0},
    {"width", 'w', "width", OPTION_ARG_OPTIONAL, "The width of the overlay.", 0},
    {"height", 'h', "height", OPTION_ARG_OPTIONAL, "The height of the overlay.", 0},
    {"x", 'x', "x", OPTION_ARG_OPTIONAL, "The x position of the overlay.", 0},
    {"y", 'y', "y", OPTION_ARG_OPTIONAL, "The y position of the overlay.", 0},
    {"ipc-edit", 'e', 0, OPTION_ARG_OPTIONAL, "Send an IPC signal to edit the overlay.", 0},
    {"ipc-visible", 'i', 0, OPTION_ARG_OPTIONAL, "Send an IPC signal to toggle the overlay.", 0},
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
  case 'V':
    arguments->verbose = true;
    break;
  case 'v':
    arguments->version = true;
    break;
  case 'l':
    arguments->layer_shell_disabled = true;
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
  case 'i':
    arguments->visible = true;
    break;
  case ARGP_KEY_ARG:
    return 0;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct argp argp = {argp_options, parse_opt, args_doc, doc, 0, 0, 0};