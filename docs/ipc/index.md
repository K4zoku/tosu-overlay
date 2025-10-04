---
generator: doxide
---


# Inter-process communication



## Types

| Name | Description |
| ---- | ----------- |
| [ipc](ipc.md) | IPC Shared Memory struct  |

## Functions

| Name | Description |
| ---- | ----------- |
| [ipc_destroy](#ipc_destroy) | Destroy the IPC shared memory  |
| [ipc_init](#ipc_init) | Initialize the IPC shared memory :material-location-enter: `key` :    The key to use for the shared memory :material-location-enter: `callback` :    The callback to call when data is received :material-location-enter: `read_only` :    If true, only read the shared memory, not create it :material-keyboard-return: **Return** :    0 on success  |

## Function Details

### ipc_destroy<a name="ipc_destroy"></a>
!!! function "void ipc_destroy()"

    Destroy the IPC shared memory
    

### ipc_init<a name="ipc_init"></a>
!!! function "int ipc_init(int key, ipc_callback_t callback, bool read_only)"

    Initialize the IPC shared memory
    
    
    :material-location-enter: `key`
    :    The key to use for the shared memory
        
    :material-location-enter: `callback`
    :    The callback to call when data is received
        
    :material-location-enter: `read_only`
    :    If true, only read the shared memory, not create it
        
    :material-keyboard-return: **Return**
    :    0 on success
    

