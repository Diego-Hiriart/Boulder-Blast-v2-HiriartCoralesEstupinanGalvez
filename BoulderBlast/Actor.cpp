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



//crea la clase Robot -de-
Robot::Robot(StudentWorl* swd, int ID, int x, int y, int health Direction d, int bonus)
	:Agent(swd, ID, x, y, health, SOUND_ENEMY_FIRE, d), m_bonus(bonus), m_continue(false)
{
	setTick();
}void Robot::doSomething() {
	m_continue = false;
	if (!isAlive())
		return;
	if (m_tick != 1)//solo se puede hacer uno por trick
	{
		decTrick();
		return;
	}

	if (doesShoot() && canAim())//se dispara y si esta frente al jugadir
	{
		shoot();
		setTick();
		return;
	}
	m_continue = true;
}

void Robot::doDamage()
{
	Actor::doDamage();
	if (isAlive())
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else {
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(m_bonus);
	}
}
bool Robot::canAim() //usado para ver si el robot puede disparar al jugador
{
	if (getWorld()->getPlayer()->getX() == getX())
	{
		if (getWorld()->getPlayer()->getY() > getY() && getDirection() == up)
		{
			for (int k = 1; k < getWorld()->getPlayer()->getY() - getY(); k++)
			{
				if (getWorld()->spaceContains(getX(), getY() + k) != nullptr)
				{
					if (getWorld()->spaceContains(getX(), getY() + k)->isBarrier())
						return false;
				}
			}
			return true;
		}

		else if (getWorld()->getPlayer()->getY() < getY() && getDirection() == down)
		{
			for (int k = 1; k < getY() - getWorld()->getPlayer()->getY(); k++)
			{
				if (getWorld()->spaceContains(getX(), getY() - k) != nullptr)
				{
					if (getWorld()->spaceContains(getX(), getY() - k)->isBarrier())
						return false;
				}
			}
			return true;
		}
	}


	else if (getWorld()->getPlayer()->getY() == getY())
	{
		if (getWorld()->getPlayer()->getX() > getX() && getDirection() == right)
		{

			for (int k = 1; k < getWorld()->getPlayer()->getX() - getX(); k++)
			{
				if (getWorld()->spaceContains(getX() + k, getY()) != nullptr)
				{
					if (getWorld()->spaceContains(getX() + k, getY())->isBarrier())
						return false;
				}
			}
			return true;

		}

		else if (getWorld()->getPlayer()->getX() < getX() && getDirection() == left)
		{
			for (int k = 1; k < getX() - getWorld()->getPlayer()->getX(); k++)
			{
				if (getWorld()->spaceContains(getX() - k, getY()) != nullptr)
				{
					if (getWorld()->spaceContains(getX() - k, getY())->isBarrier())
						return false;
				}
			}
			return true;

		}
	}

	return false;
}

void Robot::setTick()
{
	m_tick = (28 - getWorld()->getLevel()) / 4;

	if (m_tick < 3)
		m_tick = 3;
}

void Robot::decTick()
{
	m_tick--;
}
}
bool Robot::doesShoot() const
{
	return true;
}
bool Robot::shouldContinue() const
{
	return m_continue;
}

SnarlBot::SnarlBot(StudentWorld* swd, int x, int y, Direction d)
	: Robot(swd, IID_SNARLBOT, x, y, 10, d, 100)
{}
void Snarlbot::doSomething() {
	//if (isAlive()) {
	Robot::doSomething();
	if (!shouldContinue())
		return;
	bool didMove = moveRegular(getDirection());
	if (!didMove)       //si no se mueve, que retorne otra direccion
	{
		if (getDirection() == right)
			setDirection(left);

		else if (getDirection() == left)
			setDirection(right);

		else if (getDirection() == up)
			setDirection(down);

		else if (getDirection() == down)
			setDirection(up);

	}
	setTick(); //resetee tick
//}
	KleptoBot::KleptoBot(StudentWorld * swd, int ID, int x, int y, int health, int bonus)
		: Robot(swd, ID, x, y, health, right, bonus), hasGoodie(false), m_currentDirectionSteps(0)
	{
		m_distanceBeforeTurning = rand() % 6 + 1;       //distancia random entre 1-6
	}

	void KleptoBot::doSomething()
	{
		Robot::doSomething();

		if (!shouldContinue())
			return;

		bool didMove = false;

		Pickup* goodie = getWorld()->takeStealable(getX(), getY());

		if (goodie != nullptr && !hasGoodie)        //para recolectar un goodie
		{
			int stealChance = rand() % 10 + 1;  //chance de robar en random de 1-10
			if (stealChance == 5)
			{
				goodie->setDead();      //destruir el goodie
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				hasGoodie = true;
				if (goodie->getBonus() == 1000)
					goodieKind = 'L';
				else if (goodie->getBonus() == 500) //si matas al kleptobot
					goodieKind = 'R';
				else if (goodie->getBonus() == 100)
					goodieKind = 'A';
				setTick();
				return;
			}

		}

		if (m_currentDirectionSteps <= m_distanceBeforeTurning)
		{
			didMove = moveRegular(getDirection());

			if (didMove)
			{
				m_currentDirectionSteps++;
				setTick();
				return;
			}
		}


		if (!didMove)       //seleccionar una direccion random, en caso de que no se pueda mover 
		{
			m_distanceBeforeTurning = rand() % 6 + 1;

			int y = 0;
			bool doneUp = false;
			bool doneRight = false;
			bool doneDown = false;
			bool doneLeft = false;


			while (int x = rand() % 4 + 1)
			{

				if (y == 0)  //empezara con un num aleatorio del 1-4
					y = x;

				if (x == 1 && !doneUp)
				{

					if (moveRegular(up))
					{
						setDirection(up);
						setTick();
						return;
					}

					doneUp = true;
				}

				if (x == 2 && !doneRight) //si no ha sido probado
				{
					if (moveRegular(right))    //probar para mover a la derecha 
					{
						setDirection(right);        //en caso de que si se mueva se pone la direccion y el tick
						setTick();
						return;
					}
					doneRight = true;
				}

				if (x == 3 && !doneDown)
				{
					if (moveRegular(down))
					{
						setDirection(right);
						setTick();
						return;
					}
					doneDown = true;
				}
				if (x == 4 && !doneLeft)
				{
					if (moveRegular(left))
					{
						setDirection(left);
						setTick();
						return;
					}
					doneLeft = true;
				}

				if (doneUp && doneRight && doneDown && doneLeft)
					break;
			}


			// si no se puede mover, poner la direccion de cara a la primera direccion
			if (y == 1)
				setDirection(up);
			if (y == 2)
				setDirection(right);
			if (y == 3)
				setDirection(down);
			if (y == 4)
				setDirection(left);
		}

		setTick();
	}

	void KleptoBot::doDamage()
	{
		Robot::doDamage();
		if (!isAlive() && hasGoodie)      //si tiene un goodie se le agrega donde murio el robot 
		{
			getWorld()->addGoodieBack(getX(), getY(), goodieKind);

		}

	}

	bool KleptoBot::countsInFactoryCount() const
	{
		return true;    //solo el kleptobot se cuenta
	}



	RegularKleptoBot::RegularKleptoBot(StudentWorld * swd, int x, int y)
		:KleptoBot(swd, IID_KleptoBot, x, y, 5, 10)
	{}

	bool RegularKleptoBot::doesShoot() const
	{
		return false;
	}



	AngryKleptoBot::AngryKleptoBot(StudentWorld * swd, int x, int y)
		:KleptoBot(swd, IID_ANGRY_KleptoBot, x, y, 8, 20)
	{}
}