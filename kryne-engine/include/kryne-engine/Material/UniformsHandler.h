/**
 * @file
 * @author Max Godefroy
 * @date 24/11/2020
 */

#ifndef INC_KRYNE_ENGINE_UNIFORMSHANDLER_H
#define INC_KRYNE_ENGINE_UNIFORMSHANDLER_H

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <boost/variant.hpp>

#include <kryne-engine/Textures/Texture.h>

using namespace std;


/**
 * Handles the storage and update of uniform states and values.
 */
class UniformsHandler {

public:

    /**
     * Initializes the UniformHandler and link it to its shader program.
     * @param programId     The OpenGL shader program id
     */
    explicit UniformsHandler(GLuint *programId) : programId(programId) {}

protected:

    /// The program ID for the associated shader
    GLuint *programId;


public:

    /**
     * A boost variant for handling the different support uniform value types
     */
    typedef boost::variant<
            float,
            glm::vec2,
            glm::vec3,
            glm::vec4,
            glm::int32,
            glm::ivec2,
            glm::ivec3,
            glm::ivec4,
            glm::uint32,
            glm::uvec2,
            glm::uvec3,
            glm::uvec4,
            glm::mat3,
            glm::mat4,
            shared_ptr<Texture>> UniformTypes;

    /**
     * Sets the value of a uniform
     * @param name      The name of the uniform
     * @param value     The value of the uniform
     */
    void setUniform(const string &name, const UniformTypes &value);

    /**
     * Sets the value of an array uniform at a certain index
     * @param name          The name of the uniform array
     * @param value         The value of the uniform
     * @param arrayIndex    The array index to set
     */
    void setUniform(const string &name, const UniformTypes &value, uint32_t arrayIndex) {
        setUniform(name + "[" + to_string(arrayIndex) + "]", value);
    }

    /**
     * Removes a uniform from the map.
     * @param name  The name of the uniform to remove.
     * @return true if a uniform was removed.
     */
    bool removeUniform(const string &name);

    /**
     * Notify the UniformHandler that the uniform locations need to be updated (generally because the shader was recompiled)
     */
    void notifyUniformLocationsNeedUpdate();

    /**
     * Uploads the uniform data to the shader program
     */
    void updateUniforms();

protected:

    void setTexture(const shared_ptr<Texture> &texture, GLint location);

protected:

    /// The stored uniform data
    unordered_map<string, pair<UniformTypes, GLint>> uniforms {};

    /// Mutex for handling parallel read/write of uniforms
    mutex uniformsMutex;

    unordered_map<GLint, GLint> activeTextures {};

    GLint nextTextureIndex = GL_TEXTURE0;


private:

    typedef boost::variant<GLint> UniformLocationType;

    class UniformsSetter: boost::static_visitor<> {

    public:

        explicit UniformsSetter(UniformsHandler *handler): handler(handler) {}

        void operator() (const float &data, const GLint &location) {
            glUniform1f(location, data);
        }

        void operator() (const glm::vec2 &data, const GLint &location) {
            glUniform2f(location, data.x, data.y);
        }

        void operator() (const glm::vec3 &data, const GLint &location) {
            glUniform3f(location, data.x, data.y, data.z);
        }

        void operator() (const glm::vec4 &data, const GLint &location) {
            glUniform4f(location, data.x, data.y, data.z, data.w);
        }

        void operator() (const glm::int32 &data, const GLint &location) {
            glUniform1i(location, data);
        }

        void operator() (const glm::ivec2 &data, const GLint &location) {
            glUniform2i(location, data.x, data.y);
        }

        void operator() (const glm::ivec3 &data, const GLint &location) {
            glUniform3i(location, data.x, data.y, data.z);
        }

        void operator() (const glm::ivec4 &data, const GLint &location) {
            glUniform4i(location, data.x, data.y, data.z, data.w);
        }

        void operator() (const glm::uint32 &data, const GLint &location) {
            glUniform1ui(location, data);
        }

        void operator() (const glm::uvec2 &data, const GLint &location) {
            glUniform2ui(location, data.x, data.y);
        }

        void operator() (const glm::uvec3 &data, const GLint &location) {
            glUniform3ui(location, data.x, data.y, data.z);
        }

        void operator() (const glm::uvec4 &data, const GLint &location) {
            glUniform4ui(location, data.x, data.y, data.z, data.w);
        }

        void operator() (const glm::mat3 &data, const GLint &location) {
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
        }

        void operator() (const glm::mat4 &data, const GLint &location) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
        }

        void operator() (const shared_ptr<Texture> &data, const GLint &location) {
            handler->setTexture(data, location);
        }

        UniformsHandler *handler;

    };

private:

    UniformsSetter setter { this };

};


#endif //INC_KRYNE_ENGINE_UNIFORMSHANDLER_H
