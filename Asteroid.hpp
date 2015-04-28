#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Sprite.hpp"

class CAsteroid
{
  public:
    void Init     (CSprite *pSpriteAsteroid, float fXPos, float fYPos);
    void Update   ();
    void Render   ();
    bool IsAlive  () {return m_bIsAlive;}
    void SetAlive (bool bIsAlive) {m_bIsAlive = bIsAlive;}
    SDL_Rect GetRect () {return m_Rect;}

  private:
    CSprite *m_pSpriteAsteroid; // Zeiger auf Asteroiden-Sprite
    float    m_fXPos;           // X-Position des Asteroiden
    float    m_fYPos;           // Y-Position des Asteroiden
    float    m_fAnimPhase;      // Akt. Animationsphase des Asteroiden
    bool     m_bIsAlive;        // "Lebt" der Asteroid noch?
    SDL_Rect m_Rect;            // Rect des Asteroiden

};

#endif
