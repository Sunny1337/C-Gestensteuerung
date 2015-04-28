#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <list>
#include "Sprite.hpp"
#include "Shot.hpp"

class CPlayer
{
  public:
    CPlayer     ();

    void Init   ();
    void Quit   ();
    void Render ();
    void Update ();
    void Reset  ();
	void SetDir (int dir);
    list<CShot> *GetShotList () {return &m_ShotList;}
	SDL_Rect GetRect () {return m_Rect;}
	void setAlive(bool alive) {isAlive = alive;}

  private:
    void ProcessMoving   ();
    void ProcessShooting ();
    void CheckPosition   ();

    CSprite *m_pSpritePlayer; // Sprite für Spieler
    CSprite *m_pSpriteShot;   // Sprite für Laserschüsse
	CSprite *m_pSpriteExplosion;
    float m_fXPos;            // X-Position des Spielers
    float m_fYPos;            // Y-Position des Spielers
    float m_fAnimPhase;       // Aktuelle Animationsphase
	float m_fAnimPhaseExp;       // Aktuelle Animationsphase
    bool m_bShotLock;         // Darf der nächste Schuss raus?
    list<CShot> m_ShotList;   // Liste der Schüsse
	int dir;
	SDL_Rect m_Rect;
	int offsetX;
	int offsetY;
	bool isAlive;

};

#endif
