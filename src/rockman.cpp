#include "rockman.h"
#include "collisiontestingresult.h"
#include "inidictionary.h"
#include "map.h"
#include "sprite.h"
#include <string>


Rockman::Rockman(const char* a_pIniFilename, Map* a_pMap)
  : m_x(0)
  , m_y(0)
  , m_xSpeed(0)
  , m_ySpeed(0)
  , m_xSpeedAccelerating(0)
  , m_xSpeedDecelerating(0)
  , m_xSpeedJumpAccelerating(0)
  , m_xSpeedJumping(0)
  , m_xSpeedReeling(0)
  , m_xSpeedRunning(0)
  , m_ySpeedClimbingDown(0)
  , m_ySpeedClimbingUp(0)
  , m_ySpeedCollision(0)
  , m_ySpeedFalling(0)
  , m_ySpeedGravity(0)
  , m_ySpeedJumping(0)
  , m_ySpeedJumpReleased(0)
  , m_ySpeedJumpReleasedMax(0)
  , m_ySpeedMin(0)
  , m_ySpeedScrolling(0)
  , m_ySpeedReeling(0)
  , m_direction(Direction::Right)
  , m_destroyed(false)
  , m_scrolling(false)
  , m_shooting(false)
  , m_movementType(MovementType::Teleporting)
  , m_stateChanged(false)
  , m_duration(0)
  , m_durationAccelerating(0)
  , m_durationAcceleratingPreviousAnimation(0)
  , m_durationDecelerating(0)
  , m_durationDeceleratingPreviousAnimation(0)
  , m_durationInvincible(0)
  , m_durationReeling(0)
  , m_durationScrolling(0)
  , m_durationShooting(0)
  , m_shootingDuration(0)
  , m_invincibleDurationLeft(0)
  , m_teleportStopPosY(0)
  , m_boundingBoxHeightHalf(0)
  , m_boundingBoxXOffset(0)
  , m_boundingBoxYOffset(0)
  , m_bulletXOffsetLeft(0)
  , m_bulletXOffsetRight(0)
  , m_bulletYOffset(0)
  , m_currentStateHandler(&Rockman::teleportingStateHandler)
  , m_nextStateEventHandler(0)
  , m_currentYCollisionResponse(YCollisionResponse::Stop)
  , m_pMap(a_pMap)
  , m_pSprite(0)
  , m_collider(&m_boundingBox)
{
  m_x = m_pMap->getTeleportStartPosX();
  m_y = m_pMap->getTeleportStartPosY();
  m_ySpeed = m_pMap->getTeleportStartYSpeed();
  m_teleportStopPosY = m_pMap->getTeleportStopPosY();
  bool movingUp = true;
  if (m_ySpeed < FixedPoint8(0)) {
    movingUp = false;
  }
  m_ladderSonar = m_pMap->getLadderSonar(m_x, m_y, m_y - m_ySpeed, movingUp);
  m_pMap->setCurrentBoundaryByPosition(m_x, m_y);
  SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
  gameEngine->setCameraBoundary(mapBoundary);
  IniDictionary iniDictionary(a_pIniFilename);
  m_pSprite = new Sprite(iniDictionary);
  m_pSprite->setCurrentAnimationIndex(AnimationType::Teleporting);
  int xSpeedInt = iniDictionary.getIntValue("acceleratingInt", "XSpeed", 0);
  int xSpeedFraction = iniDictionary.getIntValue("acceleratingFraction", "XSpeed", 0);
  m_xSpeedAccelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = iniDictionary.getIntValue("deceleratingInt", "XSpeed", 0);
  xSpeedFraction = iniDictionary.getIntValue("deceleratingFraction", "XSpeed", 0);
  m_xSpeedDecelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = iniDictionary.getIntValue("jumpAcceleratingInt", "XSpeed", 0);
  xSpeedFraction = iniDictionary.getIntValue("jumpAcceleratingFraction", "XSpeed", 0);
  m_xSpeedJumpAccelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = iniDictionary.getIntValue("jumpingInt", "XSpeed", 0);
  xSpeedFraction = iniDictionary.getIntValue("jumpingFraction", "XSpeed", 0);
  m_xSpeedJumping = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = iniDictionary.getIntValue("reelingInt", "XSpeed", 0);
  xSpeedFraction = iniDictionary.getIntValue("reelingFraction", "XSpeed", 0);
  m_xSpeedReeling = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = iniDictionary.getIntValue("runningInt", "XSpeed", 0);
  xSpeedFraction = iniDictionary.getIntValue("runningFraction", "XSpeed", 0);
  m_xSpeedRunning = FixedPoint8(xSpeedInt, xSpeedFraction);
  int ySpeedInt = iniDictionary.getIntValue("climbingDownInt", "YSpeed", 0);
  int ySpeedFraction = iniDictionary.getIntValue("climbingDownFraction", "YSpeed", 0);
  m_ySpeedClimbingDown = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("climbingUpInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("climbingUpFraction", "YSpeed", 0);
  m_ySpeedClimbingUp = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("collisionInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("collisionFraction", "YSpeed", 0);
  m_ySpeedCollision = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("fallingInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("fallingFraction", "YSpeed", 0);
  m_ySpeedFalling = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("gravityInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("gravityFraction", "YSpeed", 0);
  m_ySpeedGravity = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("jumpingInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("jumpingFraction", "YSpeed", 0);
  m_ySpeedJumping = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("jumpReleasedInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("jumpReleasedFraction", "YSpeed", 0);
  m_ySpeedJumpReleased = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("jumpReleasedMaxInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("jumpReleasedMaxFraction", "YSpeed", 0);
  m_ySpeedJumpReleasedMax = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("minInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("minFraction", "YSpeed", 0);
  m_ySpeedMin = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("scrollingInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("scrollingFraction", "YSpeed", 0);
  m_ySpeedScrolling = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = iniDictionary.getIntValue("reelingInt", "YSpeed", 0);
  ySpeedFraction = iniDictionary.getIntValue("reelingFraction", "YSpeed", 0);
  m_ySpeedReeling = FixedPoint8(ySpeedInt, ySpeedFraction);
  m_durationAccelerating = iniDictionary.getIntValue("accelerating", "Durations", 0);
  m_durationAcceleratingPreviousAnimation = iniDictionary.getIntValue("acceleratingPreviousAnimation", "Durations", 0);
  m_durationDecelerating = iniDictionary.getIntValue("decelerating", "Durations", 0);
  m_durationDeceleratingPreviousAnimation = iniDictionary.getIntValue("deceleratingPreviousAnimation", "Durations", 0);
  m_durationInvincible = iniDictionary.getIntValue("invincible", "Durations", 0);
  m_durationReeling = iniDictionary.getIntValue("reeling", "Durations", 0);
  m_durationScrolling = iniDictionary.getIntValue("scrolling", "Durations", 0);
  m_durationShooting = iniDictionary.getIntValue("shooting", "Durations", 0);
  m_boundingBoxHeightHalf = iniDictionary.getIntValue("heightHalf", "BoundingBox", 0);
  m_boundingBoxXOffset = iniDictionary.getIntValue("xOffset", "BoundingBox", 0);
  m_boundingBoxYOffset = iniDictionary.getIntValue("yOffset", "BoundingBox", 0);
  m_boundingBox.x = m_x + m_boundingBoxXOffset;
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  m_boundingBox.w = iniDictionary.getIntValue("width", "BoundingBox", 0);
  m_boundingBox.h = iniDictionary.getIntValue("height", "BoundingBox", 0);
  const char* pBulletBitmapFilename = iniDictionary.getCStringValue("spritesheet", "Bullet", "empty");
  m_pBulletBitmap = gameEngine->loadSurface(pBulletBitmapFilename);
  m_bulletXOffsetLeft = iniDictionary.getIntValue("xOffsetLeft", "Bullet", 0);
  m_bulletXOffsetRight = iniDictionary.getIntValue("xOffsetRight", "Bullet", 0);
  m_bulletYOffset = iniDictionary.getIntValue("yOffset", "Bullet", 0);
}

Rockman::~Rockman() {
  gameEngine->unloadSurface(m_pBulletBitmap);
  delete m_pSprite;
}

void Rockman::draw() {
  int spriteBitmap = 0;
  if (m_direction == Direction::Right) {
    if (!m_shooting) {
      spriteBitmap = SpriteBitmap::RunningRight;
    }
    else {
      spriteBitmap = SpriteBitmap::ShootingRight;
    }
  }
  else if (m_direction == Direction::Left) {
    if (!m_shooting) {
      spriteBitmap = SpriteBitmap::RunningLeft;
    }
    else {
      spriteBitmap = SpriteBitmap::ShootingLeft;
    }
  }
  // Low level trick to only draw the sprite every second
  // and third time in a sequence of four.
  int firstBit = m_invincibleDurationLeft & 1;
  int secondBit = (m_invincibleDurationLeft & 2) >> 1;
  int xorBit = firstBit ^ secondBit;
  if (xorBit == 0) {
    m_pSprite->draw(m_x, m_y, spriteBitmap);
  }
  for (int iBullet = 0; iBullet < kMaximumSimultaneousBullets; iBullet++) {
    m_bullets[iBullet].draw(m_pBulletBitmap);
  }
  int ladderSonarInt = 0;
  if (m_ladderSonar.belowFeetNextFrame) {
    ladderSonarInt += 1;
  }
  if (m_ladderSonar.atFeet) {
    ladderSonarInt += 2;
  }
  if (m_ladderSonar.behindEyes) {
    ladderSonarInt += 4;
  }
  if (m_ladderSonar.atHeadTop) {
    ladderSonarInt += 8;
  }
  if (m_ladderSonar.atHeadTopNextFrame) {
    ladderSonarInt += 16;
  }
  m_debugInfo.draw(m_x.get(), m_y.get(), m_xSpeed.get(), m_ySpeed.get(), ladderSonarInt);
}

Collider Rockman::getCollider() {
  return m_collider;
}

bool Rockman::isDestroyed() {
  return m_destroyed;
}

void Rockman::receiveDamage() {
  if (m_invincibleDurationLeft == 0) {
#ifdef DEBUG
    printf("Rockman was damaged!\n");
#endif
    m_invincibleDurationLeft = m_durationInvincible;
    m_nextStateEventHandler = &Rockman::onReeling;
    m_pSprite->setCurrentAnimationIndex(AnimationType::Damaged);
    m_pSprite->setForwardBackwardAnimationOrder();
  }
}

void Rockman::reset() {
  m_x = m_pMap->getTeleportStartPosX();
  m_y = m_pMap->getTeleportStartPosY();
  m_boundingBox.x = m_x + m_boundingBoxXOffset;
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  m_ySpeed = m_pMap->getTeleportStartYSpeed();
  m_teleportStopPosY = m_pMap->getTeleportStopPosY();
  m_destroyed = false;
  m_movementType = MovementType::Teleporting;
  m_currentStateHandler = &Rockman::teleportingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Stop;
  m_pSprite->setCurrentAnimationIndex(AnimationType::Teleporting);
  m_pMap->setCurrentBoundaryByPosition(m_x, m_y);
  SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
  gameEngine->setCameraBoundary(mapBoundary);
  gameEngine->updateCamera(&m_boundingBox);
}

void Rockman::update() {
  Controls buttons = gameEngine->getControls();
  m_pSprite->update();
  if (m_invincibleDurationLeft > 0) {
    m_invincibleDurationLeft--;
  }
  if (m_nextStateEventHandler) {
    (this->*m_nextStateEventHandler)();
    m_nextStateEventHandler = 0;
    m_stateChanged = false;
  }
  if (m_scrolling) {
    (this->*m_currentStateHandler)(buttons);
  }
  else {
    if (m_movementType != MovementType::Teleporting) {
      this->shootingStateHandler(buttons);
    }
    if (m_movementType == MovementType::Grounded
        || m_movementType == MovementType::Airborne) {
      checkClimbing(buttons);
    }
    (this->*m_currentStateHandler)(buttons);
    if (m_movementType != MovementType::Teleporting) {
      if (m_xSpeed > FixedPoint8(0)) {
        handleXCollision();
      }
      bool movingUp = false;
      if (m_ySpeed > FixedPoint8(0)) {
        movingUp = true;
      }
      m_ladderSonar = m_pMap->getLadderSonar(m_x, m_y, m_y - m_ySpeed, movingUp);
      if (m_movementType == MovementType::Climbing
          && !m_ladderSonar.belowFeetNextFrame
          && !m_ladderSonar.atFeet
          && !m_ladderSonar.behindEyes
          && !m_ladderSonar.atHeadTop
          && !m_ladderSonar.atHeadTopNextFrame) {
        onFalling();
      }
      handleYCollision(buttons);
      handleLeavingScreen();
      m_ySpeed -= m_ySpeedGravity;
    }
  }
  if (buttons.jumpPressed && m_movementType == MovementType::Grounded) {
    onJumping();
  }
  m_duration++;
  if (m_shooting) {
    m_shootingDuration++;
  }
  m_stateChanged = false;
  gameEngine->updateCamera(&m_boundingBox);
}

void Rockman::standingStateHandler(Controls a_buttons) {
  if (a_buttons.horizontalDirection != Direction::None) {
    m_direction = a_buttons.horizontalDirection;
    onAccelerating();
  }
}

void Rockman::acceleratingStateHandler(Controls a_buttons) {
  if (a_buttons.horizontalDirection == Direction::None) {
    onStanding();
  }
  else {
    if (a_buttons.horizontalDirection != m_direction) {
      m_direction = a_buttons.horizontalDirection;
    }
    if (m_duration == m_durationAcceleratingPreviousAnimation) {
      m_pSprite->setCurrentAnimationIndex(AnimationType::Accelerating);
    }
    else if (m_duration == m_durationAccelerating) {
      onRunning();
    }
  }
}

void Rockman::runningStateHandler(Controls a_buttons) {
  if (a_buttons.horizontalDirection == Direction::None) {
    onDecelerating();
  }
  else {
    if (a_buttons.horizontalDirection != m_direction) {
      m_direction = a_buttons.horizontalDirection;
    }
  }
}

void Rockman::deceleratingStateHandler(Controls a_buttons) {
  if (a_buttons.horizontalDirection != Direction::None) {
    m_direction = a_buttons.horizontalDirection;
    onAccelerating();
  }
  else {
    if (m_duration == m_durationDeceleratingPreviousAnimation) {
      m_pSprite->setCurrentAnimationIndex(AnimationType::Accelerating);
    }
    else if (m_duration == m_durationDecelerating) {
      onStanding();
    }
  }
}

void Rockman::jumpingStateHandler(Controls a_buttons) {
  if (m_duration == 1) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Jumping);
  }
  if (a_buttons.horizontalDirection == Direction::None) {
    m_xSpeed = 0;
  }
  else {
    m_direction = a_buttons.horizontalDirection;
    if (a_buttons.previousHorizontalDirection == Direction::None) {
      m_xSpeed = m_xSpeedJumpAccelerating;
    }
    else {
      m_xSpeed = m_xSpeedJumping;
    }
  }
  if (a_buttons.jumpReleased && m_ySpeed > m_ySpeedJumpReleasedMax) {
    m_ySpeed = m_ySpeedJumpReleased;
  }
  if (m_ySpeed < FixedPoint8(0)) {
    m_currentYCollisionResponse = YCollisionResponse::Land;
  }
  if (m_ySpeed < m_ySpeedMin) {
    m_ySpeed = m_ySpeedMin;
  }
}

void Rockman::climbingStateHandler(Controls a_buttons) {
#ifdef DEBUG
  printf("climbingStateHandler\n");
#endif
  if (a_buttons.verticalDirection != Direction::None) {
    if (a_buttons.verticalDirection == Direction::Up) {
      m_ySpeed = m_ySpeedClimbingUp;
      m_currentYCollisionResponse = YCollisionResponse::Stop;
    }
    else if (a_buttons.verticalDirection == Direction::Down) {
      m_ySpeed = m_ySpeedClimbingDown;
      m_currentYCollisionResponse = YCollisionResponse::Land;
    }
    if (m_ladderSonar.atFeet
        && !m_ladderSonar.atHeadTop
        && !m_ladderSonar.atHeadTopNextFrame) {
      m_pSprite->setCurrentAnimationIndex(AnimationType::ClimbingTop);
    }
    else {
      m_pSprite->setCurrentAnimationIndex(AnimationType::Climbing);
    }
  }
  else {
    m_ySpeed = 0;
    m_pSprite->setCurrentAnimationIndex(AnimationType::Climbing);
    m_pSprite->resetCurrentAnimationFrameDuration();
  }
  if (m_shooting) {
    m_ySpeed = 0;
  }
  if (m_ySpeed > FixedPoint8(0)) {
    if (!m_ladderSonar.belowFeetNextFrame
        && m_ladderSonar.atFeet
        && !m_ladderSonar.behindEyes
        && !m_ladderSonar.atHeadTop
        && !m_ladderSonar.atHeadTopNextFrame) {
      int y = m_y;
      y = y % kScreenHeight;
      int tileAlignedY = y & kScreenHeight;
      SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
      y = mapBoundary.y + tileAlignedY;
      y -= m_boundingBoxHeightHalf;
      m_y = y;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
      m_ySpeed = 0;
      m_movementType = MovementType::Grounded;
      onStanding();
      return;
    }
  }
  if (a_buttons.jumpPressed && a_buttons.verticalDirection == Direction::None) {
    m_ySpeed = m_ySpeedFalling;
    if (a_buttons.horizontalDirection != Direction::None) {
      m_direction = a_buttons.horizontalDirection;
    }
    onFalling();
  }
}

void Rockman::scrollingStateHandler(Controls a_buttons) {
  if (m_duration == m_durationScrolling) {
    m_scrolling = false;
    onClimbing();
    m_pMap->setCurrentBoundaryByPosition(m_x, m_y);
    SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
    gameEngine->setCameraBoundary(mapBoundary);
  }
  else {
    if (m_ySpeed > FixedPoint8(0)) {
      m_y -= m_ySpeedScrolling;
      gameEngine->moveCamera(0, -kScrollYSpeed);
    }
    else {
      m_y += m_ySpeedScrolling;
      gameEngine->moveCamera(0, kScrollYSpeed);
    }
    m_boundingBox.y = m_y + m_boundingBoxYOffset;
  }
}

void Rockman::fallScrollingStateHandler(Controls a_buttons) {
  if (m_duration == m_durationScrolling) {
    m_scrolling = false;
    onFalling();
    m_pMap->setCurrentBoundaryByPosition(m_x, m_y);
    SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
    gameEngine->setCameraBoundary(mapBoundary);
  }
  else {
    m_y += m_ySpeedScrolling;
    m_boundingBox.y = m_y + m_boundingBoxYOffset;
    gameEngine->moveCamera(0, kScrollYSpeed);
  }
}

void Rockman::shootingStateHandler(Controls a_buttons) {
  if (a_buttons.actionPressed) {
    if (a_buttons.horizontalDirection != Direction::None) {
      m_direction = a_buttons.horizontalDirection;
    }
    int x = m_x;
    if (m_direction == Direction::Right) {
      x += m_bulletXOffsetRight;
    }
    else if (m_direction == Direction::Left) {
      x += m_bulletXOffsetLeft;
    }
    int y = m_y + m_bulletYOffset;
    bool activated = m_bullets[0].activate(x, y, m_direction);
    if (!activated) {
      activated = m_bullets[1].activate(x, y, m_direction);
      if (!activated) {
        activated = m_bullets[2].activate(x, y, m_direction);
      }
    }
    if (activated) {
      m_shooting = true;
      m_shootingDuration = 0;
    }
  }
  if (m_shooting && m_shootingDuration == m_durationShooting) {
    m_shooting = false;
  }
  for (int iBullet = 0; iBullet < kMaximumSimultaneousBullets; iBullet++) {
    m_bullets[iBullet].update();
  }
}

void Rockman::reelingStateHandler(Controls a_buttons) {
  if (m_duration == m_durationReeling) {
#ifdef DEBUG
    printf("Exiting reeling state\n");
#endif
    reverseHorizontalDirection();
    onStanding();
  }
}

void Rockman::teleportingStateHandler(Controls a_buttons) {
  if (m_y == m_teleportStopPosY) {
    m_boundingBox.x = m_x + m_boundingBoxXOffset;
    m_boundingBox.y = m_y + m_boundingBoxYOffset;
    onTransforming();
  }
  else {
    m_y += Tile::kHeight;
  }
}

void Rockman::transformingStateHandler(Controls a_buttons) {
  // nothing
}

void Rockman::onStanding() {
#ifdef DEBUG
  printf("onStanding\n");
#endif
  m_xSpeed = 0;
  m_duration = 0;
  m_pSprite->setCurrentAnimationIndex(AnimationType::Standing);
  m_currentStateHandler = &Rockman::standingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Land;
  m_movementType = MovementType::Grounded;
  m_stateChanged = true;
}

void Rockman::onAccelerating() {
  m_xSpeed = m_xSpeedAccelerating;
  m_duration = 0;
  if (m_durationAcceleratingPreviousAnimation == 0) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Accelerating);
  }
  m_currentStateHandler = &Rockman::acceleratingStateHandler;
  m_stateChanged = true;
}

void Rockman::onDecelerating() {
  m_xSpeed = m_xSpeedDecelerating;
  m_duration = 0;
  if (m_durationDeceleratingPreviousAnimation == 0) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Accelerating);
  }
  m_currentStateHandler = &Rockman::deceleratingStateHandler;
  m_stateChanged = true;
}

void Rockman::onRunning() {
  m_xSpeed = m_xSpeedRunning;
  m_duration = 0;
  m_pSprite->setCurrentAnimationIndex(AnimationType::Running);
  m_currentStateHandler = &Rockman::runningStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Land;
  m_movementType = MovementType::Grounded;
  m_stateChanged = true;
}

void Rockman::onJumping() {
#ifdef DEBUG
  printf("onJumping\n");
#endif
  m_ySpeed = m_ySpeedJumping;
  m_duration = 0;
  m_currentStateHandler = &Rockman::jumpingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Bump;
  m_movementType = MovementType::Airborne;
  m_stateChanged = true;
}

void Rockman::onFalling() {
#ifdef DEBUG
  printf("onFalling\n");
#endif
  m_ySpeed = m_ySpeedFalling;
  m_y -= m_ySpeed;
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  m_ySpeed -= m_ySpeedGravity;
  m_duration = 0;
  m_pSprite->setCurrentAnimationIndex(AnimationType::Jumping);
  m_currentStateHandler = &Rockman::jumpingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Land;
  m_movementType = MovementType::Airborne;
  m_stateChanged = true;
}

void Rockman::onClimbing() {
#ifdef DEBUG
  printf("onClimbing\n");
#endif
  m_duration = 0;
  m_currentStateHandler = &Rockman::climbingStateHandler;
  m_movementType = MovementType::Climbing;
  m_stateChanged = true;
}

void Rockman::onScrolling() {
#ifdef DEBUG
  printf("onScrolling\n");
#endif
  if (m_ySpeed > FixedPoint8(0)) {
    m_y -= m_ySpeedScrolling;
    gameEngine->moveCamera(0, -kScrollYSpeed);
  }
  else {
    m_y += m_ySpeedScrolling;
    gameEngine->moveCamera(0, kScrollYSpeed);
  }
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  m_duration = 0;
  m_currentStateHandler = &Rockman::scrollingStateHandler;
  m_scrolling = true;
  m_stateChanged = true;
}

void Rockman::onFallScrolling() {
#ifdef DEBUG
  printf("onFallScrolling\n");
#endif
  m_xSpeed = 0;
  m_y += m_ySpeedScrolling;
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  gameEngine->moveCamera(0, kScrollYSpeed);
  m_duration = 0;
  m_currentStateHandler = &Rockman::fallScrollingStateHandler;
  m_scrolling = true;
  m_stateChanged = true;
}

void Rockman::onReeling() {
#ifdef DEBUG
  printf("onReeling\n");
#endif
  m_xSpeed = m_xSpeedReeling;
  m_ySpeed = m_ySpeedReeling;
  reverseHorizontalDirection();
  m_duration = 0;
  m_currentStateHandler = &Rockman::reelingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Stop;
  m_movementType = MovementType::Airborne;
  m_stateChanged = true;
}

void Rockman::onTransforming() {
#ifdef DEBUG
  printf("onTransforming\n");
#endif
  m_duration = 0;
  m_pSprite->setCurrentAnimationIndex(AnimationType::Transforming);
  m_currentStateHandler = &Rockman::transformingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Land;
  m_movementType = MovementType::Airborne;
  m_stateChanged = true;
}

void Rockman::checkClimbing(Controls a_buttons) {
  if (a_buttons.verticalDirection == Direction::None) {
    return;
  }
  if (!(m_ladderSonar.belowFeetNextFrame
        || m_ladderSonar.atFeet
        || m_ladderSonar.behindEyes
        || m_ladderSonar.atHeadTop
        || m_ladderSonar.atHeadTopNextFrame)) {
    return;
  }
  if (a_buttons.verticalDirection == Direction::Up
      && !(m_ladderSonar.belowFeetNextFrame
           && !m_ladderSonar.atFeet
           && !m_ladderSonar.behindEyes
           && !m_ladderSonar.atHeadTop
           && !m_ladderSonar.atHeadTopNextFrame)) {
    m_xSpeed = 0;
    int x = m_x;
    int screenRelativeX = x % kScreenWidth;
    int currentTileX = x & kScreenHeight;
    int deltaX = screenRelativeX - currentTileX;
    m_x = m_x - deltaX + (Tile::kWidth / 2);
    m_boundingBox.x = m_x + m_boundingBoxXOffset;
    reverseHorizontalDirection();
    if (!m_ladderSonar.belowFeetNextFrame
        && !m_ladderSonar.atFeet
        && !m_ladderSonar.behindEyes
        && !m_ladderSonar.atHeadTop
        && m_ladderSonar.atHeadTopNextFrame) {
      // Rockman's position needs to be pushed up
      // so the sprite is fully on the ladder.
      SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
      int screenRelativeY = m_y - mapBoundary.y;
      int currentTileY = screenRelativeY & kScreenHeight;
      int deltaY = screenRelativeY - currentTileY;
      m_y = m_y - deltaY - m_boundingBox.h;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
      onFalling();
    }
    else {
      onClimbing();
    }
  }
  else if (a_buttons.verticalDirection == Direction::Down
           && !(!m_ladderSonar.belowFeetNextFrame
                && m_ladderSonar.atFeet
                && m_ladderSonar.behindEyes
                && m_ladderSonar.atHeadTop)) {
    m_xSpeed = 0;
    int x = m_x;
    int screenRelativeX = x % kScreenWidth;
    int currentTileX = x & kScreenHeight;
    int deltaX = screenRelativeX - currentTileX;
    m_x = m_x - deltaX + (Tile::kWidth / 2);
    m_boundingBox.x = m_x + m_boundingBoxXOffset;
    if (m_ladderSonar.belowFeetNextFrame
        && !m_ladderSonar.atFeet
        && !m_ladderSonar.behindEyes
        && !m_ladderSonar.atHeadTop
        && !m_ladderSonar.atHeadTopNextFrame) {
      m_y += m_boundingBoxHeightHalf;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
    }
    reverseHorizontalDirection();
    onClimbing();
  }
}

void Rockman::reverseHorizontalDirection() {
  if (m_direction == Direction::Right) {
    m_direction = Direction::Left;
  }
  else if (m_direction == Direction::Left) {
    m_direction = Direction::Right;
  }
}

void Rockman::handleLeavingScreen() {
  bool leavingScreenBottom = m_pMap->isLeavingScreenBottom(m_y);
  if (leavingScreenBottom) {
    bool enteringInvalidScreen = m_pMap->isEnteringInvalidScreen(m_x, m_y);
    if (enteringInvalidScreen) {
      // Falling into bottomless pit.
      m_destroyed = true;
    }
    else {
      if (m_movementType == MovementType::Climbing) {
        onScrolling();
      }
      else if (m_movementType == MovementType::Airborne) {
        onFallScrolling();
      }
    }
  }
  else {
    bool leavingScreenTop = m_pMap->isLeavingScreenTop(m_y);
    if (leavingScreenTop && m_movementType == MovementType::Climbing) {
      onScrolling();
    }
  }
}

void Rockman::handleXCollision() {
  int maxXDelta = m_pMap->getMaxXDelta(m_boundingBox, m_direction);
  int xDeltaInt = 0;
  if (m_direction == Direction::Right) {
    int endX = m_x + m_xSpeed;
    xDeltaInt = endX - m_x;
  }
  else if (m_direction == Direction::Left) {
    int endX = m_x - m_xSpeed;
    xDeltaInt = m_x - endX;
  }
  if (xDeltaInt == 0) {
    xDeltaInt = 1;
  }
  #ifdef DEBUG_COLLISION
    printf("handleXCollision: maxXDelta: %i\n", maxXDelta);
    printf("handleXCollision: xDelta: %i\n", xDeltaInt);
  #endif
  bool collisionDetected = false;
  if (xDeltaInt > maxXDelta) {
    collisionDetected = true;
  }
  FixedPoint8 xDelta = m_xSpeed;
  if (collisionDetected) {
    xDelta = maxXDelta;
  }
  if (m_direction == Direction::Right) {
    m_x += xDelta;
  }
  else if (m_direction == Direction::Left) {
    m_x -= xDelta;
  }
  m_boundingBox.x = m_x + m_boundingBoxXOffset;
}

void Rockman::handleYCollision(Controls a_buttons) {
  Direction::type direction = Direction::Down;
  if (m_ySpeed > FixedPoint8(0)) {
    direction = Direction::Up;
  }
  int maxYDelta = m_pMap->getMaxYDelta(m_boundingBox, direction);
  FixedPoint8 attemptedY = m_y - m_ySpeed;
  int maxY = m_y;
  CollisionTestingResult collision = { false, maxYDelta };
  if (m_ySpeed > FixedPoint8(0)) {
    maxY -= maxYDelta;
    FixedPoint8 maxYFixedPoint8 = 0;
    maxYFixedPoint8 = maxY;
    if (attemptedY < maxYFixedPoint8) {
      collision.detected = true;
    }
  }
  else {
    maxY += maxYDelta;
    FixedPoint8 maxYFixedPoint8 = maxY;
    if (attemptedY > maxYFixedPoint8) {
      collision.detected = true;
    }
  }
  #ifdef DEBUG_COLLISION
    printf("-handleYCollision: maxYDelta: %i\n", maxYDelta);
    printf("-handleYCollision: maxY: %i\n", maxY);
    printf("-handleYCollision: attemptedY: ");
    attemptedY.print();
    printf("\n");
    if (collision.detected) {
      printf("-handleYCollision: detected!\n");
    }
    printf("-----\n");
  #endif
  if (m_movementType != MovementType::Grounded) {
    if (collision.detected) {
#ifdef DEBUG
      printf("yCollision.detected\n");
#endif
      if (m_currentYCollisionResponse == YCollisionResponse::Bump) {
#ifdef DEBUG
        printf("YCollisionResponse::Bump\n");
#endif
        m_ySpeed = m_ySpeedCollision;
        m_y -= collision.maxDelta;
        m_y = (int)m_y;
        m_boundingBox.y = m_y + m_boundingBoxYOffset;
        m_currentYCollisionResponse = YCollisionResponse::Land;
      }
      else if (m_currentYCollisionResponse == YCollisionResponse::Land) {
#ifdef DEBUG
        printf("YCollisionResponse::Land\n");
#endif
        m_ySpeed = 0;
        m_y += collision.maxDelta;
        m_y = (int)m_y;
        m_boundingBox.y = m_y + m_boundingBoxYOffset;
        // If landing from a jump or fall (as opposed to getting off a ladder),
        // and a horizontal direction is pressed, immediately run.
        if (m_movementType == MovementType::Airborne) {
          if (a_buttons.horizontalDirection != Direction::None
              && a_buttons.horizontalDirection == m_direction) {
            onRunning();
          }
          else {
            onStanding();
          }
        }
        else {
          onStanding();
        }
      }
      else if (m_currentYCollisionResponse == YCollisionResponse::Stop) {
#ifdef DEBUG
        printf("YCollisionResponse::Stop\n");
#endif
        if (m_ySpeed > FixedPoint8(0)) {
          m_y -= collision.maxDelta;
        }
        else {
          m_y += collision.maxDelta;
        }
        m_y = (int)m_y;
        m_boundingBox.y = m_y + m_boundingBoxYOffset;
        m_ySpeed = 0;
      }
    }
    else {
      m_y -= m_ySpeed;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
    }
  }
  else {
    m_ySpeed = 0;
    if (collision.maxDelta > 0) {
      onFalling();
    }
  }
}
