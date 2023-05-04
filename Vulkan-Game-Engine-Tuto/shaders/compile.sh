#!/bin/bash

cd "$(dirname "$0")"

/usr/local/bin/glslc ./simple_shader.vert -o simple_shader.vert.spv
/usr/local/bin/glslc ./simple_shader.frag -o simple_shader.frag.spv
