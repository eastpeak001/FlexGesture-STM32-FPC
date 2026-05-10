#include "app_gesture.h"
#include <stdio.h>

/*
 * FlexGesture V0.3
 * Gesture recognition is split into four stages:
 * WAIT_POSE -> ARMED -> WAIT_RELEASE -> WAIT_NEUTRAL -> WAIT_POSE.
 *
 * Pose decides the gesture direction. Motion only triggers an already armed
 * pose, so rotating the hand into position does not emit a gesture.
 */
#define MOTION_THRESHOLD              10000
#define RELEASE_MOTION_THRESHOLD      4000
#define GESTURE_MAX_WAIT_RELEASE_MS   700U
#define GESTURE_DEBUG                 0
#define GESTURE_DEBUG_INTERVAL_MS     1000U
#define POSE_STABLE_MS                180U
#define POSE_ARM_MOTION_MAX           5000
#define NEUTRAL_STABLE_MS             200U

typedef enum
{
  GESTURE_STATE_WAIT_POSE = 0,
  GESTURE_STATE_ARMED,
  GESTURE_STATE_WAIT_RELEASE,
  GESTURE_STATE_WAIT_NEUTRAL
} GestureState_t;

typedef enum
{
  HAND_POSE_UNKNOWN = 0,
  HAND_POSE_FLAT,
  HAND_POSE_CCW_90,
  HAND_POSE_CW_90,
  HAND_POSE_CW_180
} HandPose_t;

static GestureState_t gesture_state = GESTURE_STATE_WAIT_POSE;
static uint32_t last_gesture_tick = 0U;
static HandPose_t stable_pose = HAND_POSE_UNKNOWN;
static HandPose_t last_pose = HAND_POSE_UNKNOWN;
static uint32_t pose_start_tick = 0U;
static uint32_t neutral_start_tick = 0U;
#if GESTURE_DEBUG
static uint32_t last_debug_tick = 0U;
#endif

static int32_t Gesture_Abs16(int16_t value)
{
  return (value < 0) ? -(int32_t)value : (int32_t)value;
}

static int32_t Gesture_MotionValue(int16_t gx, int16_t gy, int16_t gz)
{
  return Gesture_Abs16(gx) + Gesture_Abs16(gy) + Gesture_Abs16(gz);
}

#if GESTURE_DEBUG
static const char *Gesture_PoseToString(HandPose_t pose)
{
  switch (pose)
  {
    case HAND_POSE_FLAT:
      return "FLAT";
    case HAND_POSE_CCW_90:
      return "CCW_90";
    case HAND_POSE_CW_90:
      return "CW_90";
    case HAND_POSE_CW_180:
      return "CW_180";
    case HAND_POSE_UNKNOWN:
    default:
      return "UNKNOWN";
  }
}

static const char *Gesture_StateToString(GestureState_t state)
{
  switch (state)
  {
    case GESTURE_STATE_WAIT_POSE:
      return "WAIT_POSE";
    case GESTURE_STATE_ARMED:
      return "ARMED";
    case GESTURE_STATE_WAIT_RELEASE:
      return "WAIT_RELEASE";
    case GESTURE_STATE_WAIT_NEUTRAL:
      return "WAIT_NEUTRAL";
    default:
      return "UNKNOWN";
  }
}
#endif

static HandPose_t Gesture_DetectPose(int16_t ax, int16_t ay, int16_t az)
{
  int32_t ax_abs = Gesture_Abs16(ax);
  int32_t ay_abs = Gesture_Abs16(ay);
  int32_t az_abs = Gesture_Abs16(az);

  /*
   * Pose mapping is intentionally centralized here for real-world tuning.
   * Current first-pass assumption:
   *   +Z dominant: hand back/palm flat
   *   -Z dominant: clockwise about 180 degrees
   *   +X/+Y dominant: counter-clockwise about 90 degrees
   *   -X/-Y dominant: clockwise about 90 degrees
   */
  if ((az_abs >= ax_abs) && (az_abs >= ay_abs))
  {
    return (az >= 0) ? HAND_POSE_FLAT : HAND_POSE_CW_180;
  }

  if (ax_abs >= ay_abs)
  {
    return (ax >= 0) ? HAND_POSE_CCW_90 : HAND_POSE_CW_90;
  }

  return (ay >= 0) ? HAND_POSE_CCW_90 : HAND_POSE_CW_90;
}

static uint8_t Gesture_DetectMotion(int16_t ax, int16_t ay, int16_t az,
                                    int16_t gx, int16_t gy, int16_t gz)
{
  (void)ax;
  (void)ay;
  (void)az;

  return (Gesture_MotionValue(gx, gy, gz) > MOTION_THRESHOLD) ? 1U : 0U;
}

static Gesture_t Gesture_FromPose(HandPose_t pose)
{
  /* Current V0.3 mapping. FLAT is neutral and does not emit DOWN yet. */
  switch (pose)
  {
    case HAND_POSE_CCW_90:
      return GESTURE_LEFT;
    case HAND_POSE_CW_90:
      return GESTURE_RIGHT;
    case HAND_POSE_CW_180:
      return GESTURE_UP;
    case HAND_POSE_FLAT:
      return GESTURE_NONE;
    case HAND_POSE_UNKNOWN:
    default:
      return GESTURE_NONE;
  }
}

static void Gesture_DebugPrintParams(void)
{
#if GESTURE_DEBUG
  printf("DBG params motion_th=%d release_th=%d pose_stable_ms=%lu pose_arm_motion_max=%d neutral_stable_ms=%lu\r\n",
         MOTION_THRESHOLD,
         RELEASE_MOTION_THRESHOLD,
         (unsigned long)POSE_STABLE_MS,
         POSE_ARM_MOTION_MAX,
         (unsigned long)NEUTRAL_STABLE_MS);
#endif
}

static void Gesture_DebugPeriodic(uint32_t now,
                                  GestureState_t state,
                                  HandPose_t current_pose,
                                  HandPose_t locked_pose,
                                  int16_t ax,
                                  int16_t ay,
                                  int16_t az,
                                  int16_t gx,
                                  int16_t gy,
                                  int16_t gz,
                                  int32_t motion)
{
#if GESTURE_DEBUG
  if ((now - last_debug_tick) < GESTURE_DEBUG_INTERVAL_MS)
  {
    return;
  }

  last_debug_tick = now;

  if (state == GESTURE_STATE_ARMED && motion <= MOTION_THRESHOLD)
  {
    printf("DBG armed but motion low motion=%ld th=%d\r\n",
           (long)motion,
           MOTION_THRESHOLD);
    return;
  }

  if (state == GESTURE_STATE_WAIT_NEUTRAL)
  {
    printf("DBG wait neutral pose=%s stable=%s\r\n",
           Gesture_PoseToString(current_pose),
           Gesture_PoseToString(locked_pose));
    return;
  }

  if (state == GESTURE_STATE_WAIT_RELEASE)
  {
    printf("DBG wait release motion=%ld release_th=%d\r\n",
           (long)motion,
           RELEASE_MOTION_THRESHOLD);
    return;
  }

  printf("DBG state=%s pose=%s stable=%s ax=%d ay=%d az=%d gx=%d gy=%d gz=%d motion=%ld th=%d\r\n",
         Gesture_StateToString(state),
         Gesture_PoseToString(current_pose),
         Gesture_PoseToString(locked_pose),
         ax,
         ay,
         az,
         gx,
         gy,
         gz,
         (long)motion,
         MOTION_THRESHOLD);
#else
  (void)now;
  (void)state;
  (void)current_pose;
  (void)locked_pose;
  (void)ax;
  (void)ay;
  (void)az;
  (void)gx;
  (void)gy;
  (void)gz;
  (void)motion;
#endif
}

static void Gesture_ResetPoseLock(uint32_t now, HandPose_t pose)
{
  stable_pose = HAND_POSE_UNKNOWN;
  last_pose = pose;
  pose_start_tick = now;
}

static uint8_t Gesture_IsNeutralPose(HandPose_t pose)
{
  return (pose == HAND_POSE_FLAT) ? 1U : 0U;
}

static void Gesture_UpdateWaitPoseState(uint32_t now, HandPose_t current_pose, int32_t motion)
{
  /*
   * Arm only after a non-neutral pose is stable while the hand is quiet.
   * This prevents the preparation rotation from being treated as a gesture.
   */
  if (current_pose != last_pose)
  {
    Gesture_ResetPoseLock(now, current_pose);
    return;
  }

  if ((current_pose == HAND_POSE_UNKNOWN) ||
      (current_pose == HAND_POSE_FLAT) ||
      (motion >= POSE_ARM_MOTION_MAX))
  {
    pose_start_tick = now;
    return;
  }

  if ((now - pose_start_tick) >= POSE_STABLE_MS)
  {
    stable_pose = current_pose;
    gesture_state = GESTURE_STATE_ARMED;
  }
}

static void Gesture_UpdateReleaseState(uint32_t now, int32_t motion)
{
  /* After a gesture, wait for motion to stop before requiring neutral pose. */
  if ((now - last_gesture_tick) > GESTURE_MAX_WAIT_RELEASE_MS)
  {
    gesture_state = GESTURE_STATE_WAIT_NEUTRAL;
    Gesture_ResetPoseLock(now, HAND_POSE_UNKNOWN);
    neutral_start_tick = now;
    return;
  }

  if (motion < RELEASE_MOTION_THRESHOLD)
  {
    gesture_state = GESTURE_STATE_WAIT_NEUTRAL;
    Gesture_ResetPoseLock(now, HAND_POSE_UNKNOWN);
    neutral_start_tick = now;
  }
}

static void Gesture_UpdateNeutralState(uint32_t now, HandPose_t current_pose)
{
  /* A new gesture cannot be armed until the hand returns to FLAT. */
  if (Gesture_IsNeutralPose(current_pose) != 0U)
  {
    if (neutral_start_tick == 0U)
    {
      neutral_start_tick = now;
    }

    if ((now - neutral_start_tick) >= NEUTRAL_STABLE_MS)
    {
      gesture_state = GESTURE_STATE_WAIT_POSE;
      Gesture_ResetPoseLock(now, current_pose);
      neutral_start_tick = 0U;
    }
  }
  else
  {
    neutral_start_tick = 0U;
  }
}

void Gesture_Init(void)
{
  gesture_state = GESTURE_STATE_WAIT_POSE;
  last_gesture_tick = 0U;
  stable_pose = HAND_POSE_UNKNOWN;
  last_pose = HAND_POSE_UNKNOWN;
  pose_start_tick = 0U;
  neutral_start_tick = 0U;
#if GESTURE_DEBUG
  last_debug_tick = 0U;
#endif
  Gesture_DebugPrintParams();
}

Gesture_t Gesture_Update(int16_t ax, int16_t ay, int16_t az,
                         int16_t gx, int16_t gy, int16_t gz)
{
  uint32_t now = HAL_GetTick();
  int32_t motion = Gesture_MotionValue(gx, gy, gz);
  HandPose_t current_pose = Gesture_DetectPose(ax, ay, az);
  Gesture_t gesture = GESTURE_NONE;

  Gesture_DebugPeriodic(now, gesture_state, current_pose, stable_pose,
                        ax, ay, az, gx, gy, gz, motion);

  if (gesture_state == GESTURE_STATE_WAIT_RELEASE)
  {
    Gesture_UpdateReleaseState(now, motion);
    return GESTURE_NONE;
  }

  if (gesture_state == GESTURE_STATE_WAIT_NEUTRAL)
  {
    Gesture_UpdateNeutralState(now, current_pose);
    return GESTURE_NONE;
  }

  if (gesture_state == GESTURE_STATE_WAIT_POSE)
  {
    Gesture_UpdateWaitPoseState(now, current_pose, motion);
    return GESTURE_NONE;
  }

  if (Gesture_DetectMotion(ax, ay, az, gx, gy, gz) == 0U)
  {
    return GESTURE_NONE;
  }

  gesture = Gesture_FromPose(stable_pose);

  if (gesture != GESTURE_NONE)
  {
    last_gesture_tick = now;
    neutral_start_tick = 0U;
    gesture_state = GESTURE_STATE_WAIT_RELEASE;
  }

  return gesture;
}

const char *Gesture_ToString(Gesture_t gesture)
{
  switch (gesture)
  {
    case GESTURE_UP:
      return "UP";
    case GESTURE_DOWN:
      return "DOWN";
    case GESTURE_LEFT:
      return "LEFT";
    case GESTURE_RIGHT:
      return "RIGHT";
    case GESTURE_NONE:
    default:
      return "NONE";
  }
}
