#include <genesis.h>
#include <resources.h>
#include <maths.h>

#define TOP_EDGE 0
#define BOTTOM_EDGE 224
#define LEFT_EDGE 0
#define RIGHT_EDGE 320
#define SPEED 1

typedef struct
{
    fix16 x;
    fix16 y;
    fix16 vel_x;
    fix16 vel_y;
    int width;
    int height;

    int turret_rotation;
    Sprite *_body90;
    Sprite *_body0;

    Sprite *turret;
} Tank;

Tank tank;
void updateTank();
void drawBackground();

int main()
{
    VDP_init();
    SPR_init();
    JOY_init();

    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    PAL_setPalette(PAL1, tank_body_90.palette->data);

    drawBackground();

    tank.height = 16;
    tank.width = 16;
    tank.x = intToFix16((RIGHT_EDGE / 2) - (tank.width / 2));
    tank.y = intToFix16((BOTTOM_EDGE / 2) - (tank.height / 2));
    tank.vel_x = FIX16(0);
    tank.vel_y = FIX16(0);
    tank.turret_rotation = 0;

    tank.turret = SPR_addSprite(&tank_turret_90, fix16ToInt(tank.x), fix16ToInt(tank.y), TILE_ATTR(PAL1, 0, FALSE, FALSE));
    tank._body90 = SPR_addSprite(&tank_body_90, fix16ToInt(tank.x), fix16ToInt(tank.y), TILE_ATTR(PAL1, 0, FALSE, FALSE));
    tank._body0 = SPR_addSprite(&tank_body_0, fix16ToInt(tank.x), fix16ToInt(tank.y), TILE_ATTR(PAL1, 0, FALSE, FALSE));
    SPR_setVisibility(tank._body0, HIDDEN);

    SPR_update();

    while (1)
    {
        updateTank();
        SPR_update();
        VDP_waitVSync();
    }

    return (0);
}

void updateTank()
{
    u16 state = JOY_readJoypad(JOY_1);
    if (state & BUTTON_UP)
    {
        tank.vel_y = fix16Neg(SPEED);
    }
    else if (state & BUTTON_DOWN)
    {
        tank.vel_y = FIX16(SPEED);
    }
    else
    {
        tank.vel_y = FIX16(0);
    }

    if (state & BUTTON_LEFT)
    {
        tank.vel_x = fix16Neg(SPEED);
    }
    else if (state & BUTTON_RIGHT)
    {
        tank.vel_x = FIX16(SPEED);
    }
    else
    {
        tank.vel_x = FIX16(0);
    }

    tank.x = fix16Add(tank.x, tank.vel_x);
    tank.y = fix16Add(tank.y, tank.vel_y);
    if (fix16ToInt(tank.x) + tank.width > RIGHT_EDGE)
    {
        tank.x = intToFix16(RIGHT_EDGE - tank.width);
    }
    else if (fix16ToInt(tank.x) < LEFT_EDGE)
    {
        tank.x = FIX16(LEFT_EDGE);
    }

    if (fix16ToInt(tank.y) + tank.height > BOTTOM_EDGE)
    {
        tank.y = intToFix16(BOTTOM_EDGE - tank.height);
    }
    else if (fix16ToInt(tank.y) < TOP_EDGE)
    {
        tank.y = FIX16(TOP_EDGE);
    }

    if (fix16ToInt(tank.vel_x) > 0)
    {
        SPR_setPosition(tank._body0, fix16ToInt(tank.x), fix16ToInt(tank.y));
        SPR_setHFlip(tank._body0, FALSE);
        SPR_setVisibility(tank._body0, VISIBLE);
        SPR_setVisibility(tank._body90, HIDDEN);
    }
    else if (fix16ToInt(tank.vel_x) < 0)
    {
        SPR_setPosition(tank._body0, fix16ToInt(tank.x), fix16ToInt(tank.y));
        SPR_setHFlip(tank._body0, TRUE);
        SPR_setVisibility(tank._body0, VISIBLE);
        SPR_setVisibility(tank._body90, HIDDEN);
    }
    else if (fix16ToInt(tank.vel_y) > 0)
    {
        SPR_setPosition(tank._body90, fix16ToInt(tank.x), fix16ToInt(tank.y));
        SPR_setVFlip(tank._body90, TRUE);
        SPR_setVisibility(tank._body0, HIDDEN);
        SPR_setVisibility(tank._body90, VISIBLE);
    }
    else if (fix16ToInt(tank.vel_y) < 0)
    {
        SPR_setPosition(tank._body90, fix16ToInt(tank.x), fix16ToInt(tank.y));
        SPR_setVFlip(tank._body90, FALSE);
        SPR_setVisibility(tank._body0, HIDDEN);
        SPR_setVisibility(tank._body90, VISIBLE);
    }

    SPR_setPosition(tank.turret, fix16ToInt(tank.x), fix16ToInt(tank.y));
}

void drawBackground()
{
    VDP_loadTileSet(grass1.tileset, 1, DMA);
    VDP_loadTileSet(dirt1.tileset, 2, DMA);

    for (int c = 0; c < 40; c++)
        for (int r = 0; r < 28; r++)
        {
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), c, r);
        }

    //VDP_setTileMap(BG_B, )
}