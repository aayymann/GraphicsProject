#include <entities/entity.hpp>

Entity::Entity() {}

void Entity::addComponent(Component *component)
{
    components.push_back(component);
}

TransformationComponent *Entity::getTransformationComponent()
{
    for (int i = 0; i < components.size(); i++)
        if (components[i]->getComponentType() == TRANSFORMATION_COMPONENT)
            return dynamic_cast<TransformationComponent *>(components[i]);

    return nullptr;
}

CameraComponent *Entity::getCameraComponent()
{
    for (int i = 0; i < components.size(); i++)
        if (components[i]->getComponentType() == CAMERA_COMPONENT)
            return dynamic_cast<CameraComponent *>(components[i]);

    return nullptr;
}

CameraControllerComponent *Entity::getCameraComponentController()
{
    for (int i = 0; i < components.size(); i++)
        if (components[i]->getComponentType() == CONTROLLER_COMPONENT)
            return dynamic_cast<CameraControllerComponent *>(components[i]);

    return nullptr;
}

MeshRenderer *Entity::getMeshRendrer()
{
    for (int i = 0; i < components.size(); i++)
        if (components[i]->getComponentType() == MESH_RENDERER_COMPONENT)
            return dynamic_cast<MeshRenderer *>(components[i]);

    return nullptr;
}


ObjectProperties* Entity:: getObjProp(){
    return &(this->objProp);
}

void  Entity:: setCullObjProp(Culling * myCull){
    this->objProp.cull=*myCull;
}

void  Entity:: setBlendObjProp(Blending * myBlend){
    this->objProp.blend=*myBlend;
}

Entity::~Entity()
{
    for (int i = 0; i < components.size(); i++)
        delete components[i];

    components.clear();
}