#include "A1solution.h"
#include "Scene.h"
#include "Shaders.h"
#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>

using namespace std;

A1solution::A1solution()
{
}


void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

void A1solution::run(std::string s){
	Scene scene(s);

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
        scene.compileAndLinkShaders(getBeryVertexShader(), getVoronoiFragmentShader())
    };

    int shaderIndex = 2;
    bool isWireframe = false;

    cerr << "numVertices: " << scene.numVertiecies << endl;
    cerr << "numTriangles: " << scene.numTriangles << endl;
    cerr << "beryData size: " << scene.beryData.size() << endl;
    cerr << "beryIndices size: " << scene.beryIndices.size() << endl;
    cerr << "VAOb: " << scene.VAOb << endl;


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
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            shaderIndex= (shaderIndex+1) % 3;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            isWireframe = !isWireframe;
    }

    glfwTerminate();

    return;
}