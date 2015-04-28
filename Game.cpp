#include "Game.hpp"
#include <iostream>

using namespace std;

// Konstruktor
//
// Aufgabe: Allgemeine Initialisierungen
//
CGame::CGame ()
{
  m_pPlayer = NULL;
  m_pSpriteBackground = NULL;
  m_pSpriteAsteroid = NULL;

} // Konstruktor


void CGame::Reset ()
{
	  // Timer für Asteroiden zurücksetzen
  m_fAsteroidTimer = 0.0f;
  m_AsteroidList.clear();
}

// Init
//
// Aufgabe: Spieler, Hintergrund und Asteroid initialisieren
//
void CGame::Init ()
{
  // Neuen Spieler initialisieren
  m_pPlayer = new CPlayer;
  m_pPlayer->Init ();
  m_pPlayer->Reset ();

  // Hintergrundbild (Sprite) laden
  m_pSpriteBackground = new CSprite;
  m_pSpriteBackground->Load ("Data/Background.bmp");

  // Sprite für Asteroiden laden
  m_pSpriteAsteroid = new CSprite;
  m_pSpriteAsteroid->Load ("Data/Asteroid.bmp", 20, 64, 64);
  m_pSpriteAsteroid->SetColorKey (255, 0, 255);

  Reset();

  // Spiel läuft
  m_bGameRun = true;

} // Init


// Quit
//
// Aufgabe: Instanzen freigeben
//
void CGame::Quit ()
{
  // Spieler freigeben
  if (m_pPlayer != NULL)
  {
    m_pPlayer->Quit ();
    delete (m_pPlayer);
    m_pPlayer = NULL ;
  }

  // Hintergrundsprite freigeben
  if (m_pSpriteBackground != NULL)
  {
    delete (m_pSpriteBackground);
    m_pSpriteBackground = NULL;
  }

  // Asteroidensprite freigeben
  if (m_pSpriteAsteroid != NULL)
  {
    delete (m_pSpriteBackground);
    m_pSpriteBackground = NULL;
  }

} // Quit


// Run
//
// Aufgabe: Hauptschleife des Spiels
//
void CGame::Run ()
{
  // Hauptschleife des Spiels durchlaufen
  //
  while(m_bGameRun)
  {
    // Events bearbeiten
    ProcessEvents ();

    // Framework updaten und Buffer löschen
    g_pFramework->Update ();
    g_pFramework->Clear ();

    // Hintergrundbild rendern
    m_pSpriteBackground->Render ();

    // Spieler updaten und rendern
    m_pPlayer->Update ();
    m_pPlayer->Render ();

    // Neue Asteroiden hinzufügen
    SpawnAsteroids ();

    // Kollisionen prüfen
    CheckCollisions ();

    // Asteroiden rendern
    RenderAsteroids ();

    // Buffer flippen
    g_pFramework->Flip ();

  }

} // Run

bool CGame::ended()
{
	return !m_bGameRun;
}

// ProcessEvents
//
// Aufgabe: Events bearbeiten
//
void CGame::ProcessEvents ()
{
  SDL_Event Event;

  // Gab es ein Event?
  if (SDL_PollEvent (&Event))
  {
    // Ja, also schauen welches
    switch (Event.type)
    {
      // Beenden?
      case (SDL_QUIT):
      {
        m_bGameRun = false;

      } break;

      // Wurde eine Taste gedrückt?
      case (SDL_KEYDOWN):
      {
        switch (Event.key.keysym.sym)
        {
          case (SDLK_ESCAPE):
          {
            // Ja, also Spiel beenden
            m_bGameRun = false;

          } break;
        }
      } break;
    }
  }

} // ProcessEvents

CPlayer* CGame::getPlayer()
{
	return m_pPlayer;
}


// SpawnAsteroids
//
// Aufgabe: Nach Ablauf einer bestimmten Zeit neuen Asteroiden erzeugen
//
void CGame::SpawnAsteroids ()
{
  // Timer für nächsten Asteroiden erhöhen
  m_fAsteroidTimer += g_pTimer->GetElapsed ();

  // Wenn eine halbe Sekunde vergangen ist,
  // dann einen neuen Asteroiden erzeugen
  if (m_fAsteroidTimer >= 0.5f)
  {
    // Neuer Asteroid
    CAsteroid Asteroid;

    // Zufällige X-Position
    int XPos = rand()%736;

    // Asteroid initialisieren
    Asteroid.Init (m_pSpriteAsteroid, static_cast<float>(XPos), -60.0f);

    // Asteroid in Liste einfügen
    m_AsteroidList.push_back (Asteroid);

    // Zeitgeber wieder zurücksetzen
    m_fAsteroidTimer = 0.0f;

  }

} // SpawnAsteroids


bool CGame::CheckRectCollision(SDL_Rect r1, SDL_Rect r2)
{ 
	// Überschneiden sich die Rects?
      if (r1.y < r2.y + r2.h &&
          r1.y + r1.h > r2.y &&
          r1.x < r2.x + r2.w &&
          r1.x + r1.w > r2.x)
	  {
		return true;
	  }
	  else
	  {
		  return false;
	  }
}

// CheckCollisions
//
// Aufgabe: Kollisionen zwischen Asteroiden und Schüssen prüfen
//
void CGame::CheckCollisions ()
{
  // Schussliste des Spielers holen
  list<CShot> *ShotList = m_pPlayer->GetShotList ();

  // Iteratoren für Asteroiden- und Schussliste
  list<CAsteroid>::iterator ItAsteroid = m_AsteroidList.begin ();
  list<CShot>::iterator ItShot;

  // Rects für Asteroiden und Schüsse
  SDL_Rect RectAsteroid;
  SDL_Rect RectShot;

  SDL_Rect RectPlayer = m_pPlayer->GetRect();

  // Alle Asteroiden durchlaufen
  while (ItAsteroid != m_AsteroidList.end () )
  {
    // Rect des Asteroiden holen
    RectAsteroid = ItAsteroid->GetRect ();

	cout << RectPlayer.y << endl;

	if(CheckRectCollision(RectPlayer, RectAsteroid))
	{
		cout << "coll";
		m_pPlayer->setAlive(false);
			Reset();
		break;
	
	}

    // Alle Schüsse durchlaufen
    for (ItShot = ShotList->begin (); 
         ItShot != ShotList->end ();
         ++ItShot)
    {
      // Rect des Schusses holen
      RectShot = ItShot->GetRect ();

	  if(CheckRectCollision(RectShot, RectAsteroid))
      {
        // Ja, also gab es eine Kollision. Somit Schuss und
        // Asteroid deaktivieren
        ItAsteroid->SetAlive (false);
        ItShot->SetAlive (false);
      }

    }

    // Asteroid löschen, falls deaktiviert
    if (ItAsteroid->IsAlive () )
      ItAsteroid++;
    else
      ItAsteroid = m_AsteroidList.erase (ItAsteroid);

  }

} // CheckCollision


// RenderAsteroids
//
// Aufgabe: Alle Asteroiden rendern und updaten
//
void CGame::RenderAsteroids ()
{
  // Iterator für die Asteroiden-Liste
  list<CAsteroid>::iterator It;

  // Asteroiden-Liste durchlaufen
  for (It = m_AsteroidList.begin (); It != m_AsteroidList.end (); ++It)
  {
    // Asteroid rendern
    It->Render ();

    // Asteroid updaten
    It->Update ();

  }

} // RenderAsteroids
