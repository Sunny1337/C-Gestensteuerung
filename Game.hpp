#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Asteroid.hpp"

class CGame
{
  public:
    CGame ();

    void Init ();
    void Run  ();
    void Quit ();
	bool ended();
	CPlayer* getPlayer();

  private:
    void ProcessEvents   ();
    void SpawnAsteroids  ();
    void RenderAsteroids ();
    void CheckCollisions  ();
	void Reset();
	bool CheckRectCollision(SDL_Rect r1, SDL_Rect r2);

    CPlayer *m_pPlayer;             // Spieler-Instanz
    CSprite *m_pSpriteBackground;   // Sprite für den Hintergrund
    CSprite *m_pSpriteAsteroid;     // Sprite für die Asteroiden
    float    m_fAsteroidTimer;      // Zeitgeber für nächsten Asteroiden
    bool     m_bGameRun;            // Läuft das Spiel noch?
    list<CAsteroid> m_AsteroidList; // Liste der Asteroiden

};

#endif
