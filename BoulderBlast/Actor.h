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
//robot
class Agent : public Actor
{
public:
	Agent(int imageID, int startX, int startY, StudentWorld* world, Direction dir, unsigned int hitPoints);
	virtual ~Agent() {}
	
	bool moveIfPossible();

	// si se daña
	virtual bool isDamageable() const { return true; }

	// ver si puede mover o disparar
	virtual bool needsClearShot() const;

	//parar el bullet
	virtual bool stopsBullet() const { return true; }
	// bloquear el player
	virtual bool blocksPlayer() const { return true; }
};

class Robot : public Agent
{
public:
	Robot(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints, unsigned int score, Direction startDir);
	virtual ~Robot() {}
	// no se mueva el boulder
	virtual bool canPushBoulders() const { return false; }

	virtual bool needsClearShot();

	//sonido cuando muere
	virtual bool isDamageable() const;


	bool isItTime();


	virtual void damage(int damageAmt);


	virtual int getScore() { return m_score; }
private:
	int m_score;
	int currentTick;
	int m_ticks;
	bool timeToAct;
};

class SnarlBot : public Robot
{
public:
	SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual ~SnarlBot() {}
	virtual void doSomething();
};

class KleptoBot : public Robot
{
public:
	KleptoBot(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints, unsigned int score);
	virtual ~KleptoBot() {}


	virtual bool countsInFactoryCensus() const { return true; }


	void incTurn() { m_turns++; }

	
	int turnsLeft() { return m_turns; }

	// return private data member m_distanceBeforeTurning
	int distanceBeforeTurning() { return m_distanceBeforeTurning; }

	// create new random distance 
	void setNewDistanceBeforeTurning(int a) { m_distanceBeforeTurning = a; }

	//random direction
	void setRandomDirection(int times);

	
	void resetTurns() { m_turns = 0; }

	
	Goodie* myGoodie() { return m_goodie; }

	// valor de vida del goodie
	void setGoodie(Goodie* a) { m_goodie = a; }
private:
	Goodie* m_goodie;
	int m_turns;
	int m_distanceBeforeTurning;
};

class RegularKleptoBot : public KleptoBot
{
public:
	RegularKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~RegularKleptoBot() {}

	
	virtual void doSomething();

	// puede llevar los goodies
	virtual void damage(int damageAmt);
};

class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~AngryKleptoBot() {}

	//disparar
	virtual void doSomething();

	// para poner dejar el goodie donde muera 
	virtual void damage(int damageAmt);
};

class KleptoBotFactory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };

	KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
	virtual ~KleptoBotFactory() {}

	// crear kleptobots
	virtual void doSomething();


	virtual bool stopsBullet() const { return true; }

	// returna que  IIDD es 
	ProductType getProductType() { return m_productType; }


	virtual bool allowsAgentColocation() const { return false; }
private:
	int m_count;
	ProductType m_productType;
};
#endif // ACTOR_H_