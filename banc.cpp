#include "banc.h"
#include <cmath>

Banc::Banc(){
    this->nbPoissons = 0;
    this->largeur = 0;
    this->hauteur = 0;
    this->profondeur = 0;
}

Banc::Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur)
{
    this->nbPoissons = nbPoissons;
    this->largeur = largeur;
    this->hauteur = hauteur;
    this->profondeur = profondeur;
    QVector3D position;
    QVector3D vitesse;
    float distance = 5;
    for(unsigned i = 0; i < nbPoissons; ++i){
        position = QVector3D(rand_float(-largeur/2.0f , largeur/2.0f),
                             rand_float(-hauteur/2.0f , hauteur/2.0f),
                             rand_float(-profondeur/2.0f , profondeur/2.0f));
        vitesse = QVector3D(rand_float(-1 , 1), rand_float(-1 , 1), rand_float(-1 , 1));
        Poisson p(position, vitesse, 1, distance);
        this->poissons.push_back(p);
    }
}

Banc::Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur, list<Obstacle> obstacles)
{
    this->nbPoissons = nbPoissons;
    this->largeur = largeur;
    this->hauteur = hauteur;
    this->profondeur = profondeur;
    this->obstacles = obstacles;
    QVector3D position;
    QVector3D vitesse;
    bool position_ok;
    float x;
    float y;
    float z;
    float distance = 5;
    for(unsigned i = 0; i < nbPoissons; ++i){
        do{
            position_ok = true;
            position = QVector3D(rand_float(-largeur/2.0f , largeur/2.0f),
                                 rand_float(-hauteur/2.0f , hauteur/2.0f),
                                 rand_float(-profondeur/2.0f , profondeur/2.0f));
            for(Obstacle o : obstacles){
                x = position.x() - o.position.x();
                y = position.y() - o.position.y();
                z = position.z() - o.position.z();
                if( (x*x) + (y*y) + (z*z) <= o.rayon * o.rayon){
                    position_ok = false;
                    break;
                }
            }
        } while(!position_ok);

        vitesse = QVector3D(rand_float(-1 , 1), rand_float(-1 , 1), rand_float(-1 , 1));
        Poisson p(position, vitesse, 1, distance);
        this->poissons.push_back(p);
    }
}

Banc::Banc(unsigned nbPoissons, float largeur, float hauteur, float profondeur, list<Obstacle> obstacles, list<Predateur> predateurs)
{
    this->nbPoissons = nbPoissons;
    this->largeur = largeur;
    this->hauteur = hauteur;
    this->profondeur = profondeur;
    this->obstacles = obstacles;
    this->predateurs = predateurs;
    QVector3D position;
    QVector3D vitesse;
    bool position_ok;
    float x;
    float y;
    float z;
    float distance = 10.0f;
    for(unsigned i = 0; i < nbPoissons; ++i){
        do{
            position_ok = true;
            position = QVector3D(rand_float(-largeur/4.0f , largeur/4.0f),
                                 rand_float(-hauteur/4.0f , hauteur/4.0f),
                                 rand_float(-profondeur/4.0f , profondeur/4.0f));
            for(Obstacle o : obstacles){
                x = position.x() - o.position.x();
                y = position.y() - o.position.y();
                z = position.z() - o.position.z();
                if( (x*x) + (y*y) + (z*z) <= o.rayon * o.rayon){
                    position_ok = false;
                    break;
                }
            }

            for(Predateur p : predateurs){
                x = position.x() - p.position.x();
                y = position.y() - p.position.y();
                z = position.z() - p.position.z();
                if( (x*x) + (y*y) + (z*z) <= p.rayon * p.rayon){
                    position_ok = false;
                    break;
                }
            }
        } while(!position_ok);

        vitesse = QVector3D(rand_float(-1 , 1), rand_float(-1 , 1), rand_float(-1 , 1));
        Poisson p(position, vitesse, 1, distance);
        this->poissons.push_back(p);
    }
    this->poissons.front().vitesse = QVector3D(1,0,0);
}

float Banc::rand_float(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

void Banc::verification_aquarium(Poisson &p){
    if(p.position.x() - 1.0f <= -largeur/2 && p.vitesse[0] < 0){
        p.vitesse.setX(-p.vitesse.x());
    }
    if(p.position.x() + 1.0f >= largeur/2 && p.vitesse[0] > 0){
        p.vitesse.setX(-p.vitesse.x());
    }
    if(p.position.y() - 1.0f <= -hauteur/2 && p.vitesse[1] < 0){
        p.vitesse.setY(-p.vitesse.y());
    }
    if(p.position.y() + 1.0f >= hauteur/2 && p.vitesse[1] > 0){
        p.vitesse.setY(-p.vitesse.y());
    }
    if(p.position.z() - 1.0f <= -profondeur/2 && p.vitesse[2] < 0){
        p.vitesse.setZ(-p.vitesse.z());
    }
    if(p.position.z() + 1.0f >= profondeur/2 && p.vitesse[2] > 0){
        p.vitesse.setZ(-p.vitesse.z());
    }
}

QVector3D Banc::calc_detected_point(QVector3D center_sphere, QVector3D point_coord, float rayon){
    float distance_euclidienne_x = std::abs(point_coord.x() - center_sphere.x()) - rayon;
    float distance_euclidienne_y = std::abs(point_coord.y() - center_sphere.y()) - rayon;
    float distance_euclidienne_z = std::abs(point_coord.z() - center_sphere.z()) - rayon;

    float detected_point_x = point_coord.x();
    float detected_point_y = point_coord.y();
    float detected_point_z = point_coord.z();

    if(point_coord.x() - center_sphere.x() > 0) detected_point_x -= distance_euclidienne_x;
    if(point_coord.x() - center_sphere.x() < 0) detected_point_x += distance_euclidienne_x;
    if(point_coord.y() - center_sphere.y() > 0) detected_point_y -= distance_euclidienne_y;
    if(point_coord.y() - center_sphere.y() < 0) detected_point_y += distance_euclidienne_y;
    if(point_coord.z() - center_sphere.z() > 0) detected_point_z -= distance_euclidienne_z;
    if(point_coord.z() - center_sphere.z() > 0) detected_point_z += distance_euclidienne_z;

    return QVector3D(detected_point_x, detected_point_y, detected_point_z);
}

void Banc::anime(float dt){
    QVector3D separation, alignement, cohesion, vitesse;
    QVector3D separation_predateur, separation_obstacle;
    std::vector<QVector3D> vVitesse;
    float L = 0.5f;
    unsigned nbVoisinPoisson, nbVoisinPredateur, nbVoisinObstacle;
    for(Poisson &p : poissons){
        separation = QVector3D(); alignement = QVector3D(); cohesion = QVector3D();
        separation_predateur = QVector3D(); separation_obstacle = QVector3D();
        nbVoisinPoisson = 0; nbVoisinPredateur = 0; nbVoisinObstacle = 0;
        for(Poisson p_voisin : poissons){
            if(p_voisin.position == p.position) continue; //même poisson
            if(p.dans_voisinage(p_voisin.position)){
                nbVoisinPoisson++;
                separation += p_voisin.position;
                alignement += p_voisin.vitesse;
                cohesion += p_voisin.position;
            }
        }

        /*for(Obstacle o : obstacles){
            if(p.dans_voisinage_sphere(o.position, o.rayon)){
                nbVoisinObstacle++;
                separation_obstacle += calc_detected_point(o.position, p.position, o.rayon);
            }
        }*/

        /*for(Predateur pred : predateurs){
            if(p.dans_voisinage_sphere(pred.position, pred.rayon)){
                separation_predateur = calc_detected_point(pred.position, p.position, pred.rayon);
                nbVoisinPredateur++;
            }
        }*/

        if(nbVoisinPoisson > 0){
            separation = (separation - p.position) / nbVoisinPoisson;
            alignement = (p.vitesse - alignement) / nbVoisinPoisson;
            cohesion = -(cohesion - p.position) / nbVoisinPoisson;
        }
        /*if(nbVoisinObstacle > 0){
            separation_obstacle = (separation_obstacle - p.position) / nbVoisinObstacle;
        }
        if(nbVoisinPredateur > 0){
            separation_predateur = (separation_predateur - p.position) / nbVoisinPredateur;
        }*/

        vitesse = coef_separation * separation + coef_alignement * alignement + coef_cohesion * cohesion;
        vVitesse.push_back(vitesse);
    }

    unsigned cpt = 0;
    for(Poisson &p : poissons){
        p.vitesse += vVitesse[cpt];
        //p.vitesse = (1-L)*p.vitesse+L*vVitesse[cpt];
        if(p.vitesse.length() > p.vitesse_max){
            p.vitesse.normalize();
            p.vitesse *= p.vitesse_max;
        }
        verification_aquarium(p);
        p.anime(dt);
        cpt++;
    }
}

void Banc::affiche(QOpenGLShaderProgram *sp){
    list<Poisson>::iterator i;
    i = poissons.begin();
    while(i != poissons.end()){
        i->affiche(sp);
        i++;
    }
}
