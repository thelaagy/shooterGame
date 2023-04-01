#pragma bank 0

#include "declarations.h"

void main()
{
    // Load fonts
    SWITCH_ROM_MBC5(TILEnMAPBANK);
    font_t min_font;
    font_init();
    min_font = font_load(font_min);
    font_set(min_font);
    set_bkg_data(0x25, 4, UITiles);

    set_sprite_data(0, 6, ShipSprites);
    set_sprite_palette(0, 8, &SPR_PALETTE[0]);

    UBYTE s = 0;

    while (1)
    {
        scene(&s);
    }
}

void scene(UBYTE *sceneNumber)
{
    if (*sceneNumber == 0)
    {
        ////////////////////////////////////////////////////////////////////////////////////
        // Main menu
        ////////////////////////////////////////////////////////////////////////////////////

        SWITCH_ROM_MBC5(3);
        hUGE_init(&Wyrmhole);
        add_VBL(hUGE_dosound);
        enableSound(true);

        fill_win_rect(0, 0, 20, 18, 0);
        set_bkg_palette(0, 1, &UI_PALETTE[0]);

        HIDE_SPRITES;
        clearSprites();
        wait_vbl_done();

        SHOW_BKG;
        SHOW_SPRITES;
        SHOW_WIN;
        DISPLAY_ON;
        winprint("VIDEO GAMES", 0, 0);
        if (DEVICE_SUPPORTS_COLOR)
            winprint("COLOR EDITION", 0, 1);

        set_sprite_tile(0, 5);
        set_sprite_prop(0, 5);

        winprint("START GAME", 2, 4);
        winprint("SELECT GAME", 2, 5);
        menuChoice = 0;

        // Wait for start button
        while (1)
        {

            if (joypad() & J_START || joypad() & J_A)
            {
                if (menuChoice == 0)
                    *sceneNumber = 1;
                else if (menuChoice == 1)
                    *sceneNumber = 4;
                break;
            }

            // Menu change
            else if (joypad() & J_UP)
                menuChoice--;
            else if (joypad() & J_DOWN)
                menuChoice++;

            if (menuChoice == 0)
                move_sprite(0, 16, 48);
            else if (menuChoice == 1)
                move_sprite(0, 16, 56);

            if (menuChoice == 0x02)
                menuChoice = 1;
            else if (menuChoice > 1)
                menuChoice = 0;
            performantdelay(4);
        }
        enableSound(true);
        NR10_REG = 0b00010110;
        NR11_REG = 0b10000000;
        NR12_REG = 0b11110111;
        NR14_REG = 0xC3;
        performantdelay(10);
        fill_win_rect(0, 0, 20, 18, 0);

        remove_VBL(hUGE_dosound);
        enableSound(false);
    }
    else if (*sceneNumber == 1)
    {
        enableSound(false);
        DISPLAY_OFF;
        ////////////////////////////////////////////////////////////////////////////////////
        // Background load
        ////////////////////////////////////////////////////////////////////////////////////
        // Loading BKG Palette
        SWITCH_ROM_MBC5(TILEnMAPBANK);
        wait_vbl_done();

        move_bkg(0, 0);
        switch (level)
        {
        case 1:
            set_bkg_palette(1, 4, &SPACE_PALETTE[0]);
            // Loading the tiles
            set_bkg_data(0x30, 9, SpaceTileset);
            // Writing the BKG Palette into the screen
            VBK_REG = 1;
            set_bkg_tiles(0, 0, SpaceMapWidth, SpaceMapHeight, SpaceMapPLN1);
            // Writing the map on the screen
            VBK_REG = 0;
            set_bkg_tiles(0, 0, SpaceMapWidth, SpaceMapHeight, SpaceMapPLN0);
            break;
        case 2:
            set_bkg_palette(1, 4, &MARS_PALETTE[0]);
            // Loading the tiles
            set_bkg_data(0x30, 9, MarsTileset);
            // Writing the BKG Palette into the screen
            VBK_REG = 1;
            set_bkg_tiles(0, 0, SpaceMapWidth, SpaceMapHeight, MarsMapPLN1);
            // Writing the map on the screen
            VBK_REG = 0;
            set_bkg_tiles(0, 0, SpaceMapWidth, SpaceMapHeight, MarsMapPLN0);
            break;

        default:
            VBK_REG = 1;
            fill_bkg_rect(0, 0, 20, 18, 0);
            VBK_REG = 0;
            fill_bkg_rect(0, 0, 20, 18, 0);
        }

        // winprint("PRESS START", 0, 2);

        ////////////////////////////////////////////////////////////////////////////////////
        // Player Setup
        ////////////////////////////////////////////////////////////////////////////////////
        struct Ship p;
        initShip(&p, 0, 0);
        moveShip(&p, PLAYABLEWIDTH >> 1, 128);
        UBYTE playerSpeed = MaxPlayerSpeed;
        kills = 0;
        lives = 10;
        invencibility = false;
        invencibilityTime = 0;
        UBYTE bulletCooldown = 0;

        struct Bullet playerbullets[4];
        const BYTE bulletNumber = 3;

        for (i = 0; i < bulletNumber; i++)
        {
            initBullet(&playerbullets[i], &p, i + 2, 1);
            setSpeedBullet(&playerbullets[i], 0, 0);
            playerbullets[i].x = 0;
            playerbullets[i].y = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Enemies setup
        ////////////////////////////////////////////////////////////////////////////////////
        struct Ship enemies[enemiesQuant];
        for (i = 0; i < enemiesQuant; i++)
            spawnShip(&enemies[i], i);

        uint8_t bulletSpawnTimer[3];
        struct Bullet enemyBullet[3];
        for (i = 0; i < 3; i++)
        {
            initBullet(&enemyBullet[i], &p, i + 5, 1);
            setSpeedBullet(&enemyBullet[i], 0, 0);
            enemyBullet[i].x = 0;
            enemyBullet[i].y = 0;
            initarand(DIV_REG);
            bulletSpawnTimer[i] = rand() % 15 + 1;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Setup side HUD
        ////////////////////////////////////////////////////////////////////////////////////
        set_win_tile_xy(0, 0, 0x25);
        fill_win_rect(0, 1, 1, 16, 0x26);
        fill_win_rect(1, 0, 5, 18, 00);
        set_win_tile_xy(0, 17, 0x27);
        move_win(120, 0);
        winprint("LIVES", 1, 1);
        winprint("X", 5, 2);
        fill_win_rect(1, 3, 5, 1, 0x28);
        winprint("KILLS", 1, 5);
        winprint("X", 5, 6);
        fill_win_rect(1, 7, 5, 1, 0x28);

        ////////////////////////////////////////////////////////////////////////////////////
        // Setup music
        ////////////////////////////////////////////////////////////////////////////////////
        performantdelay(20);
        wait_vbl_done();
        SWITCH_ROM_MBC5(7);
        enableSound(true);

        add_VBL(hUGE_dosound);
        hUGE_init(&Stage1);

        // Turn on the display
        SHOW_BKG;
        SHOW_WIN;
        SHOW_SPRITES;
        DISPLAY_ON;

        ////////////////////////////////////////////////////////////////////////////////////
        // Gameplay loop
        ////////////////////////////////////////////////////////////////////////////////////

        while (1)
        {
            ////////////////////////////////////////////////////////////////////////////////////
            // Background scroll up
            ////////////////////////////////////////////////////////////////////////////////////
            scroll_bkg(0, -1 + -(1 * (kills / 100)));

            ////////////////////////////////////////////////////////////////////////////////////
            // Player update
            ////////////////////////////////////////////////////////////////////////////////////
            updateShip(&p);

            if (lives < 0)
            {
                // Dies
                *sceneNumber = 2;
                break;
            }

            // Invencibility cooldown and animation
            if (invencibilityTime > 0)
            {
                invencibility = true;
                if (invencibilityTime % 20 < 10)
                {
                    p.palette = 1;
                    BGP_REG = 0xE4;
                }
                else
                {
                    BGP_REG = 0xF9;
                    p.palette = 0;
                }
                invencibilityTime--;
            }
            else if (invencibility && invencibilityTime == 0)
            {
                BGP_REG = 0xE4;
                invencibility = false;
                p.palette = 0;
            }

            ////////////////////////////////////////////////////////////////////////////////////
            // HUD update
            ////////////////////////////////////////////////////////////////////////////////////
            if (lives < 100)
            {
                set_win_tile_xy(3, 2, (lives / 10) % 10 + 1);
                set_win_tile_xy(4, 2, (lives % 10) + 1);
            }
            else
            {
                set_win_tile_xy(3, 2, 0x17);
                set_win_tile_xy(4, 2, 0x0B);
            }
            winprintn(kills, 1, 6);

            ////////////////////////////////////////////////////////////////////////////////////
            // Ships AI
            ////////////////////////////////////////////////////////////////////////////////////

            for (i = 0; i < enemiesQuant; i++)
            {
                // Update ship state
                updateShip(&enemies[i]);

                // Respawn the ships
                if (!enemies[i].isAlive || (enemies[i].y > 160 && enemies[i].y < 170))
                {
                    spawnShip(&enemies[i], i);
                    break;
                }

                // Check if the ship has died
                for (j = 0; j < bulletNumber; j++)
                {
                    if (bulletCollision(&playerbullets[j], &enemies[i]))
                    {
                        // Kill the enemy ship
                        playerbullets[j].x = 0;
                        playerbullets[j].y = 0;
                        kills++;
                        // enemies[i].isAlive = false;
                        enemies[i].isAlive = false;
                    }
                }

                // Kill player if collide with them
                if (!(invencibilityTime > 0))
                    if (shipCollision(&p, &enemies[i]))
                        takeDamage(&p);
            }

            ////////////////////////////////////////////////////////////////////////////////////
            // Bullets
            ////////////////////////////////////////////////////////////////////////////////////
            initarand(DIV_REG);
            i = rand() % enemiesQuant;
            for (j = 0; j < bulletNumber; j++)
            {
                updateBullet(&playerbullets[j]);
                updateBullet(&enemyBullet[j]);

                // Kill player if collide with them
                if (!(invencibilityTime > 0))
                    if (bulletCollision(&enemyBullet[j], &p))
                        takeDamage(&p);

                if (bulletSpawnTimer[j] == 0)
                {
                    if (enemies[i].isAlive && !enemyBullet[j].onScreen)
                    {
                        initBullet(&enemyBullet[j], &enemies[i], 5 + j, 2);
                        setSpeedBullet(&enemyBullet[j], 0, 4);
                    }
                    initarand(DIV_REG);
                    bulletSpawnTimer[j] = rand() % 15 + 1;
                }
                else
                    bulletSpawnTimer[j]--;
            }

            //
            if (bulletCooldown > 0)
                bulletCooldown--;

            ////////////////////////////////////////////////////////////////////////////////////
            // Player input
            ////////////////////////////////////////////////////////////////////////////////////

            if (joypad() & J_B)
                playerSpeed = MaxPlayerSpeed >> 1;
            else
                playerSpeed = MaxPlayerSpeed;

            // Movement
            if (joypad() & J_LEFT && !(p.x - playerSpeed < 5))
                scrollShip(&p, -playerSpeed, 0);
            else if (joypad() & J_RIGHT && !(p.x + playerSpeed > PLAYABLEWIDTH))
                scrollShip(&p, playerSpeed, 0);
            if (joypad() & J_UP && !(p.y - playerSpeed < 16))
                scrollShip(&p, 0, -playerSpeed);
            else if (joypad() & J_DOWN && !(p.y + playerSpeed > 0x98))
                scrollShip(&p, 0, playerSpeed);

            // Player shot
            if (joypad() & J_A)
            {
                // Check if the bullet is offscreen
                for (i = 0; i < bulletNumber; i++)
                {
                    if (!playerbullets[i].onScreen && bulletCooldown == 0)
                    {
                        initBullet(&playerbullets[i], &p, i + 2, 1);
                        setSpeedBullet(&playerbullets[i], 0, -8);
                        bulletCooldown = 5;
                        break;
                    }
                }
            }

            if (joypad() & J_SELECT)
                reset();

            ////////////////////////////////////////////////////////////////////////////////////
            // Pause
            ////////////////////////////////////////////////////////////////////////////////////
            if (joypad() & J_START)
            {
                winprint("PAUSE", 1, 12);
                performantdelay(1);
                while (1)
                {
                    if (joypad() & J_START)
                        break;
                    performantdelay(1);
                }
                winprint("     ", 1, 12);
            }

            performantdelay(1);
        }
    }
    else if (*sceneNumber == 2)
    {
        // ////////////////////////////////////////////////////////////////////////
        //              GAME OVER
        // ////////////////////////////////////////////////////////////////////////
        enableSound(false);
        remove_VBL(hUGE_dosound);

        HIDE_SPRITES;
        SHOW_BKG;
        DISPLAY_ON;

        fill_bkg_rect(0, 0, 20, 36, 0);
        fill_win_rect(0, 0, 20, 18, 0);
        move_bkg(0, 0);
        move_win(6, 0);
        winprint("GAME OVER", 5, 1);
        winprint("PRESS START", 4, 4);
        winprint("TO RETURN TO MENU", 1, 5);

        winprintn(kills, 2, 6);
        winprint("KILLS", 9, 6);
        while (1)
        {
            if (joypad() & J_START)
            {
                *sceneNumber = 0;
                performantdelay(10);
                break;
            }
            performantdelay(1);
        }
    }
    else if (*sceneNumber == 4)
    {
        // ////////////////////////////////////////////////////////////////////////
        //              LEVEL SELECT
        // ////////////////////////////////////////////////////////////////////////
        HIDE_SPRITES;
        SHOW_BKG;
        DISPLAY_ON;

        fill_bkg_rect(0, 0, 20, 36, 0);
        fill_win_rect(0, 0, 20, 18, 0);
        move_bkg(0, 0);
        move_win(6, 0);

        winprint("LEVEL SELECT", 5, 1);
        while (1)
        {

            set_win_tile_xy(1, 2, level + 1);
            if (joypad() & J_UP)
                level++;
            else if (joypad() & J_DOWN)
                level--;

            if (joypad() & J_START || joypad() & J_A)
            {
                *sceneNumber = 1;
                break;
            }

            if (level == 0xFF)
                level = 0;
            else if (level == 16)
                level = 15;
            performantdelay(5);
        }
    }
}

// The delay function but better and way less 'costy' on the CPU
void performantdelay(int8_t time)
{
    for (i = 0; i < time; i++)
        wait_vbl_done();
}

void winprint(char text[], UBYTE x, UBYTE y)
{
    for (UBYTE i = 0; i < strlen(text); i++)
        for (UBYTE j = 0; j < strlen(fontC); j++)
            if (text[i] == fontC[j])
                set_win_tile_xy(i + x, y, j);
}

void winprintn(uint16_t number, UBYTE x, UBYTE y)
{
    set_win_tile_xy(x + 3, y, (number % 10) + 1);
    if (number > 10 && number < 100)
        set_win_tile_xy(x + 2, y, (number / 10) + 1);
    else if (number >= 100 && number < 1000)
    {
        set_win_tile_xy(x + 2, y, (number / 10) % 10 + 1);
        set_win_tile_xy(x + 1, y, number / 100 + 1);
    }
    else if (number >= 1000)
    {
        set_win_tile_xy(x + 2, y, (number / 10) % 10 + 1);
        set_win_tile_xy(x + 1, y, number / 100 + 1);
        set_win_tile_xy(x, y, number / 1000 + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool bulletCollision(struct Bullet *b, struct Ship *p)
{
    // check if a bullet collided with a ship
    if (
        (b->x + 1 < p->x + 12 &&
         b->x + 6 > p->x + 4 &&
         b->y + 1 < p->y + 8 &&
         6 + b->y > p->y) &&
        b->onScreen)
        return 1;
    return 0;
}

bool shipCollision(struct Ship *p1, struct Ship *p2)
{
    // check if a ship collided with another ship
    if ((
            p1->x + 4 < p2->x + SHIP_WIDTH &&
            p1->x + SHIP_WIDTH > p2->x + 4 &&
            p1->y < p2->y + SHIP_HEIGHT &&
            SHIP_HEIGHT + p1->y > p2->y) &&
        p1->isAlive && p2->isAlive)
        return 1;
    return 0;
}

void spawnShip(struct Ship *p, int8_t id)
{
    initShip(p, (id << 1) + 8, 4);
    changeSpriteShip(p, 2);
    initarand(DIV_REG);
    moveShip(p, rand() % PLAYABLEWIDTH + 8, 170 + (rand() % 85));

    initarand(DIV_REG);
    p->hspeed = rand() % 3 - 1 + (1 * (kills / 100));
    initarand(DIV_REG);
    p->vspeed = rand() % 3 + 1 + (1 * (kills / 100));
}

void takeDamage(struct Ship *p)
{
    lives--;
    invencibilityTime = 100;
    moveShip(p, PLAYABLEWIDTH >> 1, 128);
}

void enableSound(bool a)
{
    if (a)
        NR52_REG = 0x80;
    else
        NR52_REG = 0x00;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}

void clearSprites()
{
    for (i = 0; i < 40; i++)
    {
        set_sprite_tile(i, 0);
        set_sprite_prop(i, 0);
        move_sprite(i, 0, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
