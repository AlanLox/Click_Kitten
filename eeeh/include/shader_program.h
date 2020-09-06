#pragma once

#include <initializer_list> // initializer_list
#include <string>           // string
#include <utility>          // pair

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ===============
//    КОНСТАНТЫ
// ===============

const char* const TITLE = "Starfield";

constexpr size_t FPS_INTERVAL = 5;

constexpr GLfloat BACKGROUND[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

constexpr size_t POINT_COUNT = 1000;

constexpr GLfloat MIN_SPEED = 0.1f;
constexpr GLfloat MAX_SPEED = 0.5f;

constexpr GLfloat MIN_COLOR = 0.5f;
constexpr GLfloat MAX_COLOR = 1.0f;

constexpr GLfloat START_SIZE = 1;
constexpr GLfloat END_SIZE = 3;

constexpr GLfloat START_OPACITY = 0.0f;
constexpr GLfloat END_OPACITY = 1.0f;

constexpr GLfloat PI = 3.14159f;

// ============
//    МОДЕЛЬ
// ============

// GLfloat velocityX;   vec2 velocity
// GLfloat velocityY;   vec2 velocity
// GLfloat startTime;   float startTime
// GLfloat colorR;      vec3 color
// GLfloat colorG;      vec3 color
// GLfloat colorB;      vec3 color

const auto POINT_SHIFT_VELOCITY = nullptr;
const auto POINT_SHIFT_START_TIME = reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat));
const auto POINT_SHIFT_COLOR = reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat));

constexpr size_t POINT_FIELDS = 6;
constexpr size_t POINT_SIZE = POINT_FIELDS * sizeof(GLfloat);

// =======================
//    ОБРАБОТЧИК КЛАВИШ
// =======================

void DefaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// =========================
//    ШЕЙДЕРНАЯ ПРОГРАММА
// =========================

struct ShaderProgram {
    const GLuint id;

    ShaderProgram(std::initializer_list<std::pair<std::string, GLenum>> shaders);

    ShaderProgram(const ShaderProgram&) = delete;

    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram();
//добавил юз
void Use();
};
