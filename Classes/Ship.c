
#include "Ship.h"

// Ship class methods
void initShip(struct Ship *p, UBYTE id, UBYTE palette)
{
    p->isAlive = true;
    p->x = 0;
    p->y = 0;
    p->palette = palette;
    p->id1 = id;
    p->id2 = id + 1;

    p->hspeed = 0;
    p->vspeed = 0;

    set_sprite_tile(p->id1, 0);
    set_sprite_tile(p->id2, 0);

    set_sprite_prop(p->id1, p->palette);
    set_sprite_prop(p->id2, p->palette | S_FLIPX);
}

void moveShip(struct Ship *p, UBYTE x, UBYTE y)
{
    p->x = x;
    p->y = y;
    move_sprite(p->id1, p->x, p->y);
    move_sprite(p->id2, p->x + 8, p->y);
}

void scrollShip(struct Ship *p, int8_t x, int8_t y)
{
    if (p->isAlive)
    {
        p->x += x;
        p->y += y;
        moveShip(p, p->x, p->y);
    }
}

void updateShip(struct Ship *p)
{
    set_sprite_prop(p->id1, p->palette);
    set_sprite_prop(p->id2, p->palette | S_FLIPX);

    if (!p->isAlive)
        moveShip(p, 0, 0);
    else
    {
        if (p->x - p->hspeed < 5){
            p->hspeed = -p->hspeed;
            moveShip(p, 5, p->y);

        }
        else if (p->x + p->hspeed >= PLAYABLEWIDTH){
            p->hspeed = -p->hspeed;
            moveShip(p, PLAYABLEWIDTH, p->y);
        }
        scrollShip(p, p->hspeed, p->vspeed);
    }
}

void changeSpriteShip(struct Ship *p, UBYTE sprite)
{
    set_sprite_tile(p->id1, sprite);
    set_sprite_tile(p->id2, sprite);
}