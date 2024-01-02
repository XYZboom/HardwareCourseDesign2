//
// Created by lenovo on 2023/12/30.
//

#include "car_and_door.h"

#include <math.h>

#include "mechanical_arm.h"

#include "time_utils.h"

#include "constants.h"
#include "uart.h"
#include "gpio.h"

struct CarAndDoorState CarAndDoorStateNow;

const float doorMovePerFrame = 0.5f;
const float carXMovePerFrame = 1.0f;
const float carYMovePerFrame = 1.0f;
const float carLRMovePerFrame = 1.0f;
const float carRotatePerFrame = 5.0f;

const float carLRHeight = 5.0f;
const float doorHeight = 10.0f;
const float carBeforeX = -10.0f;
const float carBeginX = 2.6f;
const float carRedBeginY = -19.9f;
const float carGreenBeginY = 3.15f;
const float carBlueBeginY = 26.9f;
const float carBeforeAndAfterY = 32.9f;
const float carEndY = 68.25;
const float carRedEndX = -25.0f;
const float carGreenEndX = -45.0f;
const float carBlueEndX = -66.5f;

struct CarAndDoorState MainStart = {
    carBeforeX, 0, 0, 0, 0, 0, 0
};

struct CarAndDoorState RedBeginLRDown = {
    carBeginX, carRedBeginY, 0, carLRMovePerFrame, 0, 0, 0
};

struct CarAndDoorState RedBeginLRUp = {
    carBeginX, carRedBeginY, 0, carLRHeight, 0, 0, 0
};

struct CarAndDoorState GreenBeginLRDown = {
    carBeginX, carGreenBeginY, 0, carLRMovePerFrame, 0, 0, 0
};
struct CarAndDoorState GreenBeginLRUp = {
    carBeginX, carGreenBeginY, 0, carLRHeight, 0, 0, 0
};
struct CarAndDoorState BlueBeginLRDown = {
    carBeginX, carBlueBeginY, 0, carLRMovePerFrame, 0, 0, 0
};
struct CarAndDoorState BlueBeginLRUp = {
    carBeginX, carBlueBeginY, 0, carLRHeight, 0, 0, 0
};
struct CarAndDoorState RedEndLRDown = {
    carRedEndX, carEndY, 90, carLRMovePerFrame, doorHeight, 0, 0
};
struct CarAndDoorState RedEndLRUp = {
    carRedEndX, carEndY, 90, carLRHeight, doorHeight, 0, 0
};
struct CarAndDoorState GreenEndLRDown = {
    carGreenEndX, carEndY, 90, carLRMovePerFrame, 0, doorHeight, 0
};
struct CarAndDoorState GreenEndLRUp = {
    carGreenEndX, carEndY, 90, carLRHeight, 0, doorHeight, 0
};
struct CarAndDoorState BlueEndLRDown = {
    carBlueEndX, carEndY, 90, carLRMovePerFrame, 0, 0, doorHeight
};
struct CarAndDoorState BlueEndLRUp = {
    carBlueEndX, carEndY, 90, carLRHeight, 0, 0, doorHeight
};


void sendCarAndDoorCtrl(struct CarAndDoorCtrl ctrl) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(CAR_AND_DOOR_CTRL_ADDRESS);
    sendChar(ctrl.x);
    sendChar(ctrl.y);
    sendChar(ctrl.tower);
    sendChar(ctrl.liftingRod);
    sendChar(ctrl.door1);
    sendChar(ctrl.door2);
    sendChar(ctrl.door3);
    if (ctrl.x == FORWARD) {
        CarAndDoorStateNow.carX += carXMovePerFrame;
    } else if (ctrl.x == BACKWARD) {
        CarAndDoorStateNow.carX -= carXMovePerFrame;
    }
    if (ctrl.y == LEFT) {
        CarAndDoorStateNow.carY -= carYMovePerFrame;
    } else if (ctrl.y == RIGHT) {
        CarAndDoorStateNow.carY += carYMovePerFrame;
    }
    if (ctrl.tower == TOWER_CLOCKWISE) {
        CarAndDoorStateNow.carAngle += carRotatePerFrame;
    } else if (ctrl.tower == TOWER_ANTICLOCKWISE) {
        CarAndDoorStateNow.carAngle -= carRotatePerFrame;
    }
    if (ctrl.liftingRod == UP) {
        CarAndDoorStateNow.carLiftingRodHeight += carLRMovePerFrame;
    } else if (ctrl.liftingRod == DOWN) {
        CarAndDoorStateNow.carLiftingRodHeight -= carLRMovePerFrame;
    }
    if (ctrl.door1 == DOOR_UP) {
        CarAndDoorStateNow.door1Height += doorMovePerFrame;
    } else if (ctrl.door1 == DOOR_DOWN) {
        CarAndDoorStateNow.door1Height -= doorMovePerFrame;
    }
    if (ctrl.door2 == DOOR_UP) {
        CarAndDoorStateNow.door2Height += doorMovePerFrame;
    } else if (ctrl.door2 == DOOR_DOWN) {
        CarAndDoorStateNow.door2Height -= doorMovePerFrame;
    }
    if (ctrl.door3 == DOOR_UP) {
        CarAndDoorStateNow.door3Height += doorMovePerFrame;
    } else if (ctrl.door3 == DOOR_DOWN) {
        CarAndDoorStateNow.door3Height -= doorMovePerFrame;
    }
}

const float sleep_time_normal = 3;

void carAndDoorTo(struct CarAndDoorState state) {
    struct CarAndDoorCtrl ctrl;
    while (sw(0)) {
        bool end = true;
        if (state.carX - CarAndDoorStateNow.carX >= carXMovePerFrame) {
            end = false;
            ctrl.x = FORWARD;
        } else if (CarAndDoorStateNow.carX - state.carX >= carXMovePerFrame) {
            end = false;
            ctrl.x = BACKWARD;
        } else {
            ctrl.x = NO_X_MOVE;
        }
        if (state.carY - CarAndDoorStateNow.carY >= carXMovePerFrame) {
            end = false;
            ctrl.y = RIGHT;
        } else if (CarAndDoorStateNow.carY - state.carY >= carXMovePerFrame) {
            end = false;
            ctrl.y = LEFT;
        } else {
            ctrl.y = NO_Y_MOVE;
        }
        if (state.carAngle - CarAndDoorStateNow.carAngle >= carRotatePerFrame) {
            end = false;
            ctrl.tower = TOWER_CLOCKWISE;
        } else if (CarAndDoorStateNow.carAngle - state.carAngle >= carRotatePerFrame) {
            end = false;
            ctrl.tower = TOWER_ANTICLOCKWISE;
        } else {
            ctrl.tower = NO_TOWER_ROTATE;
        }
        if (state.carLiftingRodHeight - CarAndDoorStateNow.carLiftingRodHeight >= carLRMovePerFrame) {
            end = false;
            ctrl.liftingRod = UP;
        } else if (CarAndDoorStateNow.carLiftingRodHeight - state.carLiftingRodHeight >= carLRMovePerFrame) {
            end = false;
            ctrl.liftingRod = DOWN;
        } else {
            ctrl.liftingRod = NO_LR_MOVE;
        }
        if (state.door1Height - CarAndDoorStateNow.door1Height >= doorMovePerFrame) {
            end = false;
            ctrl.door1 = DOOR_UP;
        } else if (CarAndDoorStateNow.door1Height - state.door1Height >= doorMovePerFrame) {
            end = false;
            ctrl.door1 = DOOR_DOWN;
        } else {
            ctrl.door1 = NO_DOOR_MOVE;
        }
        if (state.door2Height - CarAndDoorStateNow.door2Height >= doorMovePerFrame) {
            end = false;
            ctrl.door2 = DOOR_UP;
        } else if (CarAndDoorStateNow.door2Height - state.door2Height >= doorMovePerFrame) {
            end = false;
            ctrl.door2 = DOOR_DOWN;
        } else {
            ctrl.door2 = NO_DOOR_MOVE;
        }
        if (state.door3Height - CarAndDoorStateNow.door3Height >= doorMovePerFrame) {
            end = false;
            ctrl.door3 = DOOR_UP;
        } else if (CarAndDoorStateNow.door3Height - state.door3Height >= doorMovePerFrame) {
            end = false;
            ctrl.door3 = DOOR_DOWN;
        } else {
            ctrl.door3 = NO_DOOR_MOVE;
        }
        if (end) break;
        sendCarAndDoorCtrl(ctrl);
        sleep_ms(sleep_time_normal);
    }
}
void carTransform(enum ChestColor color) {
    struct CarAndDoorState target;
    switch (color) {
        case RED:
            target = RedBeginLRUp;
            break;
        case BLUE:
            target = BlueBeginLRUp;
            break;
        case GREEN:
            target = GreenBeginLRUp;
            break;
    }
    bool arrive = false;
    while (!arrive) {
        carAndDoorTo(getTarget(&arrive, target));
        sleep_ms(sleep_time_normal);
    }
    target = CarAndDoorStateNow;
    target.carLiftingRodHeight = 0;
    carAndDoorTo(target);
    sendSuckCtrl(NO_SUCK_ACTION, NO_SUCK_ACTION, DO_SUCK);
    sleep_ms(500);
    target.carLiftingRodHeight = carLRHeight;
    switch (color) {
        case RED:
            target = RedEndLRUp;
            break;
        case BLUE:
            target = BlueEndLRUp;
            break;
        case GREEN:
            target = GreenEndLRUp;
            break;
    }
    arrive = false;
    while (!arrive) {
        carAndDoorTo(getTarget(&arrive, target));
    }
    sendSuckCtrl(NO_SUCK_ACTION, NO_SUCK_ACTION, UNDO_SUCK);
    target = CarAndDoorStateNow;
    target.carY = carBeforeAndAfterY;
    carAndDoorTo(target);
    arrive = false;
    while (!arrive) {
        carAndDoorTo(getTarget(&arrive, MainStart));
    }
}

struct CarAndDoorState getTarget(bool *arrive, struct CarAndDoorState target) {
    struct CarAndDoorState result = CarAndDoorStateNow;
    result.carAngle = target.carAngle;
    result.door1Height = target.door1Height;
    result.door2Height = target.door2Height;
    result.door3Height = target.door3Height;
    result.carLiftingRodHeight = target.carLiftingRodHeight;
    switch (getCarPositionType(CarAndDoorStateNow)) {
        case RED_BEGIN:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                    *arrive = true;
                    return target;
                case GREEN_BEGIN:
                case BLUE_BEGIN:
                case BEFORE:
                case AFTER:
                case RED_END:
                case GREEN_END:
                case BLUE_END:
                    *arrive = false;
                    result.carX = carBeforeX;
                    return result;
            }
        case GREEN_BEGIN:
            switch (getCarPositionType(target)) {
                case GREEN_BEGIN:
                    *arrive = true;
                    return target;
                case RED_BEGIN:
                case BLUE_BEGIN:
                case BEFORE:
                case AFTER:
                case RED_END:
                case GREEN_END:
                case BLUE_END:
                    *arrive = false;
                    result.carX = carBeforeX;
                    return result;
            }
        case BLUE_BEGIN:
            switch (getCarPositionType(target)) {
                case BLUE_BEGIN:
                    *arrive = true;
                    return target;
                case RED_BEGIN:
                case GREEN_BEGIN:
                case BEFORE:
                case AFTER:
                case RED_END:
                case GREEN_END:
                case BLUE_END:
                    *arrive = false;
                    result.carX = carBeforeX;
                    return result;
            }
        case BEFORE:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                    *arrive = false;
                    if (fabs(result.carY - carRedBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carRedBeginY;
                    return result;
                case GREEN_BEGIN:
                    *arrive = false;
                    if (fabs(result.carY - carGreenBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carGreenBeginY;
                    return result;
                case BLUE_BEGIN:
                    *arrive = false;
                    if (fabs(result.carY - carBlueBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carBlueBeginY;
                    return result;
                case RED_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carRedEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case GREEN_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carGreenEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BLUE_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carBlueEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BEFORE:
                    *arrive = true;
                    return target;
                case AFTER:
                    break;
            }
            break;
        case AFTER:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                case GREEN_BEGIN:
                case BLUE_BEGIN:
                case BEFORE:
                    if (fabs(CarAndDoorStateNow.carX - target.carX) < carXMovePerFrame) {
                        result.carY = carBlueBeginY;
                        return result;
                    }
                    result.carX = carBeforeX;
                    return result;
                case RED_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (fabs(result.carX - carRedEndX) < carXMovePerFrame) {
                            result.carY = carEndY;
                            return result;
                        }
                        result.carX = carRedEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case GREEN_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (fabs(result.carX - carGreenEndX) < carXMovePerFrame) {
                            result.carY = carEndY;
                            return result;
                        }
                        result.carX = carGreenEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BLUE_END:
                    *arrive = false;
                    if (fabs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (fabs(result.carX - carBlueEndX) < carXMovePerFrame) {
                            result.carY = carEndY;
                            return result;
                        }
                        result.carX = carBlueEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case AFTER:
                    break;
            }
        case RED_END:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                    *arrive = true;
                    return result;
                case GREEN_BEGIN:
                    break;
                case BLUE_BEGIN:
                    break;
                case BEFORE:
                    break;
                case AFTER:
                    break;
                case RED_END:
                    *arrive = true;
                    return result;
                case GREEN_END:
                    break;
                case BLUE_END:
                    break;
            }
            break;
        case GREEN_END:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                    *arrive = true;
                    return result;
                case GREEN_BEGIN:
                    break;
                case BLUE_BEGIN:
                    break;
                case BEFORE:
                    break;
                case AFTER:
                    break;
                case RED_END:
                    break;
                case GREEN_END:
                    *arrive = true;
                    return result;
                case BLUE_END:
                    break;
            }
            break;
        case BLUE_END:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                    *arrive = true;
                    return result;
                case GREEN_BEGIN:
                    break;
                case BLUE_BEGIN:
                    break;
                case BEFORE:
                    break;
                case AFTER:
                    break;
                case RED_END:
                    break;
                case GREEN_END:
                    break;
                case BLUE_END:
                    *arrive = true;
                    return result;
            }
            break;
    }
}

enum CarPositionType getCarPositionType(const struct CarAndDoorState state) {
    if (state.carX > -8) {
        // 小车在起始位置
        if (state.carY > 14.55) {
            return BLUE_BEGIN;
        }
        if (state.carY > -1.68) {
            return GREEN_BEGIN;
        }
        return RED_BEGIN;
    }
    if (state.carX > -12) {
        return BEFORE;
    }
    if (state.carY < 38.78) {
        return AFTER;
    }
    if (state.carX > -35) {
        return RED_END;
    }
    if (state.carX > -56.25) {
        return GREEN_END;
    }
    return BLUE_END;
}

struct CarAndDoorCtrl defaultCarAndDoorCtrl() {
    const struct CarAndDoorCtrl ctrl = {
        NO_X_MOVE, NO_Y_MOVE, NO_TOWER_ROTATE, NO_LR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE
    };
    return ctrl;
}
