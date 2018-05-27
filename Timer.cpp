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

//Importation des différent header
#include "Arduino.h"
#include "Timer.h"

//Constructeur de l'objet Timer
Timer::Timer()
{
	_isRunning = false;
	_paused = false;
	_startTime = 0;
	_stopTime = 0;
	_totalTime = 0;
}

bool Timer::start()
{
	//On détecte si le Timer a déjà démarré.
	if(!_isRunning)
	{
		_isRunning = true;
		//Affectation de la valeur de départ
		_startTime = millis();
		return true;
	}
	else
	{
		return false;
	}
}

float Timer::pause()
{
	/* On dédecte si le Timer a démarré et si il n'est pas déjà en pause.
	La méthode renvoie le tps total.
	Dans le cas contraire, la méthode renvoi -1	
	*/
	if(_isRunning && !_paused)
	{
		_paused = true;
		//Calcul du tps passé depuis la dernière pause/début
		float timePassed = millis() - _startTime;
		//Ajout du temps passé au temps total depuis la dernière pause/début
		_totalTime += timePassed;
		return _totalTime;
	}
	else
	{
		return -1;
	}
}

float Timer::resume()
{
	//On détécte si le Timer est en pause
	if(_paused)
	{
		//On remet le Timer en marche
		_paused = false;
		//Et on remet la valeur de départ à cet instant
		_startTime = millis();
		return _totalTime;
	}
	else
	{
		return -1;
	}
}

float Timer::stop()
{
	//On détécte que le Timer est bien en marche
	if(_isRunning)
	{
		//Arret du Timer
		_isRunning = false;
		//Ajout du temps passé depuis la dernière pause/début au tps total
		_totalTime += millis() - _startTime;
		//Renvoie du tps total
		float time = _totalTime;
		_totalTime = 0;
		
		return time;
	}
	else
	{
		return -1;
	}
}

/*  
	/!\ Cette méthode n'indiquera que la dernière valeur du tps total mesurée.
		Pour récupérer cette valeur, il est nécessaire de réaliser au pause() suivi d'un resume()
		afin d'actualiser la valeur du temsp total.
*/
float Timer::getTotalTime()
{
	if(_paused)
	{
		this->resume();
		this->pause();
	}
	else
	{
		this->pause();
		this->resume();
	}
	//Renvoie du tps total
	return _totalTime;
}