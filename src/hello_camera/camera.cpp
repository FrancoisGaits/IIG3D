#include "camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtc/quaternion.hpp"
#include "gtx/norm.hpp"
#include "gtc/matrix_transform.hpp"

/*------------------------------------------------------------------------------------------------------------------------*/

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 look, float zoom) : _position(position), _front(look-position), _up(up), _zoom(zoom) {

}

Camera::~Camera() {

}

glm::mat4 Camera::viewmatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::processkeyboard(Camera_Movement , GLfloat ) {

}

void Camera::processmouseclick(int button, GLfloat xpos, GLfloat ypos) {
    _mousebutton = button;
    _mousestartx = xpos;
    _mousestarty = ypos;
}

void Camera::processmousemovement(int , GLfloat , GLfloat , GLboolean ) {

}

void Camera::processmousescroll(GLfloat ) {

}

float &Camera::zoom() {
    return _zoom;
}

glm::vec3 &Camera::position() {
    return _position;
}

void Camera::setviewport(glm::vec4 viewport) {
    _viewport = viewport;
}


/*------------------------------------------------------------------------------------------------------------------------*/

// A camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
// Constructor with vectors
EulerCamera::EulerCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Camera(position, up, glm::vec3(0.0f, 0.0f, -1.0f), ZOOM), _movementspeed(SPEED), _mousesensitivity(SENSITIVTY) {
    _worldup = _up;
    _yaw = yaw;
    _pitch = pitch;
    updatecameravectors();
}

EulerCamera::~EulerCamera() {

}


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void EulerCamera::processkeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = _movementspeed * deltaTime;
    if (direction == FORWARD)
        _position += _front * velocity;
    if (direction == BACKWARD)
        _position -= _front * velocity;
    if (direction == LEFT)
        _position -= _right * velocity;
    if (direction == RIGHT)
        _position += _right * velocity;
}


// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void EulerCamera::processmousemovement(int button, GLfloat xpos, GLfloat ypos, GLboolean constrainPitch) {
    (void)button;
    float xoffset = xpos - _mousestartx;
    float yoffset = _mousestarty - ypos;
    _mousestartx = xpos;
    _mousestarty = ypos;
    xoffset *= _mousesensitivity;
    yoffset *= _mousesensitivity;
    _yaw   += xoffset;
    _pitch += yoffset;
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }
    // Update Front, Right and Up Vectors using the updated Eular angles
    updatecameravectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void EulerCamera::processmousescroll(GLfloat yoffset) {
    if (_zoom >= 1.0f && _zoom <= 45.0f)
        _zoom -= yoffset;
    if (_zoom <= 1.0f)
        _zoom = 1.0f;
    if (_zoom >= 45.0f)
        _zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void EulerCamera::updatecameravectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = std::cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldup));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up    = glm::normalize(glm::cross(_right, _front));
}


/*------------------------------------------------------------------------------------------------------------------------*/
/*                                            Trackball Camera                                                            */
/*------------------------------------------------------------------------------------------------------------------------*/


TrackballCamera::TrackballCamera(glm::vec3 position, glm::vec3 up, glm::vec3 center):
                    Camera(position, glm::normalize(up), center, ZOOM),
                    _movementspeed(1.0f), _mousesensitivity(1.0) {
    _radius = glm::length(_front);
    _front *= 1.f/_radius;
}

TrackballCamera::~TrackballCamera() {

}

void TrackballCamera::processkeyboard(Camera_Movement direction, GLfloat deltaTime) {
    (void)direction;
    (void)deltaTime;
}

void TrackballCamera::processmouseclick(int button, GLfloat xpos, GLfloat ypos) {
    Camera::processmouseclick(button, xpos, ypos);
    switch (_mousebutton) {
        case 0:
            // rotate
            _rotstart = getmouseprojectiononball(xpos, ypos);
            _rotend = _rotstart;
        break;
        case 1:
            // pan
            _panstart = getmouseonscreen(xpos, ypos);
            _panend = _panstart;
        break;
        case 2:
            // zoom
        break;
        default:
        break;
    }
}

void TrackballCamera::processmousemovement(int button, GLfloat xpos, GLfloat ypos, GLboolean constrainPitch) {
    (void)button;
    (void)constrainPitch;
    switch (_mousebutton) {
        case 0:
            // rotate
            _rotend = getmouseprojectiononball(xpos, ypos);
            rotatecamera();
        break;
        case 1:
            // pan
            _panend = getmouseonscreen(xpos, ypos);
            pancamera();
        break;
        case 2:
            // zoom
        break;
        default:
        break;
    }
}

void TrackballCamera::processmousescroll(GLfloat yoffset) {
    (void)yoffset;
}


#define   SQRT1_2  0.7071067811865476

glm::vec3 TrackballCamera::getmouseprojectiononball(float xpos, float ypos){
    glm::vec3 mouseonball = glm::vec3(
                (xpos - _viewport.z * 0.5f) / (_viewport.z * 0.5f),
                (_viewport.w * 0.5f - ypos) / (_viewport.w * 0.5f),
                0.0f
                );
    float length = glm::length(mouseonball);

    length = (length<1.0f) ? length : 1.0f;
    mouseonball.z = std::sqrt(1-length*length);
    mouseonball = glm::normalize(mouseonball);
    return mouseonball;
}

glm::vec2 TrackballCamera::getmouseonscreen(float xpos, float ypos) {
    return glm::vec2(
                (xpos - _viewport.z * 0.5f) / (_viewport.z * 0.5f),
                (ypos - _viewport.w * 0.5f) / (_viewport.w * 0.5f)
                );
}

void TrackballCamera::rotatecamera() {
    glm::vec3 direction = _rotend - _rotstart;
    float velocity = glm::length(direction);
    if (velocity > 0.0001) {
        glm::vec3 axis = glm::cross(_rotend, _rotstart);
        float length = glm::length(axis);
        axis = glm::normalize(axis);

        float angle = std::atan2(length, glm::dot(_rotstart, _rotend));

        glm::quat quaternion = glm::angleAxis(angle, axis);

        glm::vec3 center = _position + _front*_radius;
        _front = glm::normalize(glm::rotate( quaternion ,  _front));
        _up =  glm::normalize(glm::rotate(quaternion , _up));
        _position = center-_front*_radius;
        _rotstart=_rotend;
    }
}

void TrackballCamera::pancamera() {
    glm::vec2 mov = _panend - _panstart;
    if (glm::length(mov) != 0.0f) {
        mov *= _mousesensitivity*_movementspeed;
        glm::vec3 pan = glm::cross(_up,_front) * mov.x + _up * mov.y;
        _position += pan;
        _panstart = _panend;
    }
}


