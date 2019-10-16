//
// Created by max on 25/07/2019.
//

#ifndef INC_3D_DEMOS_POST_PROCESS_H
#define INC_3D_DEMOS_POST_PROCESS_H

#include "kryne-engine/Texture2D.h"
#include "FinalPostProcess.h"


using namespace std;

/**
 * A class for handling post-processing tools
 */
class PostProcess: public FinalPostProcess {

public:

    /**
     * A class for handling post-processing tools
     * @param shader    The post-processing shader
     * @param width     The width of the viewport
     * @param height    The height of the viewport
     * @param channels  The color channels for the export texture
     * @param type      The data type of the texture
     */
    PostProcess(Shader *shader, uint32_t width, uint32_t height, GLenum channels, GLenum type);

    GLuint getTexture() const;

    void draw() override;

    ~PostProcess();


private:

    GLuint fbo{};
    GLuint texture{};

};


#endif //INC_3D_DEMOS_POST_PROCESS_H
