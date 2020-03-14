#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(std::string& path, glm::vec3 color, unsigned div): _color{color} {

    mesh = loadMesh(path, div);


    mesh.load();
}

std::string Model::infoString() const {
    std::stringstream mess;
    mess << "Model :" << std::endl;
    mess << "    -triangles : " << mesh.nbTriangles() << std::endl;
    return  mess.str();
}

Mesh Model::loadMesh(std::string &path, unsigned div) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    Mesh tmpMesh;

    path = "../src/Objects/"+path;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

    if (!ret) {
        std::cerr << "Couldn't read object " << path << std::endl;
        return tmpMesh;
    } else if (shapes.size() != 1){
        std::cerr << "Too many shapes, the mesh can only be triangles" << std::endl;
        return tmpMesh;
    }

    for (long unsigned s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        int ind = 0;
        for (long unsigned f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];

                tmpMesh.addVertex(vx/div,vy/div,vz/div);
                tmpMesh.addNormal(nx,ny,nz);

            }
            tmpMesh.addTri(ind++,ind++,ind++);
            index_offset += fv;
        }
    }

    return tmpMesh;
}

void Model::draw() const {
    mesh.draw();
}

const glm::vec3 &Model::color() const {
    return _color;
}

const glm::mat4 &Model::model() const {
    return _model;
}

void Model::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}
