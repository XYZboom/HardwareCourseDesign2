//
// Created by XYZboom on 2023/9/19.
//

#ifndef DESIGN2_PROJECT_MECHANICAL_ARM_H
#define DESIGN2_PROJECT_MECHANICAL_ARM_H

enum ArmAddress {
	ARM_1_ADDRESS = 0x31,
	ARM_2_ADDRESS
};

enum ArmRotateCtrl {
	NO_ROTATE = 0x30,
	CLOCKWISE_1_DEGREE,
	CLOCKWISE_2_DEGREE,
	CLOCKWISE_3_DEGREE,
	CLOCKWISE_5_DEGREE,
	ANTICLOCKWISE_1_DEGREE,
	ANTICLOCKWISE_2_DEGREE,
	ANTICLOCKWISE_3_DEGREE,
	ANTICLOCKWISE_5_DEGREE,
};
enum ArmMoveCtrl {
	NO_MOVE = 0x30,
	LEFT_SLOWEST,
	LEFT_MIDDLE,
	LEFT_FASTEST,
	RIGHT_SLOWEST,
	RIGHT_MIDDLE,
	RIGHT_FASTEST,
};
struct ArmCtrl {
	// 机械臂的地址
	enum ArmAddress address;
	// 机械臂底座对应轴旋转
	enum ArmRotateCtrl ctrl1;
	enum ArmRotateCtrl ctrl2;
	enum ArmRotateCtrl ctrl3;
	enum ArmRotateCtrl ctrl4;
	enum ArmRotateCtrl ctrl5;
	enum ArmRotateCtrl ctrl6;
	enum ArmMoveCtrl moveCtrl;
};

void sendArmCtrl(struct ArmCtrl ctrl);


#endif //DESIGN2_PROJECT_MECHANICAL_ARM_H
