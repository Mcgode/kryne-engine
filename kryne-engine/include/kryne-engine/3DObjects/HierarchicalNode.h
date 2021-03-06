/**
 * @file
 * @author Max Godefroy
 * @date 22/04/19
 */

#ifndef INC_KRYNE_ENGINE_HIERARCHICALNODE_H
#define INC_KRYNE_ENGINE_HIERARCHICALNODE_H

#include <cstdarg>
#include <kryne-engine/Rendering/OldRenderPass.h>

#include "kryne-engine/3DObjects/BaseObject.h"
#include "kryne-engine/Core/AdditionalParameters.h"


/**
 * A hierarchical node
 */
class HierarchicalNode {

public:

    /**
     * A hierarchical node
     * @param transform The model transform matrix to apply to this node. Defaults to identity matrix.
     */
    explicit HierarchicalNode(glm::mat4 transform = glm::mat4(1.0f));

    /**
     * A hierarchical node
     * @param object An object to add to the node
     * @param transform The model transform matrix to apply to this node. Defaults to identity matrix.
     */
    explicit HierarchicalNode(BaseObject *object, glm::mat4 transform = glm::mat4(1.0f));

    /**
     * Adds a new object to the node
     * @param object The object to add to the node
     * @return The pointer to the current HierarchicalNode. Used for call chains.
     */
    HierarchicalNode *addObject(BaseObject *object);

    /**
     * Adds a new child to the node
     * @param child The child to add to the node
     * @return The pointer to the current HierarchicalNode. Used for call chains.
     */
    HierarchicalNode *addChild(HierarchicalNode *child);

    /**
     * Draws the node objects and its children to the current framebuffer
     * @param renderer   The rendering handler.
     * @param view       The view transform matrix of the scene.
     * @param model      The combined model transform matrix of the scene and the node successive parents.
     * @param params     The additional params to pass down the rendering pipeline.
     */
    void draw(OldRenderPass *renderer, glm::mat4 view, glm::mat4 model, AdditionalParameters *params);

    ~HierarchicalNode();

private:

    /// The model transform matrix to apply to this node
    glm::mat4 transform{};

    /// The list of objects of this node
    std::vector<BaseObject *> objects;

    /// The list of children of this node
    std::vector<HierarchicalNode *> children;

};

#else

class HierarchicalNode;

#endif //INC_KRYNE_ENGINE_HIERARCHICALNODE_H
