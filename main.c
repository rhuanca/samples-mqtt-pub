#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <memory.h>
#include <time.h>

#include "MQTTClient.h"
#include "transport.h"

void usage() {
    printf("MQTT stdout subscriber\n");
    printf("Usage: stdoutsub topicname <options>, where options are:\n");
    printf("  --host <hostname> (default is localhost)\n");
    printf("  --port <port> (default is 1883)\n");
    printf("  --qos <qos> (default is 2)\n");
    printf("  --clientid <clientid> (stdout-subscriber)\n");
    printf("  --username none\n");
    printf("  --password none\n");
    printf("  --topic <topic> (default is test_topic)\n");
    printf("  --message <message>\n");
    exit(-1);
}

void cfinish(int sig) {
    signal(SIGINT, NULL);
}

void stop_init(void) {
    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);
}

struct opts_struct {
    char *clientid;
    enum QoS qos;
    char *username;
    char *password;
    char *host;
    int port;
    char *topic;
    char *message;
    int repeat;
} opts =
        {
                (char *) "stdout-subscriber", QOS2, NULL, NULL, (char *) "localhost", 1883,
                (char *) "test_topic", "test_message" , 1
        };


void getopts(int argc, char **argv) {
    int count = 1;

    while (count < argc) {
        if (strcmp(argv[count], "--qos") == 0) {
            if (++count < argc) {
                if (strcmp(argv[count], "0") == 0)
                    opts.qos = QOS0;
                else if (strcmp(argv[count], "1") == 0)
                    opts.qos = QOS1;
                else if (strcmp(argv[count], "2") == 0)
                    opts.qos = QOS2;
                else
                    usage();
            }
            else
                usage();
        }
        else if (strcmp(argv[count], "--host") == 0) {
            if (++count < argc)
                opts.host = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--port") == 0) {
            if (++count < argc)
                opts.port = atoi(argv[count]);
            else
                usage();
        }
        else if (strcmp(argv[count], "--clientid") == 0) {
            if (++count < argc)
                opts.clientid = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--username") == 0) {
            if (++count < argc)
                opts.username = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--password") == 0) {
            if (++count < argc)
                opts.password = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--topic") == 0) {
            if (++count < argc)
                opts.topic = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--message") == 0) {
            if (++count < argc)
                opts.message = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--repeat") == 0) {
            if (++count < argc)
                opts.repeat = atoi(argv[count]);
            else
                usage();
        }
        count++;
    }

}

int main(int argc, char **argv) {
    unsigned char buf[100];
    int buf_len = sizeof(buf);
    int len = 0;
    int mqtt_sock_fd = 0;
    int message_len = 0;
    int counter = 0;
    int rc = 0;
    double time_spent;


    clock_t begin, end;
    MQTTString topicString = MQTTString_initializer;

    stop_init();

    if (argc < 1)
        usage();

    getopts(argc, argv);
    printf("Arguments:\n");
    printf("  host: %s\n", opts.host);
    printf("  port: %d\n", opts.port);
    printf("  topic: %s\n", opts.topic);
    printf("  message: %s\n", opts.message);
    printf("  repeat: %d\n", opts.repeat);

    printf("\n");
    printf("\n");

    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);

    // open socket
    mqtt_sock_fd = transport_open(opts.host, opts.port);

    if (mqtt_sock_fd < 0)
        return mqtt_sock_fd;



    // connect
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = opts.clientid;
    data.username.cstring = opts.username;
    data.password.cstring = opts.password;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    printf("Connecting to %s %d\n", opts.host, opts.port);
    len = MQTTSerialize_connect(buf, buf_len, &data);
    rc = transport_sendPacketBuffer(mqtt_sock_fd, buf, len);

    printf("Connection successful\n");
    topicString.cstring = opts.topic;
    message_len = strlen(opts.message);


    printf("Sending data...");

    // check time
    begin = clock();

    for(counter=0; counter<opts.repeat; counter++) {
        // publish
        len = MQTTSerialize_publish(buf, buf_len, 0, 0, 0, 0, topicString, opts.message, message_len);
        rc = transport_sendPacketBuffer(mqtt_sock_fd, buf, len);
    }

    // check time
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time spent:\n");
    printf("  secs: %.1f\n", time_spent);
    printf("  clock ticks: %.2f\n", (double)(end - begin));

    // disconnect
    len = MQTTSerialize_disconnect(buf, buf_len);
    rc = transport_sendPacketBuffer(mqtt_sock_fd, buf, len);

    transport_close(mqtt_sock_fd);
    return 0;
}
