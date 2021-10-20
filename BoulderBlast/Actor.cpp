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
	if (health <= 0) { //Matar al actor si su salud llega a cero
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
	this->getWorld()->deleteElement(this); //Quitar del mapa por completo
}

void Hole::doSomething() {
	if (this->isAlive()) {
		GraphObject* colision = this->getWorld()->getActorByCoordinates(this->getX(), this->getY());
		if (colision->getID() == 8) {
			this->morir();
			dynamic_cast<Boulder*>(colision)->morir();
		}
	}
	else {
		this->getWorld()->deleteElement(this);
	}
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
}

void Exit::JoyasPerdidas() {//Vuelve a oculatr a la salida
	this->setVisible(false);
}

void Exit::setRevelada(bool estado) {
	this->revelada = estado;
}

bool Exit::getRevelada() {
	return this->revelada;
}

void Exit::doSomething() {
	if (!this->isVisible()) {
		if (this->getWorld()->getNumJewels() <= 0) {//Si se recogieron todas la joyas, habilitar la salida
			this->JoyasRecolectadas();
		}
	}
	else if (!this->getRevelada()) {//Para que no suene al mismo tiempo que la joya
		this->getWorld()->playSound(SOUND_REVEAL_EXIT);
		this->setRevelada(true);
	}
}

void Goodie::setRecogido(bool estado) {//Definir si se ha recogido o volvió a su lugar de origen si se matoa un kleptobot
	this->recogido = estado;
	this->setVisible(!estado);//Debe ser el contrario del estado de recogido
}

bool Goodie::getRecogido() {
	return this->recogido;
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
	else if (destino->getID() == 8) {//Si hay otra piedra en el destino, solo puede moverse si no esta activa
		if (dynamic_cast<Boulder*>(destino)->isAlive()) {
			return false;
		}
		else {
			return true;
		}
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
	}
	else if (colision->getID() == 9) {//Hole, no colisiona y se mete en el hueco, ambos desapareen
		if (dynamic_cast<Hole*>(colision)->isAlive()) {
			dynamic_cast<Hole*>(colision)->morir();//Llenar hueco
			this->morir();//Quitar piedra usada
		}
		colisiona = false;
	}
	else {//En cualquier otro caso la piedra colisiona y nada pasa
		colisiona = true;
	}
	return colisiona;
}

void Boulder::doSomething() {
	this->colision(this->getX(), this->getY());
}

void Player::decreaseHealth(int amount) {
	if (this->getHealth() > 0) {
		this->setHealth(this->getHealth() - amount);
		if (this->getHealth() < 0) {
			this->setHealth(0);
		}
	}
	if (this->getHealth() <= 0) { //Matar al actor si su salud llega a cero
		this->morir();
	}
}

void Player::morir() {
	this->getWorld()->playSound(SOUND_PLAYER_DIE);
	this->eraseActor();
	this->alive = false;
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
	if (destino != nullptr) {
		switch (destino->getID()) {
		case 5://Bullet
			this->getWorld()->playSound(SOUND_PLAYER_IMPACT);
			this->decreaseHealth(2);
			dynamic_cast<Bullet*>(destino)->morir();
			colisiona = false;
			break;
		case 7://Exit
			if (dynamic_cast<Exit*>(destino)->isVisible()) {//Si la salida esta habilitada no colisionar y acabar el juego
				colisiona = false;
				this->getWorld()->victory = true;
			}
			else {//No colisionar pero no acabar el juego, no se gana aun
				colisiona = false;
			}
			break;
		case 8://Boulder
			if (dynamic_cast<Boulder*>(destino)->isAlive()) {//Si no esta usado o recogido, interactuar
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
	this->colision(this->getWorld()->getActorByCoordinates(this->getX(), this->getY()));
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
			this->setHealth(0);//Matar al actor
			this->morir();
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
		if (isAlive() && !colision(this->getX(), this->getY())) {//Si esta vivo y en su cuadro no colisiona avanzar
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
			colision(this->getX(), this->getY());//Ver si colisiona en la nueva posicion
		}
	}
}

bool Bullet::colision(int x, int y) {
	bool colisiona = true;
	GraphObject* colision = this->getWorld()->getActorByCoordinates(x, y);
	if (colision == nullptr) {
		colisiona = false;
	}
	else {
		switch (colision->getID()) {
		case 1://Snarl
			dynamic_cast<Snarlbot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			this->morir();
			break;
		case 2://klepto
			dynamic_cast<Kleptobot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			this->morir();
			break;
		case 3://Angry klepto
			dynamic_cast<AngryKleptobot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			this->morir();
			break;
		case 4://Factory
			this->morir();
			break;
		case 6://Pared
			this->morir();
			break;
		case 8://Boulder
			dynamic_cast<Boulder*>(colision)->decreaseHealth(2);
			this->morir();
			break;
		default://cuadro vacio u otra bala
			//Sigue viajando
			colisiona = false;
			break;
		}
	}
	return colisiona;
}

void Snarlbot::decreaseHealth(int amount) {
	if (this->getHealth() > 0) {
		this->setHealth(this->getHealth() - amount);
		if (this->getHealth() < 0) {
			this->setHealth(0);
		}
	}
	if (this->getHealth() <= 0) { //Matar al actor si su salud llega a cero
		this->morir();
	}
}

void Snarlbot::morir() {
	this->getWorld()->playSound(SOUND_ROBOT_DIE);
	this->getWorld()->increaseScore(100);
	this->eraseActor();
	this->alive = false;
	this->getWorld()->deleteElement(this);
}

void Snarlbot::doSomething() {
	if (isAlive()) {
		this->contador++;
		if (this->contador == this->ticks) {
			this->disparar();
			this->movimiento();
			this->contador = 0;//Resetear
		}
	}
}

int Snarlbot::Descanso() {
	int ticks = (28 - this->getWorld()->getLevel()) / 4; // levelNumber es el actual // número de nivel (0, 1, 2, etc.)

	if (ticks < 3) {
		ticks = 3; // ningún SnarlBot debe moverse con mas frecuencia que esto
	}

	return ticks;
}

void Snarlbot::disparar() {
	Direction dir = this->getDirection();
	bool encontrado = false;
	GraphObject* cuadro = nullptr;
	int x = this->getX(), y = this->getY();
	while (true) {
		switch (dir) {
		case up:
			cuadro = this->getWorld()->getActorByCoordinates(x, ++y);//para ver si hay obstaculos, debe dejar de buscar si es asi			
			break;
		case down:
			cuadro = this->getWorld()->getActorByCoordinates(x, --y);
			break;
		case left:
			cuadro = this->getWorld()->getActorByCoordinates(--x, y);
			break;
		case right:
			cuadro = this->getWorld()->getActorByCoordinates(++x, y);
			break;
		}
		if (x == this->getWorld()->getPlayer()->getX() && y == this->getWorld()->getPlayer()->getY()) {//Si el player esta en cuadro revisado, se debe disparar
			encontrado = true;
			break;
		}
		if (cuadro != nullptr) {//Si hay un obstaculo dejar de buscar
			break;
		}
	}

	if (encontrado) {//Disparar si puede ver al player, osea si el cuadro al hacer cast como player no es null	
		switch (dir) {
		case up:
			this->getWorld()->addActor(new Bullet(this->getX(), this->getY() + 1, this->getWorld(), dir));
			break;

		case down:
			this->getWorld()->addActor(new Bullet(this->getX(), this->getY() - 1, this->getWorld(), dir));
			break;

		case left:
			this->getWorld()->addActor(new Bullet(this->getX() - 1, this->getY(), this->getWorld(), dir));
			break;

		case right:
			this->getWorld()->addActor(new Bullet(this->getX() + 1, this->getY(), this->getWorld(), dir));
			break;
		}
		this->getWorld()->playSound(SOUND_ENEMY_FIRE);
	}
}

void Snarlbot::movimiento() {
	Direction dir = this->getDirection();
	switch (dir)
	{
	case up:
		if (!this->colision(this->getX(), this->getY()+1)) {
			this->moveTo(this->getX(), this->getY() + 1);
		}
		else {
			this->changeDirection(down);
		}
		break;
	case down:
		if (!this->colision(this->getX(), this->getY()-1)) {
			this->moveTo(this->getX(), this->getY() - 1);
		}
		else {
			this->changeDirection(up);
		}
		break;
	case left:
		if (!this->colision(this->getX()-1, this->getY())) {
			this->moveTo(this->getX()-1, this->getY());
		}
		else {
			this->changeDirection(right);
		}
		break;
	case right:
		if (!this->colision(this->getX()+1, this->getY())) {
			this->moveTo(this->getX()+1, this->getY());
		}
		else {
			this->changeDirection(left);
		}
		break;
	}
}

bool Snarlbot::colision(int x, int y) {
	bool colisiona = true;	
	GraphObject* destino = this->getWorld()->getActorByCoordinates(x, y);
	if (destino != nullptr) {
		switch (destino->getID()) {
		case 7://Exit
			colisiona = true;
			break;
		case 8://Boulder
			colisiona = true;
			break;
		case 10://Jewel		
			colisiona = false;
			break;
		case 11://Health		
			colisiona = false;
			break;
		case 12://live
			colisiona = false;
			break;
		case 13://Ammo	
			colisiona = false;
			break;
		default://snarl, kleptos, pared, factory, boulder, hole
			//Nada
			colisiona = true;//Por si acaso
			break;
		}
	}
	else if(destino == nullptr){
		colisiona = false;
	}
	return colisiona;
}
