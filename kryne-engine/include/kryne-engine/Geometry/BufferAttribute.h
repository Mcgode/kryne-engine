//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_BUFFERATTRIBUTE_H
#define INC_3D_DEMOS_BUFFERATTRIBUTE_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class IBufferAttribute {

public:
    virtual void bindToVAO(GLuint vao, GLuint location) = 0;

    virtual uint32_t getLength() = 0;

};


template<class T>
class BufferAttribute: public IBufferAttribute {

public:
    BufferAttribute(vector<T> data, uint32_t itemSize, GLint drawType = GL_STATIC_DRAW);

    void bindToVAO(GLuint vao, GLuint location) override;

    uint32_t getLength() override;

    ~BufferAttribute();

private:
    vector<T> data;

    uint32_t itemSize;

    GLuint vbo {};

};


#endif //INC_3D_DEMOS_BUFFERATTRIBUTE_H
