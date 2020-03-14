#include "mesh.h"

void Mesh::addQuad(unsigned a, unsigned b, unsigned c, unsigned d) {
    //triangle inferieur
    indices.emplace_back(a);
    indices.emplace_back(c);
    indices.emplace_back(d);
    //triangle superieur
    indices.emplace_back(a);
    indices.emplace_back(d);
    indices.emplace_back(b);
}

void Mesh::addTri(unsigned a, unsigned b, unsigned c) {
    indices.emplace_back(a);
    indices.emplace_back(b);
    indices.emplace_back(c);
}

void Mesh::addVertex(float x, float y, float z) {
    vertices.emplace_back(x);
    vertices.emplace_back(y);
    vertices.emplace_back(z);
}

void Mesh::addVertex(glm::vec3 &v) {
    vertices.emplace_back(v[0]);
    vertices.emplace_back(v[1]);
    vertices.emplace_back(v[2]);
}

void Mesh::addNormal(float x, float y, float z) {
    normals.emplace_back(x);
    normals.emplace_back(y);
    normals.emplace_back(z);
}

unsigned Mesh::nbTriangles() const {
    return indices.size() / 3;
}

unsigned Mesh::nbVertices() const {
    return vertices.size() / 3;
}

unsigned Mesh::nbNormals() const {
    return normals.size() / 3;
}

void Mesh::load() {
    // Initialize the geometry
    // 1. Generate geometry buffers
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_nbo);
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);
    // 2. Bind Vertex Array Object
    glBindVertexArray(_vao);
    // 3. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    // 4. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    // 5. Copy our normals array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    // 6. Copy our vertices array in a buffer for OpenGL to use
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    // 7. Copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);
    //6. Unbind the VAO
    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::clear() {
    vertices.clear();
    indices.clear();
    normals.clear();
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}
