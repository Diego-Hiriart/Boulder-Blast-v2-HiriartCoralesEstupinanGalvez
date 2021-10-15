#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <random>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
bool alive;// = false;

Actor(int graphicId, int startx, int starty, Direction dir, StudentWorld* world) : world(world), GraphObject(graphicId, startx, starty, dir) {
(*this).setVisible(true);
}
void showActor();
void eraseActor();
void setAlive();
bool isAlive();
void setHealth(int health);
int getHealth();
StudentWorld* getWorld() {
	return this->world;
}
void decreaseHealth(int amount);
void changeDirection(Direction dir);
virtual void moveActor(Actor* actor, int xDest, int yDest, Actor* actors[VIEW_WIDTH][VIEW_HEIGHT]) {};
virtual void doSomething() {};
private:
	StudentWorld* world;
	int health;// = -1; // default value.
}; 

//

// The player class. Clase del jugador. No cambiar esta clase. 
class Player : public Actor
{
public:
	Player(int startx, int starty, StudentWorld* world) : Actor(IID_PLAYER, startx, starty, right, world) {
		this->setAlive(); // player is alive
		this->setHealth(20);
	}
	void moveActor(int xDest, int yDest);
	void doSomething();

};

// Clase para paredes. No hacen mucho.
class Wall : public Actor
{
public:
	Wall(int startx, int starty, StudentWorld* world) : Actor(IID_WALL, startx, starty, none, world) {
		// WALLS DO NOT DO ANYTHING.
	}
};

#endif // ACTOR_H_
