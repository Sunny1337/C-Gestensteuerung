#include "Asteroid.hpp"

// Init
//
// Aufgabe: Asteroid initialisieren
//
void CAsteroid::Init (CSprite *pSpriteAsteroid, float fXPos, float fYPos)
{
  // Zeiger auf Sprite kopieren und Koordinaten setzen
  m_pSpriteAsteroid = pSpriteAsteroid;
  m_fXPos = fXPos;
  m_fYPos = fYPos;

  // Animation beginnt beim ersten Einzelbild
  m_fAnimPhase = 0.0f;

  // Rect initialisieren
  m_Rect.x = static_cast<int>(fXPos);
  m_Rect.y = static_cast<int>(fYPos);
  m_Rect.w = pSpriteAsteroid->GetRect().w;
  m_Rect.h = pSpriteAsteroid->GetRect().h;

  // Asteroid aktivieren
  m_bIsAlive = true;

} // Init


// Update
//
// Aufgabe: Asteroid bewegen und Position prüfen
//
void CAsteroid::Update ()
{
  // Asteroid bewegen
  m_fYPos += 200.0f * g_pTimer->GetElapsed ();

  m_Rect.y = static_cast<int>(m_fYPos);

  // Animieren
  m_fAnimPhase += 10.0f * g_pTimer->GetElapsed ();

  // Prüfen, ob die Animation am Ende angelangt ist
  if (m_fAnimPhase >= 20.0f)
    m_fAnimPhase = m_fAnimPhase - 20.0f;

  // Deaktivieren, falls außerhalb des Bildschirms
  if (m_fYPos > 590.0f)
    m_bIsAlive = false;

} // Update


// Render
//
// Aufgabe: Asteroid rendern
//
void CAsteroid::Render ()
{
  m_pSpriteAsteroid->SetPos (m_fXPos, m_fYPos);
  m_pSpriteAsteroid->Render (m_fAnimPhase);

} // Render
