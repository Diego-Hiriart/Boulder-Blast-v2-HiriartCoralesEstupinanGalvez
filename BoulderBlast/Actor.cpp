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
		this->alive == false;
	}
}
void Actor::changeDirection(Direction dir) {
	Direction d = this->getDirection();
	if (d != dir) {
		setDirection(dir);
	}
}

void Actor::disparar() {
	Direction direccion = this->getDirection();
	switch (direccion) {
	case up:
		this->getWorld()->addActor(new Bullet(this->getX(), this->getY() + 1, this->getWorld()));
		break;

	case down:
		this->getWorld()->addActor(new Bullet(this->getX(), this->getY() - 1, this->getWorld()));
		break;

	case left:
		this->getWorld()->addActor(new Bullet(this->getX() - 1, this->getY(), this->getWorld()));
		break;

	case right:
		this->getWorld()->addActor(new Bullet(this->getX() + 1, this->getY(), this->getWorld()));
		break;
	}
}


void Player::moveActor(int xDest, int yDest) {
	Actor* w = nullptr; // El personaje
	StudentWorld* world = getWorld();
	// Aqui el codigo para mover al personaje		
	if (world->getActorByCoordinates(xDest, yDest) == nullptr) {//Si el actor en las coordenadas es un pointer null, no hay obstaculos, puede moverse
		moveTo(xDest, yDest);
	}
}

void Player::doSomething() {
	if (!isVisible()) {
		setVisible(true);
	}
	StudentWorld* world = getWorld(); // world guarda el estado actual del nivel.
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