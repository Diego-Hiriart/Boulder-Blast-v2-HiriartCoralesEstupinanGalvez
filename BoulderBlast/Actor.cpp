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
}
void Actor::changeDirection(Direction dir) {
	Direction d = this->getDirection();
	if (d != dir) {
		setDirection(dir);
	}
}


void Player::moveActor(int xDest, int yDest){ 
		Actor* w = nullptr; // El personaje
		StudentWorld* world = getWorld(); 
		// Aqui el codigo para mover al personaje
		//Aqui inicia el control para saber si hay una pared en el destino, usando las posiciones de levelxx.dat en Assets	
		//Este codigo para leer el archivo y para saber que hacer en cada caso para las coordendas sale del init() de StudentWorld.cpp
		//Las funciones para leer e intepretar el documento .dat son de Level.h
		Level lev(assetDirectory);
		std::stringstream level;
		level.str(std::string());
		level << "level" << setfill('0') << setw(2) << this->getWorld()->getLevel() << ".dat";
		//cout << "Nivel:" << level.str() << "\n";
		/* Este cout es para Debug, no estaba seguro de que hace 
		* "level << " level" << setfill('0') << setw(2) << this->getWorld()->getLevel() << ".dat"; " 
		* y tenia que saber que este cargando el nivel correcto */
		Level::LoadResult result = lev.loadLevel(level.str());//Cargar el nivel para sacar lo que hay en cada coordenada
		if (result == Level::load_success) {//Seguir si encuentra el nivel; por si acaso para evitar errores, como se hace en StudentWorld.cpp
			bool puedeMoverse = false;//Siempre partir de que no podra moverse, por si acaso
			Level::MazeEntry ge = lev.getContentsOf(xDest, yDest);//Ver el contenido de las coordenadas de destino
			switch (ge){//Segun el contenido de las coordendas de destino, determinar si se puede mover o no
				case Level::empty://Si esta vacio el destino, se puede mover, cambiar puedeMoverse a true 
					puedeMoverse = true;
				break;	

				case Level::wall://Si hay una pared, no se puede mover, asegurarse de que puedeMoverse este en false
					puedeMoverse=false;
				break;

				/*Este default es necesario porque hay otros contenidos en las coordendas del nivel en el archivo .dat, 
				* pero no aparecen en la pantalla aun. El personaje debe poder moverse si no hay algo, por eso si no es una pared 
				* y tampoco esta vacio, debe poder moverse tambien (puedeMoverse=true)*/
				default:
					puedeMoverse = true;
				break;
			}			
			if (puedeMoverse) {//Si se detecto que no hay una pared, se puede mover
				this->moveTo(xDest, yDest);//Desplazamiento
			}			
		}		
	}

void Player::doSomething(){
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

	}

}