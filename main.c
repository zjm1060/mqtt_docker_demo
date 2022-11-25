/*
 * main.c
 *
 *  Created on: Nov 25, 2022
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mosquitto.h>

#define MQTT_TOPIC_DEMO	"/mqtt_demo/in_docker/test"

static void message(struct mosquitto *mosq, void *user,
    const struct mosquitto_message *msg)
{
	char *buf, *end;
	unsigned n;

	if (!strcmp(msg->topic, MQTT_TOPIC_DEMO)) {
		printf("message: %.*s\n", msg->payloadlen, (char *)msg->payload);
	}
}

static void connected(struct mosquitto *mosq, void *data, int result)
{
	int res;

	if (result) {
		fprintf(stderr, "connect failed: %d\n", result);
		exit(1);
	}

	res = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC_DEMO, 1);
	if (res < 0) {
		fprintf(stderr, "mosquitto_subscribe: %d\n", res);
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	struct mosquitto *mosq = NULL;
	time_t now = 0;

	mosquitto_lib_init();

	mosq = mosquitto_new(0, true, NULL);

	mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	mosquitto_connect_callback_set(mosq, connected);
	mosquitto_message_callback_set(mosq, message);

	now = time(0);

	while(1){
		mosquitto_loop(mosq, 1000, 1);
		if(time(0) - now >= 5){
			mosquitto_publish(mosq, NULL, MQTT_TOPIC_DEMO"/recording_start", 3, "yes", 0, false);

			now = time(0);
		}
	}

	mosquitto_lib_cleanup();

	return 0;
}

