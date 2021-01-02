#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Material *toBeAssignedMaterial, gameTemp::Mesh *pMesh)
{
    this->material = toBeAssignedMaterial;
    this->pMesh = pMesh;
}

int MeshRenderer::getComponentType()
{
    return MESH_RENDERER_COMPONENT;
}

void MeshRenderer::setMesh(gameTemp::Mesh *pMesh)
{
    this->pMesh = pMesh;
}

gameTemp::Mesh *MeshRenderer::getMesh()
{
    return this->pMesh;
}

void MeshRenderer::Draw(glm::mat4 transformMatrix)
{
    gameTemp::ShaderProgram *program = this->material->getShaderProgram();
    glUseProgram(*program);

    program->set("tint", *(this->material->getTint()));
    program->set("transform", transformMatrix);

    ObjectProperties *objProp = this->material->getObjProp();

    // Culling
    if (objProp->getCull()->enabled)
    {
        glEnable(GL_CULL_FACE);
        if (objProp->getCull()->cullFace == FRONT)
            glCullFace(GL_FRONT);

        else if (objProp->getCull()->cullFace == BACK)
            glCullFace(GL_BACK);

        if (objProp->getCull()->direction == CCW)
            glFrontFace(GL_CCW);

        else
            glFrontFace(GL_CW);
    }
    else
        glDisable(GL_CULL_FACE);

    // Blending
    if (objProp->getBlend()->enabled)
    {
        if (objProp->getBlend()->type == Constant)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);

            glBlendColor(objProp->getBlend()->constClr.r,
                         objProp->getBlend()->constClr.g,
                         objProp->getBlend()->constClr.b,
                         objProp->getBlend()->constClr.a);
        }
        else if (objProp->getBlend()->type == NotConstant)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    else
        glDisable(GL_BLEND);

    pMesh->draw();
}
