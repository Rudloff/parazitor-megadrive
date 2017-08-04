#include <genesis.h>
#include "res/images.h"
#include "types.h"

/*@only@*/ static Sprite *headSprite;
/*@only@*/ static Sprite *tailSprite;
/*@only@*/ static Sprite *bodySprite;

static s16 posX = 0;
static s16 posY = 0;
static s16 bodyPosX = 0;
static s16 bodyPosY = 0;
static s16 tailPosX = 0;
static s16 tailPosY = 0;
static u16 direction = BUTTON_DOWN;
static int gameStarted = FALSE;
static s16 coeff = 13;

static void initGame()
{
    VDP_clearPlan(PLAN_A, TRUE);
    VDP_setPalette(PAL1, head.palette->data);
    headSprite = SPR_addSprite(&head, 0, 0, (u16) TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    SPR_setPosition(headSprite, posX, posY);
    SPR_setVFlip(headSprite, TRUE);

    bodySprite = SPR_addSprite(&body, 0, 0, (u16) TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    SPR_setPosition(bodySprite, posX, posY);

    tailSprite = SPR_addSprite(&tail, 0, 0, (u16) TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    SPR_setPosition(tailSprite, posX, posY);

    gameStarted = TRUE;
}

static void joyHandler(u16 joy, /*@unused@*/ u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if (state == BUTTON_START && gameStarted == FALSE)
        {
            initGame();
        }
        if (state == BUTTON_RIGHT || state == BUTTON_LEFT || state == BUTTON_DOWN || state == BUTTON_UP) {
            direction = state;
        }
    }
}

static double atan(double x)
{
    return x - (x*x*x)/3 + (x*x*x*x*x)/5 - (x*x*x*x*x*x*x)/7;
}

static void moveBody()
{
    s16 lastMoveX = posX - bodyPosX;
    s16 lastMoveY = posY - bodyPosY;
    s16 lastMoveXabs = abs(lastMoveX);
    s16 lastMoveYabs = abs(lastMoveY);
    double thisRotation;

    if (lastMoveY == 0) {
        if (lastMoveX > 0) {
            thisRotation = 1.5707963268;
        } else {
            thisRotation = 4.7123889804;
        }
    } else if (lastMoveX == 0) {
        if (lastMoveY > 0) {
            thisRotation = 3.1415926536;
        } else {
            thisRotation = 0;
        }
    } else {
        float mySin = (float) lastMoveX / lastMoveY;
        thisRotation = atan(mySin);
        thisRotation = -thisRotation;
        if (lastMoveY > 0) {
            thisRotation -= PI;
        }
    }

    SPR_setVFlip(bodySprite, FALSE);
    SPR_setHFlip(bodySprite, FALSE);
    if (thisRotation > 5.4977871438 || thisRotation < 0.7853981634) {
        SPR_setFrame(bodySprite, 0);
    } else if (thisRotation > 0.7853981634 && thisRotation < 2.3561944902) {
        SPR_setFrame(bodySprite, 1);
    } else if (thisRotation > 2.3561944902 && thisRotation < 3.926990817) {
        SPR_setFrame(bodySprite, 0);
        SPR_setVFlip(bodySprite, TRUE);
    } else {
        SPR_setFrame(bodySprite, 1);
        SPR_setHFlip(bodySprite, TRUE);
    }

    if (lastMoveXabs > lastMoveYabs) {
        s16 ratio = lastMoveY  / lastMoveX;
        if (lastMoveX > coeff) {
            bodyPosX = posX - coeff;
            bodyPosY = posY - coeff * ratio;
        } else {
            if (lastMoveX < -coeff) {
                bodyPosX = posX + coeff;
                bodyPosY = posY + coeff * ratio;
            }
        }
    } else {
        if (lastMoveYabs > lastMoveXabs) {
            int ratio = lastMoveX  / lastMoveY;
            if (lastMoveY > coeff) {
                bodyPosY = posY - coeff;
                bodyPosX = (s16) (posX - coeff * ratio);
            } else {
                if (lastMoveY < -coeff) {
                    bodyPosY = posY + coeff;
                    bodyPosX = (s16) (posX + coeff * ratio);
                }
            }
        }
    }

    SPR_setPosition(bodySprite, bodyPosX, bodyPosY);
}

static void moveHead(int speed)
{
    SPR_setVFlip(headSprite, FALSE);
    SPR_setHFlip(headSprite, FALSE);
    if (direction == BUTTON_RIGHT) {
        SPR_setFrame(headSprite, 1);
        posX += speed;
    } else if (direction == BUTTON_LEFT) {
        SPR_setHFlip(headSprite, TRUE);
        SPR_setFrame(headSprite, 1);
        posX -= speed;
    } else if (direction == BUTTON_DOWN) {
        SPR_setVFlip(headSprite, TRUE);
        SPR_setFrame(headSprite, 0);
        posY += speed;
    }  else if (direction == BUTTON_UP) {
        SPR_setFrame(headSprite, 0);
        posY -= speed;
    }

    SPR_setPosition(headSprite, posX, posY);
}

static void moveTail()
{
    s16 lastMoveX = bodyPosX - tailPosX;
    s16 lastMoveY = bodyPosY - tailPosY;
    s16 lastMoveXabs = abs(lastMoveX);
    s16 lastMoveYabs = abs(lastMoveY);
    double thisRotation;

    if (lastMoveY == 0) {
        if (lastMoveX > 0) {
            thisRotation = 1.5707963268;
        } else {
            thisRotation = 4.7123889804;
        }
    } else if (lastMoveX == 0) {
        if (lastMoveY > 0) {
            thisRotation = 3.1415926536;
        } else {
            thisRotation = 0;
        }
    } else {
        float mySin = (float) lastMoveX / lastMoveY;
        thisRotation = atan(mySin);
        thisRotation = -thisRotation;
        if (lastMoveY > 0) {
            thisRotation -= PI;
        }
    }

    SPR_setVFlip(tailSprite, FALSE);
    SPR_setHFlip(tailSprite, FALSE);
    if (thisRotation > 5.4977871438 || thisRotation < 0.7853981634) {
        SPR_setFrame(tailSprite, 0);
    } else if (thisRotation > 0.7853981634 && thisRotation < 2.3561944902) {
        SPR_setFrame(tailSprite, 1);
    } else if (thisRotation > 2.3561944902 && thisRotation < 3.926990817) {
        SPR_setFrame(tailSprite, 0);
        SPR_setVFlip(tailSprite, TRUE);
    } else {
        SPR_setFrame(tailSprite, 1);
        SPR_setHFlip(tailSprite, TRUE);
    }

    if (lastMoveXabs > lastMoveYabs) {
        s16 ratio = lastMoveY  / lastMoveX;
        if (lastMoveX > coeff) {
            tailPosX = bodyPosX - coeff;
            tailPosY = bodyPosY - coeff * ratio;
        } else {
            if (lastMoveX < -coeff) {
                tailPosX = bodyPosX + coeff;
                tailPosY = bodyPosY + coeff * ratio;
            }
        }
    } else {
        if (lastMoveYabs > lastMoveXabs) {
            int ratio = lastMoveX  / lastMoveY;
            if (lastMoveY > coeff) {
                tailPosY = bodyPosY - coeff;
                tailPosX = (s16) (bodyPosX - coeff * ratio);
            } else {
                if (lastMoveY < -coeff) {
                    tailPosY = bodyPosY + coeff;
                    tailPosX = (s16) (bodyPosX + coeff * ratio);
                }
            }
        }
    }

    SPR_setPosition(tailSprite, tailPosX, tailPosY);
}

static void moveSprites(int speed)
{
    moveHead(speed);
    moveBody();
    moveTail();
}

int main()
{
    (void) VDP_drawImage(PLAN_A, &title, 0, 0);
    VDP_drawTextBG(PLAN_A, "Press start", 14, 25);
    VDP_setBackgroundColor((u8) 8);

    JOY_init();
    JOY_setEventHandler(&joyHandler);

    SPR_init(0, 0, 0);

    while(TRUE == TRUE)
    {
        if (gameStarted == TRUE) {
            moveSprites(3);
            SPR_update();
        }
        VDP_waitVSync();
    }

    return 0;
}
