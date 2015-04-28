#include "Shot.hpp"

// Init
//
// Aufgabe: Schuss initialisieren
//
void CShot::Init (CSprite *pSpriteShot, float fXPos, float fYPos)
{
  // Zeiger auf Sprite kopieren und Koordinaten setzen
  m_pSpriteShot = pSpriteShot;
  m_fXPos = fXPos;
  m_fYPos = fYPos;

  // Rect initialisieren
  m_Rect.x = static_cast<int>(fXPos);
  m_Rect.y = static_cast<int>(fYPos);
  m_Rect.w = pSpriteShot->GetRect().w;
  m_Rect.h = pSpriteShot->GetRect().h;

  // Schuss aktivieren
  m_bIsAlive = true;

} // Init


// Update
//
// Aufgabe: Schuss bewegen und Position pr¸fen
//
void CShot::Update ()
{
  // Schuss bewegen
  m_fYPos -= 400.0f * g_pTimer->GetElapsed ();

  m_Rect.y = static_cast<int>(m_fYPos);

  // Deaktivieren, falls auﬂerhalb des Bildschirms
  if (m_fYPos < -15.0f)
    m_bIsAlive = false;

} // Update


// Render
//
// Aufgabe: Schuss rendern
//
void CShot::Render ()
{
  if (m_bIsAlive == true)
  {
    // Koordinaten setzen und rendern
    m_pSpriteShot->SetPos (m_fXPos, m_fYPos);
    m_pSpriteShot->Render ();
  }

} // Render
