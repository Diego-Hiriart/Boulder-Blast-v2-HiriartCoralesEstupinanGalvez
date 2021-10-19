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
	// Move to the adjacent square in the direction the agent is facing
	// if it is not blocked, and return true.  Return false if the agent
	// can't move.
	bool moveIfPossible();

	// agents are able to take damage 
	virtual bool isDamageable() const { return true; }

	// Return true if this agent can push boulders (which means it's the
	// player).
	virtual bool needsClearShot() const;

	//agents stop bullets from moving
	virtual bool stopsBullet() const { return true; }
	// agents block player from moving
	virtual bool blocksPlayer() const { return true; }
};

class Robot : public Agent
{
public:
	Robot(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints, unsigned int score, Direction startDir);
	virtual ~Robot() {}
	// robots are unable to push boulders
	virtual bool canPushBoulders() const { return false; }
	// used to determine whether robot is allowed to shoot
	virtual bool needsClearShot();

	// used to play sound when damaged
	virtual bool isDamageable() const;

	//checks to see whether it can move due to tick system
	bool isItTime();

	// used to play death sound when dead
	virtual void damage(int damageAmt);

	// used to obtain private data member m_score
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

	// used when factory does its census
	virtual bool countsInFactoryCensus() const { return true; }

	// increment turns tick
	void incTurn() { m_turns++; }

	// turns left before can act
	int turnsLeft() { return m_turns; }

	// return private data member m_distanceBeforeTurning
	int distanceBeforeTurning() { return m_distanceBeforeTurning; }

	// create new random distance before turning
	void setNewDistanceBeforeTurning(int a) { m_distanceBeforeTurning = a; }

	// sets a random direction
	void setRandomDirection(int times);

	// sets m_turns back to 0 to restart cycle
	void resetTurns() { m_turns = 0; }

	// retrieves pointer to private data member Goodie since each KleptoBot may have a Goodie
	Goodie* myGoodie() { return m_goodie; }

	// set Kleptobot m_goodie value
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

	// has its own original behavior
	virtual void doSomething();

	// special case since RegularKleptoBot can carry goodie 
	virtual void damage(int damageAmt);
};

class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~AngryKleptoBot() {}

	// has RegularKleptoBot's but also tries to shoot
	virtual void doSomething();

	// used to place goodies down if dead
	virtual void damage(int damageAmt);
};

class KleptoBotFactory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };

	KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
	virtual ~KleptoBotFactory() {}

	// create KleptoBots
	virtual void doSomething();

	// blocks bullets from continuing
	virtual bool stopsBullet() const { return true; }

	// returns private data member m_productType
	ProductType getProductType() { return m_productType; }

	// does not allow to re-step onto factory
	virtual bool allowsAgentColocation() const { return false; }
private:
	int m_count;
	ProductType m_productType;
};
#endif // ACTOR_H_