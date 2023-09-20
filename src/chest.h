//
// Created by XYZboom on 2023/9/20.
//

#ifndef DESIGN2_PROJECT_CHEST_H
#define DESIGN2_PROJECT_CHEST_H

#define CHEST_CTRL_ADDRESS ((unsigned char) 0x34)
enum ChestCtrl {
	NO_CHEST = 0x30,
	OUT_CHEST
};

void sendChestCtrl(enum ChestCtrl ctrl1, enum ChestCtrl ctrl2, enum ChestCtrl ctrl3);
#endif //DESIGN2_PROJECT_CHEST_H
