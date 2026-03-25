#include "A2solution.h"
#include "Scene.h"
#include "Shaders.h"
#include "Intersect.h"
#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>

using namespace std;

A2solution::A2solution()
{
}

Scene A2solution::scene;

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        Scene* s = &A2solution::scene;
        vector<intersection> intersections = {};

        for (int i = 0; i < s->numTriangles; i++) {
            glm::vec3 sv1 = s->getScreenVertexByIndex(s->triangles.at(i * 3));
            glm::vec3 sv2 = s->getScreenVertexByIndex(s->triangles.at(i * 3 + 1));
            glm::vec3 sv3 = s->getScreenVertexByIndex(s->triangles.at(i * 3 + 2));

            intersection intersect = checkIntersect(xpos, ypos, sv1, sv2, sv3);
            if (intersect.hit) {
                glm::vec3 v1 = s->getVertexByIndex(s->triangles.at(i * 3));
                glm::vec3 v2 = s->getVertexByIndex(s->triangles.at(i * 3 + 1));
                glm::vec3 v3 = s->getVertexByIndex(s->triangles.at(i * 3 + 2));
                glm::vec3 point3D = intersect.bery.x * v1 + intersect.bery.y * v2 + intersect.bery.z * v3;
                intersect.point3D = point3D;
                intersect.index = i;
                intersections.push_back(intersect);
            }

            
        }

        if (intersections.size() > 0) {
            int minIndex = 0;
            for (int i = 0; i < intersections.size(); i++) {
                if (abs(intersections.at(i).Z) < abs(intersections.at(minIndex).Z)) minIndex = i;
            }
            intersection* i = &intersections.at(minIndex);
            cout << i->index << " " << i->bery.x << " " << i->bery.y << " " << i->bery.z << " " << i->point3D.x << " " << i->point3D.y << " " << i->point3D.z << endl;
        }
        
    }

}

void A2solution::run(std::string s){
	scene = Scene(s);
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cerr << "ERROR";
        return;
    }
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    

    GLFWwindow* window = glfwCreateWindow(scene.dimensions[0], scene.dimensions[1], "Comp371 - A1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, cursor_position_callback);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return;
    }

    glEnable(GL_DEPTH_TEST);

    scene.createBuffers();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glViewport(0, 0, scene.dimensions[0], scene.dimensions[1]);

    int programID[] = { 
        scene.compileAndLinkShaders(getFlatPhongVertexShader(), getFlatPhongFragmentShader()),
        scene.compileAndLinkShaders(getFlatPhongVertexShader(), getFlatPhongFragmentShader()),
        scene.compileAndLinkShaders(getBeryVertexShader(), getVoronoiFragmentShader()),
        scene.compileAndLinkShaders(getBeryVertexShader(), getCircleFragmentShader()),

    };

    int shaderIndex = 1;
    bool isWireframe = false;
    bool w_pressed = false;
    bool s_pressed = false;


    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID[shaderIndex]);

        GLint modelviewLoc = glGetUniformLocation(programID[shaderIndex], "modelview");
        GLint projectionLoc = glGetUniformLocation(programID[shaderIndex], "projection");
        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, scene.modelViewMatrix);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, scene.projectionMatrix);

        switch (shaderIndex) {
        case 0:
            glBindVertexArray(scene.VAOf);
            glDrawElements(GL_TRIANGLES, scene.flatIndices.size(), GL_UNSIGNED_INT, 0);
            break;
        case 1:
            glBindVertexArray(scene.VAOp);
            glDrawElements(GL_TRIANGLES, scene.triangles.size(), GL_UNSIGNED_INT, 0);
            break;
        case 2:
            glBindVertexArray(scene.VAOb);
            glDrawElements(GL_TRIANGLES, scene.beryIndices.size(), GL_UNSIGNED_INT, 0);
            break;
        case 3:
            glBindVertexArray(scene.VAOb);
            glDrawElements(GL_TRIANGLES, scene.beryIndices.size(), GL_UNSIGNED_INT, 0);
            break;
        }
        
        if (isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !s_pressed)
            shaderIndex = (shaderIndex + 1) % 4; s_pressed = true;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !w_pressed)
            isWireframe = !isWireframe; w_pressed = true;
    
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && s_pressed)
            s_pressed = false;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && w_pressed)
            w_pressed = false;
    
    
    }

    glfwTerminate();

    return;
}