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
	void decreaseHealth(int amount);
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
	void doSomething();
	bool colision(int x, int y);
	bool puedeMoverse(Direction dir);
	void moverPiedra(Direction dir);
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
		this->setAlive();
	}
	void doSomething();
};

class Jewel : public Actor {
public:
	Jewel(int startx, int starty, StudentWorld* world) : Actor(IID_JEWEL, startx, starty, none, world) {
		this->setAlive();
		this->recogido = false;
	}
	void setRecogido(bool estado);
	bool getRecogido();
private:
	bool recogido;
};

class Exit : public Actor {
public:
	Exit(int startx, int starty, StudentWorld* world) : Actor(IID_EXIT, startx, starty, none, world) {
		this->setAlive();
		this->setVisible(false);
		this->revelada = false;
	}
	void doSomething();
	void JoyasRecolectadas();//Hace visible a la salida
	void JoyasPerdidas();//Vuelve a ocultar a la salida
	void setRevelada(bool estado);
	bool getRevelada();	
private:
	bool revelada;
};

class Goodie : public Actor {
public:
	Goodie(int startx, int starty, StudentWorld* world, int ID) : Actor(ID, startx, starty, none, world) {
		this->setAlive();
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

class Snarlbot : public Actor {
public:
	Snarlbot(int startx, int starty, StudentWorld* world, Direction dir) : Actor(IID_SNARLBOT, startx, starty, dir, world) {
		this->setAlive();
		this->setHealth(10);
		this->contador = 0;
		this->ticks = Descanso();
	}
	void doSomething();
	bool colision(int x, int y);
	void disparar();
	void decreaseHealth(int amount);
	void morir();
	void movimiento();
private:
	int ticks;
	int contador;
	int Descanso();
};

class Kleptobot : public Actor {
public:
	Kleptobot(int startx, int starty, StudentWorld* world, int IID, bool angry) : Actor(IID, startx, starty, right, world) {
		this->setAlive();
		this->contador = 0;
		this->ticks = Descanso();
		this->angry = angry;
		if (!angry) {//Definir si angrykleptbot o no
			this->setHealth(5);
		}
		else {
			this->setHealth(8);
		}	
	}
	void doSomething();
	bool colision(int x, int y);
	void decreaseHealth(int amount);
	void morir();
	void setHaRobado(bool estado);
	bool getHaRobado();
	bool isAngry();
private:
	int ticks;
	int contador;
	bool angry = false;
	bool haRobado = false;
	int Descanso();
	int distanciaGiro = 0;
	GraphObject* goodieRobado = nullptr;
	void distanciaDireccion();
	bool robar();
	bool movimiento();
	bool disparar();
	void accionNormal();
	void accionAngry();
};

class KleptobotFactory : public Actor {
public:
	KleptobotFactory(int startx, int starty, StudentWorld* world, bool angry) : Actor(IID_ROBOT_FACTORY, startx, starty, none, world) {
		this->setAlive();
		this->angry = angry;	
	}
	void doSomething();
	bool puedeFabricar();
	void fabricar();
	bool isAngry();
private:
	bool angry = false;
};

#endif // ACTOR_H_
