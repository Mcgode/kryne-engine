//
// Created by max on 22/04/19.
//

#include "kryne-engine/3DObjects/HierarchicalNode.h"


HierarchicalNode::HierarchicalNode(glm::mat4 transform)
{
    this->transform = glm::mat4(transform);
}


HierarchicalNode::HierarchicalNode(BaseObject *object, glm::mat4 transform)
{
    this->objects.push_back(object);
    this->transform = glm::mat4(transform);
}


HierarchicalNode *HierarchicalNode::addObject(BaseObject *object)
{
    this->objects.push_back(object);
    return this;
}


HierarchicalNode *HierarchicalNode::addChild(HierarchicalNode *child)
{
    this->children.push_back(child);
    return this;
}


HierarchicalNode::~HierarchicalNode()
{
    for (BaseObject *obj: objects)
        delete(obj);
    for (HierarchicalNode *child: children)
        delete(child);
}


//void HierarchicalNode::draw(OldRenderPass *renderer, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
//{
//    glm::mat4 final_model = this->transform * model;
//
//    for (BaseObject *obj: objects)
//        renderer->drawInScene(obj, view, final_model, params);
//
//    for (HierarchicalNode *node: children)
//        node->draw(renderer, view, final_model, params);
//}


