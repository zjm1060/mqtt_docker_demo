#!/bin/sh

docker rmi mqtt-dev

docker build -t mqtt-dev -f dockerfile.developer .
