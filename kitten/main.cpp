#include <iostream> // cout
#include <stdexcept> // runtime_error
#include <GL/glew.h> // Библиотека GLEW и OpenGL
#include <GLFW/glfw3.h> // Библиотека GLFW
#include <shader_program.h>
#include <math.h>
#include <initializer_list>
#include <utility>
using namespace std;


double clamp(const double x, const double min, const double max) {
	if (x < min) return min; if (max < x) return max; return x;
}

// Функция, которая будет обрабатывать ошибки GLFW.
void ErrorCallback(const int code, const char* const error) {
	throw runtime_error(error);
}

void KeyCallback(GLFWwindow* const window, const int key, const int scancode, const int action, const int mods) {
	// При нажатии на Escape устанавливаем у окна флаг, что оно должно быть закрыто. 
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

double X, Y;

void CursorPositionCallback(GLFWwindow* const window, const double x, const double y) {
	int width = 0; int height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	X = clamp(x / width, 0, 1);
	Y = clamp(y / height, 0, 1);
}


void genEllipseAngles(GLfloat* arr, size_t size, GLfloat a, GLfloat b) {
	for (size_t i = 0; i < size; ++i)
		arr[i] = 2.0 * 3.1415926 * float(i) / float(size) + PI/2;
}


void genEllipse(GLfloat* arr, size_t count, GLfloat a, GLfloat b, GLfloat cx, GLfloat cy) {
	for (size_t i = 0; i < count; ++i) {
		float angle = 2.0 * 3.1415926 * float(i) / float(count);
		arr[2 * i] = cx + a * cos(angle);
		arr[2 * i + 1] = cy + b * sin(angle);
	}
	arr[2 * count - 2] = arr[0];
	arr[2 * count - 1] = arr[1];
}

void genParabola(GLfloat* parab, size_t count, GLfloat length, GLfloat width, GLfloat cx, GLfloat cy) {
	GLfloat k = length / (width * width);
	for (size_t i = count / 2, i2 = count / 2 ; i >= 0 && i2 < count; --i, ++i2) {
		GLfloat x = width / (count / 2) * (i2 - (count / 2));
		parab[2 * i] = -x + cx;
		parab[2 * i + 1] = k * x * x + cy;

		x = width / (count / 2) * ((count / 2) - i);
		parab[2 * i2] = x + cx;
		parab[2 * i2 + 1] = k * x * x + cy;
	}
	parab[0] = -width + cx;
	parab[1] = length + cy;
	parab[2 * count - 2] = width + cx;
	parab[2 * count - 1] = length + cy;
}

void genHeart(GLfloat* arr, size_t count, GLfloat a, GLfloat b, GLfloat cx, GLfloat cy) {
	for (size_t i = 0; i < count; ++i) {
		float angle = 2.0 * 3.1415926 * float(i) / float(count) + PI/2;
		arr[2 * i] = cx + a * cos(angle);
		arr[2 * i + 1] = cy + b * (sin(angle) + sqrt(fabs(cos(angle))));
	}
	arr[2 * count - 2] = arr[0];
	arr[2 * count - 1] = arr[1];
}



void baseColor(GLfloat* res, GLfloat* figure, size_t size, GLfloat R, GLfloat G, GLfloat B) {
	for (int i = 0; i < 2 * size - 2; i = i + 5) { //???
		res[i] = figure[i / 2];
		res[i + 1] = figure[i / 2 + 1];
		res[i + 2] = R;
		res[i + 3] = G;
		res[i + 4] = B;
	}
}


void createVBO(GLfloat* res, size_t resSize, initializer_list<pair<GLfloat*, size_t>> figures) {
	auto it = figures.begin();
	size_t j = 0;
	while (it != figures.end()) {
		for (size_t i = 0; i < it->second && j < resSize; ++i, ++j)
			res[j] = it->first[i];
		++it;
	}
}


bool onNose(GLFWwindow* const window, GLfloat X, GLfloat Y) {
	GLfloat width1 = -0.08 / 2 + 0.5; //0.46
	GLfloat width2 = 0.08 / 2 + 0.5; //0.54
	GLfloat length1 = 1.0 - (-0.33 / 2 + 0.5); //0.37
	GLfloat length2 = 1.0 - (-0.26 / 2 + 0.5); //0.335

	if (Y <= length1 && Y >= length2) {
		GLfloat s = (length1 - Y) / (length1 - length2) * (width2 - width1) / 2;
		if ((X <= (s + 0.5)) && (X >= (0.5 - s)))
			return true;
	}
	return false;
}

bool onForehead(GLFWwindow* const window, GLfloat X, GLfloat Y) {
	GLfloat width1 = -0.15 / 2 + 0.5; //0.425
	GLfloat width2 = 0.15 / 2 + 0.5; //0.575
	GLfloat length1 = 1.0 - (0.0 / 2 + 0.5); //0.5
	GLfloat length2 = 1.0 - (0.5 / 2 + 0.5); //0.25
	if (Y <= length1 && Y >= length2 && X <= width2 && X >= width1)  return true;
	return false;
}

//ДЛИНА И ШИРИНА ЭЛЛИПСА ТРАЕКТОРИИ!!!
void centerPos(GLFWwindow* const window, GLfloat X, GLfloat Y, GLfloat a, GLfloat b, GLfloat& cX, GLfloat& cY, const GLfloat cx, const GLfloat cy) {
	while ((X - 0.5) * (X - 0.5) + (Y - 0.5) * (Y - 0.5) > 0.25) {
		if (X > 0.5) X -= 0.005;
		else X += 0.005;
		if (Y > 0.5) Y -= 0.005;
		else Y += 0.005;
	}
	GLfloat k1 = a / 1.0, k2 = b / 1.0;
	cX = k1 * 2 * (X - 0.5) + cx;
	cY = k2 * 2 * (0.5 - Y) + cy;
	//cout << "X = " << cX << endl << "Y = " << cY << endl;
}


GLfloat offset(GLfloat time) {
	return 0.03 * sin(2 * PI / 3 * time);
}

GLfloat heartOffset(GLfloat fase) {
	GLfloat time = fmod(glfwGetTime() + fase, 3.0f);
	return 0.5 * sin(time * PI / (2 * 3) );
}


void printHearts(initializer_list<pair<GLfloat, GLfloat>> hearts) {
	auto it = hearts.begin();
	for (float i = 0.0; i < 3.0; i += 0.5f) {
		glUniform1f(8, heartOffset(i));
		glUniform2f(1, it->first, it->second); //ab
		++it;
		glUniform2f(2, it->first, it->second);  //center
		++it;
		glUniform3f(3, 1.0, 0.0, 0.0); //color
		glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //сердце
	}
}




int main() {
	ios_base::sync_with_stdio(false);

	try {
		// Инициализация GLFW
		if (glfwInit() == GLFW_FALSE)
			throw runtime_error("Failed to initialize GLFW!");

		// Создание окна
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* const window = glfwCreateWindow(640, 480, "ClickKitten", nullptr, nullptr);
		if (window == nullptr)
			throw runtime_error("Failed to create window!");
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, CursorPositionCallback);


		glfwMakeContextCurrent(window);

		// Инициализация GLEW
		{
			auto error = glewInit();
			if (error != GLEW_OK) {
				auto message = reinterpret_cast<const char*>(glewGetErrorString(error));
				throw runtime_error(string("Failed to initialize GLEW: ") + message);
			}

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
		}
		//glEnable(GL_MULTISAMPLE); //сглаживание
		glfwSetTime(0.0);
		cout << glfwGetTime() << endl;

		GLfloat ellipse[50];
		genEllipseAngles(ellipse, 50, 0.1, 0.1);

		GLfloat body[100];
		genParabola(body, 50, -1.0, 0.4, 0.0, 0.0);

		GLfloat closedEye3[100];
		genParabola(closedEye3, 50, -0.08, -0.14, -0.25, -0.05);
		GLfloat closedEye4[100];
		genParabola(closedEye4, 50, -0.08, 0.14, 0.25, -0.05);

		GLfloat nose[15] = {
			0.08, -0.26,    0.9, 0.2, 0.2,
			-0.08, -0.26,    0.9, 0.2, 0.2,
			0.0, -0.33,    0.9, 0.2, 0.2
		};
		GLfloat mouse[15] = {
			0.06, -0.45,    0.1, 0.1, 0.1,
			0.0, -0.38,    0.2, 0.2, 0.2,
			-0.06, -0.45,   0.1, 0.1, 0.1,
		};
		GLfloat ear1[15] = {
			-0.53, 0.0,    0.5, 0.5, 0.5,
			-0.45, 0.7,   0.38, 0.38, 0.38,
			-0.15, 0.35,   0.5, 0.5, 0.5
		};
		GLfloat ear2[15] = {
			0.53, 0.0,   0.5, 0.5, 0.5,
			0.45, 0.7,    0.38, 0.38, 0.38,
			0.15, 0.35,    0.5, 0.5, 0.5
		};
		GLfloat closedEye1[15] = {
			-0.35, 0.1,    0.1, 0.1, 0.1,
			-0.1, -0.05,    0.2, 0.2, 0.2,
			-0.35, -0.2,    0.1, 0.1, 0.1
		};
		GLfloat closedEye2[15] = {
			0.35, 0.1,   0.1, 0.1, 0.1,
			0.1, -0.05,    0.2, 0.2, 0.2,
			0.35, -0.2,   0.1, 0.1, 0.1
		};

		GLfloat heard[100];
		genHeart(heard, 50, 0.2, 0.2, 0.0, 0.0);

		GLfloat vecRes[440];
		createVBO(vecRes, 440, {
			pair<GLfloat*, size_t>(ellipse, 50),
			pair<GLfloat*, size_t>(body, 100),
			pair<GLfloat*, size_t>(closedEye3, 100),
			pair<GLfloat*, size_t>(closedEye4, 100),
			pair<GLfloat*, size_t>(nose, 15),
			pair<GLfloat*, size_t>(mouse, 15),
			pair<GLfloat*, size_t>(ear1, 15),
			pair<GLfloat*, size_t>(ear2, 15),
			pair<GLfloat*, size_t>(closedEye1, 15),
			pair<GLfloat*, size_t>(closedEye2, 15) });


		ShaderProgram baseShader({ pair<string,GLenum>("shader\\vertex.glsl", GL_VERTEX_SHADER),
			pair<string,GLenum>("shader\\baseColorF.glsl", GL_FRAGMENT_SHADER) });

		ShaderProgram baseShader2({ pair<string,GLenum>("shader\\baseVertex.glsl", GL_VERTEX_SHADER),
			pair<string,GLenum>("shader\\baseColorF.glsl", GL_FRAGMENT_SHADER) });

		ShaderProgram ellipseShader({ pair<string,GLenum>("shader\\moveEllipseV.glsl", GL_VERTEX_SHADER),
			pair<string,GLenum>("shader\\baseColorF.glsl", GL_FRAGMENT_SHADER) });

		ShaderProgram heartShader({ pair<string,GLenum>("shader\\heartShader.glsl", GL_VERTEX_SHADER),
			pair<string,GLenum>("shader\\baseColorF.glsl", GL_FRAGMENT_SHADER) });

		GLuint VBO = 0;
		GLuint VAO = 0;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vecRes), vecRes, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(50 * sizeof(GLfloat)));
		glEnableVertexAttribArray(6);

		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(350 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(352 * sizeof(GLfloat)));
		glEnableVertexAttribArray(5);

		glLineWidth(2);

		//прозрачность
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		cout << glfwGetTime();
		// Основной цикл
		while (!glfwWindowShouldClose(window)) {
			//glClearColor(0, X / 2, 0.5 + Y / 4, 1);
			glClearColor(0.7, 0.7, 0.9, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Код отрисовки


			// head 0.55, 0.58  center 0.0, -0.1  color 0.5, 0.5, 0.5
			// eye1 0.18, 0.16  center -0.25, -0.05  color 0.9, 0.9, 0.95
			// eye2 0.18, 0.16  center 0.25, -0.05  color 0.9, 0.9, 0.95
			// brow1 0.08, 0.08  center -0.2, 0.21  color 0.45, 0.45, 0.45
			// brow1 0.08, 0.08  center 0.2, 0.21  color 0.45, 0.45, 0.45
			// moveEyeball 0.09, 0.08 center -0.25, -0.05  color 0.3, 0.8, 0.6
			// body -1.0, 0.4 center 0.0, -1.0  color 0.45, 0.45, 0.45

			GLfloat time = glfwGetTime();


			baseShader2.Use();
			glUniform1f(8, 0.0);
			glUniform3f(7, 0.45, 0.45, 0.45); //color
			glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //тело


			ellipseShader.Use();

			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y))
				glUniform1f(8, offset(time));
				//glUniform1f(8, heartOffset());
			else glUniform1f(8, 0.0);

			glUniform2f(1, 0.55, 0.58); //ab
			glUniform2f(2, 0.0, -0.1);  //center
			glUniform3f(3, 0.5, 0.5, 0.5); //color
			glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //голова


			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y)) {
				heartShader.Use();

				printHearts({pair<GLfloat,GLfloat>(0.15, 0.13), pair<GLfloat,GLfloat>(0.0, 0.5), 
					pair<GLfloat,GLfloat>(0.1, 0.09), pair<GLfloat,GLfloat>(0.1, 0.4),
					pair<GLfloat,GLfloat>(0.11, 0.1), pair<GLfloat,GLfloat>(-0.2, 0.5),
					pair<GLfloat,GLfloat>(0.14, 0.12), pair<GLfloat,GLfloat>(0.2, 0.6),
					pair<GLfloat,GLfloat>(0.06, 0.05), pair<GLfloat,GLfloat>(-0.2, 0.55),
					pair<GLfloat,GLfloat>(0.09, 0.08), pair<GLfloat,GLfloat>(-0.05, 0.45),
					});


				/*glUniform1f(8, heartOffset(0.0));
				glUniform2f(1, 0.15, 0.13); //ab
				glUniform2f(2, 0.0, 0.5);  //center
				glUniform3f(3, 1.0, 0.0, 0.0); //color
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //сердце

				glUniform1f(8, heartOffset(0.5));
				glUniform2f(1, 0.1, 0.09); //ab
				glUniform2f(2, 0.1, 0.4);  //center
				glUniform3f(3, 1.0, 0.0, 0.0); //color
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //сердце

				glUniform1f(8, heartOffset(1.0));
				glUniform2f(1, 0.11, 0.1); //ab
				glUniform2f(2, -0.1, 0.5);  //center
				glUniform3f(3, 1.0, 0.0, 0.0); //color
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //сердце*/
			}


			baseShader.Use();

			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y))
				glUniform1f(8, offset(time));
			else glUniform1f(8, 0.0);

			glDrawArrays(GL_TRIANGLES, 6, 3); //уши
			glDrawArrays(GL_TRIANGLES, 9, 3);

			ellipseShader.Use();


			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y))
				glUniform1f(8, offset(time));
			else glUniform1f(8, 0.0);


			glUniform2f(1, 0.08, 0.08); //ab
			glUniform2f(2, -0.2, 0.21);  //center
			glUniform3f(3, 0.45, 0.45, 0.45); //color
			glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //брови
			glUniform2f(2, 0.2, 0.21);  //center
			glDrawArrays(GL_TRIANGLE_FAN, 0, 50);

			baseShader.Use();

			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y))
				glUniform1f(8, offset(time));
			else glUniform1f(8, 0.0);


			glDrawArrays(GL_TRIANGLES, 0, 3); //нос
			glDrawArrays(GL_LINE_STRIP, 3, 3); //рот

			baseShader2.Use();

			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onForehead(window, X, Y)) {
				glUniform1f(8, offset(time));
				glUniform3f(7, 0.1, 0.2, 0.2); //color
				glDrawArrays(GL_LINE_STRIP, 50, 50); //закрытые глаза
				glDrawArrays(GL_LINE_STRIP, 100, 50);
			}


			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && onNose(window, X, Y)) {
				
				baseShader.Use();
				glUniform1f(8, 0.0);
				glDrawArrays(GL_LINE_STRIP, 12, 3); //щурится
				glDrawArrays(GL_LINE_STRIP, 15, 3);
			}
			else if (!(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) || !onForehead(window, X, Y)){
				ellipseShader.Use();
				glUniform1f(8, 0.0);
				glUniform2f(1, 0.18, 0.16); //ab
				glUniform2f(2, -0.25, -0.05);  //center
				glUniform3f(3, 0.9, 0.9, 0.95); //color
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //глаза
				glUniform2f(2, 0.25, -0.05);  //center
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50);

				GLfloat cx = 0, cy = 0;
				centerPos(window, X, Y, 0.09, 0.08, cx, cy, -0.25, -0.05);
				glUniform2f(1, 0.09, 0.08); //ab
				glUniform2f(2, cx, cy);  //center
				glUniform3f(3, 0.3, 0.8, 0.6); //color
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //зрачок

				centerPos(window, X, Y, 0.09, 0.08, cx, cy, 0.25, -0.05);
				glUniform2f(2, cx, cy);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 50); //зрачок
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glBindVertexArray(0);
	}
	catch (const exception & e) {
		glfwTerminate();
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}
}

int WinMain(void*, void*, void*, int) {
	return main();
}


/*int WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd) {
	return main();
}*/