#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
#include "GraphObject.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		numeroPiedras = 0;
	}

	virtual int init();
	//Actor* getActorDamagedByBulletAt(int x, int y, Actor* actor);
	//Actor* getGoodieAt(int x, int y, Actor* act);
	/*{
		return GWSTATUS_CONTINUE_GAME;
	}*/

	virtual int move2()
	{
		  // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp();
	virtual int move();
	GraphObject* getPlayer();
	void setGameText();
	void addActor(GraphObject* g);
	GraphObject* getActorByCoordinates(int x, int y);
	void anotherGoodieAtThisLocation(int x, int y);
	void resetNumJewels();
	void increaseNumJewels();
	void reduceNumJewels();
	bool victory;
	int getNumActors();
	GraphObject* getTheActorAt(int i);
	void deleteElement(GraphObject* g);
	int getNumJewels();
	void setBonus();
	int getBonus();
	void decreaseBonus();

private:
	std::stringstream gameText; // to hold text of game to display at top 
	GraphObject* player; // the player of the game. 
	std::vector<GraphObject*> actorVector; // keeps tracks of all actors except the player
	int numJewels; // number of jewels on the map. If it reaches 0, exit appears
	int numActors; // number of actors in actorVector
	int bonus;

	/*Deben usar esta variable para contar el número de piedra*/
	int numeroPiedras;
};
#endif // STUDENTWORLD_H_