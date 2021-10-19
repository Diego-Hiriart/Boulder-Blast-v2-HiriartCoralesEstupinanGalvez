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

//intento 2
Robot::Robot(int graphicId, int startx, int starty, Direction dir,int health, int level, StudentWorld* world)
	:Actor(graphicId, startx, starty, dir, world)
{
	setVisible(true);
	m_ticksMax = (28 - level) / 4;
	m_tickContador = 1;
	m_health = health;
}
bool Robot::blocksPlayer(Direction dir) { //bloquear para que el jugador no pueda avanzar
	return true;
}
bool Robot::damagedByBullet() //cuando el jugar le dispara al robot
{
	m_health -= 2;
	getWorld()->playSound(SOUND_ROBOT_IMPACT);
	if (m_health <= 0)
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		addPuntaje();
		morir();
	}
	return true;
}

bool Robot::canDoSomething() //mientras el tick sea 1 el robot puede realizar diferentes acciones
{
	if (m_tickContador == m_ticksMax)
	{
		m_tickContador = 1;
		return true;
	}
	else 
	{
		m_tickContador++;
		return false;
	}
}

bool Robot::isBlocked(Direction dir) //si esta bloqueado va a cambiar la direction del robot
{
	switch (dir)
	{
	case up:
		if (getWorld()->getActorByCoordinates(getX(), getY() + 1) == nullptr)
		{
			return false;
		}
		else 
		
			return true;
		break;
	case down:
		if(getWorld()->getActorByCoordinates(getX(), getY() -1) == nullptr)
		{
			return false;
		}
		else

		return true;
		break;
	case left:
		if(getWorld()->getActorByCoordinates(getX()-1, getY()) == nullptr)
		{
			return false;
		}
		else

		return true;
		break;
	case right:
		if (getWorld()->getActorByCoordinates(getX() + 1, getY()) == nullptr)
		{
			return false;
		}
		else

			return true;
		break;
	default:
		return true;
	}
	return true;
}
Robot::~Robot() //declaracion del robot y su virtual 
{

}

//SNARLBOT

SnarlBot::SnarlBot(int graphicId, int startx, int starty, Direction dir, int level, StudentWorld* world)
:Robot(graphicId, startx, starty, dir, 10, level, world) 
{

}

SnarlBot::~SnarlBot() {}

void SnarlBot::cambiarDir(Direction dir)  //cambiar direcion mientras se mueve
{
	switch (dir)
	{
	case up:
		setDirection(down);
		break;
	case down:
		setDirection(up);
		break;
	case left:
		setDirection(right);
		break;
	case right:
		setDirection(left);
		break;
	default:
		return;
	}
}

void SnarlBot::addPuntaje()
{
	getWorld()->increaseScore(100);
}
//HORIZONATAL BOT

HorizontalBot::HorizontalBot(int graphicId, int startx, int starty, int level, StudentWorld* world)
	:SnarlBot(graphicId, startx, starty, right,level, world) 
{

}
void HorizontalBot::doSomething() 
{
	if (isDead()) {
		return;
	}
	if (canDoSomething() == false)
		return;
	if (canShoot(getDirection()))
	{
		fireBullet();
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return;
	}
	if (isBlocked(getDirection())) //si esta bloqueado va a cambiar de direccion
	{
		cambiarDir(getDirection());
	}
	else 
	{
		if (getDirection() == right)
		{
			moveTo(getX() + 1, getY());
		}
		if (getDirection() == left)
		{
			moveTo(getX() - 1, getY());
		}
	
	}
}
HorizontalBot::~HorizontalBot(){}

//vertical snarlbot
VerticalBot::VerticalBot(int imageID, int startX, int startY, int level, StudentWorld* world)
	:SnarlBot(imageID, startX, startY, up, level, world)
{

}

void VerticalBot::doSomething() //parecido al horizontalbot 
{


	if (isDead())
		return;
	if (canDoSomething() == false)
		return;
	if (canShoot(getDirection()))
	{
		fireBullet();
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return;
	}
	if (isBlocked(getDirection()))
	{
		cambiarDir(getDirection());
	}
	else
	{
		if (getDirection() == up)
		{
			moveTo(getX(), getY() + 1);
		}
		if (getDirection() == down)
		{
			moveTo(getX(), getY() - 1);
		}
	}
}

VerticalBot::~VerticalBot()
{

}

//Kleptobot factory

KleptoBotFactory::KleptoBotFactory(int imageID, int startX, int startY, string whatToProduce, StudentWorld* world)
	:Actor(imageID, startX, startY, none, world)
{
	setVisible(true);
	m_produce = whatToProduce; //producir kleptobots
	produced = false;
}

int KleptoBotFactory::canIProduce() {
	int count = 0;
	for (int x = getX() - 3; x <= getX() + 3; x++)
	{
		if (x >= 0 && x < 15)
		{
			for (int y = getY() - 3; y <= getY() + 3; y++)
			{
				if (y >= 0 && y < 15)
				{
					Actor* temp = getWorld()->getActorDamagedByBulletAt(x, y,this); //delclarado en StudentWorld.h
					
					if (temp != nullptr && (temp->whoAmI() == "KleptoBot" || temp->whoAmI() == "Angry KleptoBot")) //detectar que tipo de kleptobot es
						count++;
				}

			}
		}
	}
	return count;
}
void KleptoBotFactory::doSomething() //ERROR EN GETACTORDAMAGE
{


	if (canIProduce() < 3)
	{
		Actor* temp = getWorld()->getActorDamagedByBulletAt(getX(), getY(), this);
		if (temp != nullptr && (temp->whoAmI() == "KleptoBot" || temp->whoAmI() == "Angry KleptoBot"))
			return;
		int random = rand() % 50 + 1;
		if (random == 1)
		{
			getWorld()->playSound(SOUND_ROBOT_BORN);
			if (m_produce == "KleptoBot")
				getWorld()->addActor(new KleptoBot(IID_KLEPTOBOT, getX(), getY(), getWorld()->getLevel(), 5, "KleptoBot", getWorld()));//obtiene el ID DE CADA KLEPTOBOT
			else if (m_produce == "Angry KleptoBot")
			{
				getWorld()->addActor(new AngryKleptoBot(IID_KLEPTOBOT, getX(), getY(), getWorld()->getLevel(), 8, "Angry KleptoBot", getWorld()));
			}
			produced = true;
		}
	}
}

bool KleptoBotFactory::blocksPlayer(Direction dir)//Bloquea al jugador 
{
	return true;
}

bool KleptoBotFactory::damagedByBullet() {
	return true;
}

KleptoBotFactory::~KleptoBotFactory() {}

KleptoBot::KleptoBot(int imageID, int x, int y, int level, int healthOfBot, string name, StudentWorld* world)
	:Robot(imageID, x, y, right, healthOfBot, level, world)
{
	setVisible(true);
	m_hasPickedUpItem = false;
	m_distanceToMove = rand() % 6 + 1;
	whatAmIHolding = "";
}
void KleptoBot::doSomething()
{
	if (isDead())
		return;
	if (canDoSomething() == false)
		return;
	if (whoAmI() == "Angry KleptoBot")
	{
		if (canShoot(getDirection()))
		{
			fireBullet();
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			return;
		}
	}
	Actor* temp = getWorld()->getGoodieAt(getX(), getY(), this);//declarado en StudentWorld.h
	if (temp != nullptr)
	{
		string whatIsIt = temp->whoAmI();
		if (m_hasPickedUpItem == false && (whatIsIt == "Health" || whatIsIt == "ExtraLife" || whatIsIt == "Ammunition"))
		{
			int random = rand() % 10 + 1;
			if (random == 1)
			{
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				m_hasPickedUpItem = true;
				whatAmIHolding = whatIsIt;
				temp->setDead();
			}
		}
	}
	if (m_distanceToMove > 0 && !isBlocked(getDirection()))
	{
		m_distanceToMove--;
		switch (getDirection())
		{
		case up:
			moveTo(getX(), getY() + 1);
			break;
		case down:
			moveTo(getX(), getY() - 1);
			break;
		case left:
			moveTo(getX() - 1, getY());
			break;
		case right:
			moveTo(getX() + 1, getY());
			break;
		default:
			break;
		}
	}
	else
	{
		m_distanceToMove = rand() % 6 + 1;
		changeDirection();
		if (!isBlocked(getDirection()))
		{
			m_distanceToMove--;
			switch (getDirection())
			{
			case up:
				moveTo(getX(), getY() + 1);
				break;
			case down:
				moveTo(getX(), getY() - 1);
				break;
			case left:
				moveTo(getX() - 1, getY());
				break;
			case right:
				moveTo(getX() + 1, getY());
				break;
			default:
				break;
			}
		}

	}

}
void KleptoBot::changeDirection() { //cambia la direccion de manera aleatoria 
	int randomDirection = rand() % 4 + 1;
	switch (randomDirection)
	{
	case 1:
		determineDirection(up);
		break;
	case 2:
		determineDirection(down);
		break;
	case 3:
		determineDirection(right);
		break;
	case 4:
		determineDirection(left);
		break;
	default:
		cerr << "error en num random" << endl;
		break;

	}
}
void KleptoBot::determineDirection(Direction dir)
{


	Direction curr = dir;
	if (dir == up)
	{
		if (!isBlocked(up))
		{
			setDirection(up);
		}
		else if (!isBlocked(down))
		{
			setDirection(down);
		}
		else if (!isBlocked(left))
		{
			setDirection(left);
		}
		else if (!isBlocked(right))
		{
			setDirection(right);
		}
		else
			setDirection(curr);
		return;
	}
	else if (dir == down)
	{
		if (!isBlocked(down))
		{
			setDirection(down);
		}
		else if (!isBlocked(up))
		{
			setDirection(up);
		}
		else if (!isBlocked(left))
		{
			setDirection(left);
		}
		else if (!isBlocked(right))
		{
			setDirection(right);
		}
		else
			setDirection(curr);
		return;
	}
	else if (dir == right)
	{
		if (!isBlocked(right))
		{
			setDirection(right);
		}
		else if (!isBlocked(up))
		{
			setDirection(up);
		}
		else if (!isBlocked(left))
		{
			setDirection(left);
		}
		else if (!isBlocked(down))
		{
			setDirection(down);
		}
		else
			setDirection(curr);
		return;

	}
	else if (dir == left)
	{
		if (!isBlocked(left))
		{
			setDirection(left);
		}
		else if (!isBlocked(up))
		{
			setDirection(up);
		}
		else if (!isBlocked(right))
		{
			setDirection(right);
		}
		else if (!isBlocked(down))
		{
			setDirection(down);
		}
		else
			setDirection(curr);
		return;

	}
	else
	{
		cerr << "error" << endl;
	}

}

void KleptoBot::setDead()
{

	// cuadno el robot muere se crea un goodie o se deja el goodie en la posicion que murio

	if (whatAmIHolding == "ExtraLife")
		getWorld()->addActor(new ExtraLife(getX(), getY(), getWorld()));
	else if (whatAmIHolding == "Ammunition")
		getWorld()->addActor(new Ammo(getX(), getY(), getWorld()));
	else if (whatAmIHolding == "Health")
		getWorld()->addActor(new RestoreHealth(getX(), getY(), getWorld()));
	Actor::setDead();

}

void KleptoBot::addPuntaje()
{
	getWorld()->increaseScore(10); //puntaje con el que empiezan 
}

KleptoBot::~KleptoBot()//declaracion
{

}

//Angry

AngryKleptoBot::AngryKleptoBot(int imageID, int x, int y, int level, int healthOfBot, string name, StudentWorld* world)
	:KleptoBot(imageID, x, y, level, healthOfBot, name, world)
{

}

void AngryKleptoBot::addScore()
{
	getWorld()->increaseScore(20);
}

AngryKleptoBot::~AngryKleptoBot()
{

}

/*bool Robot::canShoot(Direction dir)
{
	if (dir == up)
	{
		for (int y = getY(); y < 15;y++)
		{
			Actor* temp = getWorld()->getActorByCoordinates(getX(), y);
			if (temp != nullptr)
			{
				if (temp->morir() == false)
				{
					if (temp->whoAmI() == "Player")
						return true;
					else
						return false;
				}
			}

		}
	}
	else if (dir == down)
	{
		for (int y = getY(); y >= 0; y--)
		{
			Actor* temp = getWorld()->getActorByCoordinates(getX(), y);
			if (temp != nullptr)
			{
				if (temp->isDead() == false)
				{
					if (temp->whoAmI() == "Player")
						return true;
					else
						return false;
				}
			}

		}

	}
	else if (dir == left)
	{
		for (int x = getX(); x >= 0; x--)
		{
			Actor* temp = getWorld()->getActorDamagedByBulletAt(x, getY(), this);
			if (temp != nullptr)
			{
				if (temp->isDead() == false)
				{
					if (temp->whoAmI() == "Player")
					{
						return true;
					}
					else
						return false;
				}
			}
		}
	}
	else if (dir == right)
	{
		for (int x = getX(); x < 15; x++)
		{
			Actor* temp = getWorld()->getActorByCoordinates(x, getY());
			if (temp != nullptr)
			{
				if (temp->morir() == false)
				{
					if (temp->showActor) == "Player")
						return true;
					else
						return false;
				}
			}
		}
	}
	return false;
}*/




