# Compte Rendu
_Chloé Leric_ et _Victor Duvivier_.

## Introduction
Notre projet consiste en l'application d'un ou de plusieurs algorithmes génétiques sur un mini-jeu.

[//]: # (<iframe width="560" height="315" src="https://www.youtube.com/embed/noNl_cvYLu8" frameborder="0" allow="accelerometer autoplay encrypted-media gyroscope picture-in-picture" allowfullscreen></iframe>)

## Explication générale du code
Le but de notre algorithme est de générer
### Diagramme de classe
Les différentes classes existant dans notre code sont:
+ `Forme` cette classe correspond à un rectangle physique. Cette classe contient un attribut `b2Body* body` issu de la librairie moteur physique `Box2D`. La classe forme permet d'automatiser la création de boîtes physiques et facilite l'écriture des fonctions d'affichage de ces formes sous `OpenGL`.
+ `Genome`
+ `Moustik` cette classe correspond au concept de notre animal bipède. Il possède trois attributs `Forme`, correspondant à sa tête et à ses deux jambes.

`-` public
`+` private
`#` protected

```mermaid
classDiagram
class Coord {
  - float x
  - float y
  - Coord()
  - Coord(float, float)
}
```
```mermaid
classDiagram
class Forme {
  # b2Body* body
  # float width
  # float height
  # int type
  # float diag
  # float angleIn
  - Forme(b2World*, Coord, float, float, int)
  - Forme(b2World*, Coord, int)
  - ~Forme()
  - float getX()
  - float getY()
  - Coord getPos()
  - b2Body* getBody()
  - float getAngle()
  - float getAngleIn()
  - float getDiag()
  - Coord getHL()
  - Coord getHR()
  - Coord getTL()
  - Coord getTR()
  - GLvoid drawOpenGL()
  - GLvoid drawOpenGL(float, float, float)
}
```
```mermaid
classDiagram
Moustik-->MoustikIA
class Moustik {
  # Forme* ptrHead
  # Forme* ptrLegL
  # Forme* ptrLegR
  # b2RevoluteJoint* rotuleL
  # b2RevoluteJoint* rotuleR
  # int com
  # bool dead
  # float score
  # float angleMax
  # string controlType
  # vector[int] sequence
  # bool seqWritten
  - Moustik(b2World*, Coord)
  - ~Moustik()
  - void commande(b2World*, int)
  - Coord getPos()
  - virtual void undertaker(int)
  - void updateScore()
  - void reset(b2World*)
  - float getAbs()
  - string getType()
  - GLvoid drawOpenGL()
  - void writeSeqDown(int, string, bool)
}
class MoustikIA {
  # Genome genome
  # int id
  - MoustikIA(b2World*, Coord, Genome, int)
  - MoustikIA(b2World*, Coord, vector[int], int)
  - void play(b2World*, int)
  - virtual void undertaker(int)
  - vector[int] getSeq()
}
```
```mermaid
classDiagram
class Genome {
  # int fitness
  # vector[int] seq
  # float tauxMutation
  - Genome(vector[int])
  - Genome(int)
  - Genome()
  - ~Genome()
  - vector[int] getRelativeSeq()
  - vector[int] getAbsoluteSeq()
  - int getFitness()
  - Genome* crossSplit(Genome*)
  - Genome* crossAvg(Genome*)
  - Genome* mutation()
  - bool betterThan(Genome*)
}
```
```mermaid
classDiagram
class Population {
  # vector[MoustikIA*] moustiks
  # int generation
  - Population()
  - Population(vector[Genome*], int)
  - Population(Population*)
  - ~Population()
  - vector[Genome*] getGenomes()
  - void addGenome(Genome*)
  - int getGeneration()
  - Population bests(int)
  - Population reproduction(Population)
  - Population mutateGroup(Population)
  - Population getChildren(int)
  - run()
}
```
faire une méthode pour population qui lance la simulation des moustiks un par un, qui détermine leur fitnesse à chacun et qui ensuite écrive dans des dossiers (génération/id xxx/yyy).

writeGenomes() de Population écrit un fichier "generationXXX.txt" contenant des séquences avec des retours à la ligne.

## Bibliographie
pdf
: [NatureOfCode](https://natureofcode.com/book/chapter-5-physics-libraries/)

youtube
: [DaddyLongLeg game](https://www.youtube.com/embed/noNl_cvYLu8)
