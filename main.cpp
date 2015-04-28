#include "opencv2/opencv.hpp"
#include <iostream>
#include "Framework.hpp"
#include "Game.hpp"
#include "ObjectTracking.hpp"
#include <SDL_thread.h>

using namespace std;
using namespace cv;

int thread1 (void *p)
{
	CGame* game = (CGame*)p;
	game->Run();
	SDL_Delay (100);
	return 0;
}

int main(int argc, char *argv[])
{
	// Framework initialisieren
 if (g_pFramework->Init (800, 600, 16, false) == false)
  return (0);

 // Neue Instanz des Spiels
 CGame* Game = new CGame();

 // Spiel initialisieren
 Game->Init ();

 ObjectTracking tracking;

 if(!tracking.init(0))
 {
	 return -1;
 }
 tracking.setWindowSettings();
 	
 SDL_Thread *t1;

 t1 = SDL_CreateThread(thread1, Game);

    while(true)
    {
	
		// Spiel laufen lassen. Diese Funktion läuft so lange,
		// bis das Spiel geschlossen oder die Escape-Taste
		// gedrückt wird
		 CPlayer* player = Game->getPlayer();

		 tracking.update();

		 int yMittel = tracking.getYmittel();
		 int yMittel2 = tracking.getYmittel2();
		
		 if(yMittel < yMittel2-50)
		  {
		   //cout << "links" << endl;
		   player->SetDir(1);
		  }
		  else if(yMittel < yMittel2+50 && yMittel > yMittel2-50 )
		  {
		   //cout << "mitte" << endl;
		   player->SetDir(0);
		  }
		  else if(yMittel > yMittel2+50)
		  {
		   //cout << "rechts" << endl;
		   player->SetDir(-1);
		  }
		 
        if(tracking.ended() ||
			Game->ended())
			break;
	}
	  SDL_KillThread(t1);
    // Spiel beenden
	 Game->Quit ();

	 // Framework beenden
	 g_pFramework->Quit ();
	 g_pFramework->Del ();
	// the camera will be deinitialized automatically in VideoCapture destructor
	 destroyAllWindows();
    
	 return 0;
}