#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdbool.h>
#include <rand.h>
#include <gbdk/font.h>
#include <string.h>

// Sound driver
#include "hUGEDriver.h"

// Musics
extern const hUGESong_t song_placeholder;
extern const hUGESong_t Wyrmhole;

extern const hUGESong_t Stage1;

// Classes
#include "Classes/Ship.h"
#include "Classes/Bullet.h"

// Tiles
#define TILEnMAPBANK 1
#include "Tiles/SpaceTileset.H"
#include "Tiles/MarsTileset.H"
#include "Tiles/ShipSprites.h"
#include "Tiles/UI.h"

// // Maps
#include "Maps/SpaceMap.H"
#include "Maps/MarsMap.H"

int8_t i, j;

const UWORD UI_PALETTE[] = {
    RGB(3, 1, 2),
    RGB_DARKGRAY,
    RGB(28, 28, 31),
    RGB_RED

};

const UWORD SPR_PALETTE[] = {
    ShipSpritesCGBPal0c0,
    ShipSpritesCGBPal0c1,
    ShipSpritesCGBPal0c2,
    ShipSpritesCGBPal0c3,

    ShipSpritesCGBPal1c0,
    ShipSpritesCGBPal1c1,
    ShipSpritesCGBPal1c2,
    ShipSpritesCGBPal1c3,

    ShipSpritesCGBPal2c0,
    ShipSpritesCGBPal2c1,
    ShipSpritesCGBPal2c2,
    ShipSpritesCGBPal2c3,

    ShipSpritesCGBPal3c0,
    ShipSpritesCGBPal3c1,
    ShipSpritesCGBPal3c2,
    ShipSpritesCGBPal3c3,

    ShipSpritesCGBPal4c0,
    ShipSpritesCGBPal4c1,
    ShipSpritesCGBPal4c2,
    ShipSpritesCGBPal4c3,

    ShipSpritesCGBPal5c0,
    ShipSpritesCGBPal5c1,
    ShipSpritesCGBPal5c2,
    ShipSpritesCGBPal5c3,

    ShipSpritesCGBPal6c0,
    ShipSpritesCGBPal6c1,
    ShipSpritesCGBPal6c2,
    ShipSpritesCGBPal6c3,

    ShipSpritesCGBPal7c0,
    ShipSpritesCGBPal7c1,
    ShipSpritesCGBPal7c2,
    ShipSpritesCGBPal7c3

};

#define enemiesQuant 6

uint16_t kills = 0;
int8_t lives = 0xF;
const UBYTE MaxPlayerSpeed = 3;
bool invencibility = false;
UBYTE invencibilityTime = 0;
UBYTE level = 1;
UBYTE menuChoice = 0;

// Gameplay
bool bulletCollision(struct Bullet *b, struct Ship *p);
bool shipCollision(struct Ship *p1, struct Ship *p2);

void spawnShip(struct Ship *p, int8_t id);
void takeDamage(struct Ship *p);

// Screen
const char fontC[] = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
void winprint(char text[], UBYTE x, UBYTE y);
void winprintn(uint16_t number, UBYTE x, UBYTE y);
void clearSprites();

void scene(UBYTE *sceneNumber);

// Sound
void enableSound(bool a);

// Misc
void performantdelay(int8_t time);