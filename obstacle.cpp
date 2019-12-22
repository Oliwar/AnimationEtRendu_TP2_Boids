#include "obstacle.h"

Obstacle::Obstacle(QVector3D position, float rayon)
{
    this->position = position;
    this->rayon = rayon;
}

void Obstacle::affiche(QOpenGLShaderProgram *sp, int start){
    this->modelMatrix.setToIdentity();

    sp->setUniformValue("modelMatrix", this->modelMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, start, 3362);
}
