#version 450 //Specifying that we are using GLSL 4.5.0


/*
    vec2 = built-in type in GLSL that contains 2 float values
    We initialize an array of 3 vec2, one for each vertex (e.g. corner)
    of our triangle.
    Hardcoding values in a vertex shader isn't something we typically do,
    usually we pass in our values using a vertex buffer.
*/
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

/*!!!
    The main function is executed one for each vertex we have
    So thinking back to our graphics pipeline, as input, our vertex
    shader will get each vertex from the input assembler stage and 
    then need to outputs a position.

    !!! We do not return the position using the "return" statement.
    In fact, there is a special variable called "gl_Position" to which
    we assign a value to act as our output/ Notice the capital P at "Position".
    gl_Position must be a 4 dimensional vector that maps to our output frame
    buffer image :
        - The Top left corner corresponds to (-1, -1),
        - top right = (1-, 1)
        - bottom left = (-1, 1)
        - bottom right = (1, 1)
    This means that the center is at (0, 0). Comparing to OpenGL, the sign of the Y
    coordinates is flipped.
*/
void main() {
    /*
        assign gl_Position a value
        the built-in gl_VertexIndex variable is a variable that starts at 0 and
        increments everytime our main function is called. So we can index our
        positions array using this variable. It contains the index of the current
        vertex for each time our main function is run.
        Since positions[gl_VertexIndex] is a vec2, it counts for 2 values, so note that
        0.0 is here our 3rd parameter passed to the vec4 constructor.
        The reason for a 4th component is that in subsequent graphics pipeline stages,
        the gl_Position value is turned into a normalized device coordinate by dividing
        the whole vector list by its last component. So in our case, we divide averything by
        1.0. i.e. nothing changes from now but we'll cover why this could be usefull at a 
        later time.
    */
    
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}