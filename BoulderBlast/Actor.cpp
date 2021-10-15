//Diego Hiriart - Luis Corales - Martin Galvez - Daniela Estupinan
//ISWZ2104-01 Programacion 5
//2021-Sept-30
#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include "GameWorld.h"
using namespace std;

const string assetDirectory = "Assets";//Parte del path para leer los niveles, salio de main.cpp

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/* METHODS FOR Actor CLASS*/
void Actor::showActor() {
	(*this).setVisible(true);
}
void Actor::eraseActor() {
	(*this).setVisible(false);
}
void Actor::setAlive() {
	alive = true;
}
bool Actor::isAlive() {
	return alive;
}
void Actor::setHealth(int health) {
	this->health = health;
}
int Actor::getHealth() {
	return health;
}
void Actor::decreaseHealth(int amount) {
	if (health > 0) {
		this->health -= amount;
		if (health < 0) {
			health = 0;
		}
	}
	if (health <= 0) {//Matar al actor si su salud llega a cero
		this->morir();
	}
}
void Actor::changeDirection(Direction dir) {
	Direction d = this->getDirection();
	if (d != dir) {
		setDirection(dir);
	}
}

void Actor::morir() {
	this->eraseActor();
	this->alive = false;
	this->getWorld()->deleteElement(this);
}

int Player::getAmmo() {
	return this->ammo;
}

void Player::disparar() {
	if (this->getAmmo() > 0) {
		Direction direccion = this->getDirection();
		switch (direccion) {
		case up:
			this->getWorld()->addActor(new Bullet(this->getX(), this->getY() + 1, this->getWorld(), direccion));
			break;

		case down:
			this->getWorld()->addActor(new Bullet(this->getX(), this->getY() - 1, this->getWorld(), direccion));
			break;

		case left:
			this->getWorld()->addActor(new Bullet(this->getX() - 1, this->getY(), this->getWorld(), direccion));
			break;

		case right:
			this->getWorld()->addActor(new Bullet(this->getX() + 1, this->getY(), this->getWorld(), direccion));
			break;
		}
		this->getWorld()->playSound(SOUND_PLAYER_FIRE);
		this->ammo -= 1;
	}
}


void Player::moveActor(int xDest, int yDest) {
	Actor* w = nullptr; // El personaje
	StudentWorld* world = getWorld();
	// Aqui el codigo para mover al personaje	
	GraphObject* destino = world->getActorByCoordinates(xDest, yDest);
	if (destino == nullptr) {//Si el actor en las coordenadas es un pointer null, no hay obstaculos, puede moverse				
		moveTo(xDest, yDest);
	}
	else if (!colision(destino->getID())) {//Si el metodo de colisionar retorna false, no colisiona y se puede pasar
		moveTo(xDest, yDest);
	}
}

bool Player::colision(int destino) {
	bool colisiona = true;
	switch (destino) {
	case 5://Bullet
		this->decreaseHealth(2);
		colisiona = false;
		break;
	case 10://Jewel
		this->getWorld()->increaseScore(50);
		this->getWorld()->reduceNumJewels();
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		colisiona = false;
		break;
	case 11://Health
		this->setHealth(20);
		this->getWorld()->increaseScore(500);
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		colisiona = false;
		break;
	case 12://live
		this->getWorld()->incLives();
		this->getWorld()->increaseScore(1000);
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		colisiona = false;
		break;
	case 13://Ammo
		this->getWorld()->increaseScore(100);
		this->aumentaAmmo(20);
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		colisiona = false;
		break;
	default://snarl, kleptos, pared, factory, boulder, hole
		//Nada
		colisiona = true;//Por si acaso
		break;
	}
	return colisiona;
}

void Player::aumentaAmmo(int cantidad) {
	this->ammo += 20;
}

void Player::doSomething() {
	if (!isVisible()) {
		setVisible(true);
	}
	StudentWorld* world = getWorld(); // world guarda el estado actual del nivel.
	if (this->isAlive()) {
		// Aqui el codigo para detectar las teclas.
		//La deteccion de teclas salio del ejemplo en la tarea
		int tecla;//Declaracion de variable para almacenar tecla
		this->getWorld()->getKey(tecla);//Guardar tecla presionada
		int x = this->getX(), y = this->getY();//Obtener posicion actual del actor, a esta se le modifica segun la tecla presionada
		switch (tecla) {//Switch para saber que tecla se presiono y segun ello obtener coordendas de destino para moveActor()
		case KEY_PRESS_LEFT:
			x--;//Se mueve a la izquierda (modificar posicion, coordenadas de destino a usar en moveActor)
			this->changeDirection(left);//Ver a la izq
			this->moveActor(x, y);//Mover al actor a la posicion modificada, se verifica si se puede mover dentro del metodo
			break;

		case KEY_PRESS_UP:
			y++;//Se mueve hacia arriba
			this->changeDirection(up);//Ver arriba
			this->moveActor(x, y);
			break;

		case KEY_PRESS_DOWN:
			y--;//Se mueve para abajo
			this->changeDirection(down);//Ver abajo
			this->moveActor(x, y);
			break;

		case KEY_PRESS_RIGHT:
			x++;//Actualizar posicion a la derecha
			this->changeDirection(right);//Ver a la derecha
			this->moveActor(x, y);
			break;

		case KEY_PRESS_ESCAPE:
			this->decreaseHealth(this->getHealth());//Matar al actor
			break;

		case KEY_PRESS_SPACE:
			this->disparar();
			break;
		}
	}
}


void Bullet::moveActor(int xDest, int yDest) {
	moveTo(xDest, yDest);
}

void Bullet::doSomething() {
	if (isAlive()) {	
		colision(this->getX(), this->getY());
		Direction direccion = this->getDirection();
		switch (direccion) {
		case up:		
			moveActor(this->getX(), this->getY() + 1);
			break;

		case down:	
			moveActor(this->getX(), this->getY() - 1);			
			break;

		case left:			
			moveActor(this->getX() - 1, this->getY());		
			break;

		case right:		
			moveActor(this->getX() + 1, this->getY());			
			break;
		}
		colision(this->getX(), this->getY());
	}
}

bool Bullet::colision(int x, int y) {
	bool colisiona = false;
	GraphObject* colision = this->getWorld()->getActorByCoordinates(x, y);
	if (colision == nullptr) {
		colisiona = false;
	}
	else {
		switch (colision->getID()) {
		case 0://Player
			this->eraseActor();
			this->alive = false;
			dynamic_cast<Player*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 1://Snarl
			this->eraseActor();
			this->alive = false;
			dynamic_cast<Snarlbot*>(colision)->decreaseHealth(2);	
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 2://klepto
			this->eraseActor();
			this->alive = false;
			dynamic_cast<Kleptobot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 3://Angry klepto
			this->eraseActor();
			this->alive = false;
			break;
		case 4://Factory
			this->eraseActor();
			this->alive = false;
			break;
		case 6://Pared
			this->eraseActor();
			this->alive = false;
			break;
		case 8:
			this->eraseActor();
			this->alive = false;
			break;
		default://cuadro vacio u otra bala
			//Sigue viajando
			colisiona = false;
			break;
		}
	}	
	return colisiona;
}

void Snarlbot::doSomething() {
	if (isAlive()) {
		
	}
}