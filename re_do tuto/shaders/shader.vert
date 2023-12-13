#version 450

//array of colors
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

//array of positions
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

layout(location = 0) out vec3 fragColor; //create our color output variable

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0); //gl_Position is used as output global variable
    fragColor = colors[gl_VertexIndex];
}
