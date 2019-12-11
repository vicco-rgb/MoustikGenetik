# Compte Rendu
_Chloé Leric_ et _Victor Duvivier_.

Notre projet consiste en l'application d'un ou de plusieurs algorithmes génétiques sur un mini-jeu. Nous nous sommes inspirés du mini-jeu [Daddy Long Legs](https://play.google.com/store/apps/details?id=com.setsnail.daddylonglegs&hl=fr)

<img src="imgs/daddy.jpg" height=200>

## Sujet

Le but de notre algorithme est de générer un ensemble de `moustiks` et de les faire jouer dans un environnement physique 2D. Un algorithme génétique est ensuite appliqué pour améliorer les performances de déplacement des moustiks. Le critère utilisé pour départager les individus est la distance (positive) maximale parcourue par l'utilisateur.

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
Population--MoustikIA
MoustikIA--Genome
Moustik--|>MoustikIA
Moustik--Coord
Moustik--Forme

class Coord {
  - float x
  - float y
  - Coord()
  - Coord(float, float)
}

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

class Moustik {
  # Forme* ptrHead
  # Forme* ptrLegL
  # Forme* ptrLegR
  # b2RevoluteJoint* rotuleL
  # b2RevoluteJoint* rotuleR
  # int com
  # bool dead
  # float angleMax
  # string controlType
  # Genome* genome
  # bool seqWritten
  - Moustik(b2World*, Coord)
  - ~Moustik()
  - void commande(b2World*, int)
  - Coord getPos()
  - bool isDead()
  - int getAge()
  - void upAge()
  - virtual bool undertaker(int)
  - void updateFitness()
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
  - Genome* getGenome();
  - void setGenome(Genome*)
  - void setID(string)
  - string getID()
  - void activation(bool)
  - void play(b2World*, int)
  - virtual bool undertaker(int)
}

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
  - float getFitness()
  - void setFitness(float)
  - void addAbsoluteDate(int);
  - Genome* crossSplit(Genome*)
  - Genome* crossAvg(Genome*)
  - Genome* mutation()
  - bool betterThan(Genome*)
}

class Population {
  # vector[MoustikIA*] moustiks
  # int generation
  - Population()
  - Population(vector<MoustikIA*>, int)
  - Population(Population*)
  - ~Population()
  - vector<MoustikIA*> getMoustiks()
  - void addMoustik(MoustikIA*)
  - int getGeneration()
  - void setGeneration(int)
  - Population bests(int)
  - Population reproduction(Population)
  - Population mutateGroup(Population)
  - Population getChildren(int)
  - void playLive(int)
  - void playOff()
  - void writeGenomes()
  - vector<Genome*> readGenomes(string)
}
```

writeGenomes() de Population écrit un fichier "generationXXX.txt" contenant des séquences avec des retours à la ligne.

## Bibliographie
pdf
: [NatureOfCode](https://natureofcode.com/book/chapter-5-physics-libraries/)

youtube
: [DaddyLongLeg game](https://www.youtube.com/embed/noNl_cvYLu8)