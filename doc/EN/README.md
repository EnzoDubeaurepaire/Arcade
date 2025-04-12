# 📚 Documentation : Add a Display module or a Game

Welcome in the Arcade's  documentation.
This guide explains how to integrate a new **graphics module** or **game** into the existing architecture.

---

## 📦 Create a Display module
A display module is a dynamic library (.so) that implements the `Arcade::IDisplayModule` interface.

### 📁 File to include
```c++
#include "IDisplayModule"
```

### 📌 Methods to implement
| Method                          | Role                                                                |  
|:---------------------------------|:----------------------------------------------------------------------|  
| `openWindow()` | Opens the window                                          |  
| `closeWindow()` | Closes the window                                           |  
| `initObject(map<string, unique_ptr<IObject>>& objects)` | Initializes the objects to be displayed (loading textures, properties, etc.) |  
| `display(map<string, unique_ptr<IObject>>& objects)` | Display the objects on screen                                         |  
| `getInput()` | Get the Keyboard / Mouse inputs                                         |  
| `getMousePos()` | Get the position of the mouse                                    |  
| `getName()` | Returns the name of the Display module (example : "SFML", "SDL") |

### 📌 Declaration example

```c++
class MyDisplayModule : public Arcade::IDisplayModule {
	// Implement the methods here
};
```

### 📌 Compilation
The module must be compiled into a `.so` dynamic library:
```bash
g++ -shared -fPIC -o lib_mydisplay.so MyDisplayModule.cpp
```
The created library should then be placed in the `./lib/` folder.

---

## 🎮 Create a Game

A game is a dynamic library (.so) that implements the `Arcade::IGameModule` interface.

### 📁 File to include
```c++
#include "IGameModule"
```

### 📌 Methods to implement
| Method                         | Role                                                                |  
|:---------------------------------|:----------------------------------------------------------------------|  
| `update(pair<int, int> mousePos, int input)` | Updates game state based on keyboard/mouse input                                           |  
| `getObjects` | Returns the map of game objects (sprites, texts, etc.)                                        |  
| `getName()` | Returns the name of the game (example: "SNAKE", "PACMAN") |  
| `getScore()` | Returns the player's score                                         |  

### 📌 Declaration example

```c++
class MyGame : public Arcade::IGameModule {
	// Implement the methods here
};
```

### 📌 Compilation
The module must be compiled into a `.so` dynamic library:
```bash
g++ -shared -fPIC -o lib_mygame.so MyGame.cpp
```
The created library should then be placed in the `./lib/` folder.

---

## 📐 About Objects (IObject)

Objects to be displayed or manipulated in the game are classes implementing `Arcade::IObject`.

### 📌 Types of objects available

- SPRITE
- TEXT

### 📌 Available properties
Use `IObject::TextProperties` and `IObject::SpriteProperties` via the `IObject::Properties` variant to store graphical properties of objects.

---

## 📝 Example of complete implementation
A working example of a graphics module and game is available in the repository, you can get inspiration from it.