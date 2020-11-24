/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_3D_DEMOS_BUFFERATTRIBUTE_H
#define INC_3D_DEMOS_BUFFERATTRIBUTE_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class BufferAttribute {

public:
    BufferAttribute(vector<float> data, uint32_t itemSize, GLint drawType = GL_STATIC_DRAW);

    void bindToVAO(GLuint vao, GLuint location) const;

    uint32_t getLength();

    ~BufferAttribute();

private:
    vector<float> data;

    uint32_t itemSize;

    GLuint vbo {};

};


#endif //INC_3D_DEMOS_BUFFERATTRIBUTE_H
