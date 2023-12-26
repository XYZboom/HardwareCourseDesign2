//
// Created by XYZboom on 2023/9/19.
//

#ifndef DESIGN2_PROJECT_MECHANICAL_ARM_H
#define DESIGN2_PROJECT_MECHANICAL_ARM_H

#define AXLE_NUM 6
enum ArmAddress {
	ARM_1_ADDRESS = 0x31,
	ARM_2_ADDRESS,
	ARM_SUCK_ADDRESS = 0x35
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
	enum ArmRotateCtrl rotateCtrl[AXLE_NUM];
	enum ArmMoveCtrl moveCtrl;
};
enum ArmSuckCtrl {
	NO_SUCK_ACTION = 0x30,
	DO_SUCK,
	UNDO_SUCK
};

void sendArmCtrl(struct ArmCtrl ctrl);

struct ArmState {
	int degree[AXLE_NUM];
	int pos;
};

void armTo(enum ArmAddress address, struct ArmState target);

void arm1Up();

void arm1Left();

void sendSuckCtrl(enum ArmSuckCtrl ctrl1, enum ArmSuckCtrl ctrl2, enum ArmSuckCtrl ctrl3);

#endif //DESIGN2_PROJECT_MECHANICAL_ARM_H
