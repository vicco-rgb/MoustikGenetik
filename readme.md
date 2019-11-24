# INSTALLATION Box2D
installer les librairies nécessaires à l'affichage : GLUT et OpenGL
```bash
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
installer la libraire qui permet de manipuler Box2D
```bash
sudo apt-cache search box2d
sudo apt-get install libbox2d-dev
```
# BUILD
Une fois la librairie installée :
```bash
cd P1RV/build
cmake ..
make
./main.out
```


# COMMANDES DE JEU
Appuyer sur 's' pour changer la jambe d'appui.
Appuyer sur 'q' pour quitter le jeu.

Si un problème est rencontré vous pouvez faire remonter l'information par mail :
 - victor.duvivier@eleves.ec-nantes.fr; chloe.leric@eleves.ec-nantes.fr
