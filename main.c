#include <stdio.h>
#include <stdlib.h>


#include <signal.h>
#include <memory.h>

#include "MQTTClient.h"
#include "transport.h"


void usage()
{
    printf("MQTT stdout subscriber\n");
    printf("Usage: stdoutsub topicname <options>, where options are:\n");
    printf("  --host <hostname> (default is localhost)\n");
    printf("  --port <port> (default is 1883)\n");
    printf("  --qos <qos> (default is 2)\n");
    printf("  --delimiter <delim> (default is \\n)\n");
    printf("  --clientid <clientid> (default is hostname+timestamp)\n");
    printf("  --username none\n");
    printf("  --password none\n");
    printf("  --showtopics <on or off> (default is on if the topic has a wildcard, else off)\n");
    exit(-1);
}

void cfinish(int sig)
{
    signal(SIGINT, NULL);
}


void stop_init(void)
{
    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);
}

struct opts_struct
{
    char* clientid;
    int nodelimiter;
    char* delimiter;
    enum QoS qos;
    char* username;
    char* password;
    char* host;
    int port;
    char* topic;
} opts =
        {
                (char*)"stdout-subscriber", 0, (char*)"\n", QOS2, NULL, NULL, (char*)"localhost", 1883, (char*) "test_topic"
        };


void getopts(int argc, char** argv)
{
    int count = 1;

    while (count < argc)
    {
        if (strcmp(argv[count], "--qos") == 0)
        {
            if (++count < argc)
            {
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
        else if (strcmp(argv[count], "--host") == 0)
        {
            printf(">>> reading...host...\n");
            if (++count < argc)
                opts.host = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--port") == 0)
        {
            if (++count < argc)
                opts.port = atoi(argv[count]);
            else
                usage();
        }
        else if (strcmp(argv[count], "--clientid") == 0)
        {
            if (++count < argc)
                opts.clientid = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--username") == 0)
        {
            if (++count < argc)
                opts.username = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--password") == 0)
        {
            if (++count < argc)
                opts.password = argv[count];
            else
                usage();
        }
        else if (strcmp(argv[count], "--delimiter") == 0)
        {
            if (++count < argc)
                opts.delimiter = argv[count];
            else
                opts.nodelimiter = 1;
        }
        else if (strcmp(argv[count], "--topic") == 0)
        {
            if (++count < argc)
                opts.topic = argv[count];
            else
                usage();
        }
        count++;
    }

}

int main(int argc, char** argv)
{
    int rc = 0;
    unsigned char buf[100];
    unsigned char readbuf[100];
    int buflen = sizeof(buf);

    int len = 0;

    char* payload = "hola de nuevo....ops...111.. este mensage no quiere ser publicado...";
    int payloadlen = strlen(payload);
    int mysock = 0;

    stop_init();

    MQTTString topicString = MQTTString_initializer;


    if (argc < 2)
        usage();

    getopts(argc, argv);

    printf("host: %s\n", opts.host );
    printf("topic: %s\n", opts.topic);

    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);

    mysock = transport_open(opts.host, opts.port);


    if(mysock < 0)
        return mysock;


    printf("hello----------------2\n");


    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = opts.clientid;
    data.username.cstring = opts.username;
    data.password.cstring = opts.password;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    len = MQTTSerialize_connect(buf, buflen, &data);
    rc = transport_sendPacketBuffer(mysock, buf, len);


    printf("Connecting to %s %d\n", opts.host, opts.port);


    topicString.cstring = "test";

    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);

    printf("len = %d\n", len);
    printf("mysock = %d\n", mysock);


    rc = transport_sendPacketBuffer(mysock, buf, len);

    printf("result 1 = %d\n", rc);

    len = MQTTSerialize_disconnect(buf, buflen);
    rc = transport_sendPacketBuffer(mysock, buf, len);

    printf("result 2 = %d\n", rc);

    transport_close(mysock);


    return 0;
}
