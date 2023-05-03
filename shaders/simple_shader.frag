#version 450

/*
    Unlike the vertex shader, there is not a built-in output
    variable, so we need declare it ourselves. We do so with
    layout.
    First, we have the layout qualifier, which takes a location value,
    best on how we setup the graphics pipeline, a fragment shaderis
    capable of outputting to multiple different locations, for now,
    we only using location 0.
    Next, we specify that this variable is to be used as an output
    using the "out" qualifier.
    Finally, we declare the type and the name of the variable.
*/

layout (location = 0) out vec4 outColor;


void main() {
    /*
        outColor will contains the red, green, blue and alpha channels
        in this order.
        The value of each parameter needs to to be on a zero to one range.
        So in this case, we have full red value, no green, no blue and alpha
        at 1 to be fully opaque.
        The fragment shader runs on a per-fragment basis, which are determined by
        what pixels are geometry mostly contains during the rasterization stage.
    */
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}