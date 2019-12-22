#include "poisson.h"
#include <cmath>

Poisson::Poisson(){
    this->position = QVector3D(0, 0, 0);
    this->vitesse = QVector3D(0, 0, 0);
    this->taille = 0;
    this->distance = 0;
}

Poisson::Poisson(QVector3D position, QVector3D vitesse, float taille, float distance){
    this->position = position;
    this->vitesse = vitesse;
    this->taille = taille;
    this->distance = distance;
}

void Poisson::anime(float dt){
    position += vitesse*dt;
}

bool Poisson::dans_voisinage(QVector3D p){
    return this->position.distanceToPoint(p) <= distance;
}

bool Poisson::dans_voisinage_sphere(QVector3D p, float rayon){
    return std::abs(this->position.distanceToPoint(p) - rayon) <= distance;
}

void Poisson::affiche(QOpenGLShaderProgram *sp){
    this->modelMatrix.setToIdentity();
    this->modelMatrix.translate(position);

    if(vitesse.x() != 0.0f || vitesse.z() != 0.0f){ //rotation sur l'axe Y sur 360°
        int signeAngle=1;
        if(vitesse.z() > 0) signeAngle=-1;
        float angleY = signeAngle * std::acos(vitesse.x()/std::sqrt(std::pow(vitesse.x(), 2.0f) + std::pow(vitesse.z(), 2.0f))) * 180.0f / static_cast<float>(M_PI) + 180.0f;
        modelMatrix.rotate(angleY,0,1,0);
    }
    if(vitesse.length() != 0.0f){ //rotation sur l'axe Z sur 180°
        float angleZ = std::acos(vitesse.y()/vitesse.length())*180.0f / static_cast<float>(M_PI);
        modelMatrix.rotate(angleZ,0,0,1);
    }

    sp->setUniformValue("modelMatrix", this->modelMatrix);
    sp->setUniformValue("fishSize", this->taille);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
