#ifndef ROCKMAN_H
#define ROCKMAN_H

#include "globals.h"
#include "bullet.h"
#include "collider.h"
#include "controls.h"
#include "debuginfo.h"
#include "direction.h"
#include "fixedpoint8.h"
#include "laddersonar.h"

class Map;
class Sprite;

class Rockman {
  private:
  static const int kMaximumSimultaneousBullets = 3;
  static const int kScrollYSpeed = 4;
  struct AnimationType {
    enum type {
      Standing = 0,
      Accelerating = 1,
      Running = 2,
      Jumping = 3,
      Climbing = 4,
      ClimbingTop = 5,
      Damaged = 6,
      Teleporting = 7,
      Transforming = 8
  };
};
  struct MovementType {
    enum type {
      Grounded,
      Airborne,
      Climbing,
      Teleporting
    };
  };
  struct SpriteBitmap {
    enum type {
      RunningRight = 0,
      RunningLeft = 1,
      ShootingRight = 2,
      ShootingLeft = 3
    };
  };
  struct YCollisionResponse {
    enum type {
      Bump,
      Land,
      Stop
    };
  };
  typedef void (Rockman::*RockmanStateHandler)(Controls a_buttons);
  typedef void (Rockman::*RockmanEventHandler)();
  FixedPoint8 m_x;
  FixedPoint8 m_y;
  FixedPoint8 m_xSpeed;
  FixedPoint8 m_ySpeed;
// physics constants loaded from file
  FixedPoint8 m_xSpeedAccelerating;
  FixedPoint8 m_xSpeedDecelerating;
  FixedPoint8 m_xSpeedJumpAccelerating;
  FixedPoint8 m_xSpeedJumping;
  FixedPoint8 m_xSpeedReeling;
  FixedPoint8 m_xSpeedRunning;
  FixedPoint8 m_ySpeedClimbingDown;
  FixedPoint8 m_ySpeedClimbingUp;
  FixedPoint8 m_ySpeedCollision;
  FixedPoint8 m_ySpeedFalling;
  FixedPoint8 m_ySpeedGravity;
  FixedPoint8 m_ySpeedJumping;
  FixedPoint8 m_ySpeedJumpReleased;
  FixedPoint8 m_ySpeedJumpReleasedMax;
  FixedPoint8 m_ySpeedMin;
  FixedPoint8 m_ySpeedScrolling;
  FixedPoint8 m_ySpeedReeling;
// end constants
  Direction::type m_direction;
  bool m_destroyed;
  bool m_scrolling;
  bool m_shooting;
  MovementType::type m_movementType;
  bool m_stateChanged;
  int m_duration;
// duration constants loaded from file
  int m_durationAccelerating;
  int m_durationAcceleratingPreviousAnimation;
  int m_durationDecelerating;
  int m_durationDeceleratingPreviousAnimation;
  int m_durationInvincible;
  int m_durationReeling;
  int m_durationScrolling;
  int m_durationShooting;
// end constants
  int m_shootingDuration;
  int m_invincibleDurationLeft;
  int m_teleportStopPosY;
// coordinate constants loaded from file
  int m_boundingBoxHeightHalf;
  int m_boundingBoxXOffset;
  int m_boundingBoxYOffset;
  int m_bulletXOffsetLeft;
  int m_bulletXOffsetRight;
  int m_bulletYOffset;
// end constants
  RockmanStateHandler m_currentStateHandler;
  RockmanEventHandler m_nextStateEventHandler;
  YCollisionResponse::type m_currentYCollisionResponse;
  Bullet m_bullets[kMaximumSimultaneousBullets];
  SDL_Surface* m_pBulletBitmap;
  Map* m_pMap;
  Sprite* m_pSprite;
  SDL_Rect m_boundingBox;
  Collider m_collider;
  LadderSonar m_ladderSonar;
  DebugInfo m_debugInfo;
  void standingStateHandler(Controls a_buttons);
  void acceleratingStateHandler(Controls a_buttons);
  void runningStateHandler(Controls a_buttons);
  void deceleratingStateHandler(Controls a_buttons);
  void jumpingStateHandler(Controls a_buttons);
  void climbingStateHandler(Controls a_buttons);
  void scrollingStateHandler(Controls a_buttons);
  void fallScrollingStateHandler(Controls a_buttons);
  void shootingStateHandler(Controls a_buttons);
  void reelingStateHandler(Controls a_buttons);
  void teleportingStateHandler(Controls a_buttons);
  void transformingStateHandler(Controls a_buttons);
  void onStanding();
  void onAccelerating();
  void onDecelerating();
  void onRunning();
  void onJumping();
  void onFalling();
  void onClimbing();
  void onScrolling();
  void onFallScrolling();
  void onReeling();
  void onTransforming();
  void checkClimbing(Controls a_buttons);
  void reverseHorizontalDirection();
  void handleLeavingScreen();
  void handleXCollision();
  void handleYCollision(Controls a_buttons);

  public:
  Rockman(const char* a_pIniFilename, Map* a_pMap);
  ~Rockman();
  void draw();
  Collider getCollider();
  bool isDestroyed();
  void receiveDamage();
  void reset();
  void update();
};

#endif
