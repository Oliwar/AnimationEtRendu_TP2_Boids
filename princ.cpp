// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QColorDialog>
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
}

void Princ::on_doubleSpinBox_vitesse_max_poisson_valueChanged(double arg1)
{
    for(Poisson &p : glarea->banc.poissons){
        p.vitesse_max = static_cast<float>(arg1);
    }
}

void Princ::on_doubleSpinBox_vitesse_max_predateur_valueChanged(double arg1)
{
    for(Predateur &p : glarea->predateurs){
        p.vitesse_max = static_cast<float>(arg1);
    }
}

void Princ::on_doubleSpinBox_coef_separation_valueChanged(double arg1)
{
    glarea->banc.coef_separation = static_cast<float>(arg1);;
}

void Princ::on_doubleSpinBox_coef_alignement_valueChanged(double arg1)
{
    glarea->banc.coef_alignement = static_cast<float>(arg1);;
}

void Princ::on_doubleSpinBox_coef_cohesion_valueChanged(double arg1)
{
    glarea->banc.coef_cohesion = static_cast<float>(arg1);;
}

void Princ::on_doubleSpinBox_rayon_detection_valueChanged(double arg1)
{
    for(Poisson &p : glarea->banc.poissons){
        p.distance = static_cast<float>(arg1);
    }
}
