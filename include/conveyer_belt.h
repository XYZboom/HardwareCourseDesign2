//
// Created by lenovo on 2023/12/30.
//

#ifndef CONVEYER_BELT_H
#define CONVEYER_BELT_H

#define CONVEYER_BELT_ADDRESS ((unsigned char) 0x36)
enum ConveyerBeltCtrl {
    NO_BELT = 0x30,
    OPEN_BELT,
    CLOSE_BELT
};

void sendConveyerBeltCtrl(enum ConveyerBeltCtrl ctrl2, enum ConveyerBeltCtrl ctrl3);

#endif //CONVEYER_BELT_H
