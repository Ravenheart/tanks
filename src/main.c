#include <genesis.h>
#include <resources.h>
#include <maths.h>

#define TOP_EDGE 0
#define BOTTOM_EDGE 224
#define LEFT_EDGE 0
#define RIGHT_EDGE 320

#define TURRET_UP BUTTON_Y
#define TURRET_DOWN BUTTON_A
#define TURRET_LEFT BUTTON_X
#define TURRET_RIGHT BUTTON_B

#define SPEED 1

typedef struct
{
    Vect2D_s32 pos;
    Vect2D_s16 body_dir;
    int width;
    int height;

    Sprite *body90;
    Sprite *body0;

    Sprite *turret90;
    Sprite *turret0;
} Tank;

Tank tank;
void updateTank();
void drawBackground();

int main()
{
    VDP_init();
    SPR_init();
    JOY_init();
    JOY_setSupport(JOY_1, JOY_SUPPORT_6BTN);

    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    PAL_setPalette(PAL1, tank_body_90.palette->data);

    drawBackground();

    tank.height = 16;
    tank.width = 16;
    tank.pos.x = (RIGHT_EDGE / 2) - (tank.width / 2);
    tank.pos.y = (BOTTOM_EDGE / 2) - (tank.height / 2);
    tank.body_dir.x = 0;
    tank.body_dir.y = 0;

    tank.turret90 = SPR_addSprite(&tank_turret_90, tank.pos.x, tank.pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    tank.turret0 = SPR_addSprite(&tank_turret_0, tank.pos.x, tank.pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    tank.body90 = SPR_addSprite(&tank_body_90, tank.pos.x, tank.pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    tank.body0 = SPR_addSprite(&tank_body_0, tank.pos.x, tank.pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    SPR_setVisibility(tank.body0, HIDDEN);
    SPR_setVisibility(tank.turret0, HIDDEN);

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
    tank.body_dir.x = 0;
    tank.body_dir.y = 0;
    u16 state = JOY_readJoypad(JOY_1);
    if (state & BUTTON_UP)
    {
        tank.body_dir.y = -1;
    }
    else if (state & BUTTON_DOWN)
    {
        tank.body_dir.y = 1;
    }
    else if (state & BUTTON_LEFT)
    {
        tank.body_dir.x = -1;
    }
    else if (state & BUTTON_RIGHT)
    {
        tank.body_dir.x = 1;
    }

    tank.pos.x = tank.pos.x + (tank.body_dir.x * SPEED);
    tank.pos.y = tank.pos.y + (tank.body_dir.y * SPEED);
    if (tank.pos.x + tank.width > RIGHT_EDGE)
    {
        tank.pos.x = RIGHT_EDGE - tank.width;
    }
    else if (tank.pos.x < LEFT_EDGE)
    {
        tank.pos.x = LEFT_EDGE;
    }

    if (tank.pos.y + tank.height > BOTTOM_EDGE)
    {
        tank.pos.y = BOTTOM_EDGE - tank.height;
    }
    else if (tank.pos.y < TOP_EDGE)
    {
        tank.pos.y = TOP_EDGE;
    }

    if (tank.body_dir.x > 0)
    {
        SPR_setPosition(tank.body0, tank.pos.x, tank.pos.y);
        SPR_setHFlip(tank.body0, FALSE);
        SPR_setVisibility(tank.body0, VISIBLE);
        SPR_setVisibility(tank.body90, HIDDEN);
    }
    else if (tank.body_dir.x < 0)
    {
        SPR_setPosition(tank.body0, tank.pos.x, tank.pos.y);
        SPR_setHFlip(tank.body0, TRUE);
        SPR_setVisibility(tank.body0, VISIBLE);
        SPR_setVisibility(tank.body90, HIDDEN);
    }
    else if (tank.body_dir.y > 0)
    {
        SPR_setPosition(tank.body90, tank.pos.x, tank.pos.y);
        SPR_setVFlip(tank.body90, TRUE);
        SPR_setVisibility(tank.body0, HIDDEN);
        SPR_setVisibility(tank.body90, VISIBLE);
    }
    else if (tank.body_dir.y < 0)
    {
        SPR_setPosition(tank.body90, tank.pos.x, tank.pos.y);
        SPR_setVFlip(tank.body90, FALSE);
        SPR_setVisibility(tank.body0, HIDDEN);
        SPR_setVisibility(tank.body90, VISIBLE);
    }

    //turret
    //RIGHT
    if (state & BUTTON_B)
    {
        SPR_setHFlip(tank.turret0, FALSE);
        SPR_setVisibility(tank.turret0, VISIBLE);
        SPR_setVisibility(tank.turret90, HIDDEN);
    }
    //LEFT
    else if (state & BUTTON_X)
    {
        SPR_setHFlip(tank.turret0, TRUE);
        SPR_setVisibility(tank.turret0, VISIBLE);
        SPR_setVisibility(tank.turret90, HIDDEN);
    }
    //DOWN
    else if (state & BUTTON_A)
    {
        SPR_setVFlip(tank.turret90, TRUE);
        SPR_setVisibility(tank.turret0, HIDDEN);
        SPR_setVisibility(tank.turret90, VISIBLE);
    }
    //UP
    else if (state & BUTTON_Y)
    {
        SPR_setVFlip(tank.turret90, FALSE);
        SPR_setVisibility(tank.turret0, HIDDEN);
        SPR_setVisibility(tank.turret90, VISIBLE);
    }

    SPR_setPosition(tank.turret0, tank.pos.x, tank.pos.y);
    SPR_setPosition(tank.turret90, tank.pos.x, tank.pos.y);
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