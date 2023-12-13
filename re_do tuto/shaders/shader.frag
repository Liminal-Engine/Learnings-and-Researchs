#version 450

/*
There is not built-in variable to ouput the color so we must create it.
layout(location = 0) : specify the index of the framebuffer.
*/
layout(location = 0) out vec4 outColor;

/*
    Match the vertex shader fragColor var to our variable
    They don't need to use the same name since they will be
    linked by the "location" directive.
*/
layout(location = 0) in vec3 fragColor;

void main() {
    outColor = vec4(fragColor, 1.0); //add alpha channel
}