#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <algorithm>
#include "GraphObject.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	StudentWorld* s = new StudentWorld(assetDir);
	return s;
	//return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init()
{
	victory = false;
	Level lev(assetDirectory());
	std::stringstream level;
	level.str(std::string());
	if (getLevel() > 4) { // if completed level 99 then end game
		return GWSTATUS_PLAYER_WON;
	}
	level << "level" << setfill('0') << setw(2) << getLevel() << ".dat";
	
	Level::LoadResult result = lev.loadLevel(level.str());
	if (result == Level::load_fail_file_not_found) {
		return GWSTATUS_PLAYER_WON; // level not found, assume no more levels so player wins!
	}
	else if (result == Level::load_fail_bad_format) {
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success) {
		bool visible = false;
		setBonus();
		for (int x = 0; x < VIEW_WIDTH; x++) {
			for (int y = 0; y < VIEW_HEIGHT; y++) {
				Level::MazeEntry ge = lev.getContentsOf(x, y);
				visible = false;
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::player:
					if (player != nullptr) {
						return GWSTATUS_LEVEL_ERROR;
					}
					player = new Player(x, y, this);
					break;
				case Level::wall:
					addActor(new Wall(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::exit:
					addActor(new Exit(x, y, this));//Se inicia invisible en el constructor
					break;

				case Level::horiz_snarlbot:
					addActor(new Snarlbot(x, y, this, GraphObject::Direction::right));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::vert_snarlbot:
					addActor(new Snarlbot(x, y, this, GraphObject::Direction::down));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::kleptobot_factory:
					addActor(new KleptobotFactory(x, y, this, false));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::angry_kleptobot_factory:
					addActor(new KleptobotFactory(x, y, this, true));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::boulder:
					addActor(new Boulder(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::hole:
					addActor(new Hole(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::jewel:
					this->increaseNumJewels();
					addActor(new Jewel(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::restore_health:
					addActor(new RestoreHealth(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::extra_life:
					addActor(new ExtraLife(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;

				case Level::ammo:
					addActor(new Ammo(x, y, this));
					getActorByCoordinates(x, y)->setVisible(true);
					break;
				}
			}
		}
	}
	gameText << "Score: " << getScore() << " Level : " << getLevel() << " Lives : " << getLives() << " Health : " << (dynamic_cast<Player*>(player)->getHealth() * 5) << "% Ammo : " << 10 << " Bonus : " << getBonus();
	setGameStatText(gameText.str());
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	bool endGame = false;
	bool levelRetry = false;
	if (dynamic_cast<Actor*>(player)->alive) {
		dynamic_cast<Player*>(player)->doSomething();
		if (victory) {
			increaseScore(getBonus()); // give any extra points from the bonus
			increaseScore(2000); // grant points for completing the level
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		if (!(dynamic_cast<Actor*>(player)->alive)) {
			levelRetry = true;
			goto exit;
		}
	}
	else {
		endGame = true;
	}
	bool turn;
	for (int i = 0; i < actorVector.size(); i++) {
		int g = actorVector.size();
		if (dynamic_cast<Wall*>(actorVector[i]) == nullptr)
			dynamic_cast<Actor*>(actorVector[i])->doSomething();
		if (g > actorVector.size()) {
			i -= (g - actorVector.size()); // since new object will be at current location
		}
	}
	if (dynamic_cast<Actor*>(getPlayer())->getHealth() <= 0) {
		goto exit;
	}
	if (getBonus() > 0) {
		decreaseBonus();
	}
	setGameText();
	return GWSTATUS_CONTINUE_GAME;
exit: // if player dies or user decides to restart level
	this->decLives();
	if (!levelRetry) {
		this->playSound(SOUND_PLAYER_DIE); // should not play if player decides to retry, like in the original game
	}
	return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp() { // delete bullets, actors, and the player on the field
	delete player;
	//resetNumJewels();
	player = nullptr;
	for (int i = 0; i < getNumActors(); i++) {
		deleteElement(getTheActorAt(i));
		i--;
	}
	victory = false; // reset value for next level
}

GraphObject* StudentWorld::getPlayer() {
	return dynamic_cast<Player*>(player);
}

void StudentWorld::setGameText() {
	gameText.str(std::string());
	gameText << "Score: " << setfill('0') << setw(7) << getScore();
	gameText << "  Level: " << setfill('0') << setw(2) << getLevel();
	gameText << "  Lives: " << setfill(' ') << setw(2) << getLives();
	gameText << "  Health: " << setfill(' ') << setw(3) << (dynamic_cast<Player*>(player)->getHealth() * 5);
	gameText << "  Ammo: " << setfill(' ') << setw(3) << dynamic_cast<Player*>(player)->getAmmo();
	gameText << "  Bonus: " << setfill(' ') << setw(4) << getBonus();
	setGameStatText(gameText.str());
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


void StudentWorld::addActor(GraphObject* g) {
	int i = 0;
	actorVector.push_back(g);
}


GraphObject* StudentWorld::getActorByCoordinates(int x, int y) {
	for (int i = 0; i < actorVector.size(); i++) {
		if ((actorVector[i])->getX() == x && (actorVector[i])->getY() == y) {
			return actorVector[i];
		}
	}
	return nullptr;
}

//Modificacion ligera de getActorByCoordinates(), retorna solo si el objeto es el IID que se quiere, osea cierto actor
GraphObject* StudentWorld::getActorPorCoordIID(int x, int y, int IID) {
	for (int i = 0; i < actorVector.size(); i++) {
		if ((actorVector[i])->getX() == x && (actorVector[i])->getY() == y) {
			GraphObject* posible = actorVector[i];		
			if (posible->getID() == IID) {//Retornar solo si es el graph que quiero			
				return actorVector[i];
			}			
		}
	}
	return nullptr;
}

//Los mismo que getActorByCoordinates() pero inicia desde el final para encontrar elementos que se anadieron luego de cargar el mapa
GraphObject* StudentWorld::getActorPorCoordReversa(int x, int y) {
	int i = actorVector.size() - 1;
	while (i > 0) {
		if ((actorVector[i])->getX() == x && (actorVector[i])->getY() == y) {				
			return actorVector[i];
		}
		i--;
	}
	return nullptr;
}

int StudentWorld::getNumActors() {
	numActors = actorVector.size();
	return numActors;
}

GraphObject* StudentWorld::getTheActorAt(int i) {
	return actorVector[i];
}

void StudentWorld::deleteElement(GraphObject* g) {
	actorVector.erase(std::remove(actorVector.begin(), actorVector.end(), g), actorVector.end());
	delete g;
}

void StudentWorld::setBonus() {
	bonus = 1000;
}
int StudentWorld::getBonus() {
	return bonus;
}
void StudentWorld::decreaseBonus() {
	bonus--;
}

void StudentWorld::reduceNumJewels() {
	numJewels--;
}

void StudentWorld::increaseNumJewels() {
	numJewels++;
}

int StudentWorld::getNumJewels() {
	return this->numJewels;
}

