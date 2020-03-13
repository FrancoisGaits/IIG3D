#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(std::string& path, glm::vec3 color): _color{color} {

    //mesh = loadMesh(path);

    const float c1 = 1.f/sqrtf(1 + 1.61803398875f*1.61803398875f);
    const float c2 = 1.61803398875f*c1;

    mesh.vertices = {
            c1 , c2 , 0,
            c2 , 0, c1,
            0, c1, c2,

            -c1, c2 , 0,
            0, c1, -c2,
            c2, 0, -c1,

            c1, -c2, 0,
            0, -c1, c2,
            -c2, 0, c1,

            -c2, 0, -c1,
            0, -c1, -c2,
            -c1, -c2, 0
    };

    mesh.normals = mesh.vertices;

    mesh.indices = {
            0, 2, 1,
            0, 1, 5,
            0, 5, 4,
            0, 4, 3,
            0, 3, 2,

            11, 10, 6,
            11, 6, 7,
            11, 7, 8,
            11, 8, 9,
            11, 9, 10,

            1, 7, 6,
            5, 6, 10,
            4, 10, 9,
            3, 9, 8,
            2, 8, 7,

            10, 4, 5,
            9, 3, 4,
            8, 2, 3,
            7, 1, 2,
            6, 5, 1
    };

    mesh.load();
}

std::string Model::infoString() const {
    std::stringstream mess;
    mess << "Model :" << std::endl;
    mess << "    -triangles : " << mesh.nbTriangles() << std::endl;
    return  mess.str();
}

Mesh Model::loadMesh(std::string &path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    Mesh tmpMesh;

    path = "../src/Objects/"+path;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

    if (!err.empty()) {
        std::cout << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Couldn't read object " << path << std::endl;
        return tmpMesh;
    }

    for (size_t k = 0; k < attrib.vertices.size(); k += 3) {
        std::cout << attrib.vertices[k + 0] << " " << attrib.vertices[k + 1] << " " << attrib.vertices[k + 2] << std::endl;
        tmpMesh.addVertex(
                attrib.vertices[k + 0],
                attrib.vertices[k + 1],
                attrib.vertices[k + 2]);
    }

    for (size_t k = 0; k < attrib.normals.size(); k += 3) {
        std::cout << attrib.normals[k + 0] << " " << attrib.normals[k + 1] << " " << attrib.normals[k + 2] << std::endl;
        tmpMesh.addNormal(
                attrib.normals[k + 0],
                attrib.normals[k + 1],
                attrib.normals[k + 2]);
    }

    for (size_t i = 0; i < shapes.size(); i++) {
        // face
        int face_index = 0;
        for (size_t k = 0; k < shapes[i].mesh.indices.size(); k += shapes[i].mesh.num_face_vertices[face_index++]) {
            unsigned char v_per_f = shapes[i].mesh.num_face_vertices[face_index];
            std::vector<int> face;
            for(int l = 0; l < v_per_f; l++){
                const tinyobj::index_t& ref = shapes[i].mesh.indices[k + l];
                face.emplace_back(ref.vertex_index + 1);
            }
            std::cout << face[0] << " " << face[1] << " " << face[2] << std::endl;
            //mesh.addTri(face[0],face[1],face[2]);
            tmpMesh.addTri(0,1,2);
        }
    }

    return tmpMesh;
}

void Model::draw() const {
    std::cout << std::endl;
    std::cout << mesh.nbVertices();
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
