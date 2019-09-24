#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib-2.0/gio/gio.h>
#include <libnm/NetworkManager.h>
#include <string.h>

static void
show_conn (NMConnection *conn)
{
    NMSettingConnection *s_con;
    guint64 ts;
    char *ts_str;
    char ts_real_str[64];
    const char *val1, *val2, *val3, *val4, *val5;

    s_con = nm_connection_get_setting_connection (conn);

    if (s_con) {
        ts = nm_setting_connection_get_timestamp (s_con);
        ts_str = g_strdup_printf ("%" G_GUINT64_FORMAT, ts);
        strftime (ts_real_str, sizeof (ts_real_str), "%c", localtime ((time_t *) &ts));

        val1 = nm_setting_connection_get_id (s_con);
        val2 = nm_setting_connection_get_uuid (s_con);
        val3 = nm_setting_connection_get_connection_type (s_con);
        val4 = nm_connection_get_path (conn);
        val5 = ts ? ts_real_str : "never";

        printf ("%-25s | %s | %-15s | %-43s | %s\n",
            val1,
            val2,
            val3,
            val4,
            val5);

            g_free (ts_str);
    }
}

int
main(int argc, char **argv) {
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

    conns = nm_client_get_connections (client);
    printf("Connections:\n============\n");

    for (i = 0; i < conns->len; i++) {
                show_conn (conns->pdata[i]);
    };

    g_object_unref (client);

    return EXIT_SUCCESS;
}