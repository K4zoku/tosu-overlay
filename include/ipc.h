#pragma once

#ifndef IPC_HEADER_H
#define IPC_HEADER_H

#include <stdbool.h>

/**
 * IPC Shared Memory struct
 * @ingroup ipc
 */
struct ipc {
    int pid; ///< Main process pid for signal handling
    char data[64]; ///< Extra data, preserve for future use
};

#define TO_IPC_COMMAND_TOGGLE_VISIBILITY (1 << 0)
#define TO_IPC_COMMAND_TOGGLE_EDIT       (1 << 1)

typedef struct ipc * ipc_t;

typedef void(*ipc_callback_t)(const char * data);

/**
 * Initialize the IPC shared memory
 *
 * @param key The key to use for the shared memory
 * @param callback The callback to call when data is received
 * @param read_only If true, only read the shared memory, not create it
 * @return 0 on success
 *
 * @ingroup ipc
 */
int ipc_init(int key, ipc_callback_t callback, bool read_only);

/**
 * Destroy the IPC shared memory
 * @ingroup ipc
 */
void ipc_destroy();

extern ipc_t ipc;

#endif
