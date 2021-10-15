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
		this->ammo = 20;
	}
	void moveActor(int xDest, int yDest);
	void colision(int xDest, int yDest);
	void doSomething();
	void disparar();
	void aumentaAmmo(int cantidad);
	int getAmmo();
private:
	int ammo;

};

// Clase para paredes. No hacen mucho.
class Wall : public Actor
{
public:
	Wall(int startx, int starty, StudentWorld* world) : Actor(IID_WALL, startx, starty, none, world) {
		// WALLS DO NOT DO ANYTHING.
	}
};

class Boulder : public Actor {
public:
	Boulder(int startx, int starty, StudentWorld* world) : Actor(IID_BOULDER, startx, starty, none, world) {
		this->setAlive();
		this->setHealth(10);
	}
};

class Bullet : public Actor {
public:
	Bullet(int startx, int starty, StudentWorld* world, Direction dir) : Actor(IID_BULLET, startx, starty, dir, world) {
		this->setAlive();
	}
	void moveActor(int xDest, int yDest);
	void doSomething();
	void colision();
};

class Hole : public Actor {
public:
	Hole(int startx, int starty, StudentWorld* world) : Actor(IID_HOLE, startx, starty, none, world) {

	}
};

class Jewel : public Actor {
public:
	Jewel(int startx, int starty, StudentWorld* world) : Actor(IID_JEWEL, startx, starty, none, world) {

	}
};

class Exit : public Actor {
public:
	Exit(int startx, int starty, StudentWorld* world) : Actor(IID_EXIT, startx, starty, none, world) {
		this->setVisible(false);
	}
	void JoyasRecolectadas() {//Hace visible a la salida
		this->setVisible(true);
	}
};

class ExtraLife : public Actor {
public:
	ExtraLife(int startx, int starty, StudentWorld* world) : Actor(IID_EXTRA_LIFE, startx, starty, none, world) {

	}
};

class RestoreHealth : public Actor {
public:
	RestoreHealth(int startx, int starty, StudentWorld* world) : Actor(IID_RESTORE_HEALTH, startx, starty, none, world) {

	}
};

class Ammo : public Actor {
public:
	Ammo(int startx, int starty, StudentWorld* world) : Actor(IID_AMMO, startx, starty, none, world) {

	}
};

class SnarlbotVert : public Actor {
public:
	SnarlbotVert(int startx, int starty, StudentWorld* world) : Actor(IID_SNARLBOT, startx, starty, down, world) {
		this->setAlive();
		this->setHealth(10);
	}
};

class SnarlbotHoriz : public Actor {
public:
	SnarlbotHoriz(int startx, int starty, StudentWorld* world) : Actor(IID_SNARLBOT, startx, starty, right, world) {
		this->setAlive();
		this->setHealth(10);
	}
};

class Kleptobot : public Actor {
public:
	Kleptobot(int startx, int starty, StudentWorld* world) : Actor(IID_KLEPTOBOT, startx, starty, right, world) {
		this->setAlive();
		this->setHealth(5);
	}
};

class AngryKleptobot : public Actor {
public:
	AngryKleptobot(int startx, int starty, StudentWorld* world) : Actor(IID_ANGRY_KLEPTOBOT, startx, starty, right, world) {
		this->setAlive();
		this->setHealth(8);		
	}
};

class KleptobotFactory : public Actor {
public:
	KleptobotFactory(int startx, int starty, StudentWorld* world) : Actor(IID_ROBOT_FACTORY, startx, starty, right, world) {
		//Produce kleptobots normales
	}
};

class AngryKleptobotFactory : public KleptobotFactory {
public:
	AngryKleptobotFactory(int startx, int starty, StudentWorld* world) : KleptobotFactory(startx, starty, world) {
		//Produce angry kleptobots
	}
};

#endif // ACTOR_H_
