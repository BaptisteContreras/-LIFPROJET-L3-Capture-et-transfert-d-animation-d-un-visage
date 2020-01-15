# LIFPROJET L3

## Introduction

Projet réalisé dans le cadre de l'UE LIFPROJET,  L3 informatique, université Claude Bernard 1.

2018/2019

Sujet : **AM1 Capture et transfert d animation d un visage vers un autres par image warping et Deep Learning GAN**

**Objectif : transfert d’un sourire sur un visage neutre**

## Organisation du projet

Le code est réparti ainsi :

### data/
* **haarcascade_frontalface_alt2.xml** : modèle entrainé pour le Haar cascade classifier 
* **lbfmodel.yaml** :  modèle pour les landmarks
* **train/** : Dossier contenant les données pre-traitées avant l'entraînement de notre réseau de neurones
* Dans ce dossier on retrouve aussi l'image utilisée par défaut comme second média 

### src/
* **main.cpp** :  point d'entrée du programme, lance l'application QT
* **mainwindow.cpp** :  application QT. Contient le code pour la détection de visage et l'affichage de la fenêtre ainsi que la gestion des événements
* **worker.cpp** : class contenant le code nécessaire à la transformation d'une image
* **ui_mainwindow.ui** : fichier XML dans lequel les composant de l'interface graphique sont déclarés.
* **train /** : dossier contenant la class permettant de faire le pré traitement des données qui vont servir à entrainer notre réseau de neurones
* **struct/** : dossier contenant les structures pixel et FloatPixel (pixel dont les composant RGB sont compris entre 0 et 1) 
* **CMakeLists.txt** : pour compiler avec Clion


Pour appliquer les modifications faitent sur l'interface graphique il faut utiliser la commande ```./ui.sh ``` sous *Linux*

## Installation

### Dépendances
Pour que le projet fonctionne, il faut impérativement : 

*  Opencv version **3.4.5**
* QT version **5.9.5**
* g++

Il est conseillé d'utiliser *Linux* comme système d'exploitation

**Un ordinateur avec une webcam fonctionelle est requis.**

### Installation avec script (Recommandé)
> Se rendre à la racine du projet

> ./installation.sh

Pour lancer le programme : 

> ./launcher.sh

Ou pour lancer en mode training : 

> ./launchTraining.sh

### Installation AVEC CLION 
> Ouvrir le projet avec Clion

> Editer configuration

> Vérifier que le Target est : **AM1**

> Dans le CMakeList.txt, enlever les lignes suivantes :

> set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

> set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

> set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

> Lancer avec **MAJ + F10**

### Installation  SANS CLION
> cd src/

> cmake .

> make

> cd bin/

> ./AM1