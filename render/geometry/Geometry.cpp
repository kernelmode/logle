/* 
 * File:   Geometry.cpp
 * Author: kdubovikov
 * 
 * Created on December 7, 2014, 3:39 PM
 */

#include "Geometry.h"

const std::string Geometry::MVP_UNIFORM_NAME = "MVP";
const std::string Geometry::VIEW_UNIFORM_NAME = "V";
const std::string Geometry::MODEL_UNIFORM_NAME = "M";
const std::string Geometry::VERTEX_BUFFER_NAME = "vertexBuffer";
const std::string Geometry::UV_BUFFER_NAME = "uvBuffer";

Geometry::Geometry(ShaderManager& shaderManager, BufferManager& bufferManager) :
bufferManager(bufferManager),
shaderManager(shaderManager) {
    static int objectId = 0;
    this->objectId = objectId++;
    printf("New object id - %d\n", this->objectId);
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    printf("VAO Id - %d\n", vertexArrayId);
}


void Geometry::prepare() {
    shaderManager.enable();
}

void Geometry::applyUniforms(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    glBindVertexArray(vertexArrayId);
    glUseProgram(shaderManager.getShaderProgramId());
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    shaderManager.addUniform(MVP_UNIFORM_NAME, mvp);
    shaderManager.addUniform(VIEW_UNIFORM_NAME, viewMatrix);
    shaderManager.addUniform(MODEL_UNIFORM_NAME, modelMatrix);
}

void Geometry::translate(glm::vec3 to) {
    modelMatrix = glm::translate(modelMatrix, to);
}


void Geometry::prepareBuffers() {
    bufferManager.addBuffer(VERTEX_BUFFER_NAME + std::to_string(objectId), verticies);
    bufferManager.addBuffer(UV_BUFFER_NAME + std::to_string(objectId), uvs);
}

void Geometry::preRender() {
    glBindVertexArray(vertexArrayId);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferManager.getBufferId(VERTEX_BUFFER_NAME + std::to_string(objectId)));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, bufferManager.getBufferId(UV_BUFFER_NAME + std::to_string(objectId)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void Geometry::render() {
    preRender();
    glDrawArrays(GL_TRIANGLES, 0, verticies.size());
    postRender();
}

void Geometry::postRender() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    shaderManager.disable();
    glBindVertexArray(0);
}

glm::mat4& Geometry::getModelMatrix() {
    return modelMatrix;
}

ShaderManager& Geometry::getShaderManager() {
    return shaderManager;
}

int Geometry::getObjectId() {
    return objectId;
}


Geometry::~Geometry() {
    glDeleteVertexArrays(1, &vertexArrayId);
}