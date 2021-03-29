/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_TEXTURE_H
#define KRYNE_ENGINE_TEXTURE_H

#include <stb_image.h>
#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <kryne-engine/Dispatch/Dispatcher.h>


using namespace glm;


/**
 * The base Texture abstract class
 */
class Texture {

public:

    /**
     * Binds this texture to the GL context GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP value.
     */
    inline void bindTexture() const
    {
        glBindTexture(this->textureType, this->id);
    }

    /**
     * Set the filtering for the texture. Will also automatically generate mip maps if needed.
     *
     * @param minFilter The minifying filter. Can be set to a mipmap filtering value, the mip maps will be automatically
     *                  generated
     *
     * @param magFilter The magnifying filter for the texture.
     */
    void setFiltering(GLint minFilter, GLint magFilter);

    /**
     * Delete texture from GL context
     */
    ~Texture();

    GLuint getId() const { return id; }

protected:

    /**
     * Initializes an abstract texture.
     * @param id            The GL context texture id.
     * @param textureType   The texture type.
     */
    Texture(GLuint id, GLenum textureType);

protected:

    /// The GL context texture id.
    GLuint id;

    /// The type of the texture.
    GLenum textureType;

    /// The minifying and magnifying filtering parameters of the texture.
    ivec2 filterParams {};

    /// True if mip maps have already been generated.
    bool mipMapped = false;

};


#endif //KRYNE_ENGINE_TEXTURE_H
