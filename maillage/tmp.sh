#!/bin/bash

# goursat : 0
# sphere : 2
# tore : 3
# hyperbole : 4
# tore 2 trous : 5
# boule enfoncee : 6
# boite oeuf : 7
# conoide Plucker : 8
# plan : 9
# sphere param : 10
# trinoide param : 11
# plan param : 12
# enneper param : 13
# tmp param : 14
# catenoid param : 15
# catenoid2 param (jonction catenoid sphere) : 16
# plan param polaire : 17
# cube arrondi (norme 4) : 18
# tore : 19
# enneper2 (jonction sphere-enneper) : 20
type=10
amp_goursat=1
amp_boite=1
fonc_trans=0
nbptr=35
h=0.01
R1=1
R2=0.3
dir_init="/libre"
N=20
nb_dir=6
size_exp=1.0
nb_discr=1
nb_discr_etoi=9
path_curv="carre2.txt"
#path_pas="tore.txt"
discr1=0.4
discr2=0.4
dir_courbe_param=0

type_test=8
# affichage vecteurs : 0
# maillage geodésique ortho : 1
# maillage rosace : 2
# maillage sing primal pas lisse : 3
# maillage sing dual pas lisse : 4
# maillage sing dual lisse : 5
# maillage diagonale par géodésique : 6
# maillage ligne paramétrage : 7 (uniquement surf. param)
# maillage diagonale par ligne paramétrage : 8
# maillage diagonale par une ellipse : 9
# maillage diagonale par une courbe de Bezier : 10
# maillage diagonale par un carre arrondi : 11
# maillage primal par un cercle : 12
# maillage sing -1/2 par dual : 13
# maillage sing triple rosace pas lisse : 14
# maillage primal dual : 15
# maillage singularité dual par ligne paramétrage : 16
# maillage singularité dual par courbe géodésique : 17
# maillage diagonale par un triangle arrondi : 18 (uniquement plan param polaire)

bool_simu=0
bool_etoile=0
bool_sing=2
bool_suppr=0
# bool_init : 0 : initialisation de la simulation par le maillage obtenu
#             1 : initialisation lineaire pour la simulation
bool_init=0

param="-dir dummy -h $h -dir_courbe_param $dir_courbe_param -discr1 $discr1 -discr2 $discr2 -path_angles $path_curv -type_test $type_test -nbptr $nbptr -type $type -bool_sing $bool_sing -N $N -bool_suppr $bool_suppr -nb_dir $nb_dir -size_exp $size_exp -nb_discr $nb_discr -bool_init $bool_init -bool_etoile $bool_etoile -nb_discr_etoi $nb_discr_etoi -bool_simu $bool_simu"
echo $param
./maillage $param
