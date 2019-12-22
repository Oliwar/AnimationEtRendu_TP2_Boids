#include "predateur.h"
#include <cmath>

Predateur::Predateur(QVector3D position, float rayon, QVector3D vitesse)
{
    this->position = position;
    this->rayon = rayon;
    this->vitesse = vitesse;
}

Poisson Predateur::trouver_proie(list<Poisson> &poissons){
    Poisson proie;
    float dist_min = INT_MAX;
    float dist;
    for(Poisson &p : poissons){
        dist = this->position.distanceToPoint(p.position);
        if(dist < dist_min){
            dist_min = dist;
            proie = p;
        }
    }
    return proie;
}

void Predateur::manger_proie(Poisson &proie, list<Poisson> &poissons){
    list<Poisson>::iterator i;
    i = poissons.begin();
    while(i != poissons.end()){
        if(i->position == proie.position){
            i = poissons.erase(i);
            proie.~Poisson();
            return;
        }
        i++;
    }
}

void Predateur::anime(float dt, list<Poisson> &poissons){
    Poisson proie = trouver_proie(poissons);
    this->vitesse = proie.position - this->position;
    if(this->vitesse.length() > this->vitesse_max){
        this->vitesse.normalize();
        this->vitesse *= this->vitesse_max;
    }
    this->position += this->vitesse*dt;
    if(this->position.distanceToPoint(proie.position) <= this->rayon)
        manger_proie(proie, poissons);
}

void Predateur::affiche(QOpenGLShaderProgram *sp, int start){
    this->modelMatrix.setToIdentity();
    this->modelMatrix.translate(position);

    sp->setUniformValue("modelMatrix", this->modelMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, start, 3362);
}
