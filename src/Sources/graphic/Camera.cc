// Copyright 2019
#include "Camera.h"
#include "App.h"
#include "Input.h"

using glm::mat4;
using glm::vec3;
using lucyrt::graphic::Camera;

Camera::Camera() { Reset(); }

mat4 Camera::GetWorldToCamera() const {
  vec3 pos = transform.GetPos();
  mat4 projection = glm::perspective(
      glm::radians(fov), App::GetContext()->GetAspect(), near_plane, far_plane);
  mat4 view = lookAt(pos, pos + transform.GetForward(), transform.GetUp());
  return projection * view;
}

void Camera::Reset() {
  transform = lucyrt::graphic::TransformationMatrix();
  transform.SetPos(vec3(0, 0, 5));
  near_plane = 0.01f;
  far_plane = 1000.0f;
  fov = 60;
  speed = 1;

  yaw = -90;
  pitch = 0;
}

void Camera::Tick() {
  wasd();
  mouse();
}

void Camera::OnGUI() {
  ImGui::Text("Camera");
  if (ImGui::Button("Reset")) {
    Reset();
  }
  vec3 pos = transform.GetPos();
  float campos[3] = {pos.x, pos.y, pos.z};
  if (ImGui::InputFloat3("Position", campos)) {
    transform.SetPos(vec3(campos[0], campos[1], campos[2]));
  }
  ImGui::SliderFloat("FOV", &fov, 30.0f, 150.0f);
  ImGui::SliderFloat("Speed", &speed, 0.1f, 10.0f);
  ImGui::Separator();
}

void Camera::wasd() {
  GLfloat cameraSpeed = App::GetContext()->GetDeltaTime() * speed;
  vec3 pos = transform.GetPos();
  vec3 forward = transform.GetForward();
  vec3 right = transform.GetRight();
  if (Input::GetKey(GLFW_KEY_W) == GLFW_PRESS) pos += cameraSpeed * forward;
  if (Input::GetKey(GLFW_KEY_S) == GLFW_PRESS) pos -= cameraSpeed * forward;
  if (Input::GetKey(GLFW_KEY_Q) == GLFW_PRESS)
    pos += cameraSpeed * vec3(0, 1, 0);
  if (Input::GetKey(GLFW_KEY_E) == GLFW_PRESS)
    pos -= cameraSpeed * vec3(0, 1, 0);
  if (Input::GetKey(GLFW_KEY_A) == GLFW_PRESS) pos -= right * cameraSpeed;
  if (Input::GetKey(GLFW_KEY_D) == GLFW_PRESS) pos += right * cameraSpeed;
  transform.SetPos(pos);
}

void Camera::mouse() {
  if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    GLfloat xoffset = Input::GetMouseX() - lastX;
    GLfloat yoffset = lastY - Input::GetMouseY();
    lastX = Input::GetMouseX();
    lastY = Input::GetMouseY();
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    vec3 front;
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    transform.SetForward(front);
  } else if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
    GLfloat xoffset = Input::GetMouseX() - lastX;
    GLfloat yoffset = lastY - Input::GetMouseY();
    lastX = Input::GetMouseX();
    lastY = Input::GetMouseY();
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    vec3 pos = transform.GetPos();
    vec3 right = transform.GetRight();

    pos += yoffset * (-vec3(0, 1, 0));
    pos += xoffset * (-right);
    transform.SetPos(pos);
  } else {
    lastX = Input::GetMouseX();
    lastY = Input::GetMouseY();
  }
}
