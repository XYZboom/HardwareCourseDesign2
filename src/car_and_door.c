//
// Created by lenovo on 2023/12/30.
//

#include "car_and_door.h"

#include "time_utils.h"

#include "constants.h"
#include "uart.h"

struct CarAndDoorState CarAndDoorStateNow;

const float doorMovePerFrame = 0.5f;
const float carXMovePerFrame = 1.0f;
const float carYMovePerFrame = 1.0f;
const float carLRMovePerFrame = 1.0f;
const float carRotatePerFrame = 5.0f;

const float carBeforeX = -10.0f;
const float carBeginX = 2.6f;
const float carRedBeginY = -19.9f;
const float carGreenBeginY = 3.15f;
const float carBlueBeginY = 25.9f;
const float carBeforeAndAfterY = 32.9f;
const float carEndY = 68.25;
const float carRedEndX = -25.0f;
const float carGreenEndX = -45.0f;
const float carBlueEndX = -66.5f;

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

void carAndDoorTo(struct CarAndDoorState state) {
    struct CarAndDoorCtrl ctrl;
    while (true) {
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
        float *doorHeight = &state.door1Height;
        float *doorHeightNow = &CarAndDoorStateNow.door1Height;
        enum DoorCtrl *doorCtrl = &ctrl.door1;
        for (int i = 0; i < 3; ++i) {
            if (doorHeight[i] - doorHeightNow[i] >= doorMovePerFrame) {
                end = false;
                doorCtrl[i] = DOOR_UP;
            } else if (doorHeightNow[i] - doorHeight[i] >= doorMovePerFrame) {
                end = false;
                doorCtrl[i] = DOOR_DOWN;
            } else {
                doorCtrl[i] = NO_DOOR_MOVE;
            }
        }
        if (end) break;
        sendCarAndDoorCtrl(ctrl);
        sleep_ms(80);
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
                    if (abs(result.carY - carRedBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carRedBeginY;
                    return result;
                case GREEN_BEGIN:
                    *arrive = false;
                    if (abs(result.carY - carGreenBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carGreenBeginY;
                    return result;
                case BLUE_BEGIN:
                    *arrive = false;
                    if (abs(result.carY - carBlueBeginY) < carYMovePerFrame) {
                        result.carX = carBeginX;
                        return result;
                    }
                    result.carY = carBlueBeginY;
                    return result;
                case RED_END:
                    *arrive = false;
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carRedEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case GREEN_END:
                    *arrive = false;
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carGreenEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BLUE_END:
                    *arrive = false;
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        result.carX = carBlueEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BEFORE:
                case AFTER:
                    break;
            }
            break;
        case AFTER:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
                case GREEN_BEGIN:
                case BLUE_BEGIN:
                    if (abs(CarAndDoorStateNow.carX - target.carX) < carXMovePerFrame) {
                        result.carX = carBeforeX;
                        return result;
                    }
                    result.carY = carBlueBeginY;
                case RED_END:
                    *arrive = false;
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (abs(result.carX - carRedEndX) < carXMovePerFrame) {
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
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (abs(result.carX - carRedEndX) < carXMovePerFrame) {
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
                    if (abs(result.carY - carBeforeAndAfterY) < carYMovePerFrame) {
                        if (abs(result.carX - carRedEndX) < carXMovePerFrame) {
                            result.carY = carEndY;
                            return result;
                        }
                        result.carX = carBlueEndX;
                        return result;
                    }
                    result.carY = carBeforeAndAfterY;
                    return result;
                case BEFORE:
                    break;
                case AFTER:
                    break;
            }
        case RED_END:
            switch (getCarPositionType(target)) {
                case RED_BEGIN:
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
                    break;
            }
            break;
        case GREEN_END:
            break;
        case BLUE_END:
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
