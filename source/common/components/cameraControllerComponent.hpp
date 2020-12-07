#ifndef CAMERA_CONTROLLER_COMPONENT_HPP
#define CAMERA_CONTROLLER_COMPONENT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <application.hpp>
#include <cameraComponent.hpp>

using namespace std;

using gameTemp::Application;

class CameraControllerComponent : public Component
{
private:
    Application *app;
    CameraComponent *camera;

    float yaw, pitch;
    glm::vec3 position;

    float yaw_sensitivity, pitch_sensitivity, fov_sensitivity;
    glm::vec3 position_sensitivity;
    float speedup_factor = 5.0f; // A speed multiplier if "Left Shift" is held.

    bool mouse_locked = false; //wether the mouse is inside the game window or not

public:
    CameraControllerComponent(Application *application, Entity *myEntity)
    {
        this->app = application;

        int width, height = 0;
        glm::ivec2 wh = app->getFrameBufferSize();
        width = wh.x;
        height = wh.y;

        camera = myEntity->getCameraComponent();
        camera->setupPerspective(glm::pi<float>() / 2, static_cast<float>(width) / height, 0.1f, 100.0f);
        yaw_sensitivity = pitch_sensitivity = 0.01f;
        position_sensitivity = {3.0f, 3.0f, 3.0f};
        fov_sensitivity = glm::pi<float>() / 10;

        position = camera->getEyePosition();
        auto direction = camera->getDirection();
        yaw = glm::atan(-direction.z, direction.x);
        float base_length = glm::sqrt(direction.x * direction.x + direction.z * direction.z);
        pitch = glm::atan(direction.y, base_length);
    }

    void Initialize(Application *application, Entity *myEntity)
    { //REVIEW: Remove this
        this->app = application;
        int width, height = 0;
        glm::ivec2 wh = app->getFrameBufferSize();
        width = wh.x;
        height = wh.y;

        camera = myEntity->getCameraComponent();
        camera->setupPerspective(glm::pi<float>() / 2, static_cast<float>(width) / height, 0.1f, 100.0f);
        yaw_sensitivity = pitch_sensitivity = 0.01f;
        position_sensitivity = {3.0f, 3.0f, 3.0f};
        fov_sensitivity = glm::pi<float>() / 10;

        position = camera->getEyePosition();
        auto direction = camera->getDirection();
        yaw = glm::atan(-direction.z, direction.x);
        float base_length = glm::sqrt(direction.x * direction.x + direction.z * direction.z);
        pitch = glm::atan(direction.y, base_length);
    }

    void release()
    {
        if (mouse_locked)
        {
            mouse_locked = false;
            app->getMouse().unlockMouse(app->getWindow());
        }
    }

    void update(double delta_time)
    {
        if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
        {
            app->getMouse().lockMouse(app->getWindow());
            mouse_locked = true;
        }
        else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
        {
            app->getMouse().unlockMouse(app->getWindow());
            mouse_locked = false;
        }

        if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
        {
            glm::vec2 delta = app->getMouse().getMouseDelta();
            pitch -= delta.y * pitch_sensitivity;
            yaw -= delta.x * yaw_sensitivity;
        }

        if (pitch < -glm::half_pi<float>() * 0.99f)
            pitch = -glm::half_pi<float>() * 0.99f;
        if (pitch > glm::half_pi<float>() * 0.99f)
            pitch = glm::half_pi<float>() * 0.99f;
        yaw = glm::wrapAngle(yaw);

        float fov = camera->getVerticalFieldOfView() + app->getMouse().getScrollOffset().y * fov_sensitivity;
        fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f);
        camera->setVerticalFieldOfView(fov);

        glm::vec3 front = camera->Forward(), up = camera->Up(), right = camera->Right();

        glm::vec3 current_sensitivity = this->position_sensitivity;
        if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
            current_sensitivity *= speedup_factor;

        if (app->getKeyboard().isPressed(GLFW_KEY_UP))
            position += front * ((float)delta_time * current_sensitivity.z);
        if (app->getKeyboard().isPressed(GLFW_KEY_DOWN))
            position -= front * ((float)delta_time * current_sensitivity.z);
        if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            position += up * ((float)delta_time * current_sensitivity.y);
        if (app->getKeyboard().isPressed(GLFW_KEY_C))
            position -= up * ((float)delta_time * current_sensitivity.y);
        if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
            position += right * ((float)delta_time * current_sensitivity.x);
        if (app->getKeyboard().isPressed(GLFW_KEY_LEFT))
            position -= right * ((float)delta_time * current_sensitivity.x);

        camera->setDirection(glm::vec3(glm::cos(yaw), 0, -glm::sin(yaw)) * glm::cos(pitch) + glm::vec3(0, glm::sin(pitch), 0));
        camera->setEyePosition(position);
    }

    [[nodiscard]] float getYaw() const { return yaw; }
    [[nodiscard]] float getPitch() const { return pitch; }
    [[nodiscard]] glm::vec3 getPosition() const { return position; }

    [[nodiscard]] float getYawSensitivity() const { return yaw_sensitivity; }
    [[nodiscard]] float getPitchSensitivity() const { return pitch_sensitivity; }
    [[nodiscard]] float getFieldOfViewSensitivity() const { return fov_sensitivity; }
    [[nodiscard]] glm::vec3 getPositionSensitivity() const { return position_sensitivity; }
    [[nodiscard]] float getSpeedUpFactor() const { return speedup_factor; }

    void setYaw(float _yaw)
    {
        this->yaw = glm::wrapAngle(_yaw);
    }

    void setPitch(float _pitch)
    {
        const float v = 0.99f * glm::pi<float>() / 2;
        if (_pitch > v)
            _pitch = v;
        else if (_pitch < -v)
            _pitch = -v;
        this->pitch = _pitch;
    }

    void setPosition(glm::vec3 _pos)
    {
        this->position = _pos;
    }

    void setYawSensitivity(float sensitivity) { this->yaw_sensitivity = sensitivity; }
    void setPitchSensitivity(float sensitivity) { this->pitch_sensitivity = sensitivity; }
    void setFieldOfViewSensitivity(float sensitivity) { this->fov_sensitivity = sensitivity; }
    void setPositionSensitivity(glm::vec3 sensitivity) { this->position_sensitivity = sensitivity; }
};

#endif