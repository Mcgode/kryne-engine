//
// Created by max on 22/04/19.
//

#ifndef INC_3D_DEMOS_HIERARCHICALNODE_H
#define INC_3D_DEMOS_HIERARCHICALNODE_H

#include <stdarg.h>

#include "BaseObject.h"


/**
 * A hierarchical node
 */
class HierarchicalNode {

public:

    /**
     * A hierarchical node
     * @param transform The model transform matrix to apply to this node
     */
    explicit HierarchicalNode(glm::mat4 transform = glm::mat4());

    /**
     * A hierarchical node
     * @param object An object to add to the node
     * @param transform The model transform matrix to apply to this node
     */
    explicit HierarchicalNode(BaseObject *object, glm::mat4 transform = glm::mat4());

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

    ~HierarchicalNode();

private:

    /// The model transform matrix to apply to this node
    glm::mat4 transform{};

    /// The list of objects of this node
    std::vector<BaseObject *> objects;

    /// The list of children of this node
    std::vector<HierarchicalNode *> children;

};


#endif //INC_3D_DEMOS_HIERARCHICALNODE_H
