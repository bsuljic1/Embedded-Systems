#include "mbed.h"
#include "fonts.h"
#include "stm32f413h_discovery_lcd.h"
#define M_PI 3.14159265358979323846

#define GAME_BACKGROUND (uint32_t)0x010200

#define REFRESH_PERIOD 0.15
#define GAME_PERIOD 0.3

#define BROJ_BOJA 8

#define MAX_X 34
#define MAX_Y 34

InterruptIn bLeft(p5);
InterruptIn bUp(p6);
InterruptIn bRight(p7);
InterruptIn bDown(p8);

AnalogIn potHor(p15);
AnalogIn potVer(p16);

Ticker gameTicker;
Ticker mainTicker;

enum Mode{
    MAIN_MENU,
    COLOR_CHOOSE,
    GAME,
    END_GAME
};
Mode mode = MAIN_MENU;

enum Smjer {
    LEFT, UP, RIGHT, DOWN
};
Smjer smjerPrve = UP;
Smjer smjerDruge = DOWN;

uint16_t boje[BROJ_BOJA] = {LCD_COLOR_GREEN, LCD_COLOR_YELLOW, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE, LCD_COLOR_GRAY, LCD_COLOR_DARKRED, LCD_COLOR_ORANGE, LCD_COLOR_BROWN};

int bojaPrve = 0;
int bojaDruge = 1;

int prvaX[MAX_X*MAX_Y] = {5, 5, 5, 5};
int prvaY[MAX_X*MAX_Y] = {30, 29, 28, 27};
int drugaX[MAX_X*MAX_Y] = {29, 29, 29, 29};
int drugaY[MAX_X*MAX_Y] = {4, 5, 6, 7};

int metaX, metaY;

int duzinaPrve = 4;
int duzinaDruge = 4;

int scorePrve = 0;
int scoreDruge = 0;

bool pobjednikPrvi = true;

void iscrtajKvadratic(int x, int y){
    BSP_LCD_FillRect (x*239/MAX_X+1, y*239/MAX_Y+1, 5, 5);
}

void iscrtajMetu(int x, int y){
    BSP_LCD_FillCircle (x*239/MAX_X+1, y*239/MAX_Y+1, 3);
}

void initGame(){
    smjerPrve = UP;
    smjerDruge = DOWN;

    prvaX[0] = prvaX[1] = prvaX[2] = prvaX[3] = 5;
    prvaY[0] = 30;
    prvaY[1] = 29;
    prvaY[2] = 28;
    prvaY[3] = 27;
    drugaX[0] = drugaX[1] = drugaX[2] = drugaX[3] = 29;
    drugaY[0] = 4;
    drugaY[1] = 5;
    drugaY[2] = 6;
    drugaY[3] = 7;

    duzinaPrve = 4;
    duzinaDruge = 4;

    scorePrve = 0;
    scoreDruge = 0;
    
    mode = GAME;
}

void postaviMainMenu(){
    BSP_LCD_Clear(LCD_COLOR_DARKGREEN);
    BSP_LCD_SetBackColor (LCD_COLOR_DARKGREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
     //ivice
    BSP_LCD_DrawRect(0, 0, 239, 239);
    BSP_LCD_DrawRect(1, 1, 237, 237);
    BSP_LCD_DrawRect(2, 2, 235, 235);
    BSP_LCD_DrawRect(3, 3, 233, 233);
    BSP_LCD_DrawRect(4, 4, 231, 231);
    BSP_LCD_FillRect(235,235,5,5);
   
    BSP_LCD_SetTextColor (LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)"DOUBLE SNAKE", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Start game", CENTER_MODE);
    
    //zuta zmija
    BSP_LCD_FillRect (190, 190, 5, 5);
    BSP_LCD_FillRect (190, 183, 5, 5);
    BSP_LCD_FillRect (190, 176, 5, 5);
    BSP_LCD_FillRect (190, 169, 5, 5);
    BSP_LCD_FillRect (190, 162, 5, 5);
    BSP_LCD_FillRect (190, 155, 5, 5);
    BSP_LCD_FillRect (190, 148, 5, 5);
    BSP_LCD_FillRect (190, 141, 5, 5);
    BSP_LCD_FillRect (190, 134, 5, 5);
    BSP_LCD_FillRect (184, 134, 5, 5);
    BSP_LCD_FillRect (178, 134, 5, 5);
    BSP_LCD_FillRect (172, 134, 5, 5);
    BSP_LCD_FillRect (172, 141, 5, 5);
    BSP_LCD_FillRect (172, 148, 5, 5);
    BSP_LCD_FillRect (172, 155, 5, 5);
    BSP_LCD_FillRect (166, 155, 5, 5);
    //glava zute zmije
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_FillRect (160, 155, 5, 5);
    
    //meta
    BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
    BSP_LCD_FillCircle (130, 145, 3);
    
    //zelena zmija
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    //glava zelene zmije
    BSP_LCD_FillRect (100, 155, 5, 5);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect (100, 162, 5, 5);
    BSP_LCD_FillRect (100, 169, 5, 5);
    BSP_LCD_FillRect (100, 176, 5, 5);
    BSP_LCD_FillRect (94, 176, 5, 5);
    BSP_LCD_FillRect (88, 176, 5, 5);
    BSP_LCD_FillRect (82, 176, 5, 5);
    BSP_LCD_FillRect (76, 176, 5, 5);
    BSP_LCD_FillRect (70, 176, 5, 5);
    BSP_LCD_FillRect (64, 176, 5, 5);
    BSP_LCD_FillRect (58, 176, 5, 5);
    BSP_LCD_FillRect (52, 176, 5, 5);
    BSP_LCD_FillRect (52, 183, 5, 5);
    BSP_LCD_FillRect (52, 190, 5, 5);
    BSP_LCD_FillRect (52, 197, 5, 5);
    BSP_LCD_FillRect (46, 197, 5, 5);
    BSP_LCD_FillRect (40, 197, 5, 5);
    BSP_LCD_FillRect (34, 197, 5, 5);
}

void postaviColorChoose(){
    BSP_LCD_Clear(LCD_COLOR_DARKGREEN);
    BSP_LCD_SetBackColor (LCD_COLOR_DARKGREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
     //ivice
    BSP_LCD_DrawRect(0, 0, 239, 239);
    BSP_LCD_DrawRect(1, 1, 237, 237);
    BSP_LCD_DrawRect(2, 2, 235, 235);
    BSP_LCD_DrawRect(3, 3, 233, 233);
    BSP_LCD_DrawRect(4, 4, 231, 231);
    BSP_LCD_FillRect(235,235,5,5);
    
    BSP_LCD_SetTextColor (LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(10, 10, (uint8_t*)"<", LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, 40, (uint8_t*)"COLOR CHOOSE", CENTER_MODE);
    BSP_LCD_DisplayStringAt(215, 210, (uint8_t*)">", LEFT_MODE);
    
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(40, 85, (uint8_t*)"Player 1", LEFT_MODE);
    BSP_LCD_DisplayStringAt(140, 85, (uint8_t*)"Player 2", LEFT_MODE);
    
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(65, 115, (uint8_t*)"^", LEFT_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(65, 190, (uint8_t*)"v", LEFT_MODE);
    
    
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(165, 115, (uint8_t*)"^", LEFT_MODE);
     BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(165, 190, (uint8_t*)"v", LEFT_MODE);
    
    
    BSP_LCD_SetFont(&Font20);
    //kvadratici za boje
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_DrawRect(40, 125, 60 ,60);
    BSP_LCD_DrawRect(41, 126, 58 ,58);
    BSP_LCD_DrawRect(42, 127, 56 ,56);
    //desni kvadratic
    BSP_LCD_DrawRect(140, 125, 60 ,60);
    BSP_LCD_DrawRect(141, 126, 58 ,58);
    BSP_LCD_DrawRect(142, 127, 56 ,56);
    
    //boje
    BSP_LCD_SetTextColor(boje[bojaPrve]);
    BSP_LCD_FillRect(43,128, 55,54);
    BSP_LCD_SetTextColor(boje[bojaDruge]);
    BSP_LCD_FillRect(143,128,55,54);
}

void postaviGame(){
    char buffer[64];
    
    BSP_LCD_Clear(GAME_BACKGROUND);
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_SetBackColor (GAME_BACKGROUND);
    BSP_LCD_SetFont(&Font8);
     //ivice
    BSP_LCD_DrawRect(0, 0, 239, 239);
    BSP_LCD_DrawRect(1, 1, 237, 237);
    BSP_LCD_DrawRect(2, 2, 235, 235);
    BSP_LCD_DrawRect(3, 3, 233, 233);
    BSP_LCD_DrawRect(4, 4, 231, 231);
    BSP_LCD_FillRect(235,235,5,5);
    
    BSP_LCD_SetTextColor(boje[bojaPrve]);

    for(int i = 0; i < duzinaPrve - 1; i++)
        iscrtajKvadratic(prvaX[i], prvaY[i]);
    
    sprintf(buffer, "Player 1: %d", scorePrve);
    BSP_LCD_DisplayStringAt(10, 10, (uint8_t*)buffer, LEFT_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    iscrtajKvadratic(prvaX[duzinaPrve - 1], prvaY[duzinaPrve - 1]);

    BSP_LCD_SetTextColor(boje[bojaDruge]);

    for(int i = 0; i < duzinaDruge - 1; i++)
        iscrtajKvadratic(drugaX[i], drugaY[i]);

    sprintf(buffer, "Player 2: %d", scoreDruge);
    BSP_LCD_DisplayStringAt(10, 20, (uint8_t*)buffer, LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    iscrtajKvadratic(drugaX[duzinaDruge - 1], drugaY[duzinaDruge - 1]);

    BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
    iscrtajMetu(metaX, metaY);
}


void postaviEndGame(){
    BSP_LCD_Clear(LCD_COLOR_DARKGREEN);
    BSP_LCD_SetBackColor (LCD_COLOR_DARKGREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
     //ivice
    BSP_LCD_DrawRect(0, 0, 239, 239);
    BSP_LCD_DrawRect(1, 1, 237, 237);
    BSP_LCD_DrawRect(2, 2, 235, 235);
    BSP_LCD_DrawRect(3, 3, 233, 233);
    BSP_LCD_DrawRect(4, 4, 231, 231);
    BSP_LCD_FillRect(235,235,5,5);
    
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, 20, (uint8_t*)"GAME OVER", CENTER_MODE);
    
    //ispisivanje scoreova
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);

    char buffer[64];
    
    sprintf(buffer, "Player 1: %d", scorePrve);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t*)buffer, CENTER_MODE);

    sprintf(buffer, "Player 2: %d", scoreDruge);
    BSP_LCD_DisplayStringAt(0, 105, (uint8_t*)buffer, CENTER_MODE);
    
    //ko je pobjednik
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    if(pobjednikPrvi)
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)"Player 1 wins", CENTER_MODE);
    else
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)"Player 2 wins", CENTER_MODE);
    
    //play again
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(120, 215, (uint8_t*)"Play again",LEFT_MODE);
    
    //povratak na main menu
    BSP_LCD_DisplayStringAt(8, 215, (uint8_t*)"Home", LEFT_MODE);
}

void postaviEkran(){
    switch (mode) {
        case MAIN_MENU:
            postaviMainMenu();
            break;
        case COLOR_CHOOSE:
            postaviColorChoose();
            break;
        case GAME:
            postaviGame();
            break;
        case END_GAME:
            postaviEndGame();
            break;
    }
}

void postaviMetu(){
    bool postavljena = true;
    do{
        postavljena = true;
        
        metaX = (rand() % (MAX_X - 2)) + 1;
        metaY = (rand() % (MAX_Y - 2)) + 1;
        
        for(int i = 0; i < duzinaPrve; i++)
            if(prvaX[i] == metaX && prvaY[i] == metaY)
                postavljena = false;
        
        for(int i = 0; i < duzinaDruge; i++)
            if(drugaX[i] == metaX && drugaY[i] == metaY)
                postavljena = false;
    }while(!postavljena);
}

void gameTick(){
    if(mode != GAME) return;
    
    int prvaGlavaX = prvaX[duzinaPrve - 1];
    int prvaGlavaY = prvaY[duzinaPrve - 1];
    
    switch(smjerPrve){
        case UP:
            prvaY[duzinaPrve - 1] = prvaY[duzinaPrve - 1] - 1;
            if(prvaY[duzinaPrve - 1] == 0)
                prvaY[duzinaPrve - 1] = MAX_Y - 1;
            break;
        case DOWN:
            prvaY[duzinaPrve - 1] = (prvaY[duzinaPrve - 1] + 1) % MAX_Y;
            if(prvaY[duzinaPrve - 1] == 0) prvaY[duzinaPrve - 1] = 1;
            break;
        case LEFT:
            prvaX[duzinaPrve - 1] = prvaX[duzinaPrve - 1] - 1;
            if(prvaX[duzinaPrve - 1] == 0)
                prvaX[duzinaPrve - 1] = MAX_X - 1;
            break;
        case RIGHT:
            prvaX[duzinaPrve - 1] = (prvaX[duzinaPrve - 1] + 1) % MAX_X;
            if(prvaX[duzinaPrve - 1] == 0) prvaX[duzinaPrve - 1] = 1;
            break;
    }

    if(prvaX[duzinaPrve - 1] == metaX && prvaY[duzinaPrve - 1] == metaY){
        scorePrve++;
        duzinaPrve++;
        prvaX[duzinaPrve - 1] = prvaX[duzinaPrve - 2];
        prvaY[duzinaPrve - 1] = prvaY[duzinaPrve - 2];
        postaviMetu();
    }else
        for(int i = 0; i < duzinaPrve - 2; i++){
            prvaX[i] = prvaX[i+1];
            prvaY[i] = prvaY[i+1];
        }
    prvaX[duzinaPrve - 2] = prvaGlavaX;
    prvaY[duzinaPrve - 2] = prvaGlavaY;
    
    int drugaGlavaX = drugaX[duzinaDruge - 1];
    int drugaGlavaY = drugaY[duzinaDruge - 1];
    
    switch(smjerDruge){
        case UP:
            drugaY[duzinaDruge - 1] = drugaY[duzinaDruge - 1] - 1;
            if(drugaY[duzinaDruge - 1] == 0)
                drugaY[duzinaDruge - 1] = MAX_Y - 1;
            break;
        case DOWN:
            drugaY[duzinaDruge - 1] = (drugaY[duzinaDruge - 1] + 1) % MAX_Y;
            if(drugaY[duzinaPrve - 1] == 0) drugaY[duzinaPrve - 1] = 1;
            break;
        case LEFT:
            drugaX[duzinaDruge - 1] = drugaX[duzinaDruge - 1] - 1;
            if(drugaX[duzinaDruge - 1] == 0)
                drugaX[duzinaDruge - 1] = MAX_X - 1;
            break;
        case RIGHT:
            drugaX[duzinaDruge - 1] = (drugaX[duzinaDruge - 1] + 1) % MAX_X;
            if(drugaX[duzinaPrve - 1] == 0) drugaX[duzinaPrve - 1] = 1;
            break;
    }

    if(drugaX[duzinaDruge - 1] == metaX && drugaY[duzinaDruge - 1] == metaY){
        scoreDruge++;
        duzinaDruge++;
        drugaX[duzinaDruge - 1] = drugaX[duzinaDruge - 2];
        drugaY[duzinaDruge - 1] = drugaY[duzinaDruge - 2];
        postaviMetu();
    }else
        for(int i = 0; i < duzinaDruge - 2; i++){
            drugaX[i] = drugaX[i+1];
            drugaY[i] = drugaY[i+1];
        }
    drugaX[duzinaDruge - 2] = drugaGlavaX;
    drugaY[duzinaDruge - 2] = drugaGlavaY;
    
    for(int i = 0; i < duzinaPrve; i++){
        if(prvaX[duzinaPrve - 1] == prvaX[i] && prvaY[duzinaPrve - 1] == prvaY[i] && i != duzinaPrve - 1){
            pobjednikPrvi = false;
            mode = END_GAME;
            break;
        }
        if(drugaX[duzinaDruge - 1] == prvaX[i] && drugaY[duzinaDruge - 1] == prvaY[i]){
            pobjednikPrvi = true;
            mode = END_GAME;
            break;
        }
    }
    
    //for(int i = 0; i < duzinaDruge; i++)
    //    printf("%d %dx%d\n", i, drugaX[i], drugaY[i]);
    
    for(int i = 0; i < duzinaDruge; i++){
        if(prvaX[duzinaPrve - 1] == drugaX[i] && prvaY[duzinaPrve - 1] == drugaY[i]){
            pobjednikPrvi = false;
            mode = END_GAME;
            break;
        }
        if(drugaX[duzinaDruge - 1] == drugaX[i] && drugaY[duzinaDruge - 1] == drugaY[i] && i != duzinaDruge - 1){
            pobjednikPrvi = true;
            mode = END_GAME;
            break;
        }
    }
}

void prviKontroler(Smjer smjer){
    switch (mode) {
        case MAIN_MENU:
            mode = COLOR_CHOOSE;
            break;
        case COLOR_CHOOSE:
            if(smjer == RIGHT)
                initGame();
            else if(smjer == LEFT)
                mode = MAIN_MENU;
            else if(smjer == UP){
                bojaPrve = (bojaPrve + 1) % BROJ_BOJA;
                if(bojaDruge == bojaPrve)
                    bojaPrve = (bojaPrve + 1) % BROJ_BOJA;
            }else{
                bojaPrve = bojaPrve - 1;
                if(bojaPrve == -1) bojaPrve = BROJ_BOJA - 1;
                if(bojaDruge == bojaPrve){
                    bojaPrve = bojaPrve - 1;
                    if(bojaPrve == -1) bojaPrve = BROJ_BOJA - 1;
                }
            }
            break;
        case GAME:
            if(abs(smjerPrve - smjer) != 2)
                smjerPrve = smjer;
            break;
        case END_GAME:
            if(smjer == LEFT)
                mode = MAIN_MENU;
            else if(smjer == RIGHT)
                initGame();
            break;
    }
}

void drugiKontroler(Smjer smjer){
    switch (mode) {
        case MAIN_MENU:
            mode = COLOR_CHOOSE;
            break;
        case COLOR_CHOOSE:
            if(smjer == RIGHT)
                initGame();
            else if(smjer == LEFT)
                mode = MAIN_MENU;
            else if(smjer == UP){
                bojaDruge = (bojaDruge + 1) % BROJ_BOJA;
                if(bojaDruge == bojaPrve)
                    bojaDruge = (bojaDruge + 1) % BROJ_BOJA;
            }else{
                bojaDruge = bojaDruge - 1;
                if(bojaDruge == -1) bojaDruge = BROJ_BOJA - 1;
                if(bojaDruge == bojaPrve){
                    bojaDruge = bojaDruge - 1;
                    if(bojaDruge == -1) bojaDruge = BROJ_BOJA - 1;
                }
            }
            break;
        case GAME:
            if(abs(smjerDruge - smjer) != 2)
                smjerDruge = smjer;
            break;
        case END_GAME:
            if(smjer == LEFT)
                mode = MAIN_MENU;
            else if(smjer == RIGHT)
                initGame();
            break;
    }
}

void clickLeft(){
    drugiKontroler(LEFT);
}

void clickUp(){
    drugiKontroler(UP);
}

void clickRight(){
    drugiKontroler(RIGHT);
}

void clickDown(){
    drugiKontroler(DOWN);
}

int main() {
    bLeft.rise(&clickLeft);
    bUp.rise(&clickUp);
    bRight.rise(&clickRight);
    bDown.rise(&clickDown);

    gameTicker.attach(&gameTick, GAME_PERIOD);
    mainTicker.attach(&postaviEkran, REFRESH_PERIOD);

    BSP_LCD_Init();
    
    srand(time(NULL));
    
    postaviMetu();
    
    bool potAktivan = true;
    
    while (1) {
        if(potAktivan){
            if(potHor.read() > 1.f/3 && potHor.read() < 2.f/3 && potVer.read() > 1./3 && potVer.read() < 2.f/3)
                potAktivan = false;
        }else{
            potAktivan = true;
            if(potHor.read() < 1.f/3)
                prviKontroler(LEFT);
            else if(potHor.read() > 2.f/3)
                prviKontroler(RIGHT);
            else if(potVer.read() < 1.f/3)
                prviKontroler(DOWN);
            else if(potVer.read() > 2.f/3)
                prviKontroler(UP);
            else{
                potAktivan = false;
            }
        }
        
        wait(0.11);
    }
}

