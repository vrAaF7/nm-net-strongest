#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <glib-2.0/glib.h>
#include <glib-2.0/gio/gio.h>

#include <libnm/NetworkManager.h>
#include <NetworkManager.h>

#include <string.h>

int
main (void) {
    NMClient *client;
    GError *error = NULL;
    const GPtrArray *conns;
    guint i;

    if (! (client = nm_client_new (NULL, &error))) {
        g_message ("Error: Could NOT connect to NetworkManager: %s.",
            error->message);
        g_error_free (error);
        return EXIT_FAILURE;
    }

    if (! nm_client_get_nm_running (client)) {
        g_message ("Error: Can NOT obtain connections: NetworkManager is not running.");
        return EXIT_FAILURE;
    }

    conns = nm_client_get_active_connections (client);
    printf ("Connections:\n============\n");

    for (i = 0; i < conns->len; i++) {
        NMActiveConnection *conn;
        const char *uuid, *id;
        const GPtrArray *devices;
        NMDevice *device;
        const GPtrArray *accessPoints;
        guint ap;

        conn = conns->pdata[i];
        uuid = nm_active_connection_get_uuid (conn);
        id = nm_active_connection_get_id (conn);

        devices = nm_active_connection_get_devices (conn);
        device = g_ptr_array_index (devices, 0);

        if (NM_IS_DEVICE_WIFI (device)) {
            nm_device_wifi_request_scan(
                ((NMDeviceWifi *) device),
                NULL,
                NULL);

            printf("Sleeping to allow NetworkManager to update.\n");
            sleep(2); /* we should wait for NM to update */

            accessPoints = nm_device_wifi_get_access_points (NM_DEVICE_WIFI (device));

            for (ap = 0; ap < accessPoints->len; ap++) {
                NMAccessPoint *accessPoint;
                GBytes *accessPoint_SSID_GB;
                gsize len;
                guint8 *bytes;
                char *accessPoint_SSID;

                accessPoint = accessPoints->pdata[ap];
                accessPoint_SSID_GB = nm_access_point_get_ssid (accessPoint);
                if (accessPoint_SSID_GB == NULL) {
                    /* this is OK to have as a value
                     *
                     * we just silently handle it, and
                     * iterate to the next element in
                     * the array. */
                    continue;
                }

                bytes = (guint8*)g_bytes_unref_to_data (accessPoint_SSID_GB, &len);
                accessPoint_SSID = nm_utils_ssid_to_utf8 (bytes, len);

                printf ("SSID: %s | UUID: %s | ID: %s | WAP BSSID: %s\n",
                        accessPoint_SSID,
                        uuid,
                        id,
                        nm_access_point_get_bssid (accessPoint));
            }

        }

    }

    g_object_unref (client);

    return EXIT_SUCCESS;
}
