#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib-2.0/gio/gio.h>
#include <libnm/NetworkManager.h>
#include <NetworkManager.h>

int
main (void) {
    NMClient *client;
    GError *error = NULL;
    const GPtrArray *conns;
    int i;

    if (! (client = nm_client_new (NULL, &error))) {
        g_message("Error: Could NOT connect to NetworkManager: %s.",
            error->message);
        g_error_free (error);
        return EXIT_FAILURE;
    }

    if (! nm_client_get_nm_running (client)) {
        g_message ("Error: Can NOT obtain connections: NetworkManager is not running.");
        return EXIT_FAILURE;
    }

    conns = nm_client_get_active_connections (client);
    printf("Connections:\n============\n");

    for (i = 0; i < (int) conns->len; i++) {
        NMActiveConnection *conn;
        const char *uuid, *type, *id;
        conn = conns->pdata[i];
        const GPtrArray *devices;
        NMDevice *device;

        uuid = nm_active_connection_get_uuid (conn);
        type = nm_active_connection_get_connection_type (conn);
        id = nm_active_connection_get_id (conn);

        devices = nm_active_connection_get_devices (conn);
        device = g_ptr_array_index (devices, 0);

        if (NM_IS_DEVICE_WIFI (device)) {
            printf("UUID: %s | Name: %s | Type: %s\n", uuid, id, type);
        }
    }

    g_object_unref (client);

    return EXIT_SUCCESS;
}
