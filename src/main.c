#include "app.h"
#include "args.h"
#include "ipc.h"

static const char *NAME = "tosu-overlay";
static const char *VERSION = "1.0.0";
static const char *GIT_URL = "https://github.com/K4zoku/tosu-overlay";

bool edit_mode = false;
bool visible = true;

static void ipc_callback(__attribute__((unused)) const char *data) {
  int command = *(int *)data;

  if (command & TO_IPC_COMMAND_TOGGLE_VISIBILITY) {
    app_set_visible(visible = !visible);
  }
    
  if (command & TO_IPC_COMMAND_TOGGLE_EDIT) {
    app_set_edit_mode(edit_mode = !edit_mode);
  }
}

int main(int argc, char *argv[]) {
  // set defaults for options
  options.tosu_url = "http://127.0.0.1:24050";
  error_t error = argp_parse(&argp, argc, argv, 0, 0, &options);

  if (error != 0) {
    fprintf(stderr, "Failed to parse arguments\n");
    return error;
  }

  if (options.version) {
    printf("%s v%s\n%s\n", NAME, VERSION, GIT_URL);
    return 0;
  }

  // if edit flag present, send a signal and exit
  if (options.edit) {
    ipc_init(24050, NULL, true);
    fprintf(stderr, "Sending edit signal...\n");
    ipc->data[0] = TO_IPC_COMMAND_TOGGLE_EDIT;
    kill(ipc->pid, SIGUSR2);
    ipc_destroy();
    return 0;
  }

  // if visible flag present, send a signal and exit
  if (options.visible) {
    ipc_init(24050, NULL, true);
    fprintf(stderr, "Sending visible signal...\n");
    ipc->data[0] = TO_IPC_COMMAND_TOGGLE_VISIBILITY;
    kill(ipc->pid, SIGUSR2);
    ipc_destroy();
    return 0;
  }

  ipc_init(24050, ipc_callback, false);
  app_init();

  ipc_destroy();
  app_destroy();
  return 0;
}
