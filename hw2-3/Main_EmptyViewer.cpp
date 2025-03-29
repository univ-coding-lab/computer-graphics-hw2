#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <random>

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

Vec3 phongShading(const Vec3& point, const Vec3& normal, const Vec3& viewDir,
    const Light& light, const Material& mat) {
    Vec3 lightDir = (light.position - point).normalized();
    Vec3 reflectDir = (normal * 2.0f * normal.dot(lightDir) - lightDir).normalized();

    Vec3 ambient = mat.ka * light.intensity;

    float diff = std::max(normal.dot(lightDir), 0.0f);
    Vec3 diffuse = mat.kd * diff * light.intensity;

    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), mat.specular_power);
    Vec3 specular = mat.ks * spec * light.intensity;

    return ambient + diffuse + specular;
}

void render() {
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

    auto* s1 = new Sphere(Vec3(-4, 0, -7), 1);
    s1->material = { {0.2f, 0.0f, 0.0f}, {1, 0, 0}, {0, 0, 0}, 0 };

    auto* s2 = new Sphere(Vec3(0, 0, -7), 2);
    s2->material = { {0, 0.2f, 0}, {0, 0.5f, 0}, {0.5f, 0.5f, 0.5f}, 32 };

    auto* s3 = new Sphere(Vec3(4, 0, -7), 1);
    s3->material = { {0, 0, 0.2f}, {0, 0, 1}, {0, 0, 0}, 0 };

    auto* plane = new Plane(-2);
    plane->material = { {0.2f, 0.2f, 0.2f}, {1, 1, 1}, {0, 0, 0}, 0 };

    scene.addObject(s1);
    scene.addObject(s2);
    scene.addObject(s3);
    scene.addObject(plane);

    Light light = { Vec3(-4, 4, -3), Vec3(1, 1, 1) };

    OutputImage.clear();
    OutputImage.reserve(Width * Height * 3);

    int samplesPerPixel = 64;
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int j = 0; j < Height; ++j) {
        for (int i = 0; i < Width; ++i) {
            Vec3 pixelColor(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                float u_offset = dist(rng);
                float v_offset = dist(rng);

                float u_scalar = camera.l + (camera.r - camera.l) * (i + u_offset) / camera.nx;
                float v_scalar = camera.b + (camera.t - camera.b) * (j + v_offset) / camera.ny;

                Vec3 direction = (camera.u * u_scalar) + (camera.v * v_scalar) - (camera.w * camera.d);
                Ray ray(camera.eye, direction.normalized());

                HitInfo hit = scene.closestIntersection(ray);
                Vec3 sampleColor(0, 0, 0);

                if (hit.hit) {
                    if (scene.isInShadow(hit.point, light)) {
                        sampleColor = hit.object->material.ka * light.intensity;
                    }
                    else {
                        Vec3 viewDir = -ray.direction;
                        sampleColor = phongShading(hit.point, hit.normal, viewDir, light, hit.object->material);
                    }
                }

                pixelColor += sampleColor;
            }

            pixelColor = pixelColor / float(samplesPerPixel);

            auto gammaCorrect = [](float c) {
                return std::pow(std::min(c, 1.0f), 1.0f / 2.2f);
                };

            OutputImage.push_back(gammaCorrect(pixelColor.x));
            OutputImage.push_back(gammaCorrect(pixelColor.y));
            OutputImage.push_back(gammaCorrect(pixelColor.z));
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
