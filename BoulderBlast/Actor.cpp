//Diego Hiriart - Luis Corales - Martin Galvez - Daniela Estupinan
//ISWZ2104-01 Programacion 5
//2021-Sept-30
#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include "GameWorld.h"
#include <algorithm>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
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

void Jewel::setRecogido(bool estado) {
	this->recogido = estado;
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

void Goodie::setRecogido(bool estado) {
	this->recogido = estado;
	this->setVisible(!estado);//Debe ser el contrario del estado de recogido
}

bool Goodie::getRecogido() {
	return this->recogido;
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
		this->decreaseHealth(2);
		colisiona = false;
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
			dynamic_cast<SnarlBot*>(colision)->decreaseHealth(2);	
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 2://klepto
			this->eraseActor();
			this->alive = false;
			dynamic_cast<KleptoBot*>(colision)->decreaseHealth(2);
			this->getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		case 3://Angry klepto
			this->eraseActor();
			this->alive = false;
			dynamic_cast<AngryKleptoBot*>(colision)->decreaseHealth(2);
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
void Robot::damage(int damageAmt)
{
	Actor::damage(damageAmt);							// plays robot die sound if dead
	if (getHealth() <= 0)
		getWorld()->playSound(SOUND_ROBOT_DIE);
}

// SNARLBOT

SnarlBot::SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir) : Robot(world, startX, startY, IID_SNARLBOT, 10, 100, startDir)
{
}

void SnarlBot::doSomething()
{
	if (isItTime())										
	{
		if (needsClearShot())							
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			//getWorld()->colisiona(this);
			
		}
		else if (!(moveIfPossible()))					// tries to move but can not so reverses direction and tries to shoot + move
		{
			if (getDirection() == right)
			{
				setDirection(left);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == left)
			{
				setDirection(right);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == up)
			{
				setDirection(down);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == down)
			{
				setDirection(up);
				if (!needsClearShot())
					moveIfPossible();
			}
		}
	}
}

// KLEPTOBOT

KleptoBot::KleptoBot(StudentWorld* world, int startX, int startY, int imageID,
	unsigned int hitPoints, unsigned int score) : Robot(world, startX, startY, imageID, hitPoints, score, right)
{
	m_distanceBeforeTurning = (rand() % 6) + 1;
	m_goodie = nullptr;
	m_turns = 0;
}

void KleptoBot::setRandomDirection(int times)
{
	int i = rand() % times;
	switch (i)
	{
	case 1:
		setDirection(left);
		break;
	case 2:
		setDirection(right);
		break;
	case 3:
		setDirection(up);
		break;
	case 4:
		setDirection(down);
		break;
	}
}
// REGULAR KLEPTOBOT

RegularKleptoBot::RegularKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(world, startX, startY, IID_KLEPTOBOT, 5, 10)
{
}

void RegularKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	if (isItTime())
	{
		Actor* a = getActorByCoordinates(getX(), getY());			// check list from beginning since goodies are placed in front
		if (a->esRecogido() && myGoodie() == nullptr)			// only steal if does not have a goodie and the actor is a goodie
		{
			if ((rand() % 10) == 1)								// random chance to steal
			{
				setGoodie(dynamic_cast<Goodie*>(a));
				//a->~GraphObject(Goodie);									//makes it so that goodie doesn't do anything anymore while its stolen
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				a->setVisible(false);							// hides the goodie
				return;
			}
		}
		if (turnsLeft() < distanceBeforeTurning() && moveIfPossible())	// try to move if there are still turns left
		{
			incTurn();
		}
		else
		{
			setNewDistanceBeforeTurning((rand() % 6) + 1);				// set new random distance and reset values
			resetTurns();

			for (int i = 4; i > 0; i--)									// keep setting random direction until it can move
			{
				setRandomDirection(i);
				if (moveIfPossible())
				{
					incTurn();
					break;
				}
			}

		}
	}
}

void RegularKleptoBot::damage(int damageAmt)
{
	Robot::damage(damageAmt);
	if (getScore() <= 0)						// drop goodie if stole at current location when dead
	{
		if (myGoodie() != nullptr)
		{
			myGoodie()->setVisible(true);
			//myGoodie()->notStolen();
			myGoodie()->moveTo(getX(), getY());
		}
	}
}

//ANGRYKLEPTOBOT

AngryKleptoBot::AngryKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(world, startX, startY, IID_ANGRY_KLEPTOBOT, 8, 20)
{
}

void AngryKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	if (isItTime())							// checks to see if it is allowed to act
	{
		if (needsClearShot())				// shoot if clear shot towards player
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			//getWorld()->getPlayer(this);
			return;
		}
		Actor* a = getDirection(getX(), getY());
		if (a->isStealable() && myGoodie() == nullptr)			// same as regularkleptobot
		{
			if ((rand() % 10) == 1)
			{
				setGoodie(dynamic_cast<Goodie*>(a));
				//a->esRecogido();
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				a->setVisible(false);
				return;
			}
		}
		if (turnsLeft() < distanceBeforeTurning() && moveIfPossible())
		{
			incTurn();
		}
		else
		{
			setNewDistanceBeforeTurning(rand() % 6 + 1);
			resetTurns();

			for (int i = 4; i > 0; i--)
			{
				setRandomDirection(i);
				if (moveIfPossible())
				{
					incTurn();
					break;
				}
			}

		}
	}
}

void AngryKleptoBot::damage(int damageAmt)
{
	Robot::damage(damageAmt);
	if (getHealth() <= 0)
	{
		if (myGoodie() != nullptr)					// drop current goodie if have at current location when dead
		{
			myGoodie()->setVisible(true);
			myGoodie()->getRecogido();
			myGoodie()->moveTo(getX(), getY());
		}
	}
}



