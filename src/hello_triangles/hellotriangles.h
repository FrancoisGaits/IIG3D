#ifndef SIMPLETRIANGLE_H
#define SIMPLETRIANGLE_H

#include "opengldemo.h"

/** Simple drawing demonstration
 */
class SimpleTriangle : public OpenGLDemo {
public:
    explicit SimpleTriangle(int width, int height);
    ~SimpleTriangle() override;

    void draw() override;

    bool keyboard(unsigned char k) override;
private:
    // A simple geometry
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    // OpenGL object for geometry
    // Vertex Array Buffer
    GLuint _vao;
    // Vertex Buffer Object
    GLuint _vbo;
    // Normal buffer
    GLuint _nbo;
    // Face buffer
    GLuint _ebo;

    // Shader program for rendering
    GLuint _program;

    // Different availableprograms
    GLuint _programcolor;
    GLuint _programnormal;

};

#endif // SIMPLETRIANGLE_H
