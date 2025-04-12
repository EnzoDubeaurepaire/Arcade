# 📚 Documentation : Ajouter un module graphique ou un jeu

Bienvenue dans la documentation de l'Arcade.  
Ce guide vous explique comment intégrer un nouveau **module graphique** ou un **jeu** à l'architecture existante.

---

## 📦 Créer un module graphique
Un module graphique est une bibliothèque dynamique (.so) qui implémente l'interface `Arcade::IDisplayModule`.

### 📁 Fichier à inclure
```c++
#include "IDisplayModule"
```

### 📌 Méthodes à implémenter
| Méthode                          | Rôle                                                                 |  
|:---------------------------------|:----------------------------------------------------------------------|  
| `openWindow()` | Ouvre la fenêtre graphique                                           |  
| `closeWindow()` | Ferme la fenêtre graphique                                           |  
| `initObject(map<string, unique_ptr<IObject>>& objects)` | Initialise les objets à afficher (chargement des textures, propriétés…) |  
| `display(map<string, unique_ptr<IObject>>& objects)` | Affiche les objets à l'écran                                         |  
| `getInput()` | Récupère les entrées clavier                                         |  
| `getMousePos()` | Récupère la position de la souris                                    |  
| `getName()` | Renvoie le nom du module graphique (exemple : "SFML", "SDL") |

### 📌 Exemple de déclaration

```c++
class MyDisplayModule : public Arcade::IDisplayModule {
	// Implémentation des méthodes ici
};
```

### 📌 Compilation
Le module doit être compilé en bibliothèque dynamique `.so` :
```bash
g++ -shared -fPIC -o lib_mydisplay.so MyDisplayModule.cpp
```
La bibliothèque créée doit ensuite être placée dans le dossier `./lib/`.

---

## 🎮 Créer un jeu

Un jeu est une bibliothèque dynamique (.so) qui implémente l'interface `Arcade::IGameModule`.

### 📁 Fichier à inclure
```c++
#include "IGameModule"
```

### 📌 Méthodes à implémenter
| Méthode                          | Rôle                                                                 |  
|:---------------------------------|:----------------------------------------------------------------------|  
| `update(pair<int, int> mousePos, int input)` | Met à jour l'état du jeu en fonction des entrées clavier / souris                                           |  
| `getObjects` | Renvoie la map des objets du jeu (sprites, textes…)                                           |  
| `getName()` | Renvoie le nom du jeu (exemple : "SNAKE", "PACMAN") |  
| `getScore()` | Renvoie le score du joueur                                         |  

### 📌 Exemple de déclaration

```c++
class MyGame : public Arcade::IGameModule {
	// Implémentation des méthodes ici
};
```

### 📌 Compilation
Le module doit être compilé en bibliothèque dynamique `.so` :
```bash
g++ -shared -fPIC -o lib_mygame.so MyGame.cpp
```
La bibliothèque créée doit ensuite être placée dans le dossier `./lib/`.

---

## 📐 À propos des objets (IObject)

Les objets à afficher ou à manipuler dans le jeu sont des classes implémentant `Arcade::IObject`.

### 📌 Types d'objets disponibles

- SPRITE
- TEXT

### 📌 Propriétés disponibles
Utilisez `IObject::TextProperties` et `IObject::SpriteProperties` via le variant `IObject::Properties` pour stocker les propriétés graphiques des objets.

---

## 📝 Exemple d'implémentation complète
Un exemple fonctionnel de module graphique et de jeu est disponible dans le dépôt, vous pouvez vous en inspirer.