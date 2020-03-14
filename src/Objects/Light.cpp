#include "Light.h"

Light::Light(LightType type, glm::vec3 position, glm::vec3 color, glm::vec3 focus, float limit) : _type{type},
                                                                                                  _position{position},
                                                                                                  _color{color},
                                                                                                  _focus{focus},
                                                                                                  _limit{limit} {
}

const glm::vec3 &Light::position() const {
    return _position;
}

const glm::vec3 &Light::color() const {
    return _color;
}

const glm::vec3 &Light::focus() const {
    return _focus;
}

float Light::limit() const {
    return _limit;
}

LightType Light::type() const {
    return _type;
}

bool Light::isSpot() const {
    return _type == SPOT;
}

std::string Light::infoString() const {
    std::stringstream mess;
    mess << (isSpot() ? "Spot Light :" : "Point Light :") << std::endl;
    mess << "    -position : [" << position().x << ", " << position().y << ", " << position().z << "]" << std::endl;
    mess << "    -color : { r : " << color().x << ", g : " << color().y << ", b : " << color().z << " }" << std::endl;
    if (isSpot())
        mess << "    -focusing : [" << focus().x << ", " << focus().y << ", " << focus().z << "]" << std::endl;
    return mess.str();
}

