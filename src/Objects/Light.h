#ifndef IIG3D_LIGHT_H
#define IIG3D_LIGHT_H

#include <glm/glm.hpp>
#include <sstream>
#include <string>

enum LightType {
    POINT = 0,
    SPOT
};

class Light {
public:
    explicit Light(LightType type, glm::vec3 position, glm::vec3 color, glm::vec3 focus = glm::vec3(0),
                   float limit = .9f);

    const glm::vec3 &position() const;
    const glm::vec3 &color() const;
    const glm::vec3 &focus() const; //L'endroit vers lequel le spot est braqué

    //Definit cos(angle max) entre la direction de la lumière vers le fragment et la direction du spot
    float limit() const;
    LightType type() const;

    bool isSpot() const;

    std::string infoString() const;

private:
    LightType _type;
    glm::vec3 _position;
    glm::vec3 _color;

    glm::vec3 _focus;
    float _limit;
};


#endif
