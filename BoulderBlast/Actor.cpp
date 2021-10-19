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
}

void Hole::llenar() {
	this->llenado = true;
	this->morir();
	this->getWorld()->deleteElement(this);
}

bool Hole::getLlenado() {
	return this->llenado;
}

void Jewel::setRecogido(bool estado) {
	this->recogido = estado;
	this->setVisible(!estado);//Debe ser el contrario del estado de recogido
}

bool Jewel::getRecogido() {
	return this->recogido;
}

void Exit::JoyasRecolectadas() {//Hace visible a la salida
	this->setVisible(true);
	this->accesible = true;
}

void Exit::JoyasPerdidas() {//Vuelve a oculatr a la salida
	this->setVisible(false);
	this->accesible = false;
}

bool Exit::getAccesible() {
	return this->accesible;
}

int Exit::getIterator() {
	return this->i;
}

void Exit::setIterator(int i) {
	this->i = i;
}

void Exit::doSomething() {
	if (this->getWorld()->getNumJewels() <= 0) {//Si se recogieron todas la joyas, habilitar la salida
		this->JoyasRecolectadas();
		if (this->getIterator() == 0) {
			// ESPERAR TICKS PARA QUE NO SUENE AL MISMO TIEMPO QUE LA JOYA?
			this->getWorld()->playSound(SOUND_REVEAL_EXIT);
			this->setIterator(1);
		}
	}	
}

void Goodie::setRecogido(bool estado) {//Definir si se ha recogido o volvió a su lugar de origen si se matoa un kleptobot
	this->recogido = estado;
	this->setVisible(!estado);//Debe ser el contrario del estado de recogido
}

bool Goodie::getRecogido() {
	return this->recogido;
}

void Boulder::usar() {
	this->usada = true;
	this->morir();//Debe ser removida totalmente
}

bool Boulder::getUsada() {
	return this->usada;
}

void Boulder::moverPiedra(Direction dir) {
	switch (dir)
	{
	case up:
		this->moveTo(this->getX(), this->getY() + 1);
		break;
	case down:
		this->moveTo(this->getX(), this->getY() - 1);
		break;
	case left:
		this->moveTo(this->getX() - 1, this->getY());
		break;
	case right:
		this->moveTo(this->getX() + 1, this->getY());
		break;
	}
}

bool Boulder::puedeMoverse(Direction dir) {
	GraphObject* destino = nullptr;
	switch (dir)
	{
	case up:
		destino = this->getWorld()->getActorByCoordinates(this->getX(), this->getY() + 1);
		break;
	case down:
		destino = this->getWorld()->getActorByCoordinates(this->getX(), this->getY() - 1);
		break;
	case left:
		destino = this->getWorld()->getActorByCoordinates(this->getX() - 1, this->getY());
		break;
	case right:
		destino = this->getWorld()->getActorByCoordinates(this->getX() + 1, this->getY());
		break;
	}
	if (destino == nullptr) {
		return true;
	}
	else if (destino->getID() == 9) {//Hole
		return true;
	}
	else {//En cualquier otro caso la piedra no se puede mover
		return false;
	}
}

bool Boulder::colision(int x, int y) {
	bool colisiona = true;
	GraphObject* colision = this->getWorld()->getActorByCoordinates(x, y);
	if (colision == nullptr) {//No colisiona y ninguna accion es tomada
		colisiona = false;
	}else if (colision->getID() == 9) {//Hole, no colisiona y se mete en el hueco, ambos desapareen
		colisiona = false;
		dynamic_cast<Hole*>(colision)->llenar();//Llenar hueco
		this->usar();//Quitar piedra usada
	}else {//En cualquier otro caso la piedra colisiona y nada pasa
		colisiona = true;
	}
	return colisiona;
}

void Boulder::doSomething() {
	this->colision(this->getX(), this->getY());
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
	else if (!colision(destino)) {//Si el metodo de colisionar retorna false, no colisiona y se puede pasar
		moveTo(xDest, yDest);
	}
}

bool Player::colision(GraphObject* destino) {
	bool colisiona = true;
	switch (destino->getID()) {
	case 5://Bullet
		colisiona = false;
		break;
	case 7://Exit
		if (dynamic_cast<Exit*>(destino)->getAccesible() == 1) {//Si la salida esta habilitada no colisionar y acabar el juego
			colisiona = false;
			this->getWorld()->victory = true;
		}
		else {//No colisionar pero no acabar el juego, no se gana aun
			colisiona = false;
		}
		break;
	case 8://Boulder
		if (!dynamic_cast<Boulder*>(destino)->getUsada()) {//Si no esta usado o recogido, interactuar
			if (dynamic_cast<Boulder*>(destino)->puedeMoverse(this->getDirection())) {//Moverse y no colisionar si la piedra puede ser movida
				dynamic_cast<Boulder*>(destino)->moverPiedra(this->getDirection());//Mover la piedra
				colisiona = false;
			}
			else {
				colisiona = true;//Si la piedra no se puede mover, hay colision
			}			
		}
		else {
			colisiona = false;//Si ya se uso la piedra, no hay nada con que colisionar
		}		
		break;
	case 10://Jewel
		if (!dynamic_cast<Jewel*>(destino)->getRecogido()) {
			this->getWorld()->increaseScore(50);
			this->getWorld()->reduceNumJewels();
			this->getWorld()->playSound(SOUND_GOT_GOODIE);
			dynamic_cast<Jewel*>(destino)->setRecogido(true);
		}
		colisiona = false;
		break;
	case 11://Health
		if (!dynamic_cast<RestoreHealth*>(destino)->getRecogido()) {
			this->setHealth(20);
			this->getWorld()->increaseScore(500);
			this->getWorld()->playSound(SOUND_GOT_GOODIE);
			dynamic_cast<RestoreHealth*>(destino)->setRecogido(true);
		}
		colisiona = false;
		break;
	case 12://live
		if (!dynamic_cast<ExtraLife*>(destino)->getRecogido()) {
			this->getWorld()->incLives();
			this->getWorld()->increaseScore(1000);
			this->getWorld()->playSound(SOUND_GOT_GOODIE);
			colisiona = false;
			dynamic_cast<ExtraLife*>(destino)->setRecogido(true);
		}
		break;
	case 13://Ammo
		if (!dynamic_cast<Ammo*>(destino)->getRecogido()) {
			this->getWorld()->increaseScore(100);
			this->aumentaAmmo(20);
			this->getWorld()->playSound(SOUND_GOT_GOODIE);
			dynamic_cast<Ammo*>(destino)->setRecogido(true);
		}
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
		if (isAlive()) {//Pudo haber impactado con algo en la colision anterior
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
			dynamic_cast<AngryKleptobot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 4://Factory
			this->eraseActor();
			this->alive = false;
			break;
		case 6://Pared
			this->eraseActor();
			this->alive = false;
			break;
		case 8://Boulder
			this->eraseActor();
			this->alive = false;
			dynamic_cast<Boulder*>(colision)->decreaseHealth(2);
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