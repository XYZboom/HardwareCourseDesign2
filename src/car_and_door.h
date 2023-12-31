//
// Created by lenovo on 2023/12/30.
//

#ifndef CAR_AND_DOOR_H
#define CAR_AND_DOOR_H

#define CAR_AND_DOOR_CTRL_ADDRESS ((unsigned char) 0x33)

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

void sendCarAndDoorCtrl(struct CarAndDoorCtrl ctrl);

struct CarAndDoorCtrl defaultCarAndDoorCtrl();

void carAndDoorTo(struct CarAndDoorState state);

#endif //CAR_AND_DOOR_H
