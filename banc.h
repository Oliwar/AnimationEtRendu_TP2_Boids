#ifndef BANC_H
#define BANC_H

#include "obstacle.h"
#include "poisson.h"
#include "predateur.h"
#include <list>

using namespace std;

class Banc
{
public:
    Banc();
    Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur);
    Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur, list<Obstacle> obstacles);
    Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur, list<Obstacle> obstacles, list<Predateur> predateurs);

    float rand_float(float min, float max);

    void anime(float dt);
    void affiche(QOpenGLShaderProgram *sp);

    unsigned nbPoissons;
    list<Poisson> poissons;
    float largeur, hauteur, profondeur;
    list<Obstacle> obstacles;
    list<Predateur> predateurs;
    float coef_separation = 1.0f;
    float coef_alignement = 1.0f;
    float coef_cohesion = 1.0f;

    QMatrix4x4 modelMatrix;
private:
    void verification_aquarium(Poisson *p);
    QVector3D calc_detected_point(QVector3D center_sphere, QVector3D point_coord, float rayon);
    //void verification_aquarium(Poisson p, QVector3D &separation, unsigned &nbVoisin);
    void verification_aquarium(Poisson &p);

};

#endif // BANC_H
