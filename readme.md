# Installation librairies (Box2D & OpenGL)

installer les librairies nécessaires à l'affichage : GLUT et OpenGL
```bash
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
installer la libraire qui permet de manipuler Box2D
```bash
sudo apt-cache search box2d
sudo apt-get install libbox2d-dev
```

# Build

Une fois la librairie installée :
```bash
cd P1RV/build
cmake ..
make
./exec
```



# Modes de jeu : modifications `main.cpp`

Dans la fonction `Init`, dans la boucle `while`, vous pouvez modifier le nombre d'individus dans la population `HomoSapiens` générée. Elle doit être d'au moins 3.

Dans la fonction `main`, vous pouvez mettre en commentaire ou enlever les commentaires des quelques lignes suivant l'appel à la fonction `Init()`. Ces quelques lignes permettent la génération sans affichage de `N` générations de populations. Le résultat de chaque génération est alors écrit dans `Project/sequences/`. 

Après avoir généré les moustiques en `offline` si demandé, le programme vas lancer une fenêtre de jeu `OpenGL` dans lequel vous pouvez jouer en utilisant les commandes décrites dans la partie suivante. Le résulat des parties sont écrites dans le dossier `Project/sequences/`.

# Commades de jeu

key  | action
--|--
`s`  |  change jambe d'appui
`q`/`esc`  |  quitte le jeu
`c`  | passer le jeu en IA ou humain 
`g`  | affiche la grille

Si un problème est rencontré vous pouvez faire remonter l'information par mail :
 - victor.duvivier@eleves.ec-nantes.fr
 - chloe.leric@eleves.ec-nantes.fr
