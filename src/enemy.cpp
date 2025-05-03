#include "enemy.h"
#include "camera.h"
#include "rockman.h"
#include "sprite.h"


Enemy::Enemy()
  : m_collider(&m_boundingBox)
  , m_pSprite(0)
{
  m_boundingBox.x = 0x70; // FIXME: This should not be a constant!
  m_boundingBox.y = 0x838; // FIXME: This should not be a constant!
  m_boundingBox.w = 16;
  m_boundingBox.h = 8;
  IniDictionary iniDictionary("enemy.ini");
  m_pSprite = new Sprite(iniDictionary);
}

Enemy::~Enemy() {
  delete m_pSprite;
}

void Enemy::draw(Camera& a_camera) {
  int x = a_camera.getScreenMappedXCoordinate(m_boundingBox.x);
  int y = a_camera.getScreenMappedYCoordinate(m_boundingBox.y);
  m_pSprite->draw(x, y, 0);
}

Collider Enemy::getCollider() {
  return m_collider;
}

void Enemy::hit(Rockman* a_pRockman) {
  a_pRockman->receiveDamage();
}

void Enemy::update() {
  m_pSprite->update();
}
