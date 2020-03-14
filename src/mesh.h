#ifndef IIG3D_BASETP_MESH_H
#define IIG3D_BASETP_MESH_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>
#include "glm/glm.hpp"

#endif

#include <vector>

class Mesh {
private :
    // OpenGL object for geometry
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;


public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;

    void addQuad(unsigned a, unsigned b, unsigned c, unsigned d);

    void addTri(unsigned a, unsigned b, unsigned c);

    void addVertex(float x, float y, float z);

    void addVertex(glm::vec3& v);

    void addNormal(float x, float y, float z);

    unsigned nbTriangles() const;

    unsigned nbVertices() const;

    unsigned nbNormals() const;

    void load();

    void draw() const;

    void clear();

    ~Mesh();

};


#endif //IIG3D_BASETP_MESH_H
