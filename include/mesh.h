#include <GL/gl.h>


class Mesh{
public:
    Mesh();
    void draw();

private:
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    
};