
#include <gb/gb.h>
#include <stdbool.h>

#define SHIP_WIDTH 12
#define SHIP_HEIGHT 8

#define PLAYABLEWIDTH 0x6C


// Ship "Class"
const struct Ship
{
    UBYTE id1, id2;
    UBYTE x, y;
    UBYTE palette;
    bool isAlive;

    int8_t hspeed, vspeed;
};

void initShip(struct Ship *p, UBYTE id, UBYTE palette);
void moveShip(struct Ship *p, UBYTE x, UBYTE y);
void scrollShip(struct Ship *p, BYTE x, BYTE y);
void updateShip(struct Ship *p);
void changeSpriteShip(struct Ship *p, UBYTE sprite);