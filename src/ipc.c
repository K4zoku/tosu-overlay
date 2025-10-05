#include "ipc.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int ipc_key;
ipc_callback_t ipc_callback;
ipc_t ipc;
int shmid;
bool ipc_read_only = false;

static void ipc_callback_noop(__attribute__((unused)) const char *data) {}

static void signal_handler(__attribute__((unused)) int signum) {
  ipc_callback(ipc->data);
}

int ipc_init(int key, ipc_callback_t callback, bool read_only) {
  ipc_key = key;
  ipc_callback = callback ? callback : ipc_callback_noop;
  ipc_read_only = read_only;

  shmid = shmget(key, sizeof(struct ipc), read_only ? 0 : IPC_CREAT | 0666);
  if (shmid == -1) {
    fprintf(stderr, "Failed to %s shared memory\nError: %d (%s)\n", read_only ? "read" : "create", errno, strerror(errno));
    return errno;
  }
  ipc = (ipc_t)shmat(shmid, NULL, 0);

  if (read_only) {
    return 0;
  }

  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGUSR2, &sa, NULL);

  ipc->pid = getpid();

  return 0;
}

void ipc_destroy() {
  shmdt((void *)ipc);
  if (!ipc_read_only) {
    shmctl(shmid, IPC_RMID, NULL);
  }
}
