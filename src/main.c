#include "args.h"
#include "ipc.h"
#include "app.h"

bool edit_mode = false;

static void ipc_callback(__attribute__((unused)) const char *data) {
  app_set_edit_mode(edit_mode = !edit_mode);
}

int main(int argc, char *argv[]) {
  // set defaults for options
  options.tosu_url = "http://127.0.0.1:24050";
  options.monitor = 0;
  options.width = 1920;
  options.height = 1080;
  options.x = 0;
  options.y = 0;
  error_t error = argp_parse(&argp, argc, argv, 0, 0, &options);

  if (error != 0) {
    fprintf(stderr, "Failed to parse arguments\n");
    return error;
  }

  // if edit flag present, send a signal and exit
  if (options.edit) {
    ipc_init(24050, NULL, true);
    fprintf(stderr, "Sending edit signal...\n");
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
