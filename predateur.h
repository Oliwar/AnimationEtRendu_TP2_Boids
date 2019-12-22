#ifndef PREDATEUR_H
#define PREDATEUR_H

#include "poisson.h"

#include <list>

using namespace std;

class Predateur
{
public:
    Predateur(QVector3D position, float rayon, QVector3D vitesse);

    QVector3D position;
    float rayon;
    QVector3D vitesse;
    float vitesse_max = 1.0f;

    void anime(float dt, list<Poisson> &poissons);
    void affiche(QOpenGLShaderProgram *sp, int start);

    QMatrix4x4 modelMatrix;
private:
    Poisson trouver_proie(list<Poisson> &poissons);
    void manger_proie(Poisson &proie, list<Poisson> &poissons);
};

#endif // PREDATEUR_H
