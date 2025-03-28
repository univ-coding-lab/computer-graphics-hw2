#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

#include "Ray.h"
#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"

int Width = 512;
int Height = 512;

std::vector<float> OutputImage;

GLuint texID;

void render()
{
    Camera camera(
        Vec3(0, 0, 0),
        Vec3(1, 0, 0),
        Vec3(0, 1, 0),
        Vec3(0, 0, 1),
        -0.1f, 0.1f,
        -0.1f, 0.1f,
        0.1f,
        Width, Height
    );

    Scene scene;
    scene.addObject(new Sphere(Vec3(-4, 0, -7), 1));
    scene.addObject(new Sphere(Vec3(0, 0, -7), 2));
    scene.addObject(new Sphere(Vec3(4, 0, -7), 1));
    scene.addObject(new Plane(-2));

    for (int j = 0; j < Height; ++j) {
        for (int i = 0; i < Width; ++i) {
            Ray ray = camera.generateRay(i, j);
            if (scene.closestIntersection(ray)) {
                OutputImage.push_back(255);
                OutputImage.push_back(255);
                OutputImage.push_back(255);
            }
            else {
                OutputImage.push_back(0);
                OutputImage.push_back(0);
                OutputImage.push_back(0);
            }
        }
    }
}

void resize_callback(GLFWwindow*, int nw, int nh)
{
    Width = nw;
    Height = nh;
    glViewport(0, 0, nw, nh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, static_cast<double>(Width)
        , 0.0, static_cast<double>(Height)
        , 1.0, -1.0);
    OutputImage.reserve(Width * Height * 3);
    render();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glfwSetFramebufferSizeCallback(window, resize_callback);
	resize_callback(NULL, Width, Height);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
		
		glfwSwapBuffers(window);

		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
