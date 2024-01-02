//
// Created by lenovo on 2023/12/30.
//

#ifndef CAR_AND_DOOR_H
#define CAR_AND_DOOR_H

#define CAR_AND_DOOR_CTRL_ADDRESS ((unsigned char) 0x33)
#include <stdbool.h>
#include "mechanical_arm.h"

enum CarXMoveCtrl {
    NO_X_MOVE = 0x30,
    FORWARD,
    BACKWARD
};

enum CarYMoveCtrl {
    NO_Y_MOVE = 0x30,
    LEFT,
    RIGHT
};

enum CarTowerCtrl {
    NO_TOWER_ROTATE = 0x30,
    TOWER_ANTICLOCKWISE,
    TOWER_CLOCKWISE,
};

enum CarLiftingRodCtrl {
    NO_LR_MOVE = 0x30,
    UP,
    DOWN
};

enum DoorCtrl {
    NO_DOOR_MOVE = 0x30,
    DOOR_UP,
    DOOR_DOWN
};

struct CarAndDoorCtrl {
    enum CarXMoveCtrl x;
    enum CarYMoveCtrl y;
    enum CarTowerCtrl tower;
    enum CarLiftingRodCtrl liftingRod;
    enum DoorCtrl door1;
    enum DoorCtrl door2;
    enum DoorCtrl door3;
};

struct CarAndDoorState {
    float carX;
    float carY;
    float carAngle;
    float carLiftingRodHeight;
    float door1Height;
    float door2Height;
    float door3Height;
};

enum CarPositionType {
    RED_BEGIN,
    GREEN_BEGIN,
    BLUE_BEGIN,
    BEFORE,
    AFTER,
    RED_END,
    GREEN_END,
    BLUE_END
};

void sendCarAndDoorCtrl(struct CarAndDoorCtrl ctrl);

struct CarAndDoorCtrl defaultCarAndDoorCtrl();

void carAndDoorTo(struct CarAndDoorState state);

enum CarPositionType getCarPositionType(struct CarAndDoorState state);

extern struct CarAndDoorState CarAndDoorStateNow;

extern const float carBeforeX;
extern const float carBeginX;
extern const float carRedBeginY;
extern const float carGreenBeginY;
extern const float carBlueBeginY;
extern const float carBeforeAndAfterY;
extern const float carEndY;
extern const float carRedEndX;
extern const float carGreenEndX;
extern const float carBlueEndX;

extern struct CarAndDoorState MainStart;
// 到位，升高抓手
extern struct CarAndDoorState RedBeginLRDown;
// 到位，降低抓手
extern struct CarAndDoorState RedBeginLRUp;
extern struct CarAndDoorState GreenBeginLRDown;
extern struct CarAndDoorState GreenBeginLRUp;
extern struct CarAndDoorState BlueBeginLRDown;
extern struct CarAndDoorState BlueBeginLRUp;
// 到位，升高抓手
extern struct CarAndDoorState RedEndLRDown;
// 到位，降低抓手
extern struct CarAndDoorState RedEndLRUp;
extern struct CarAndDoorState GreenEndLRDown;
extern struct CarAndDoorState GreenEndLRUp;
extern struct CarAndDoorState BlueEndLRDown;
extern struct CarAndDoorState BlueEndLRUp;

struct CarAndDoorState getTarget(bool *arrive, struct CarAndDoorState target);

void carTransform(enum ChestColor color);

#endif //CAR_AND_DOOR_H
