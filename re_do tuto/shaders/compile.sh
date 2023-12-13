#!/bin/bash

cd "$(dirname "$0")"

/usr/local/bin/glslc ./shader.vert -o shader.vert.spv
/usr/local/bin/glslc ./shader.frag -o shader.frag.spv