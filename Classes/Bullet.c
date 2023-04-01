// #include <gb/gb.h>
// #include <stdbool.h>
#include "Ship.h"
#include "Bullet.h"

UBYTE playerbulletsOnScreen = 0;

// Bullet methods
void initBullet(struct Bullet *b, struct Ship *shooter, UBYTE id, UBYTE palette)
{
    b->x = shooter->x + 4;
    b->y = shooter->y;
    b->palette = palette;
    b->id = id;
    b->onScreen = true;

    set_sprite_tile(b->id, 1);
    set_sprite_prop(b->id, palette);
    move_sprite(b->id, b->x, b->y);
}
inline void setSpeedBullet(struct Bullet *b, int8_t hspeed, int8_t vspeed) {b->vspeed = vspeed;b->hspeed = hspeed;}


void updateBullet(struct Bullet *b)
{
    

    b->x += b->hspeed;
    b->y += b->vspeed;

    move_sprite(b->id, b->x, b->y);

    if (b->y <= 8 || b->x <= 8)
    {
        b->onScreen = false;
        playerbulletsOnScreen--;
        b->hspeed = 0;
        b->vspeed = 0;
        b->x = 0;
        b->y = 0;
    }
    else
    {
        b->onScreen = true;
    }
}