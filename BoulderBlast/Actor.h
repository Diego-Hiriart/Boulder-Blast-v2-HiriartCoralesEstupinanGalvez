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
	virtual bool isDead() const;
	virtual void setDead();
	std::string whoAmI() const;
	void fireBullet();
	void setHealth(int health);
	int getHealth();
	void morir();
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
	bool colision(GraphObject* destino);
	void doSomething();
	void disparar();
	void aumentaAmmo(int cantidad);
	void morir();
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
		this->setVisible(true);
	}
	void moveActor(int xDest, int yDest);
	void doSomething();
	bool colision(int x, int y);
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
	void setRecogido(bool estado);
	bool getRecogido();
private:
	bool recogido;
};

class Exit : public Actor {
public:
	Exit(int startx, int starty, StudentWorld* world) : Actor(IID_EXIT, startx, starty, none, world) {
		this->setVisible(false);
		this->accesible = false;
	}
	void JoyasRecolectadas();//Hace visible a la salida
	void JoyasPerdidas();//Vuelve a ocultar a la salida
private:
	bool accesible = false;
};

class Goodie : public Actor {
public:
	Goodie(int startx, int starty, StudentWorld* world, int ID) : Actor(ID, startx, starty, none, world) {
		this->recogido;
	}
	void setRecogido(bool estado);
	bool getRecogido();
private:
	bool recogido = false;
};

class ExtraLife : public Goodie {
public:
	ExtraLife(int startx, int starty, StudentWorld* world, int ID=IID_EXTRA_LIFE) : Goodie(startx, starty, world, ID) {

	}
};

class RestoreHealth : public Goodie {
public:
	RestoreHealth(int startx, int starty, StudentWorld* world, int ID=IID_RESTORE_HEALTH) : Goodie(startx, starty, world, ID) {

	}
};

class Ammo : public Goodie {
public:
	Ammo(int startx, int starty, StudentWorld* world, int ID = IID_AMMO) : Goodie(startx, starty, world, ID) {

	}
};


//intento 2 robot 
class Robot : public Actor
{
public:
	Robot(int graphicId, int startx, int starty, Direction dir, int health, int level, StudentWorld* world);
	virtual bool blocksPlayer(Direction dir);
	virtual bool damagedByBullet();
	virtual bool canDoSomething();
	virtual bool isBlocked(Direction dir);
	virtual bool canShoot(Direction dir);
	virtual void addPuntaje() = 0;
	virtual ~Robot();

private:
	int m_ticksMax;
	int m_health;
	int m_tickContador;
};
class SnarlBot : public Robot
{
public:
	SnarlBot(int graphicId, int startx, int starty, Direction dir, int level, StudentWorld* world);
	virtual void cambiarDir(Direction dir);
	virtual void addPuntaje();
	virtual ~SnarlBot();
};

class HorizontalBot : public SnarlBot
{
public:
	HorizontalBot(int graphicId, int startx, int starty, int level, StudentWorld* world);
	virtual void doSomething();
	virtual ~HorizontalBot();
};

class VerticalBot : public SnarlBot
{
public:
	VerticalBot(int graphicId, int startx, int starty, int level, StudentWorld* world);
	virtual void doSomething();
	virtual ~VerticalBot();
};
class KleptoBotFactory : public Actor
{
public:
	KleptoBotFactory(int imageID, int startX, int startY, std::string whatToProduce, StudentWorld* world);
	virtual void doSomething();
	virtual bool blocksPlayer(Direction dir);
	virtual bool damagedByBullet();
	virtual int canIProduce();
	virtual ~KleptoBotFactory();
private:
	std::string m_produce;
	bool produced;
};
class KleptoBot : public Robot
{
public:
	KleptoBot(int graphicId, int startX, int startY, int level, int healthOfBot, std::string name, StudentWorld* world);
	virtual void addPuntaje();
	virtual void doSomething();
	virtual void setDead();
	virtual ~KleptoBot();
private:
	void determineDirection(Direction dir);
	void changeDirection();
	int m_health;
	bool m_hasPickedUpItem;
	int m_distanceToMove;
	std::string whatAmIHolding;

};
class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(int imageID, int startX, int startY, int level, int healthOfBot, std::string name, StudentWorld* world);
	virtual void addScore();
	virtual ~AngryKleptoBot();
};

#endif // ACTOR_H_