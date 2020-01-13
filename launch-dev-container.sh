#!/bin/bash

docker run --hostname builder -it -v /Users/spanou/development/cpp/embedded_time_date:/development  spanou/systemc-dev:min
