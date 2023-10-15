#include "Player.h"

ObjectPlayer *Player;

bool32 Player_HandleGroundMovement_Hook(bool32 skipped)
{
    RSDK_THIS(Player);

    if (self->controlLock > 0) {
        self->controlLock--;

        if (!self->invertGravity)
            self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;
    }
    else {
        bool32 left  = false;
        bool32 right = false;

        if (self->invertGravity) {
            right = self->left;
            left  = self->right;
        }
        else {
            left  = self->left;
            right = self->right;
        }

        if (left) {
            if (self->groundVel > -self->topSpeed) {
                if (self->groundVel <= 0) {
                    self->groundVel -= self->acceleration;
                }
                else {
                    if (!self->collisionMode && self->groundVel > 0x40000 && !Zone->autoScrollSpeed) {
                        self->direction = FLIP_NONE;
                        self->skidding  = 24;
                    }

                    if (self->groundVel < self->skidSpeed)
                        self->groundVel = -abs(self->skidSpeed);
                    else
                        self->groundVel -= self->skidSpeed;
                }
            }

            if (self->groundVel <= 0 && self->skidding < 1)
                self->direction = FLIP_X;
        }

        if (right) {
            if (self->groundVel < self->topSpeed) {
                if (self->groundVel >= 0) {
                    self->groundVel += self->acceleration;
                }
                else {
                    if (!self->collisionMode && self->groundVel < -0x40000 && !Zone->autoScrollSpeed) {
                        self->direction = FLIP_X;
                        self->skidding  = 24;
                    }

                    if (self->groundVel > -self->skidSpeed)
                        self->groundVel = abs(self->skidSpeed);
                    else
                        self->groundVel += self->skidSpeed;
                }
            }

            if (self->groundVel >= 0 && self->skidding < 1)
                self->direction = FLIP_NONE;
        }

        if (self->left || self->right) {
            if (!self->invertGravity) {
                self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;

                if (self->right) {
                    if (!self->left) {
                        if (self->angle > 0xC0 && self->angle < 0xE4 && self->groundVel > -0x20000 && self->groundVel < 0x28000) {
                            self->controlLock = 30;
                        }
                    }
                }
                else if (self->left) {
                    if (self->angle > 0x1C && self->angle < 0x40 && self->groundVel > -0x28000 && self->groundVel < 0x20000) {
                        self->controlLock = 30;
                    }
                }
            }
        }
        else {
            if (self->groundVel <= 0) {
                self->groundVel += self->deceleration;

                if (self->groundVel > 0)
                    self->groundVel = 0;
            }
            else {
                self->groundVel -= self->deceleration;

                if (self->groundVel < 0)
                    self->groundVel = 0;
            }

            if (!self->invertGravity) {
                if (self->groundVel > 0x2000 || self->groundVel < -0x2000)
                    self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;

                if (self->angle > 0xC0 && self->angle < 0xE4) {
                    if (abs(self->groundVel) < 0x10000)
                        self->controlLock = 30;
                }

                if (self->angle > 0x1C && self->angle < 0x40) {
                    if (abs(self->groundVel) < 0x10000)
                        self->controlLock = 30;
                }
            }
        }
    }

    if (self->left && self->groundVel < -self->topSpeed) {
    	self->groundVel = self->topSpeed;
    }

    if (self->right && self->groundVel > self->topSpeed) {
    	self->groundVel = self->topSpeed;
    }

    if (!self->invertGravity && self->collisionMode != CMODE_FLOOR && self->collisionMode <= CMODE_RWALL) {
        if (self->angle >= 0x40 && self->angle <= 0xC0 && abs(self->groundVel) < 0x20000) {
            self->velocity.x    = self->groundVel * RSDK.Cos256(self->angle) >> 8;
            self->velocity.y    = self->groundVel * RSDK.Sin256(self->angle) >> 8;
            self->onGround      = false;
            self->angle         = 0;
            self->collisionMode = CMODE_FLOOR;
        }
    }
}
bool32 Player_HandleAirFriction_Hook(bool32 skipped)
{
    RSDK_THIS(Player);

    if (self->velocity.y > -0x40000 && self->velocity.y < 0)
        self->velocity.x -= self->velocity.x >> 5;

    if (self->left) {
        if (self->velocity.x > -self->topSpeed)
            self->velocity.x -= self->airAcceleration;

        self->direction = FLIP_X;
    }

    if (self->right) {
        if (self->velocity.x < self->topSpeed)
            self->velocity.x += self->airAcceleration;

        self->direction = FLIP_NONE;
    }

    if (self->left && self->groundVel < -self->topSpeed) {
    	self->groundVel = self->topSpeed;
    }

    if (self->right && self->groundVel > self->topSpeed) {
    	self->groundVel = self->topSpeed;
    }
}
