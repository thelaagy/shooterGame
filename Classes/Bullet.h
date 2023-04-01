
#include <gb/gb.h>
#include <stdbool.h>

// Bullets class
const struct Bullet
{
    UBYTE id;
    UBYTE palette;
    UBYTE x, y;
    int8_t hspeed, vspeed;
    bool onScreen;
};

void initBullet(struct Bullet *b, struct Ship *shooter, UBYTE id, UBYTE palette);
void setSpeedBullet(struct Bullet *b, int8_t hspeed, int8_t vspeed);
void updateBullet(struct Bullet *b);