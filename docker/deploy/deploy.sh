#!/bin/sh

docker rmi mqtt-deploy

docker build -t mqtt-deploy -f dockerfile.deploy .
