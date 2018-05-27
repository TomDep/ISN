/* -------------------------------------------------------------------------
	Création d'une classe Timer pour un projet d'ISN:
	  Compteur Vélo.
	Objectif:
 	  Pouvoir chronomètrer le temps avec Arduino afin d'avoir une valeur
 	  pour le calcul pour la vitesse moyenne.
	Méthodes:
	  - start
	  - stop
	  - pause
	  - resume
	  - getTotalTime
   -------------------------------------------------------------------------*/


//Condition pour éviter d'appeler plusieur fois la class
#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer
{
public:
	//Constructeur:
	Timer();
	//Méthodes:
	bool start();
	float pause();
	float stop();
	float resume();
	//Assesseur:
	float getTotalTime();
private:
	//Variables:
	bool _isRunning;
	bool _paused;
	float _startTime;
	float _stopTime;
	float _totalTime;
};

#endif