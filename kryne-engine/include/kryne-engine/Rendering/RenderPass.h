/**
 * @file
 * @author Max Godefroy
 * @date 10/10/2019
 */

#ifndef INC_3D_DEMOS_RENDER_PASS_H
#define INC_3D_DEMOS_RENDER_PASS_H


#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/HierarchicalNode.h>
#include <kryne-engine/3DObjects/BaseObject.h>

class RenderPass {

public:


    /**
     * The method for the rendering the current pass
     * @param window    The current rendering window.
     * @param rootNodes The nodes to render.
     * @param params    Additional parameters to pass down during the rendering.
     */
    virtual void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) = 0;

    /**
     * Draws an object in the scene for the current pass.
     * @param obj        The object to draw
     * @param projection The projection transform matrix
     * @param view       The view transform matrix
     * @param model      The model transform matrix
     * @param params     The additional parameters to pass down to the object
     */
    virtual void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) = 0;

};


#else

class RenderPass;

#endif //INC_3D_DEMOS_RENDER_PASS_H
