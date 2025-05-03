#include "rockman.h"
#include "camera.h"
#include "globals.h"
#include "inidictionary.h"
#include "level.h"
#include "map.h"
#include "servicelocator.h"
#include "sprite.h"
#include <string>


Rockman::Rockman(IniDictionary& a_iniDictionary, Level* a_pLevel, Map* a_pMap)
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
  , m_ySpeedScrollingFirstFrame(0)
  , m_ySpeedReeling(0)
  , m_direction(Direction::Right)
  , m_collisionDetected(false)
  , m_destroyed(false)
  , m_scrolling(false)
  , m_shooting(false)
  , m_movementType(MovementType::Teleporting)
  , m_stateChanged(false)
  , m_adjustPositionAfterScroll(false)
  , m_checkForLadderAfterCollisionCheck(false)
  , m_firstAcceleratingFrameIsRunningSpeed(false)
  , m_firstDeceleratingFrameIsRunningSpeed(false)
  , m_firstScrollingFrameIsDifferentSpeed(false)
  , m_noDirectionReversalAtLadderGrab(false)
  , m_noGravityApplicationAfterJump(false)
  , m_noJumpDelay(false)
  , m_noLadderGrabAssistAfterCollision(false)
  , m_noUpwardsPushAtLadderBottomGrab(false)
  , m_onlyIntAdjustmentAtYCollision(false)
  , m_resetFractionAtScrollStart(false)
  , m_setFractionAtScrollStart(false)
  , m_scrollStartFraction(0)
  , m_scrollDownEndPosY(0)
  , m_scrollUpEndPosY(0)
  , m_duration(0)
  , m_durationAccelerating(0)
  , m_durationAcceleratingPreviousAnimation(0)
  , m_durationDecelerating(0)
  , m_durationDeceleratingPreviousAnimation(0)
  , m_durationInvincible(0)
  , m_durationReeling(0)
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
  , m_pLevel(a_pLevel)
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
  m_pSprite = new Sprite(a_iniDictionary);
  m_pSprite->setCurrentAnimationIndex(AnimationType::Teleporting);
  m_adjustPositionAfterScroll = a_iniDictionary.getBoolValue("adjustPositionAfterScroll", "Options", false);
  m_checkForLadderAfterCollisionCheck = a_iniDictionary.getBoolValue("checkForLadderAfterCollisionCheck", "Options", false);
  m_firstAcceleratingFrameIsRunningSpeed = a_iniDictionary.getBoolValue("firstAcceleratingFrameIsWalkingSpeed", "Options", false);
  m_firstDeceleratingFrameIsRunningSpeed = a_iniDictionary.getBoolValue("firstDeceleratingFrameIsWalkingSpeed", "Options", false);
  m_firstScrollingFrameIsDifferentSpeed = a_iniDictionary.getBoolValue("firstScrollingFrameIsDifferentSpeed", "Options", false);
  m_noDirectionReversalAtLadderGrab = a_iniDictionary.getBoolValue("noDirectionReversalAtLadderGrab", "Options", false);
  m_noGravityApplicationAfterJump = a_iniDictionary.getBoolValue("noGravityApplicationAfterJump", "Options", false);
  m_noJumpDelay = a_iniDictionary.getBoolValue("noJumpDelay", "Options", false);
  m_noLadderGrabAssistAfterCollision = a_iniDictionary.getBoolValue("noLadderGrabAssistAfterCollision", "Options", false);
  m_noUpwardsPushAtLadderBottomGrab = a_iniDictionary.getBoolValue("noUpwardsPushAtLadderBottomGrab", "Options", false);
  m_onlyIntAdjustmentAtYCollision = a_iniDictionary.getBoolValue("onlyIntAdjustmentAtYCollision", "Options", false);
  m_resetFractionAtScrollStart = a_iniDictionary.getBoolValue("resetFractionAtScrollStart", "Options", false);
  m_setFractionAtScrollStart = a_iniDictionary.getBoolValue("setFractionAtScrollStart", "Options", false);
  int xSpeedInt = a_iniDictionary.getIntValue("acceleratingInt", "XSpeed", 0);
  int xSpeedFraction = a_iniDictionary.getIntValue("acceleratingFraction", "XSpeed", 0);
  m_xSpeedAccelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = a_iniDictionary.getIntValue("deceleratingInt", "XSpeed", 0);
  xSpeedFraction = a_iniDictionary.getIntValue("deceleratingFraction", "XSpeed", 0);
  m_xSpeedDecelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = a_iniDictionary.getIntValue("jumpAcceleratingInt", "XSpeed", 0);
  xSpeedFraction = a_iniDictionary.getIntValue("jumpAcceleratingFraction", "XSpeed", 0);
  m_xSpeedJumpAccelerating = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = a_iniDictionary.getIntValue("jumpingInt", "XSpeed", 0);
  xSpeedFraction = a_iniDictionary.getIntValue("jumpingFraction", "XSpeed", 0);
  m_xSpeedJumping = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = a_iniDictionary.getIntValue("reelingInt", "XSpeed", 0);
  xSpeedFraction = a_iniDictionary.getIntValue("reelingFraction", "XSpeed", 0);
  m_xSpeedReeling = FixedPoint8(xSpeedInt, xSpeedFraction);
  xSpeedInt = a_iniDictionary.getIntValue("runningInt", "XSpeed", 0);
  xSpeedFraction = a_iniDictionary.getIntValue("runningFraction", "XSpeed", 0);
  m_xSpeedRunning = FixedPoint8(xSpeedInt, xSpeedFraction);
  int ySpeedInt = a_iniDictionary.getIntValue("climbingDownInt", "YSpeed", 0);
  int ySpeedFraction = a_iniDictionary.getIntValue("climbingDownFraction", "YSpeed", 0);
  m_ySpeedClimbingDown = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("climbingUpInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("climbingUpFraction", "YSpeed", 0);
  m_ySpeedClimbingUp = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("collisionInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("collisionFraction", "YSpeed", 0);
  m_ySpeedCollision = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("fallingInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("fallingFraction", "YSpeed", 0);
  m_ySpeedFalling = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("gravityInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("gravityFraction", "YSpeed", 0);
  m_ySpeedGravity = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("jumpingInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("jumpingFraction", "YSpeed", 0);
  m_ySpeedJumping = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("jumpReleasedInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("jumpReleasedFraction", "YSpeed", 0);
  m_ySpeedJumpReleased = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("jumpReleasedMaxInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("jumpReleasedMaxFraction", "YSpeed", 0);
  m_ySpeedJumpReleasedMax = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("minInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("minFraction", "YSpeed", 0);
  m_ySpeedMin = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("scrollingInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("scrollingFraction", "YSpeed", 0);
  m_ySpeedScrolling = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("scrollingFirstFrameInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("scrollingFirstFrameFraction", "YSpeed", 0);
  m_ySpeedScrollingFirstFrame = FixedPoint8(ySpeedInt, ySpeedFraction);
  ySpeedInt = a_iniDictionary.getIntValue("reelingInt", "YSpeed", 0);
  ySpeedFraction = a_iniDictionary.getIntValue("reelingFraction", "YSpeed", 0);
  m_ySpeedReeling = FixedPoint8(ySpeedInt, ySpeedFraction);
  m_scrollStartFraction = a_iniDictionary.getIntValue("startFraction", "ScrollConstants", 0);
  m_scrollDownEndPosY = a_iniDictionary.getIntValue("downEndPosY", "ScrollConstants", 0);
  m_scrollUpEndPosY = a_iniDictionary.getIntValue("upEndPosY", "ScrollConstants", 0);
  m_durationAccelerating = a_iniDictionary.getIntValue("accelerating", "Durations", 0);
  m_durationAcceleratingPreviousAnimation = a_iniDictionary.getIntValue("acceleratingPreviousAnimation", "Durations", 0);
  m_durationDecelerating = a_iniDictionary.getIntValue("decelerating", "Durations", 0);
  m_durationDeceleratingPreviousAnimation = a_iniDictionary.getIntValue("deceleratingPreviousAnimation", "Durations", 0);
  m_durationInvincible = a_iniDictionary.getIntValue("invincible", "Durations", 0);
  m_durationReeling = a_iniDictionary.getIntValue("reeling", "Durations", 0);
  m_durationShooting = a_iniDictionary.getIntValue("shooting", "Durations", 0);
  m_boundingBoxHeightHalf = a_iniDictionary.getIntValue("heightHalf", "BoundingBox", 0);
  m_boundingBoxXOffset = a_iniDictionary.getIntValue("xOffset", "BoundingBox", 0);
  m_boundingBoxYOffset = a_iniDictionary.getIntValue("yOffset", "BoundingBox", 0);
  m_boundingBox.x = m_x + m_boundingBoxXOffset;
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  m_boundingBox.w = a_iniDictionary.getIntValue("width", "BoundingBox", 0);
  m_boundingBox.h = a_iniDictionary.getIntValue("height", "BoundingBox", 0);
  const char* pBulletBitmapFilename = a_iniDictionary.getCStringValue("spritesheet", "Bullet", "empty");
  VideoService& videoService = ServiceLocator::getVideoService();
  m_pBulletBitmap = videoService.loadSurface(pBulletBitmapFilename);
  m_bulletXOffsetLeft = a_iniDictionary.getIntValue("xOffsetLeft", "Bullet", 0);
  m_bulletXOffsetRight = a_iniDictionary.getIntValue("xOffsetRight", "Bullet", 0);
  m_bulletYOffset = a_iniDictionary.getIntValue("yOffset", "Bullet", 0);
}

Rockman::~Rockman() {
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.unloadSurface(m_pBulletBitmap);
  delete m_pSprite;
}

void Rockman::draw(Camera& a_camera) {
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
    int x = a_camera.getScreenMappedXCoordinate(m_x);
    int y = a_camera.getScreenMappedYCoordinate(m_y);
    m_pSprite->draw(x, y, spriteBitmap);
  }
  for (int iBullet = 0; iBullet < kMaximumSimultaneousBullets; iBullet++) {
    m_bullets[iBullet].draw(m_pBulletBitmap, a_camera);
  }
  int ladderSonarInt = m_ladderSonar.getAsInt();
  m_debugInfo.draw(m_x.get(), m_y.get(), m_xSpeed.get(), m_ySpeed.get(), ladderSonarInt);
}

const SDL_Rect& Rockman::getBoundingBox() {
  return m_boundingBox;
}

Collider Rockman::getCollider() {
  return m_collider;
}

int Rockman::getX() {
  return m_x;
}

int Rockman::getY() {
  return m_y;
}

bool Rockman::isDestroyed() {
  return m_destroyed;
}

void Rockman::onScrollingStart() {
#ifdef DEBUG
  printf("onScrolling\n");
#endif
  m_scrolling = true;
  if (m_resetFractionAtScrollStart) {
    m_y = (int)m_y;
  }
  if (m_setFractionAtScrollStart) {
    m_y = (int)m_y;
    m_y += FixedPoint8(0, m_scrollStartFraction);
  }
  else if (m_firstScrollingFrameIsDifferentSpeed) {
    if (m_ySpeed > FixedPoint8(0)) {
      m_y -= m_ySpeedScrollingFirstFrame;
    }
    else {
      m_y += m_ySpeedScrollingFirstFrame;
    }
  }
  else {
    scrollingStateHandler();
  }
}

void Rockman::onScrollingStop() {
#ifdef DEBUG
  printf("onScrollingStop\n");
#endif
  m_scrolling = false;
  SDL_Rect mapBoundary = m_pMap->getCurrentBoundary();
  if (m_adjustPositionAfterScroll) {
    if (m_ySpeed > FixedPoint8(0)) {
      m_y = mapBoundary.y + m_scrollUpEndPosY;
    }
    else {
      m_y = mapBoundary.y + m_scrollDownEndPosY;
    }
  }
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
}

void Rockman::update(Controls a_controls, Camera& a_camera) {
  m_pSprite->update();
  if (m_scrolling) {
    scrollingStateHandler();
    return;
  }
  if (m_invincibleDurationLeft > 0) {
    m_invincibleDurationLeft--;
  }
  if (m_nextStateEventHandler) {
    (this->*m_nextStateEventHandler)();
    m_nextStateEventHandler = 0;
    m_stateChanged = false;
  }
  if (m_movementType != MovementType::Teleporting) {
    this->shootingStateHandler(a_controls, a_camera);
  }
  if (!m_checkForLadderAfterCollisionCheck) {
    if (m_movementType == MovementType::Grounded
        || m_movementType == MovementType::Airborne) {
      checkClimbing(a_controls);
    }
  }
  (this->*m_currentStateHandler)(a_controls);
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
    if (m_noJumpDelay && a_controls.jumpPressed && m_movementType == MovementType::Grounded) {
      onJumping(true);
      m_collisionDetected = handleYCollision(a_controls);
      if (!m_noGravityApplicationAfterJump) {
        m_ySpeed -= m_ySpeedGravity;
      }
    }
    else {
      m_collisionDetected = handleYCollision(a_controls);
      if (!m_collisionDetected) {
        m_ySpeed -= m_ySpeedGravity;
      }
    }
  }
  if (m_checkForLadderAfterCollisionCheck) {
    if (m_movementType == MovementType::Grounded
        || m_movementType == MovementType::Airborne) {
      checkClimbing(a_controls);
    }
  }
  if (!m_noJumpDelay && a_controls.jumpPressed && m_movementType == MovementType::Grounded) {
    onJumping();
  }
  m_duration++;
  if (m_shooting) {
    m_shootingDuration++;
  }
  m_stateChanged = false;
}

void Rockman::standingStateHandler(Controls a_buttons) {
  if (a_buttons.horizontalDirection != Direction::None) {
    m_direction = a_buttons.horizontalDirection;
    onAccelerating();
  }
}

void Rockman::acceleratingStateHandler(Controls a_buttons) {
  // Set m_xSpeed because onAccelerating might have set it differently.
  m_xSpeed = m_xSpeedAccelerating;
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
  // Set m_xSpeed because onDecelerating might have set it differently.
  m_xSpeed = m_xSpeedDecelerating;
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
      if (m_ladderSonar.atHeadTop) {
        m_pSprite->setCurrentAnimationIndex(AnimationType::Climbing);
      }
      else {
        m_pSprite->setCurrentAnimationIndex(AnimationType::ClimbingTop);
      }
      m_currentYCollisionResponse = YCollisionResponse::Stop;
    }
    else if (a_buttons.verticalDirection == Direction::Down) {
      m_ySpeed = m_ySpeedClimbingDown;
      if (m_ladderSonar.behindEyes || m_ladderSonar.atHeadTop) {
        m_pSprite->setCurrentAnimationIndex(AnimationType::Climbing);
      }
      else {
        m_pSprite->setCurrentAnimationIndex(AnimationType::ClimbingTop);
      }
      m_currentYCollisionResponse = YCollisionResponse::Land;
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
      int deltaTileY = m_y % Tile::kHeight;
      m_y = m_y - deltaTileY - m_boundingBoxHeightHalf;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
      m_ySpeed = m_ySpeedCollision;
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

void Rockman::scrollingStateHandler() {
  if (m_ySpeed > FixedPoint8(0)) {
    m_y -= m_ySpeedScrolling;
  }
  else {
    m_y += m_ySpeedScrolling;
  }
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
}

void Rockman::shootingStateHandler(Controls a_buttons, Camera& a_camera) {
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
    m_bullets[iBullet].update(a_camera);
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
    m_boundingBox.y = m_y + m_boundingBoxYOffset;
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
  if (m_firstAcceleratingFrameIsRunningSpeed) {
    m_xSpeed = m_xSpeedRunning;
  }
  else {
    m_xSpeed = m_xSpeedAccelerating;
  }
  m_duration = 0;
  if (m_durationAcceleratingPreviousAnimation == 0) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Accelerating);
  }
  m_currentStateHandler = &Rockman::acceleratingStateHandler;
  m_stateChanged = true;
}

void Rockman::onDecelerating() {
  if (m_firstDeceleratingFrameIsRunningSpeed) {
    m_xSpeed = m_xSpeedRunning;
  }
  else {
    m_xSpeed = m_xSpeedDecelerating;
  }
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

void Rockman::onJumping(bool a_setAnimation) {
#ifdef DEBUG
  printf("onJumping\n");
#endif
  m_ySpeed = m_ySpeedJumping;
  m_duration = 0;
  if (a_setAnimation) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Jumping);
  }
  m_currentStateHandler = &Rockman::jumpingStateHandler;
  m_currentYCollisionResponse = YCollisionResponse::Bump;
  m_movementType = MovementType::Airborne;
  m_stateChanged = true;
}

void Rockman::onFalling(bool a_afterCollisionHandling) {
#ifdef DEBUG
  printf("onFalling\n");
#endif
  m_ySpeed = m_ySpeedFalling;
  if (a_afterCollisionHandling) {
    m_y -= m_ySpeed;
  }
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
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
  if (m_ladderSonar.atHeadTop) {
    m_pSprite->setCurrentAnimationIndex(AnimationType::Climbing);
  }
  else {
    m_pSprite->setCurrentAnimationIndex(AnimationType::ClimbingTop);
  }
  m_currentStateHandler = &Rockman::climbingStateHandler;
  m_movementType = MovementType::Climbing;
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
  if (m_noLadderGrabAssistAfterCollision && m_collisionDetected
      && (!m_ladderSonar.belowFeetNextFrame
          && !m_ladderSonar.atFeet
          && !m_ladderSonar.behindEyes
          && !m_ladderSonar.atHeadTop
          && m_ladderSonar.atHeadTopNextFrame)) {
    return;
  }
  if (a_buttons.verticalDirection == Direction::Up
      && !(m_ladderSonar.belowFeetNextFrame
           && !m_ladderSonar.atFeet
           && !m_ladderSonar.behindEyes
           && !m_ladderSonar.atHeadTop
           && !m_ladderSonar.atHeadTopNextFrame)) {
    m_xSpeed = 0;
    int deltaTileX = m_x % Tile::kWidth;
    m_x = m_x - deltaTileX + (Tile::kWidth / 2);
    m_boundingBox.x = m_x + m_boundingBoxXOffset;
    if (!m_noDirectionReversalAtLadderGrab) {
      reverseHorizontalDirection();
    }
    if (!m_noUpwardsPushAtLadderBottomGrab) {
      if (!m_ladderSonar.belowFeetNextFrame
          && !m_ladderSonar.atFeet
          && !m_ladderSonar.behindEyes
          && !m_ladderSonar.atHeadTop
          && m_ladderSonar.atHeadTopNextFrame) {
        // Rockman's position needs to be pushed up
        // so the sprite is fully on the ladder.
        int deltaTileY = m_y % Tile::kHeight;
        m_y = m_y - deltaTileY - m_boundingBox.h;
        m_boundingBox.y = m_y + m_boundingBoxYOffset;
        onFalling();
      }
      else {
        onClimbing();
      }
    }
    else {
      bool movingUp = false;
      if (m_ySpeed > FixedPoint8(0)) {
        movingUp = true;
      }
      m_ladderSonar = m_pMap->getLadderSonar(m_x, m_y, m_y - m_ySpeed, movingUp);
      onClimbing();
    }
  }
  else if (a_buttons.verticalDirection == Direction::Down
           && !(!m_ladderSonar.belowFeetNextFrame
                && m_ladderSonar.atFeet
                && m_ladderSonar.behindEyes
                && m_ladderSonar.atHeadTop)) {
    m_xSpeed = 0;
    int deltaTileX = m_x % Tile::kWidth;
    m_x = m_x - deltaTileX + (Tile::kWidth / 2);
    m_boundingBox.x = m_x + m_boundingBoxXOffset;
    if (m_ladderSonar.belowFeetNextFrame
        && !m_ladderSonar.atFeet
        && !m_ladderSonar.behindEyes
        && !m_ladderSonar.atHeadTop
        && !m_ladderSonar.atHeadTopNextFrame) {
      m_y += m_boundingBoxHeightHalf;
      m_boundingBox.y = m_y + m_boundingBoxYOffset;
    }
    if (!m_noDirectionReversalAtLadderGrab) {
      reverseHorizontalDirection();
    }
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

void Rockman::handleXCollision() {
  int maxXDelta = m_pMap->getMaxXDelta(m_boundingBox, m_direction);
  FixedPoint8 attemptedX = 0;
  if (m_direction == Direction::Right) {
    FixedPoint8 maxX = (int)(m_x + maxXDelta);
    attemptedX = m_x + m_xSpeed;
    if (attemptedX > maxX) {
      // Only the integer part should be adjusted.
      FixedPoint8 attemptedXFraction = attemptedX - (int)attemptedX;
      m_x = attemptedXFraction + maxX;
    }
    else {
      m_x = attemptedX;
    }
  }
  else if (m_direction == Direction::Left) {
    FixedPoint8 maxX = (int)(m_x - maxXDelta);
    attemptedX = m_x - m_xSpeed;
    if (attemptedX < maxX) {
      // Only the integer part should be adjusted.
      FixedPoint8 attemptedXFraction = attemptedX - (int)attemptedX;
      m_x = attemptedXFraction + maxX;
    }
    else {
      m_x = attemptedX;
    }
  }
  m_boundingBox.x = m_x + m_boundingBoxXOffset;
}

bool Rockman::handleYCollision(Controls a_buttons) {
  FixedPoint8 attemptedY = m_y - m_ySpeed;
  Direction::type direction = Direction::None;
  bool isLeavingScreen = false;
  if (m_ySpeed > FixedPoint8(0)) {
    direction = Direction::Up;
    if (m_movementType == MovementType::Climbing) {
      isLeavingScreen = m_pMap->isLeavingScreenTop(attemptedY);
    }
  }
  else {
    direction = Direction::Down;
    isLeavingScreen = m_pMap->isLeavingScreenBottom(attemptedY);
    if (isLeavingScreen) {
      bool isEnteringInvalidScreen = m_pMap->isEnteringInvalidScreen(m_x, attemptedY);
      if (isEnteringInvalidScreen) {
        reset();
        m_pLevel->onReset();
        return true;
      }
    }
  }
  if (isLeavingScreen) {
    onScrollingStart();
    m_pLevel->onScrollStart(direction);
    return true;
  }
  bool collisionDetected = false;
  int maxYDelta = m_pMap->getMaxYDelta(m_boundingBox, direction);
  FixedPoint8 maxY = 0;
  if (direction == Direction::Up) {
    maxY = (int)(m_y - maxYDelta);
    if (attemptedY < maxY) {
      collisionDetected = true;
    }
  }
  else if (direction == Direction::Down) {
    maxY = (int)(m_y + maxYDelta);
    if (attemptedY > maxY) {
      collisionDetected = true;
    }
  }
  if (collisionDetected) {
    if (!m_onlyIntAdjustmentAtYCollision) {
      m_y = maxY;
    }
    else {
      // Only the integer part should be adjusted.
      FixedPoint8 attemptedYFraction = attemptedY - (int)attemptedY;
      m_y = attemptedYFraction + maxY;
    }
  }
  else {
    m_y -= m_ySpeed;
  }
  m_boundingBox.y = m_y + m_boundingBoxYOffset;
  if (m_movementType != MovementType::Grounded) {
    if (collisionDetected) {
      if (m_currentYCollisionResponse == YCollisionResponse::Bump) {
        m_ySpeed = m_ySpeedCollision;
        m_currentYCollisionResponse = YCollisionResponse::Land;
      }
      else if (m_currentYCollisionResponse == YCollisionResponse::Land) {
        m_ySpeed = m_ySpeedCollision;
        // If landing from a jump or fall (as opposed to getting off a ladder),
        // and a horizontal direction is pressed, immediately run.
        if (m_movementType == MovementType::Airborne
            && a_buttons.horizontalDirection != Direction::None
            && a_buttons.horizontalDirection == m_direction) {
          onRunning();
        }
        else {
          onStanding();
        }
      }
      else if (m_currentYCollisionResponse == YCollisionResponse::Stop) {
        m_ySpeed = 0;
      }
    }
  }
  else {
    m_ySpeed = m_ySpeedCollision;
    if (!collisionDetected) {
      onFalling(true);
    }
  }
  return collisionDetected;
}
