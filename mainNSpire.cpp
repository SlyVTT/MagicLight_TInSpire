#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string>

//#include <gint/rtc.h>
//#include <gint/display.h>
//#include <gint/keyboard.h>
//#include <gint/gint.h>
//#include <gint/bfile.h>
//#include <gint/std/stdlib.h>

#include "./Managers/KeyManagerLTE.hpp"
#include "./Managers/MouseManagerLTE.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include "structs.h"
//#include "primitives.h"

//#define DEBUG 1

#define R_ROUGE 0
#define R_VERT 1
#define R_BLEU 2
#define R_NOIR 3
#define R_ROSE 4
#define R_CYAN 5
#define R_JAUNE 6
#define R_BLANC 7

#define LAYER_BELOW_RAYS 0
#define LAYER_ABOVE_RAYS 1

unsigned int SIZE = 16;
unsigned int OFFSET_X = 10;
unsigned int OFFSET_Y = 10;

unsigned int OFFSET_X_MINIMAP = 270;
unsigned int OFFSET_Y_MINIMAP = 160;
unsigned char SCALE_X_MINIMAP = 8;
unsigned char SCALE_Y_MINIMAP = 6;

unsigned char ALPHA_OPAQUE_DEFAULT = 80; //191
unsigned char ALPHA_TRANSPARENT_DEFAULT = 40; //62

unsigned char ALPHA_OPAQUE = ALPHA_OPAQUE_DEFAULT;
unsigned char ALPHA_TRANSPARENT = ALPHA_TRANSPARENT_DEFAULT;
unsigned char ALPHA_BLOQUE = 1;

unsigned char SPEEDUP_RAY = 1;
unsigned char SPEEDUP_WALL = 1;

char LEVELCOLOR=0;

const char* Revision = "Revision 0.9B";
const char* RevShort = "v0.9B";


std::string filepath = "/documents/MagicLight/MagicLight.save.tns";


bool saveexist = false;
unsigned int sizeoffile;
GameSaveStatus sauvegarde;
LevelSaveStatus partielevel[33];


#define NB_LEVELS 6


unsigned char NbReceptorsLevel;
unsigned char NbDoorsLevel;
unsigned char NbBlocksLevel;
unsigned char NbTreasureLevel;
unsigned char NbObstacleLevel;
unsigned char NbMonsterLevel;
unsigned char NbBossLevel;
unsigned char NbBulletLevel;

unsigned int selected = JOUEUR;

unsigned int score;
unsigned char life = 3;
unsigned char lifeMax = 5;

unsigned char SIZE_MAP_X=13;
unsigned char  SIZE_MAP_Y=11;

Player joueur;
Light lumiere;
//MapItem carte[SIZE_MAP_X][SIZE_MAP_Y];
Wall murs[4];

Blocks* BlocRouge;
Blocks* BlocVert ;
Blocks* BlocBleu;
Blocks* BlocNoir;


Blocks* BlocksCollection=NULL;
Receptor* ReceptorCollection=NULL;
Door* DoorCollection=NULL;
Treasure* TreasureCollection=NULL;
Obstacle* ObstaclesCollection=NULL;
Monster* MonstersCollection=NULL;
Boss* BossCollection=NULL;
Bullet* BulletCollection=NULL;


Minimap Map[ 33 ];


unsigned char ALPHA_VALIDE = 255;
unsigned char ALPHA_NON_VALIDE = 100;

unsigned char EPSILON = 16;


bool BlackFrag=false;
bool WhiteFrag=false;
bool RedFrag=false;
bool GreenFrag=false;
bool BlueFrag=false;


//extern font_t font_fantasy;
//extern font_t font_tiny;


bool GotoMainMenu=false;
bool doneMain = false;
bool donePause = false;
bool doneOption = false;
bool doneGame = false;
bool doneStart = false;
bool doneTitle = false;
bool doneLoose = false;
bool doneDifficulty = false;
bool doneStory = false;
bool doneEnd = false;
bool attackboss = false;

unsigned int compteur_mouvement = 0;
unsigned char frame_cursor = 0;
unsigned char frame_light = 0;
unsigned char frame_monster = 0;
unsigned char frame_boss = 0;
unsigned char frame=0;

unsigned char frame_boss_attack=0;
unsigned int xbossattack;
unsigned int ybossattack;
unsigned char frame_bullet=0;
unsigned char attacktype;
unsigned char attackpattern;

bool mouvement=false;
orientations direction=HAUT;

unsigned char nextLevel;
unsigned char currentLevel;
unsigned char selectStartMenu = 0;
unsigned char selectDifficultyMenu = 0;
unsigned char selectOptionMenu = 0;
unsigned char selectOptionPause = 0;
unsigned char selectOptionLoose = 0;
unsigned char selectStoryMenu = 0;
unsigned char selectEndMenu = 0;
unsigned char difficulty = 0;


SDL_Surface *maintitle;
SDL_Surface *leveltiles;
SDL_Surface *sprites;
SDL_Surface *player;
SDL_Surface *light;
SDL_Surface *cursor;
SDL_Surface *parchemin;
SDL_Surface *treasures;
SDL_Surface *chests;
SDL_Surface *monstres;
SDL_Surface *hearts;
SDL_Surface *bossmages;
SDL_Surface *bigboss;
SDL_Surface *bigparch;
SDL_Surface *bulletsbicolor;
SDL_Surface *fragments;

SDL_Surface *screenCalcul;


nSDL_Font *fontfantasyblack;
nSDL_Font *fontfantasygrey;
nSDL_Font *fontfantasyred;
nSDL_Font *fontfantasywhite;
nSDL_Font *fonttinyblack;
nSDL_Font *fonttinygrey;
nSDL_Font *fonttinywhite;

SDL_Event event;

unsigned char ZOOMFACTOR = 1;

void initSDL(void);
void updateMainScreen( void );
void loadLevel( void );
void exitAndFree( void );
void initWalls( void );
int _sgn( int x );
int _abs( int x );
int computeLightModification( unsigned int currentCellX, unsigned int currentCellY, unsigned char* R, unsigned char* G, unsigned char* B, unsigned char* A );
void rayCast( SDL_Surface *surf, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char* Rs, unsigned char* Gs, unsigned char* Bs, unsigned char* As );
void renderMap( SDL_Surface *surf );
void renderWalls( SDL_Surface *surf );
void renderObstacles( SDL_Surface *surf, int passnumber );
void renderTreasures( SDL_Surface* surf );
void renderMovable( SDL_Surface *surf );
void renderPlayer( SDL_Surface *surf );
void renderLight( SDL_Surface *surf );
bool isValidMove( selection selected, unsigned char x, unsigned char y, orientations sens );
void gameMechanics(  selection what, orientations touche );
void renderReceptors( SDL_Surface* surf );
void checkReceptors( SDL_Surface* surf  );
void renderDoors( SDL_Surface* surf );
void checkDoors( SDL_Surface* surf );
uint32_t _getpixel(SDL_Surface *surface, int x, int y);
void getPixelRGB( SDL_Surface* surf, unsigned int x, unsigned int y, unsigned char *R1, unsigned char* G1, unsigned char *B1 );

/*
bool is_existing( const std::string& file )
{
       struct stat buffer;
       return (stat (file.c_str(), &buffer) == 0);
}
*/

void TakeScreenShot( SDL_Surface *screen )
{
       std::string namebegin = "/documents/MagicLight/Screenshots/scrsh";
       std::string nameend = ".bmp.tns";
       std::string namemiddle = "000";
       bool screenshotdone = false;
       int i=0;

       do
       {
              std::string fullname = namebegin + namemiddle + nameend;
              if (!is_existing( fullname ))
              {
                     SDL_SaveBMP( screen, fullname.c_str() );
                     screenshotdone = true;
              }
              else
              {
                     i++;
                     namemiddle = std::to_string( i );
                     if (namemiddle.length()==1)
                     {
                            namemiddle = "00" + namemiddle;
                     };
                     if (namemiddle.length()==2)
                     {
                            namemiddle = "0" + namemiddle;
                     };
              }
       }
       while (!screenshotdone);
}


void is_save_existing( void )
{
       saveexist=is_existing( filepath );
}

bool is_save_existing_boolean( void )
{
       return is_existing( filepath);
}

void delete_save( void )
{
       remove( filepath.c_str() );
}

void saveprogress( void )
{
       sizeoffile = sizeof( GameSaveStatus ) + 33*sizeof( LevelSaveStatus );
       unsigned int sizecopy = sizeoffile;

       unsigned char* buffer;
       buffer = (unsigned char*) malloc( sizeoffile );

       buffer[0] = sauvegarde.currentScore[0];
       buffer[1] = sauvegarde.currentScore[1];

       buffer[2] = sauvegarde.currentLevel;

       for (unsigned char k = 0; k<=32; k++ )
              buffer[3+k] = sauvegarde.visitedLevels[k];

       buffer[36] = sauvegarde.currentDifficulty;
       buffer[37] = sauvegarde.currentLife;
       buffer[38] = sauvegarde.redFrag;
       buffer[39] = sauvegarde.greenFrag;
       buffer[40] = sauvegarde.blueFrag;
       buffer[41] = sauvegarde.blackFrag;
       buffer[42] = sauvegarde.whiteFrag;
       buffer[43] = sauvegarde.terminator;

       ///TODO implement Room Saves

       int index;

       for (unsigned char k = 0; k<=32; k++ )
       {
              index = sizeof( GameSaveStatus )+k*sizeof( LevelSaveStatus );

              //for( int i=0; i<8; i++ )
              buffer[index+0] =  partielevel[k].chestStatusSave[ 0 ];
              buffer[index+1] =  partielevel[k].chestStatusSave[ 1 ];
              buffer[index+2] =  partielevel[k].chestStatusSave[ 2 ];
              buffer[index+3] =  partielevel[k].chestStatusSave[ 3 ];
              buffer[index+4] =  partielevel[k].chestStatusSave[ 4 ];
              buffer[index+5] =  partielevel[k].chestStatusSave[ 5 ];
              buffer[index+6] =  partielevel[k].chestStatusSave[ 6 ];
              buffer[index+7] =  partielevel[k].chestStatusSave[ 7 ];

              //for( unsigned char i=8; i<12; i++ )
              buffer[index+8] =  partielevel[k].doorStatusSave[ 0 ];
              buffer[index+9] =  partielevel[k].doorStatusSave[ 1 ];
              buffer[index+10] =  partielevel[k].doorStatusSave[ 2 ];
              buffer[index+11] =  partielevel[k].doorStatusSave[ 3 ];

              //for( unsigned char i=12; i<20; i++ )
              buffer[index+12] =  partielevel[k].bossStatusSave[ 0 ];
              buffer[index+13] =  partielevel[k].bossStatusSave[ 1 ];
              buffer[index+14] =  partielevel[k].bossStatusSave[ 2 ];
              buffer[index+15] =  partielevel[k].bossStatusSave[ 3 ];
              buffer[index+16] =  partielevel[k].bossStatusSave[ 4 ];
              buffer[index+17] =  partielevel[k].bossStatusSave[ 5 ];
              buffer[index+18] =  partielevel[k].bossStatusSave[ 6 ];
              buffer[index+19] =  partielevel[k].bossStatusSave[ 7 ];

              //for( unsigned char i=20; i<28; i++ )
              buffer[index+20] =  partielevel[k].monsterStatusSave[ 0 ];
              buffer[index+21] =  partielevel[k].monsterStatusSave[ 1 ];
              buffer[index+22] =  partielevel[k].monsterStatusSave[ 2 ];
              buffer[index+23] =  partielevel[k].monsterStatusSave[ 3 ];
              buffer[index+24] =  partielevel[k].monsterStatusSave[ 4 ];
              buffer[index+25] =  partielevel[k].monsterStatusSave[ 5 ];
              buffer[index+26] =  partielevel[k].monsterStatusSave[ 6 ];
              buffer[index+27] =  partielevel[k].monsterStatusSave[ 7 ];

              //for( unsigned char i=28; i<30; i++ )
              buffer[index+28] =  partielevel[k].redBlockSave[ 0 ];
              buffer[index+29] =  partielevel[k].redBlockSave[ 1 ];

              //for( unsigned char i=30; i<32; i++ )
              buffer[index+30] =  partielevel[k].greenBlockSave[ 0 ];
              buffer[index+31] =  partielevel[k].greenBlockSave[ 1 ];

              //for( unsigned char i=32; i<34; i++ )
              buffer[index+32] =  partielevel[k].blueBlockSave[ 0 ];
              buffer[index+33] =  partielevel[k].blueBlockSave[ 1 ];

              //for( unsigned char i=34; i<36; i++ )
              buffer[index+34] =  partielevel[k].blackBlockSave[ 0 ];
              buffer[index+35] =  partielevel[k].blackBlockSave[ 1 ];

              //for( unsigned char i=36; i<38; i++ )
              buffer[index+36] =  partielevel[k].lightSave[ 0 ];
              buffer[index+37] =  partielevel[k].lightSave[ 1 ];
       }

       /*
       int file;

       BFile_Remove( filepath );
       BFile_Create( filepath, BFile_File, &sizecopy );

       file = BFile_Open( filepath, BFile_WriteOnly );
       BFile_Write( file, buffer, sizeoffile );

       BFile_Close( file );

       */

       FILE *fptr;
       fptr = fopen( filepath.c_str(), "wb" );
       fwrite( buffer, sizeoffile, 1, fptr );
       fclose( fptr );

       free( buffer );
}

void loadprogress( void )
{
       sizeoffile = sizeof( GameSaveStatus ) + 33*sizeof( LevelSaveStatus );
       unsigned int sizecopy = sizeoffile;

       unsigned char* buffer;
       buffer = (unsigned char*) malloc( sizeoffile );


       /*
             int file;
             file = BFile_Open( filepath, BFile_ReadOnly );
             BFile_Read( file, buffer, sizeoffile, -1 );
             BFile_Close( file );
       */

       FILE *fptr;
       fptr = fopen( filepath.c_str(), "rb" );
       fread( buffer, sizeoffile, 1, fptr );
       fclose( fptr );

       sauvegarde.currentScore[0] = buffer[0];
       sauvegarde.currentScore[1] = buffer[1];

       sauvegarde.currentLevel = buffer[2];

       for (unsigned char k = 0; k<=32; k++ )
              sauvegarde.visitedLevels[k] = buffer[3+k];

       sauvegarde.currentDifficulty = buffer[36];
       sauvegarde.currentLife = buffer[37];
       sauvegarde.redFrag = buffer[38];
       sauvegarde.greenFrag = buffer[39];
       sauvegarde.blueFrag = buffer[40];
       sauvegarde.blackFrag = buffer[41];
       sauvegarde.whiteFrag = buffer[42];
       sauvegarde.terminator = buffer[43];

       int index;

       for (int k = 0; k<=32; k++ )
       {
              index = sizeof(GameSaveStatus)+k*sizeof( LevelSaveStatus );

              //for( unsigned char i=0; i<8; i++ )
              partielevel[k].chestStatusSave[ 0 ] = buffer[index+0];
              partielevel[k].chestStatusSave[ 1 ] = buffer[index+1];
              partielevel[k].chestStatusSave[ 2 ] = buffer[index+2];
              partielevel[k].chestStatusSave[ 3 ] = buffer[index+3];
              partielevel[k].chestStatusSave[ 4 ] = buffer[index+4];
              partielevel[k].chestStatusSave[ 5 ] = buffer[index+5];
              partielevel[k].chestStatusSave[ 6 ] = buffer[index+6];
              partielevel[k].chestStatusSave[ 7 ] = buffer[index+7];

              //for( unsigned char i=8; i<12; i++ )
              partielevel[k].doorStatusSave[ 0 ] = buffer[index+8];
              partielevel[k].doorStatusSave[ 1 ] = buffer[index+9];
              partielevel[k].doorStatusSave[ 2 ] = buffer[index+10];
              partielevel[k].doorStatusSave[ 3 ] = buffer[index+11];

              //for( unsigned char i=12; i<20; i++ )
              partielevel[k].bossStatusSave[ 0 ] = buffer[index+12];
              partielevel[k].bossStatusSave[ 1 ] = buffer[index+13];
              partielevel[k].bossStatusSave[ 2 ] = buffer[index+14];
              partielevel[k].bossStatusSave[ 3 ] = buffer[index+15];
              partielevel[k].bossStatusSave[ 4 ] = buffer[index+16];
              partielevel[k].bossStatusSave[ 5 ] = buffer[index+17];
              partielevel[k].bossStatusSave[ 6 ] = buffer[index+18];
              partielevel[k].bossStatusSave[ 7 ] = buffer[index+19];

              //for( unsigned char i=20; i<28; i++ )
              partielevel[k].monsterStatusSave[ 0 ] = buffer[index+20];
              partielevel[k].monsterStatusSave[ 1 ] = buffer[index+21];
              partielevel[k].monsterStatusSave[ 2 ] = buffer[index+22];
              partielevel[k].monsterStatusSave[ 3 ] = buffer[index+23];
              partielevel[k].monsterStatusSave[ 4 ] = buffer[index+24];
              partielevel[k].monsterStatusSave[ 5 ] = buffer[index+25];
              partielevel[k].monsterStatusSave[ 6 ] = buffer[index+26];
              partielevel[k].monsterStatusSave[ 7 ] = buffer[index+27];

              //for( unsigned char i=28; i<30; i++ )
              partielevel[k].redBlockSave[ 0 ] = buffer[index+28];
              partielevel[k].redBlockSave[ 1 ] = buffer[index+29];

              //for( unsigned char i=30; i<32; i++ )
              partielevel[k].greenBlockSave[ 0 ] = buffer[index+30];
              partielevel[k].greenBlockSave[ 1 ] = buffer[index+31];

              //for( unsigned char i=32; i<34; i++ )
              partielevel[k].blueBlockSave[ 0 ] = buffer[index+32];
              partielevel[k].blueBlockSave[ 1 ] = buffer[index+33];

              //for( unsigned char i=34; i<36; i++ )
              partielevel[k].blackBlockSave[ 0 ] = buffer[index+34];
              partielevel[k].blackBlockSave[ 1 ] = buffer[index+35];

              //for( unsigned char i=36; i<38; i++ )
              partielevel[k].lightSave[ 0 ] = buffer[index+36];
              partielevel[k].lightSave[ 1 ] = buffer[index+37];
       }

       free( buffer );

}

void updateCurrentState( void  )
{
       sauvegarde.currentScore[0] = score/256;
       sauvegarde.currentScore[1] = score%256;
       sauvegarde.currentLevel = currentLevel;

       for( unsigned char k=0; k<=32; k++ )
              if (Map[k].visited==true) sauvegarde.visitedLevels[k] = 1;
              else sauvegarde.visitedLevels[k] = 0;

       sauvegarde.currentDifficulty = difficulty;
       sauvegarde.currentLife = life;

       if (RedFrag==true) sauvegarde.redFrag=true;
       else sauvegarde.redFrag=false;

       if (GreenFrag==true) sauvegarde.greenFrag=true;
       else sauvegarde.greenFrag=false;

       if (BlueFrag==true) sauvegarde.blueFrag=true;
       else sauvegarde.blueFrag=false;

       if (BlackFrag==true) sauvegarde.blackFrag=true;
       else sauvegarde.blackFrag=false;

       if (WhiteFrag==true) sauvegarde.whiteFrag=true;
       else sauvegarde.whiteFrag=false;


       for( unsigned char i=0; i<NbTreasureLevel; i++ )
              if (TreasureCollection[i].isopen==false && TreasureCollection[i].isvisible==true)
                     partielevel[currentLevel].chestStatusSave[i]=0x00;
              else if (TreasureCollection[i].isopen==true && TreasureCollection[i].isvisible==true)
                     partielevel[currentLevel].chestStatusSave[i]=0x01;
              else if (TreasureCollection[i].isvisible==false)
                     partielevel[currentLevel].chestStatusSave[i]=0x02;


       for( unsigned char i=0; i<NbDoorsLevel; i++ )
              if (DoorCollection[i].isopen==true)
                     partielevel[currentLevel].doorStatusSave[i]=0x01;
              else
                     partielevel[currentLevel].doorStatusSave[i]=0x00;


       //TODO : save boss status
       //for( int i=0; i<NbBossLevel; i++ )

       //TODO : save monster status
       //for( int i=0; i<NbMonsterLevel; i++ )

       partielevel[currentLevel].redBlockSave[0]=BlocRouge->x;
       partielevel[currentLevel].redBlockSave[1]=BlocRouge->y;

       partielevel[currentLevel].greenBlockSave[0]=BlocVert->x;
       partielevel[currentLevel].greenBlockSave[1]=BlocVert->y;

       partielevel[currentLevel].blueBlockSave[0]=BlocBleu->x;
       partielevel[currentLevel].blueBlockSave[1]=BlocBleu->y;

       partielevel[currentLevel].blackBlockSave[0]=BlocNoir->x;
       partielevel[currentLevel].blackBlockSave[1]=BlocNoir->y;

       partielevel[currentLevel].lightSave[0]=lumiere.x;
       partielevel[currentLevel].lightSave[1]=lumiere.y;
}


void adjustLevel( int lev )
{

       for( int i=0; i<NbTreasureLevel; i++ )
              if (partielevel[lev].chestStatusSave[i]==0)
              {
                     TreasureCollection[i].isopen = false;
                     TreasureCollection[i].isvisible = true;
              }
              else if (partielevel[lev].chestStatusSave[i]==1)
              {
                     TreasureCollection[i].isopen = true;
                     TreasureCollection[i].isvisible = true;
              }
              else if (partielevel[lev].chestStatusSave[i]==2)
              {
                     TreasureCollection[i].isopen = true;
                     TreasureCollection[i].isvisible = false;
              }

       for( int i=0; i<NbDoorsLevel; i++ )
              if (partielevel[lev].doorStatusSave[i]==1) DoorCollection[i].isopen=true;
              else DoorCollection[i].isopen = false;

       for( int i=0; i<NbBossLevel; i++ )
       {
              //TODO : make appropriate code
       }

       for( int i=0; i<NbMonsterLevel; i++ )
       {
              //TODO : make appropriate code
       }

       BlocRouge->x = partielevel[lev].redBlockSave[0];
       BlocRouge->y = partielevel[lev].redBlockSave[1];

       BlocVert->x = partielevel[lev].greenBlockSave[0];
       BlocVert->y = partielevel[lev].greenBlockSave[1];

       BlocBleu->x = partielevel[lev].blueBlockSave[0];
       BlocBleu->y = partielevel[lev].blueBlockSave[1];

       BlocNoir->x = partielevel[lev].blackBlockSave[0];
       BlocNoir->y = partielevel[lev].blackBlockSave[1];

       lumiere.x = partielevel[lev].lightSave[0];
       lumiere.y = partielevel[lev].lightSave[1];

}


void loadLevel( unsigned char numLevel )
{
       free(BlocksCollection);
       free(ReceptorCollection);
       free(DoorCollection);
       free(TreasureCollection);
       free(ObstaclesCollection);
       free(MonstersCollection);

       BlocksCollection=NULL;
       ReceptorCollection=NULL;
       DoorCollection=NULL;
       TreasureCollection=NULL;
       ObstaclesCollection=NULL;
       MonstersCollection=NULL;

       attackboss=false;
       bool needcorrection=false;



       // if (sauvegarde.visitedLevels[numLevel]==1)   // The level to be loaded has already been played so we will have to make corrections as per save state
       if (Map[numLevel].visited==true)       needcorrection = true;
       else needcorrection = false;

       if (numLevel==0) // VALIDATED [OK]
       {
              currentLevel = numLevel;

              LEVELCOLOR = 0;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 1;
              NbBlocksLevel = 4;
              NbTreasureLevel = 0;
              NbObstacleLevel = 16;
              NbMonsterLevel = 0;
              NbBossLevel = 0;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=0;
              Map[numLevel].y=0;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=false;
              Map[numLevel].doorNorth=true;
              Map[numLevel].doorEast=false;
              Map[numLevel].doorSouth=false;

              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              ObstaclesCollection[0] = {4,1,BLOCK_STONE};
              ObstaclesCollection[1] = {8,1,BLOCK_STONE};
              ObstaclesCollection[2] = {1,3,BLOCK_STONE};
              ObstaclesCollection[3] = {11,3,BLOCK_STONE};
              ObstaclesCollection[4] = {1,7,BLOCK_STONE};
              ObstaclesCollection[5] = {11,7,BLOCK_STONE};
              ObstaclesCollection[6] = {4,9,BLOCK_STONE};
              ObstaclesCollection[7] = {8,9,BLOCK_STONE};

              ObstaclesCollection[8] = {5,4,BLOCK_WATER};
              ObstaclesCollection[9] = {6,4,BLOCK_WATER};
              ObstaclesCollection[10] = {7,4,BLOCK_WATER};
              ObstaclesCollection[11] = {5,5,BLOCK_WATER};
              ObstaclesCollection[12] = {7,5,BLOCK_WATER};
              ObstaclesCollection[13] = {5,6,BLOCK_WATER};
              ObstaclesCollection[14] = {6,6,BLOCK_WATER};
              ObstaclesCollection[15] = {7,6,BLOCK_WATER};


              BlocksCollection[0] = { 2, 4, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 10, 4, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 10, 6, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 2, 6, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 2, 0, BAS, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 10, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 10, 10, HAUT, 0, 0, 255, false }; // Receptor BLUE
              ReceptorCollection[3] = { 2, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
              // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              DoorCollection[0] = { 6,0, HAUT, false, true, 1, { R_ROUGE, -1, -1 }, 2}; // Door NORTH
              //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
              //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
              //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

              //TreasureCollection[0] = {1,1, T_RED, false, WHITE_FRAGMENT, 100, true };
              //TreasureCollection[1] = {2,1, T_YELLOW, false, BLACK_FRAGMENT, 100, true };
              //TreasureCollection[2] = {3,1, T_GREEN, false, RED_FRAGMENT, 100, true };
              //TreasureCollection[3] = {4,1, T_BLUE, false, GREEN_FRAGMENT, 100, true };
              //TreasureCollection[4] = {5,1, T_BLUE, false, BLUE_FRAGMENT, 100, true };

              //MonstersCollection[0] = { 6, 4, 2, 10, HORIZONTAL, GAUCHE, BLOB };
              //MonstersCollection[1] = { 7, 5, 2, 8, VERTICAL, HAUT, SKELETON };
       }
       else if (numLevel==1) // VALIDATED [OK]
       {
              currentLevel = numLevel;

              LEVELCOLOR = 1;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 2;
              NbBlocksLevel = 4;
              NbTreasureLevel = 0;
              NbObstacleLevel = 16;
              NbMonsterLevel = 3;
              NbBossLevel = 0;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=-1;
              Map[numLevel].y=-1;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=false;
              Map[numLevel].doorNorth=true;
              Map[numLevel].doorEast=true;
              Map[numLevel].doorSouth=false;

              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              ObstaclesCollection[0] = {1,1,BLOCK_STONE};
              ObstaclesCollection[1] = {11,1,BLOCK_STONE};
              ObstaclesCollection[2] = {1,9,BLOCK_STONE};
              ObstaclesCollection[3] = {11,9,BLOCK_STONE};

              ObstaclesCollection[4] = {1,2,BLOCK_WATER};
              ObstaclesCollection[5] = {2,1,BLOCK_WATER};
              ObstaclesCollection[6] = {2,2,BLOCK_WATER};

              ObstaclesCollection[7] = {10,1,BLOCK_WATER};
              ObstaclesCollection[8] = {10,2,BLOCK_WATER};
              ObstaclesCollection[9] = {11,2,BLOCK_WATER};

              ObstaclesCollection[10] = {10,9,BLOCK_WATER};
              ObstaclesCollection[11] = {10,8,BLOCK_WATER};
              ObstaclesCollection[12] = {11,8,BLOCK_WATER};

              ObstaclesCollection[13] = {1,8,BLOCK_WATER};
              ObstaclesCollection[14] = {2,8,BLOCK_WATER};
              ObstaclesCollection[15] = {2,9,BLOCK_WATER};

              BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
              ReceptorCollection[3] = { 6, 10, HAUT, 0, 0, 0, false }; // Receptor BLACK
              // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              DoorCollection[0] = { 6,0, HAUT, false, true, 2, { R_ROUGE,R_BLEU, -1 }, 4}; // Door NORTH
              //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
              DoorCollection[1] = { 12, 5, DROITE, false, true, 2, { R_VERT, R_NOIR, -1 }, 2}; // Door EAST
              //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

              //TreasureCollection[0] = {2,2, T_RED, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[1] = {11,9, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[2] = {10,4, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100,  true };

              MonstersCollection[0] = { 6, 2, 1, 11, HORIZONTAL, DROITE, BAT };
              MonstersCollection[1] = { 1, 5, 1, 9, VERTICAL, BAS, SPIDER };
              MonstersCollection[2] = { 10, 5, 1, 9, VERTICAL, HAUT, GHOST };

       }
       else if (numLevel==2) // VALIDATED [OK]
       {
              currentLevel = numLevel;

              LEVELCOLOR = 2;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 3;
              NbBlocksLevel = 4;
              NbTreasureLevel = 4;
              NbObstacleLevel = 24;
              NbMonsterLevel = 0;
              NbBossLevel = 0;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=0;
              Map[numLevel].y=-1;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=true;
              Map[numLevel].doorNorth=false;
              Map[numLevel].doorEast=true;
              Map[numLevel].doorSouth=true;

              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              ObstaclesCollection[0] = {1,4,BLOCK_STONE};
              ObstaclesCollection[1] = {2,4,BLOCK_STONE};
              ObstaclesCollection[2] = {1,6,BLOCK_STONE};
              ObstaclesCollection[3] = {2,6,BLOCK_STONE};
              ObstaclesCollection[4] = {10,4,BLOCK_STONE};
              ObstaclesCollection[5] = {11,4,BLOCK_STONE};
              ObstaclesCollection[6] = {10,6,BLOCK_STONE};
              ObstaclesCollection[7] = {11,6,BLOCK_STONE};
              ObstaclesCollection[8] = {5,8,BLOCK_STONE};
              ObstaclesCollection[9] = {5,9,BLOCK_STONE};
              ObstaclesCollection[10] = {7,8,BLOCK_STONE};
              ObstaclesCollection[11] = {7,9,BLOCK_STONE};
              ObstaclesCollection[12] = {5,2,BLOCK_WATER};
              ObstaclesCollection[13] = {6,2,BLOCK_WATER};
              ObstaclesCollection[14] = {7,2,BLOCK_WATER};
              ObstaclesCollection[15] = {5,3,BLOCK_WATER};
              ObstaclesCollection[16] = {7,3,BLOCK_WATER};
              ObstaclesCollection[17] = {5,4,BLOCK_WATER};
              ObstaclesCollection[18] = {7,4,BLOCK_WATER};
              ObstaclesCollection[19] = {5,5,BLOCK_WATER};
              ObstaclesCollection[20] = {7,5,BLOCK_WATER};
              ObstaclesCollection[21] = {5,6,BLOCK_WATER};
              ObstaclesCollection[22] = {6,6,BLOCK_WATER};
              ObstaclesCollection[23] = {7,6,BLOCK_WATER};

              BlocksCollection[0] = { 2, 3, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 3, 1, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 10, 3, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 9, 1, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 0, 3, DROITE, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 2, 0, BAS, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 12, 3, GAUCHE, 0, 0, 255, false }; // Receptor BLUE
              ReceptorCollection[3] = { 10, 0, BAS, 0, 0, 0, false }; // Receptor BLACK
              //ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              //ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              //ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              //ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              //DoorCollection[0] = { 6,0, HAUT, false, true, 1, { R_ROUGE,R_BLEU, R_VERT }, 1}; // Door NORTH
              DoorCollection[0] = { 0, 5, GAUCHE, false, true, 2, { R_ROUGE, R_VERT, -1 }, 1}; // Door WEST
              DoorCollection[1] = { 12, 5, DROITE, false, true, 2, { R_BLEU, R_NOIR, -1 }, 3}; // Door EAST
              DoorCollection[2] = { 6, 10, BAS, false, true, 2, { R_ROUGE, R_BLEU, -1 }, 0}; // Door SOUTH

              TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100,  true };
              TreasureCollection[1] = {11,1, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
              TreasureCollection[2] = {1,9, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
              TreasureCollection[3] = {11,9, T_BLUE, false, PIERRE_BLANCHE, 100,  true };

       }
       else if (numLevel==3)
       {
              currentLevel = numLevel;

              LEVELCOLOR = 3;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 2;
              NbBlocksLevel = 4;
              NbTreasureLevel = 0;
              NbObstacleLevel = 8;
              NbMonsterLevel = 0;
              NbBossLevel = 0;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=1;
              Map[numLevel].y=-1;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=true;
              Map[numLevel].doorNorth=true;
              Map[numLevel].doorEast=false;
              Map[numLevel].doorSouth=false;

              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              ObstaclesCollection[0] = {2,2,BLOCK_SUN};
              ObstaclesCollection[1] = {10,2,BLOCK_SUN};
              ObstaclesCollection[2] = {2,8,BLOCK_SUN};
              ObstaclesCollection[3] = {10,8,BLOCK_SUN};
              ObstaclesCollection[4] = {4,5,BLOCK_WATER};
              ObstaclesCollection[5] = {8,5,BLOCK_WATER};
              ObstaclesCollection[6] = {6,3,BLOCK_WATER};
              ObstaclesCollection[7] = {6,7,BLOCK_WATER};

              BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
              // ReceptorCollection[3] = { 10, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
              // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              ReceptorCollection[3] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 5}; // Door NORTH
              DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, 3, -1 }, 2}; // Door WEST
              //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
              //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

              //TreasureCollection[0] = {2,2, T_RED, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[1] = {11,9, T_YELLOW, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[2] = {10,4, T_GREEN, false, PIERRE_BLANCHE, 100,  true };
              //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100,  true };

       }
       else if (numLevel==4)
       {
              currentLevel = numLevel;

              LEVELCOLOR = 0;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 1;
              NbBlocksLevel = 4;
              NbTreasureLevel = 0;
              NbObstacleLevel = 0;
              NbMonsterLevel = 0;
              NbBossLevel = 1;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=-1;
              Map[numLevel].y=-2;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=false;
              Map[numLevel].doorNorth=true;
              Map[numLevel].doorEast=false;
              Map[numLevel].doorSouth=true;

              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              //ObstaclesCollection[0] = {2,2,BLOCK_SUN};
              //ObstaclesCollection[1] = {10,2,BLOCK_SUN};
              //ObstaclesCollection[2] = {2,8,BLOCK_SUN};
              //ObstaclesCollection[3] = {10,8,BLOCK_SUN};
              //ObstaclesCollection[4] = {4,5,BLOCK_WATER};
              //ObstaclesCollection[5] = {8,5,BLOCK_WATER};
              //ObstaclesCollection[6] = {6,3,BLOCK_WATER};
              //ObstaclesCollection[7] = {6,7,BLOCK_WATER};

              BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
              ReceptorCollection[3] = { 12, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
              // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              //DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 2}; // Door NORTH
              //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
              //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
              DoorCollection[0] = { 6, 10, BAS, false, true, 1, { R_NOIR, -1, -1 }, 1}; // Door SOUTH

              //TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[1] = {11,9, T_YELLOW, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[2] = {10,4, T_GREEN, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100, true };

              BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_RED };
              //BossCollection[1] = { 2, 5, 2, 8, VERTICAL, HAUT, B_GREEN };
              //BossCollection[2] = { 6, 9, 2, 10, HORIZONTAL, GAUCHE, B_BLUE };
              //BossCollection[3] = { 10, 5, 2, 8, VERTICAL, HAUT, B_BLACK };

       }
       else if (numLevel==5)
       {
              currentLevel = numLevel;

              LEVELCOLOR = 0;
              SIZE_MAP_X=13;
              SIZE_MAP_Y=11;

              NbReceptorsLevel = 4;
              NbDoorsLevel = 1;
              NbBlocksLevel = 4;
              NbTreasureLevel = 0;
              NbObstacleLevel = 0;
              NbMonsterLevel = 0;
              NbBossLevel = 1;

              BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
              ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
              DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
              TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
              ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
              MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
              BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

              Map[numLevel].x=1;
              Map[numLevel].y=-2;
              Map[numLevel].R=255;
              Map[numLevel].G=255;
              Map[numLevel].B=255;
              Map[numLevel].A=255;
              Map[numLevel].visited=true;
              Map[numLevel].doorWest=false;
              Map[numLevel].doorNorth=true;
              Map[numLevel].doorEast=false;
              Map[numLevel].doorSouth=true;


              joueur = { 6, 9, HAUT };
              lumiere = { 6, 5, 1,1,1,0, false };

              //ObstaclesCollection[0] = {2,2,BLOCK_SUN};
              //ObstaclesCollection[1] = {10,2,BLOCK_SUN};
              //ObstaclesCollection[2] = {2,8,BLOCK_SUN};
              //ObstaclesCollection[3] = {10,8,BLOCK_SUN};
              //ObstaclesCollection[4] = {4,5,BLOCK_WATER};
              //ObstaclesCollection[5] = {8,5,BLOCK_WATER};
              //ObstaclesCollection[6] = {6,3,BLOCK_WATER};
              //ObstaclesCollection[7] = {6,7,BLOCK_WATER};

              BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
              BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
              BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
              BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

              BlocRouge = &BlocksCollection[0];
              BlocVert = &BlocksCollection[1];
              BlocBleu = &BlocksCollection[2];
              BlocNoir = &BlocksCollection[3];

              ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
              ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
              ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
              ReceptorCollection[3] = { 12, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
              // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
              // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
              // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
              // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

              //DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 2}; // Door NORTH
              //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
              //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
              DoorCollection[0] = { 6, 10, BAS, false, true, 1, { R_NOIR, -1, -1 }, 3}; // Door SOUTH

              //TreasureCollection[0] = {1,1, T_RED, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[1] = {11,9, T_YELLOW, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[2] = {10,4, T_GREEN, false, PIERRE_BLANCHE, 100, true };
              //TreasureCollection[3] = {1,1, T_BLUE, true, PIERRE_BLANCHE, 100, true };

              //BossCollection[0] = { 6, 3, 2, 10, HORIZONTAL, GAUCHE, BIGBOSS };
              BossCollection[0] = { 6, 3, 2, 10, HORIZONTAL, GAUCHE, B_BLUE };
       }

       if (needcorrection==true)
              adjustLevel( numLevel );
}


void synchroniseGame( void )
{
       score = sauvegarde.currentScore[0]*256+sauvegarde.currentScore[1];
       currentLevel = sauvegarde.currentLevel;
       difficulty = sauvegarde.currentDifficulty;

       life = sauvegarde.currentLife;

       RedFrag = sauvegarde.redFrag==1 ? true : false;
       GreenFrag = sauvegarde.greenFrag==1 ? true : false;
       BlueFrag = sauvegarde.blueFrag==1 ? true : false;
       BlackFrag = sauvegarde.blackFrag==1 ? true : false;
       WhiteFrag = sauvegarde.whiteFrag==1 ? true : false;

       for( unsigned char k =0; k<33; k++)
       {
              if (sauvegarde.visitedLevels[k]==0x01)
              {
                     loadLevel( k );
              }
              else Map[k].visited= false;
       }
}


void updateTreasures( selection what )
{
       if (what==JOUEUR)
       {
              unsigned char X = joueur.x;
              unsigned char Y= joueur.y;

              for (unsigned char k=0; k<NbTreasureLevel; k++)
              {
                     if (X==TreasureCollection[k].x && Y==TreasureCollection[k].y)
                     {
                            if (TreasureCollection[k].isopen==false)
                            {
                                   score+=TreasureCollection[k].scoreboost;
                                   TreasureCollection[k].isopen=true;

                                   if (TreasureCollection[k].inside == BLACK_FRAGMENT)  BlackFrag=true;
                                   if (TreasureCollection[k].inside == WHITE_FRAGMENT)  WhiteFrag=true;
                                   if (TreasureCollection[k].inside == RED_FRAGMENT)  RedFrag=true;
                                   if (TreasureCollection[k].inside == GREEN_FRAGMENT)  GreenFrag=true;
                                   if (TreasureCollection[k].inside == BLUE_FRAGMENT)  BlueFrag=true;

                                   if (TreasureCollection[k].inside == HEART_SINGLE)
                                   {
                                          if (difficulty==0 && life<5) life++;
                                          else if (difficulty==1 && life<3) life++;
                                          else if (difficulty==2 && life<1) life++;    // this is just theorical as Life < 1 means that you loose !!
                                   }

                                   if (TreasureCollection[k].inside == HEART_FULL)
                                   {
                                          if (difficulty==0) life=5;
                                          else if (difficulty==1) life=3;
                                          else if (difficulty==2) life=1;    // this is just theorical as Life < 1 means that you loose !!
                                   }

                            }
                            else
                            {
                                   TreasureCollection[k].isvisible=false;
                            }
                     }
              }
       }
}

void get_inputs_option(void)
{
       KeyManager::Logic();
       MouseManager::Logic();

       if (KeyManager::kbESC_Release_Event())
       {
              doneOption = true;
       }
       if (MouseManager::kbUP_Press_Event() && selectOptionMenu>0)
       {
              selectOptionMenu--;
       }
       if (MouseManager::kbDOWN_Press_Event() && selectOptionMenu<3)
       {
              selectOptionMenu++;
       }
       if (MouseManager::kbLEFT() && selectOptionMenu==1)
       {
              if (ALPHA_TRANSPARENT>0)
                     ALPHA_TRANSPARENT--;
       }
       if (MouseManager::kbRIGHT()  && selectOptionMenu==1)
       {
              if (ALPHA_TRANSPARENT<255)
                     ALPHA_TRANSPARENT++;
       }
       if (MouseManager::kbLEFT()  && selectOptionMenu==2)
       {
              if (ALPHA_OPAQUE>0)
                     ALPHA_OPAQUE--;
       }
       if (MouseManager::kbRIGHT()  && selectOptionMenu==2)
       {
              if (ALPHA_OPAQUE<255)
                     ALPHA_OPAQUE++;
       }
       if (KeyManager::kbENTER_Press_Event()  && selectOptionMenu==3)
       {
              ALPHA_OPAQUE = ALPHA_OPAQUE_DEFAULT;
              ALPHA_TRANSPARENT = ALPHA_TRANSPARENT_DEFAULT;
       }
       if (KeyManager::kbENTER_Release_Event()  && selectOptionMenu!=3)
       {
              doneOption = true;
       }
       if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

void drawOption( void )
{
       doneOption = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       unsigned int lX = 0;
       unsigned int lY = 0;

       frame_light = 0;

       SDL_Rect source, dest;


       while (!doneOption)
       {
              //dimage( 0, 10, &maintitle);

              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 85, "Back to Game");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 84, "Back to Game");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 100, "Adjust Color Alpha");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 99, "Adjust Color Alpha");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 115, "Adjust Black Alpha");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 114, "Adjust Black Alpha");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 130, "Set Default Settings");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 129, "Set Default Settings");

              boxRGBA( screenCalcul,180, 95,  213, 107,  255, 0, 0, ALPHA_TRANSPARENT );
              boxRGBA( screenCalcul,214, 95,  246, 107,  0, 255, 0, ALPHA_TRANSPARENT );
              boxRGBA(screenCalcul, 247, 95,  280, 107,  0, 0, 255, ALPHA_TRANSPARENT );
              rectangleRGBA(screenCalcul,180, 95,  280, 107,  255, 255, 255, 255);

              nSDL_DrawString(screenCalcul, fonttinywhite, 225, 97, "%d", ALPHA_TRANSPARENT);

              boxRGBA( screenCalcul,180, 110,  280, 122,  0, 0, 0, ALPHA_OPAQUE );
              rectangleRGBA(screenCalcul, 180, 110,  280, 122,  255, 255, 255, 255);
              nSDL_DrawString(screenCalcul, fonttinywhite, 225, 112, "%d", ALPHA_OPAQUE);



              if (selectOptionMenu == 0)
              {
                     lX = 30;
                     lY = 79;
              }
              else if (selectOptionMenu == 1)
              {
                     lX = 30;
                     lY = 94;
              }
              else if (selectOptionMenu == 2)
              {
                     lX = 30;
                     lY = 109;
              }
              else if (selectOptionMenu == 3)
              {
                     lX = 30;
                     lY = 124;
              }

              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;

              frame_light%=6;

              source.x = 16*frame_light;
              source.y = 0;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;

              frame_light++;

              SDL_BlitSurface( light, &source, screenCalcul, &dest );

              /*
                            dfont( &font_tiny );
                            dprint( 90, 160, C_RGB(150,150,150), "[UP] [DOWN] : select / [LEFT] [RIGHT] : adjust");
                            dprint( 89, 159, C_BLACK, "[UP] [DOWN] : select / [LEFT] [RIGHT] : adjust");
                            dprint( 90, 170, C_RGB(150,150,150), "[EXE] : validate / [EXIT] : escape");
                            dprint( 89, 169, C_BLACK,"[EXE] : validate / [EXIT] : escape");
              */

              nSDL_DrawString(screenCalcul, fonttinygrey, 50, 150, "[UP/DOWN] select [LEFT/RIGHT] adjust");
              nSDL_DrawString(screenCalcul, fonttinyblack, 49, 149, "[UP/DOWN] select [LEFT/RIGHT] adjust");
              nSDL_DrawString(screenCalcul, fonttinygrey, 50, 160, "[ENTER] validate [EXIT] escape");
              nSDL_DrawString(screenCalcul, fonttinyblack, 49, 159,  "[ENTER] validate [EXIT] escape");

              //dupdate();

              SDL_Flip( screenCalcul );

              get_inputs_option();
       }
}

void get_inputs_quit(void)
{
       KeyManager::Logic();
       MouseManager::Logic();

       if (MouseManager::kbUP_Press_Event() && selectOptionPause>0)
       {
              selectOptionPause--;
       }
       if (MouseManager::kbDOWN_Press_Event() && selectOptionPause<2)
       {
              selectOptionPause++;
       }
       if (KeyManager::kbENTER_Release_Event()  )
       {
              donePause = true;
       }
       if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

void drawQuit( void )
{
       donePause = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       unsigned int lX = 0;
       unsigned int lY = 0;

       frame_light = 0;
       selectOptionPause = 0;

       SDL_Rect source, dest;

       while (!donePause)
       {

              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 85,  "Game Currently in Pause Mode");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 84,  "Game Currently in Pause Mode");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 95,  "Are You Sure You Want To Leave ?");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 94,  "Are You Sure You Want To Leave ?");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 115,  "No - Get Me Back To Game");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 114, "No - Get Me Back To Game");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 130, "Yes - For Now, But I'll Come Back !");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 129, "Yes - For Now, But I'll Come Back !");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 145, "Please, Send Me Back to Main Menu");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 144, "Please, Send Me Back to Main Menu");


              if (selectOptionPause == 0)
              {
                     lX = 30;
                     lY = 109;
                     GotoMainMenu=false;
              }
              else if (selectOptionPause == 1)
              {
                     lX = 30;
                     lY = 124;
                     GotoMainMenu=false;
              }
              else if (selectOptionPause == 2)
              {
                     lX = 30;
                     lY = 139;
                     GotoMainMenu=true;
              }

              frame_light%=6;

              source.x = 16*frame_light;
              source.y = 0;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;
              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;

              frame_light++;

              SDL_BlitSurface( light, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fonttinygrey, 25, 160, "[UP/DOWN] select [ENTER] validate");
              nSDL_DrawString(screenCalcul, fonttinyblack, 24, 159,   "[UP/DOWN] select [ENTER] validate");

              SDL_Flip( screenCalcul );

              get_inputs_quit();

       }
}

void get_inputs_loose(void)
{

       KeyManager::Logic();
       MouseManager::Logic();

       if (MouseManager::kbUP_Press_Event() && selectOptionLoose>0)
       {
              selectOptionLoose--;
       }
       if (MouseManager::kbDOWN_Press_Event() && selectOptionLoose<1)
       {
              selectOptionLoose++;
       }
       if (KeyManager::kbENTER_Release_Event()  )
       {
              doneLoose = true;
       }
       if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

static int drawLoose( void )
{
       doneLoose = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       SDL_Rect source, dest;

       unsigned int lX = 0;
       unsigned int lY = 0;

       frame_light = 0;
       selectOptionLoose = 0;

       while (!doneLoose)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 85,  "You Loose : Be Careful Of the Monsters" );
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 84,  "You Loose : Be Careful Of the Monsters" );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 95,  "RetryThis Level ?" );
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 94,  "RetryThis Level ?" );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 115,  "Yes - I Never Give Up !!!");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 114, "Yes - I Never Give Up !!!");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 130, "No - Better If I Quit !!! ");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 129, "No - Better If I Quit !!! ");


              if (selectOptionLoose == 0)
              {
                     lX = 30;
                     lY = 109;
              }
              else if (selectOptionLoose == 1)
              {
                     lX = 30;
                     lY = 124;
              }

              frame_light%=6;

              source.x = 16*frame_light;
              source.y = 0;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;
              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;

              frame_light++;

              SDL_BlitSurface( light, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fonttinygrey, 25, 160, "[UP/DOWN] select [ENTER] validate");
              nSDL_DrawString(screenCalcul, fonttinyblack, 24, 159,   "[UP/DOWN] select [ENTER] validate");

              SDL_Flip( screenCalcul );

              get_inputs_loose();

       }

       return selectOptionLoose;
}

void get_inputs_difficultymenu(void)
{

       KeyManager::Logic();
       MouseManager::Logic();

       if (MouseManager::kbUP_Press_Event() && selectStartMenu>0)
       {
              selectDifficultyMenu--;
       }
       if (MouseManager::kbDOWN_Press_Event() && selectStartMenu<2)
       {
              selectDifficultyMenu++;
       }
       if (KeyManager::kbENTER_Release_Event()  )
       {
              doneDifficulty = true;
       }
              if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

unsigned char drawDifficultyMenu( void )
{
       doneDifficulty = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       SDL_Rect source, dest;

       unsigned int lX = 0;
       unsigned int lY = 0;

       frame_light = 0;

       while (!doneDifficulty)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 90,  "Sweet Like a Cherry Cake");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 89,  "Sweet Like a Cherry Cake");


              for( unsigned char k = 0; k< 5; k++)
              {
                     //dsubimage( 250 + k*8, 100, &hearts, 8, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 90;
                     source.x = 8;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );

              }
              for( unsigned char k = 5; k< 5; k++)
              {
                     //dsubimage( 250 + k*8, 100, &hearts, 0, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 90;
                     source.x = 0;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );
              }

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 110,  "Acid Like an Orange Juice");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 109,  "Acid Like an Orange Juice");

              //dfont( &font_fantasy );
              //dprint( 90, 120, C_RGB(150,150,150), "Acid Like an Orange Juice");
              //dprint( 89, 119, C_BLACK, "Acid Like an Orange Juice");

              for( unsigned char k = 0; k< 3; k++)
              {
                     //dsubimage( 250 + k*8, 120, &hearts, 8, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 110;
                     source.x = 8;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );
              }
              for( unsigned char k = 3; k< 5; k++)
              {
                     //dsubimage( 250 + k*8, 120, &hearts, 0, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 110;
                     source.x = 0;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );
              }

              //dfont( &font_fantasy );
              //dprint( 90, 140, C_RGB(150,150,150), "Bitter Like a Dark Beer");
              //dprint( 89, 139, C_BLACK, "Bitter Like a Dark Beer");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 130,  "Bitter Like a Dark Beer");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 129,  "Bitter Like a Dark Beer");


              for( unsigned char k = 0; k< 1; k++)
              {
                     //dsubimage( 250 + k*8, 140, &hearts, 8, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 130;
                     source.x = 8;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );
              }
              for( unsigned char k = 1; k< 5; k++)
              {
                     //dsubimage( 250 + k*8, 140, &hearts, 0, 0, 8, 8,  DIMAGE_NONE);
                     dest.w = 8;
                     dest.h = 8;
                     source.w = 8;
                     source.h = 8;
                     dest.x = 230+k*8;
                     dest.y = 130;
                     source.x = 0;
                     source.y = 0;
                     SDL_BlitSurface( hearts, &source, screenCalcul, &dest );
              }




              if (selectDifficultyMenu == 0)
              {
                     lX = 30;
                     lY = 84;
              }
              else if (selectDifficultyMenu == 1)
              {
                     lX = 30;
                     lY = 104;
              }
              else if (selectDifficultyMenu == 2)
              {
                     lX = 30;
                     lY = 124;
              }

              frame_light%=6;

              source.x = 16*frame_light;
              source.y = 0;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;
              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;

              frame_light++;

              SDL_BlitSurface( light, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fonttinygrey, 50, 160, "[UP/DOWN] select [ENTER] validate");
              nSDL_DrawString(screenCalcul, fonttinyblack, 49, 159,   "[UP/DOWN] select [ENTER] validate");

              SDL_Flip( screenCalcul );

              get_inputs_difficultymenu();
       }

       return selectDifficultyMenu;
}

void get_inputs_startmenu(void)
{

       KeyManager::Logic();
       MouseManager::Logic();

       if (KeyManager::kbESC_Release_Event())
       {
              doneLoose = true;
              selectOptionLoose = 1;
       }
       if (MouseManager::kbUP_Press_Event() && saveexist==false && selectStartMenu>1)
       {
              selectStartMenu--;
       }
       if (MouseManager::kbUP_Press_Event() && saveexist==true && selectStartMenu>0)
       {
              selectStartMenu--;
       }
       if (MouseManager::kbDOWN_Press_Event() && selectOptionLoose<2)
       {
              selectStartMenu++;
       }
       if (KeyManager::kbENTER_Release_Event()  )
       {
              doneStart = true;
       }
              if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

unsigned char drawStartMenu( void )
{
       doneStart = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       unsigned int lX = 0;
       unsigned int lY = 0;

       SDL_Rect source, dest;

       frame_light = 0;

       saveexist = false;

       is_save_existing();

       if (saveexist==false) selectStartMenu = 1;
       else selectStartMenu = 0;

       while (!doneStart)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );


              if (saveexist==false)
              {
                     //dfont( &font_fantasy );
                     //dprint( 80, 100, C_RGB(150,150,150), "Continue Previous Game");
                     //dprint( 79, 99, C_RGB(240,240,240), "Continue Previous Game");

                     //dfont( &font_fantasy );
                     //dprint( 80, 120, C_RGB(150,150,150), "Start New Game");
                     //dprint( 79, 119, C_BLACK, "Start New Game");

                     nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 90,  "Continue Previous Game");
                     nSDL_DrawString(screenCalcul, fontfantasywhite, 49, 89,  "Continue Previous Game");

                     nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 110,  "New Game");
                     nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 109,  "New Game");

              }
              else
              {
                     //dfont( &font_fantasy );
                     //dprint( 80, 100, C_RGB(150,150,150), "Continue Previous Game");
                     //dprint( 79, 99, C_BLACK, "Continue Previous Game");

                     //dfont( &font_fantasy );
                     //dprint( 80, 120, C_RGB(150,150,150), "Start New Game (Will Trash Previous Save)");
                     //dprint( 79, 119, C_RED, "Start New Game (Will Trash Previous Save)");

                     nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 90,  "Continue Previous Game");
                     nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 89,  "Continue Previous Game");

                     nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 110,  "New Game (Will Trash Previous Save)");
                     nSDL_DrawString(screenCalcul, fontfantasyred, 49, 109,  "New Game (Will Trash Previous Save)");

              }


              //dfont( &font_fantasy );
              //dprint( 80, 140, C_RGB(150,150,150), "Quit And Back To OS");
              //dprint( 79, 139, C_BLACK, "Quit And Back To OS");

              nSDL_DrawString(screenCalcul, fontfantasygrey, 50, 130,  "Quit And Back To OS");
              nSDL_DrawString(screenCalcul, fontfantasyblack, 49, 129, "Quit And Back To OS");

              if (selectStartMenu == 0)
              {
                     lX = 30;
                     lY = 84;
              }
              else if (selectStartMenu == 1)
              {
                     lX = 30;
                     lY = 104;
              }
              else if (selectStartMenu == 2)
              {
                     lX = 30;
                     lY = 124;
              }

              frame_light%=6;

              source.x = 16*frame_light;
              source.y = 0;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;
              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;

              frame_light++;

              SDL_BlitSurface( light, &source, screenCalcul, &dest );

              nSDL_DrawString(screenCalcul, fonttinygrey, 55, 160, "[UP/DOWN] select [EXE] validate");
              nSDL_DrawString(screenCalcul, fonttinyblack, 54, 159,   "[UP/DOWN] select [EXE] validate");

              SDL_Flip( screenCalcul );

              get_inputs_startmenu();
       }

       return selectStartMenu;
}

static int get_inputs_story(void)
{
       KeyManager::Logic();
       MouseManager::Logic();

       if (KeyManager::kbENTER_Release_Event() )
       {
              doneStory = true;
       }
              if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

void drawStoryMenu( void )
{
       doneStory = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       SDL_Rect source, dest;

       selectStoryMenu = 1;

       while (!doneStory)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = bigparch->w;
              dest.h = bigparch->h;
              source.w = bigparch->w;
              source.h = bigparch->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( bigparch, &source, screenCalcul, &dest );

              /*
              dfont( &font_fantasy );
              dprint( 70, 45, C_RGB(150,150,150), "A long time ago, in a peaceful and colorful ");
              dprint( 69, 44, C_BLACK, "A long time ago, in a peaceful and colorful");

              dfont( &font_fantasy );
              dprint( 55, 57, C_RGB(150,150,150), "village came a Dark Sorcerer with the terrible");
              dprint( 54, 56, C_BLACK, "village came a Dark Sorcerer with the terrible");

              dfont( &font_fantasy );
              dprint( 55, 69, C_RGB(150,150,150), "ambition of stoling all lights and colors. Making" );
              dprint( 54, 68, C_BLACK, "ambition of stoling all lights and colors. Making " );

              dfont( &font_fantasy );
              dprint( 60, 81, C_RGB(150,150,150), "darkness and fear the only remaining things " );
              dprint( 59, 80, C_BLACK,  "darkness and fear the only remaining things" );

              dfont( &font_fantasy );
              dprint( 60, 93, C_RGB(150,150,150), "here ... The Sorcerer returned to his Castle" );
              dprint( 59, 92, C_BLACK,  "here ... The Sorcerer returned to his Castle" );

              dfont( &font_fantasy );
              dprint( 60, 105, C_RGB(150,150,150), "and ruled his Daughters to protect his loot..." );
              dprint( 59, 104, C_BLACK,  "and ruled his Daughters to protect his loot..." );

              dfont( &font_fantasy );
              dprint( 65, 117, C_RGB(150,150,150), "Guided only by courage, you entered the" );
              dprint( 64, 116, C_BLACK, "Guided only by courage, you entered the" );

              dfont( &font_fantasy );
              dprint( 65, 129, C_RGB(150,150,150), "castle to defeat all dangers and help the" );
              dprint( 64, 128, C_BLACK,  "castle to defeat all dangers and help the" );

              dfont( &font_fantasy );
              dprint( 65, 141, C_RGB(150,150,150), "villagers ... Good luck my Friend !!!" );
              dprint( 64, 140, C_BLACK,  "villagers ... Good luck my Friend !!!" );
              */


              nSDL_DrawString(screenCalcul, fonttinygrey, 85, 160, "[ENTER] skip");
              nSDL_DrawString(screenCalcul, fonttinyblack, 84, 159,    "[ENTER] skip");

              SDL_Flip( screenCalcul );

              get_inputs_story();
       }
}

void launch_Boss_Attack( void )
{

       SDL_Rect source, dest;

       if (frame_boss_attack==0)
       {
              attackboss=true;
              xbossattack=BossCollection[0].xcur*SIZE+OFFSET_X+SIZE/2;
              ybossattack=BossCollection[0].ycur*SIZE+OFFSET_Y+SIZE/2;

              srand( SDL_GetTicks() );
              attacktype = rand() % 8;
              attackpattern = rand() % 2;

              if (attacktype==0) NbBulletLevel=5;
              else if (attacktype==1) NbBulletLevel=10;
              else if (attacktype==2) NbBulletLevel=20;
              else if (attacktype==3) NbBulletLevel=10;
              else if (attacktype==4) NbBulletLevel=20;
              else if (attacktype==5) NbBulletLevel=10;
              else if (attacktype==6) NbBulletLevel=20;
              else NbBulletLevel=40;

              free( BulletCollection );
              BulletCollection = (Bullet*) malloc( NbBulletLevel * sizeof( Bullet ) );

              for( unsigned char k = 0; k<NbBulletLevel; k++)
              {
                     BulletCollection[k].visible = true;
              }

       }

       if ( attackpattern == 0)
       {
              for( unsigned char k=0; k<NbBulletLevel; k++)
              {
                     if ( BulletCollection[k].visible == true )
                     {
                            int xpart = xbossattack + 2*frame_boss_attack*cos( k*360/NbBulletLevel*3.1415927/180.0 );
                            int ypart = ybossattack + 2*frame_boss_attack*sin( k*360/NbBulletLevel*3.1415927/180.0 );

                            unsigned char cellX = (xpart-OFFSET_X)/SIZE;
                            unsigned char cellY = (ypart-OFFSET_Y)/SIZE;

                            for (unsigned char j = 0; j<NbObstacleLevel; j++)
                            {
                                   if (ObstaclesCollection[j].x==cellX && ObstaclesCollection[j].y==cellY && ObstaclesCollection[j].type!=BLOCK_WATER)
                                   {
                                          BulletCollection[k].visible = false;
                                   }
                            }
                            for (unsigned char j = 0; j<NbBlocksLevel; j++)
                            {
                                   if (BlocksCollection[j].x==cellX && BlocksCollection[j].y==cellY)
                                   {
                                          BulletCollection[k].visible = false;
                                   }
                            }
                            if (cellX==joueur.x && cellY==joueur.y)
                            {
                                   if (life>0) life--;
                                   BulletCollection[k].visible = false;
                            }

                            if (xpart<OFFSET_X+SIZE || xpart>OFFSET_X+SIZE*(SIZE_MAP_X-1) || ypart <OFFSET_Y+SIZE || ypart>OFFSET_Y+SIZE*(SIZE_MAP_Y-1))
                            {
                                   BulletCollection[k].visible = false;
                            }


                            if ( BulletCollection[k].visible == true )
                            {
                                   //dsubimage( xpart-8, ypart-8, &bulletsbicolor, frame_bullet*16,attacktype*16,16,16,  DIMAGE_NONE);
                                   dest.w =16;
                                   dest.h =16;
                                   source.w =16;
                                   source.h = 16;
                                   dest.x = xpart-8;
                                   dest.y = xpart-8;
                                   source.x = frame_bullet*16;
                                   source.y = attacktype*16;
                                   SDL_BlitSurface( bulletsbicolor, &source, screenCalcul, &dest );
                            }
                     }

              }
       }
       else if ( attackpattern == 1)
       {
              for( unsigned char k=0; k<NbBulletLevel; k++)
              {
                     if ( BulletCollection[k].visible == true )
                     {
                            int xpart = xbossattack + 2*frame_boss_attack*cos( (frame_boss_attack*2+k*360/NbBulletLevel)*3.1415927/180.0 );
                            int ypart = ybossattack + 2*frame_boss_attack*sin( (frame_boss_attack*2+k*360/NbBulletLevel)*3.1415927/180.0 );

                            unsigned char cellX = (xpart-OFFSET_X)/SIZE;
                            unsigned char cellY = (ypart-OFFSET_Y)/SIZE;

                            for (unsigned char j = 0; j<NbObstacleLevel; j++)
                            {
                                   if (ObstaclesCollection[j].x==cellX && ObstaclesCollection[j].y==cellY && ObstaclesCollection[j].type!=BLOCK_WATER)
                                   {
                                          BulletCollection[k].visible = false;
                                   }
                            }
                            for (unsigned char j = 0; j<NbBlocksLevel; j++)
                            {
                                   if (BlocksCollection[j].x==cellX && BlocksCollection[j].y==cellY)
                                   {
                                          BulletCollection[k].visible = false;
                                   }
                            }
                            if (cellX==joueur.x && cellY==joueur.y)
                            {
                                   if (life>0) life--;
                                   BulletCollection[k].visible = false;
                            }

                            if (xpart<OFFSET_X+SIZE || xpart>OFFSET_X+SIZE*(SIZE_MAP_X-1) || ypart <OFFSET_Y+SIZE || ypart>OFFSET_Y+SIZE*(SIZE_MAP_Y-1))
                            {
                                   BulletCollection[k].visible = false;
                            }


                            if ( BulletCollection[k].visible == true )
                            {
                                   //dsubimage( xpart-8, ypart-8, &bulletsbicolor, frame_bullet*16,attacktype*16,16,16,  DIMAGE_NONE);
                                   dest.w =16;
                                   dest.h =16;
                                   source.w =16;
                                   source.h = 16;
                                   dest.x = xpart-8;
                                   dest.y = xpart-8;
                                   source.x = frame_bullet*16;
                                   source.y = attacktype*16;
                                   SDL_BlitSurface( bulletsbicolor, &source, screenCalcul, &dest );
                            }
                     }

              }
       }




       frame_bullet++;
       if (frame_bullet==8) frame_bullet=0;

       frame_boss_attack++;
       if (frame_boss_attack==100)
       {
              frame_boss_attack=0;
              attackboss=false;
              free( BulletCollection );
              BulletCollection=NULL;
       }
}

void get_inputs_title(void)
{

       KeyManager::Logic();
       MouseManager::Logic();

       if (KeyManager::kbTAB_Press_Event()  )
       {
              doneTitle = true;
       }
       if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

}

void drawTitle( void )
{
       doneTitle = false;

       KeyManager::ResetState();
       MouseManager::ResetState();

       unsigned int positionGuy = 75;
       int dir = 1;

       frame = 0;
       unsigned char flickers = 0;

       SDL_Rect source, dest;

       while (!doneTitle)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = maintitle->w;
              dest.h = maintitle->h;
              source.w = maintitle->w;
              source.h = maintitle->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( maintitle, &source, screenCalcul, &dest );


              dest.w = 16;
              dest.h = 16;
              source.w = 16;
              source.h = 16;

              if (dir==1) // the guy is going to the right
              {
                     if (positionGuy>=270) dir=-1;

                     if (frame==0)
                     {
                            //dsubimage( positionGuy, 100, &player, 0,16,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 0;
                            source.y = 16;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }
                     else if (frame==1)
                     {
                            //dsubimage( positionGuy, 100, &player, 16,16,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 16;
                            source.y = 16;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }
                     else if (frame==2)
                     {
                            //dsubimage( positionGuy, 100, &player, 32,16,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 32;
                            source.y = 16;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }

                     frame++;
                     positionGuy+=1;
                     if  (frame==3) frame=0;
              }
              else if (dir==-1) // the guy is going to the left
              {
                     if (positionGuy<=35) dir=1;

                     if (frame==0)
                     {
                            //dsubimage( positionGuy, 100, &player, 0,48,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 0;
                            source.y = 48;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }
                     else if (frame==1)
                     {
                            //dsubimage( positionGuy, 100, &player, 16,48,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 16;
                            source.y = 48;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }
                     else if (frame==2)
                     {
                            //dsubimage( positionGuy, 100, &player, 32,48,16,16,  DIMAGE_NONE);
                            dest.x = positionGuy;
                            dest.y = 100;
                            source.x = 32;
                            source.y = 48;
                            SDL_BlitSurface( player, &source, screenCalcul, &dest );
                     }

                     frame++;
                     positionGuy-=1;
                     if  (frame==3) frame=0;
              }

              if (flickers<20)
              {
                     //dfont( &font_fantasy );
                     //dprint( 120, 130, C_RGB(150,150,150), "[ Press EXE Key To Start ]");
                     //dprint( 119, 129, C_BLACK, "[ Press EXE Key To Start ]");

                     nSDL_DrawString(screenCalcul, fontfantasygrey, 70, 130, "[ Press TAB Key To Start ]");
                     nSDL_DrawString(screenCalcul, fontfantasyblack, 69, 129,   "[ Press TAB Key To Start ]");

              }

              //dfont( &font_tiny );
              //dprint( 110, 156, C_RGB(150,150,150), "Welcome to Magic Light %s", Revision);
              //dprint( 109, 155, C_BLACK, "Welcome to Magic Light %s", Revision);
              //dprint( 110, 166, C_RGB(150,150,150), "                    12/2021 - by SlyVTT                     ");
              //dprint( 109, 165, C_BLACK, "                    12/2021 - by SlyVTT                     ");

              nSDL_DrawString(screenCalcul, fonttinygrey, 40, 156, "Welcome to Magic Light %s", Revision);
              nSDL_DrawString(screenCalcul, fonttinyblack, 39, 155, "Welcome to Magic Light %s", Revision);
              nSDL_DrawString(screenCalcul, fonttinygrey, 70, 166, "     12/2021 - by SlyVTT");
              nSDL_DrawString(screenCalcul, fonttinyblack, 69, 165,   "     12/2021 - by SlyVTT");

              flickers++;
              if (flickers==40) flickers=0;

              SDL_Flip( screenCalcul );

              get_inputs_title();
       }
}



void get_inputs(void)
{
       unsigned char choice_exit=0;

       KeyManager::Logic();
       MouseManager::Logic();

/*
       if (KeyManager::kbESC() && KeyManager::kbCTRL())
       {
              doneGame = true;
       }
*/
       if (KeyManager::kbESC())
       {
              drawQuit();
       }

       if (KeyManager::kbTAB())
       {
              if (selected > 1) selected--;
       }

       if (KeyManager::kbDOC_Press_Event())
       {
              drawOption();
       }

       if (KeyManager::kbMENU())
       {
              if (selected < 6) selected++;
       }

       if (KeyManager::kbCTRL() && KeyManager::kbDOT_Press_Event() )
       {
              TakeScreenShot( screenCalcul );
       }

       if (MouseManager::kbLEFT())
       {
              gameMechanics( selected, GAUCHE);
       }

       if (MouseManager::kbRIGHT())
       {
              gameMechanics( selected, DROITE);
       }

       if (MouseManager::kbUP())
       {
              gameMechanics( selected, HAUT);
       }

       if (MouseManager::kbDOWN())
       {
              gameMechanics( selected, BAS);
       }

       if (KeyManager::kbCTRL())
       {
              updateTreasures( selected);
       }

       KeyManager::ResetState();
       MouseManager::ResetState();

}



void get_inputs_end(void)
{
       KeyManager::Logic();
       MouseManager::Logic();

       if (KeyManager::kbENTER_Press_Event()  )
       {
              doneEnd = true;
              GotoMainMenu = true;
       }

       KeyManager::ResetState();
       MouseManager::ResetState();
}

void drawEndMenu( void )
{
       doneEnd = false;

       SDL_Rect source, dest;

       selectEndMenu = 0;

       while (!doneEnd)
       {
              SDL_FillRect( screenCalcul, NULL, 0x00000000);

              dest.w = bigparch->w;
              dest.h = bigparch->h;
              source.w = bigparch->w;
              source.h = bigparch->h;
              dest.x = 0;
              dest.y = 0;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( bigparch, &source, screenCalcul, &dest );

              /*
              dfont( &font_fantasy );
              dprint( 70, 45, C_RGB(150,150,150), "A long time ago, in a peaceful and colorful ");
              dprint( 69, 44, C_BLACK, "A long time ago, in a peaceful and colorful");

              dfont( &font_fantasy );
              dprint( 55, 57, C_RGB(150,150,150), "village came a Dark Sorcerer with the terrible");
              dprint( 54, 56, C_BLACK, "village came a Dark Sorcerer with the terrible");

              dfont( &font_fantasy );
              dprint( 55, 69, C_RGB(150,150,150), "ambition of stoling all lights and colors. Making" );
              dprint( 54, 68, C_BLACK, "ambition of stoling all lights and colors. Making " );

              dfont( &font_fantasy );
              dprint( 60, 81, C_RGB(150,150,150), "darkness and fear the only remaining things " );
              dprint( 59, 80, C_BLACK,  "darkness and fear the only remaining things" );

              dfont( &font_fantasy );
              dprint( 60, 93, C_RGB(150,150,150), "here ... The Sorcerer returned to his Castle" );
              dprint( 59, 92, C_BLACK,  "here ... The Sorcerer returned to his Castle" );

              dfont( &font_fantasy );
              dprint( 60, 105, C_RGB(150,150,150), "and ruled his Daughters to protect his loot..." );
              dprint( 59, 104, C_BLACK,  "and ruled his Daughters to protect his loot..." );

              dfont( &font_fantasy );
              dprint( 65, 117, C_RGB(150,150,150), "Guided only by courage, you entered the" );
              dprint( 64, 116, C_BLACK, "Guided only by courage, you entered the" );

              dfont( &font_fantasy );
              dprint( 65, 129, C_RGB(150,150,150), "castle to defeat all dangers and help the" );
              dprint( 64, 128, C_BLACK,  "castle to defeat all dangers and help the" );

              dfont( &font_fantasy );
              dprint( 65, 141, C_RGB(150,150,150), "villagers ... Good luck my Friend !!!" );
              dprint( 64, 140, C_BLACK,  "villagers ... Good luck my Friend !!!" );
              */


              nSDL_DrawString(screenCalcul, fonttinygrey, 25, 160, "[EXE / EXIT] skip");
              nSDL_DrawString(screenCalcul, fonttinyblack, 24, 159,    "[EXE / EXIT] skip");

              SDL_Flip( screenCalcul );

              get_inputs_end();
       }
}

void the_end( void )
{
       drawEndMenu( );
}


void loadResources( void )
{
       light = IMG_Load( "/documents/MagicLight/Resources/light.bmp.tns" );
       uint32_t colortransparent = _getpixel( light, 1, 1 );
       SDL_SetColorKey(light, SDL_SRCCOLORKEY, colortransparent );

       leveltiles = IMG_Load( "/documents/MagicLight/Resources/levelsimple.bmp.tns" );
       SDL_SetColorKey(leveltiles, SDL_SRCCOLORKEY, colortransparent);

       sprites = IMG_Load( "/documents/MagicLight/Resources/sprites.bmp.tns" );
       SDL_SetColorKey(sprites, SDL_SRCCOLORKEY, colortransparent);

       player = IMG_Load( "/documents/MagicLight/Resources/player.bmp.tns" );
       SDL_SetColorKey(player, SDL_SRCCOLORKEY, colortransparent);

       chests = IMG_Load( "/documents/MagicLight/Resources/chests.bmp.tns" );
       SDL_SetColorKey(chests, SDL_SRCCOLORKEY, colortransparent);

       parchemin  = IMG_Load( "/documents/MagicLight/Resources/parchSmall.bmp.tns" );
       SDL_SetColorKey(parchemin, SDL_SRCCOLORKEY, colortransparent);

       cursor  = IMG_Load( "/documents/MagicLight/Resources/cursor.bmp.tns" );
       SDL_SetColorKey(cursor, SDL_SRCCOLORKEY, colortransparent);

       monstres  = IMG_Load( "/documents/MagicLight/Resources/monstres.bmp.tns" );
       SDL_SetColorKey(monstres, SDL_SRCCOLORKEY, colortransparent);

       bigboss  = IMG_Load( "/documents/MagicLight/Resources/bigboss.bmp.tns" );
       SDL_SetColorKey(bigboss, SDL_SRCCOLORKEY, colortransparent);

       bossmages  = IMG_Load( "/documents/MagicLight/Resources/sorcerer.bmp.tns" );
       SDL_SetColorKey(bossmages, SDL_SRCCOLORKEY, colortransparent);

       hearts = IMG_Load( "/documents/MagicLight/Resources/hearts.bmp.tns" );
       SDL_SetColorKey(hearts, SDL_SRCCOLORKEY, colortransparent);

       fragments = IMG_Load( "/documents/MagicLight/Resources/fragments.bmp.tns" );
       SDL_SetColorKey(fragments, SDL_SRCCOLORKEY, colortransparent);

       maintitle = IMG_Load( "/documents/MagicLight/Resources/maintitle.bmp.tns" );
       SDL_SetColorKey(maintitle, SDL_SRCCOLORKEY, colortransparent);

       bigparch = IMG_Load( "/documents/MagicLight/Resources/bigparchvide.bmp.tns" );
       SDL_SetColorKey(bigparch, SDL_SRCCOLORKEY, colortransparent);

       bulletsbicolor  = IMG_Load( "/documents/MagicLight/Resources/bulletsbicolor.bmp.tns" );
       SDL_SetColorKey(bulletsbicolor, SDL_SRCCOLORKEY, colortransparent);


       fontfantasyblack = nSDL_LoadFont(NSDL_FONT_FANTASY, 0, 0, 0);
       fontfantasygrey = nSDL_LoadFont(NSDL_FONT_FANTASY, 150, 150, 150);
       fontfantasyred = nSDL_LoadFont(NSDL_FONT_FANTASY, 255, 0, 0);
       fontfantasywhite = nSDL_LoadFont(NSDL_FONT_FANTASY, 240, 240, 240);
       fonttinyblack = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 0, 0, 0);
       fonttinygrey = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 150, 150, 150);
       fonttinywhite = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 255, 255, 255);
}

bool checkNextPositionMonster( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction )
{

       for( unsigned char k=0; k<NbObstacleLevel; k++)
       {
              if (ObstaclesCollection[k].x==Xtarget  && ObstaclesCollection[k].y==Ytarget)
                     return false;
       }

       //if (Xtarget>=lumiere.x-1 && Xtarget<=lumiere.x+1 && Ytarget>=lumiere.y-1 && Ytarget<=lumiere.y+1)
       if (Xtarget==lumiere.x && Ytarget==lumiere.y)
       {
              return false;
       }
       else if (Xtarget==joueur.x && Ytarget==joueur.y)
       {
              if (life>0) life--;
              return false;
       }
       else if (Xtarget==BlocRouge->x && Ytarget==BlocRouge->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y-1, HAUT))
                     {
                            BlocRouge->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y+1, BAS))
                     {
                            BlocRouge->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocRouge->x-1, BlocRouge->y, GAUCHE))
                     {
                            BlocRouge->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocRouge->x+1, BlocRouge->y, DROITE))
                     {
                            BlocRouge->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocVert->x && Ytarget==BlocVert->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocVert->x, BlocVert->y-1, HAUT))
                     {
                            BlocVert->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocVert->x, BlocVert->y+1, BAS))
                     {
                            BlocVert->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocVert->x-1, BlocVert->y, GAUCHE))
                     {
                            BlocVert->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocVert->x+1, BlocVert->y, DROITE))
                     {
                            BlocVert->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocBleu->x && Ytarget==BlocBleu->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y-1, HAUT))
                     {
                            BlocBleu->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y+1, BAS))
                     {
                            BlocBleu->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocBleu->x-1, BlocBleu->y, GAUCHE))
                     {
                            BlocBleu->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocBleu->x+1, BlocBleu->y, DROITE))
                     {
                            BlocBleu->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocNoir->x && Ytarget==BlocNoir->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y-1, HAUT))
                     {
                            BlocNoir->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y+1, BAS))
                     {
                            BlocNoir->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocNoir->x-1, BlocNoir->y, GAUCHE))
                     {
                            BlocNoir->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocNoir->x+1, BlocNoir->y, DROITE))
                     {
                            BlocNoir->x+=1;
                            return true;
                     }
                     else return false;
              }
       }

       return true;
}

void updateMonsters( void )
{
       for( unsigned char k=0; k<NbMonsterLevel; k++ )
       {
              unsigned int X = MonstersCollection[k].xcur;
              unsigned int Y = MonstersCollection[k].ycur;
              unsigned char mini = MonstersCollection[k].mini;
              unsigned char maxi = MonstersCollection[k].maxi;
              unsigned int direction = MonstersCollection[k].direction;
              unsigned int sens = MonstersCollection[k].sens;

              if (direction==VERTICAL)
              {
                     if (sens==BAS)
                     {
                            if (Y<maxi && checkNextPositionMonster( X, Y+1, BAS)==true)
                            {
                                   MonstersCollection[k].ycur++;
                                   MonstersCollection[k].sens=BAS;
                            }
                            else
                            {
                                   MonstersCollection[k].sens=HAUT;
                            }
                     }
                     else if (sens==HAUT)
                     {
                            if (Y>mini && checkNextPositionMonster( X, Y-1, HAUT)==true)
                            {
                                   MonstersCollection[k].ycur--;
                                   MonstersCollection[k].sens=HAUT;
                            }
                            else
                            {
                                   MonstersCollection[k].sens=BAS;
                            }
                     }
              }
              else if (direction==HORIZONTAL)
              {

                     if (sens==DROITE)
                     {
                            if (X<maxi && checkNextPositionMonster( X+1, Y, DROITE)==true)
                            {
                                   MonstersCollection[k].xcur++;
                                   MonstersCollection[k].sens=DROITE;
                            }
                            else
                            {
                                   MonstersCollection[k].sens=GAUCHE;
                            }
                     }
                     else if (sens==GAUCHE)
                     {
                            if (X>mini && checkNextPositionMonster( X-1, Y, GAUCHE)==true)
                            {
                                   MonstersCollection[k].xcur--;
                                   MonstersCollection[k].sens=GAUCHE;
                            }
                            else
                            {
                                   MonstersCollection[k].sens=DROITE;
                            }
                     }
              }

       }
}

void renderMonsters( SDL_Surface *surf )
{
       for( unsigned char k=0; k<NbMonsterLevel; k++ )
       {
              unsigned int lX = SIZE*MonstersCollection[k].xcur+OFFSET_X;
              unsigned int lY = SIZE*MonstersCollection[k].ycur+OFFSET_Y;
              unsigned char mini = MonstersCollection[k].mini;
              unsigned char maxi = MonstersCollection[k].maxi;
              unsigned int direction = MonstersCollection[k].direction;
              unsigned int sens = MonstersCollection[k].sens;
              unsigned int type = MonstersCollection[k].kind;

              unsigned int OFFSET_X_MONSTER = 48*type;

              SDL_Rect source, dest;

              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;
              source.w = SIZE * ZOOMFACTOR;
              source.h = SIZE * ZOOMFACTOR;

              if (direction==VERTICAL)
              {

                     /*
                                       if (true)
                                      {
                                         _lineRGBA( lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                         _lineRGBA( lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                         _lineRGBA( lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                      }
                     */
                     if (sens==BAS && MonstersCollection[k].ycur<maxi)
                     {
                            if (frame_monster==0)
                            {
                                   //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==1)
                            {
                                   //dsubimage( lX, lY+3, &monstres, OFFSET_X_MONSTER+16,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY+3;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==2)
                            {
                                   //dsubimage( lX, lY+5, &monstres, OFFSET_X_MONSTER+32,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY+5;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==3)
                            {
                                   //dsubimage( lX, lY+8, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY+8;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==4)
                            {
                                   //dsubimage( lX, lY+11, &monstres, OFFSET_X_MONSTER+16,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY+11;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==5)
                            {
                                   //dsubimage( lX, lY+13, &monstres, OFFSET_X_MONSTER+32,0,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY+13;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 0;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                     }
                     else if (sens==BAS && MonstersCollection[k].ycur==maxi)
                     {
                            //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,0,16,16,  DIMAGE_NONE);
                            dest.x = lX;
                            dest.y = lY;
                            source.x = OFFSET_X_MONSTER;
                            source.y = 0;
                            SDL_BlitSurface( monstres, &source, surf, &dest );
                     }
                     else if (sens==HAUT && MonstersCollection[k].ycur>mini)
                     {
                            if (frame_monster==0)
                            {
                                   //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==1)
                            {
                                   //dsubimage( lX, lY-3, &monstres, OFFSET_X_MONSTER+16,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY-3;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==2)
                            {
                                   //dsubimage( lX, lY-5, &monstres, OFFSET_X_MONSTER+32,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY-5;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==3)
                            {
                                   //dsubimage( lX, lY-8, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY-8;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==4)
                            {
                                   //dsubimage( lX, lY-11, &monstres, OFFSET_X_MONSTER+16,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY-11;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==5)
                            {
                                   //dsubimage( lX, lY-13, &monstres, OFFSET_X_MONSTER+32,32,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY-13;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 32;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                     }
                     else if (sens==HAUT && MonstersCollection[k].ycur==mini)
                     {
                            //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,32,16,16,  DIMAGE_NONE);
                            dest.x = lX;
                            dest.y = lY;
                            source.x = OFFSET_X_MONSTER;
                            source.y = 32;
                            SDL_BlitSurface( monstres, &source, surf, &dest );
                     }
              }

              else if (direction==HORIZONTAL)
              {

                     /*
                                      if (true)
                                      {
                                         _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                         _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                         _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                                      }
                     */

                     if (sens==GAUCHE && MonstersCollection[k].xcur>mini)
                     {
                            if (frame_monster==0)
                            {
                                   //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==1)
                            {
                                   //dsubimage( lX-3, lY, &monstres, OFFSET_X_MONSTER+16,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX-3;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==2)
                            {
                                   //dsubimage( lX-5, lY, &monstres, OFFSET_X_MONSTER+32,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX-5;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==3)
                            {
                                   //dsubimage( lX-8, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX-8;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==4)
                            {
                                   //dsubimage( lX-11, lY, &monstres, OFFSET_X_MONSTER+16,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX-11;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==5)
                            {
                                   //dsubimage( lX-13, lY, &monstres, OFFSET_X_MONSTER+32,48,16,16,  DIMAGE_NONE);
                                   dest.x = lX-13;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 48;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                     }
                     else if (sens==GAUCHE && MonstersCollection[k].xcur==mini)
                     {
                            //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,48,16,16,  DIMAGE_NONE);
                            dest.x = lX;
                            dest.y = lY;
                            source.x = OFFSET_X_MONSTER;
                            source.y = 48;
                            SDL_BlitSurface( monstres, &source, surf, &dest );
                     }
                     else if (sens==DROITE && MonstersCollection[k].xcur<maxi)
                     {
                            if (frame_monster==0)
                            {
                                   //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==1)
                            {
                                   //dsubimage( lX+3, lY, &monstres, OFFSET_X_MONSTER+16,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX+3;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==2)
                            {
                                   //dsubimage( lX+5, lY, &monstres, OFFSET_X_MONSTER+32,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX+5;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==3)
                            {
                                   //dsubimage( lX+8, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX+8;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==4)
                            {
                                   //dsubimage( lX+11, lY, &monstres, OFFSET_X_MONSTER+16,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX+11;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+16;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                            else if (frame_monster==5)
                            {
                                   //dsubimage( lX+13, lY, &monstres, OFFSET_X_MONSTER+32,16,16,16,  DIMAGE_NONE);
                                   dest.x = lX+13;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER+32;
                                   source.y = 16;
                                   SDL_BlitSurface( monstres, &source, surf, &dest );
                            }
                     }
                     else if (sens==DROITE && MonstersCollection[k].xcur==maxi)
                     {
                            //dsubimage( lX, lY, &monstres, OFFSET_X_MONSTER,16,16,16,  DIMAGE_NONE);
                            dest.x = lX;
                            dest.y = lY;
                            source.x = OFFSET_X_MONSTER;
                            source.y = 16;
                            SDL_BlitSurface( monstres, &source, surf, &dest );
                     }
              }

       }

       frame_monster++;
       if (frame_monster==6)
       {
              updateMonsters();
              frame_monster=0;
       }

}

bool checkNextPositionBoss( unsigned int Xtarget, unsigned int Ytarget, unsigned int direction )
{

       for( unsigned char k=0; k<NbObstacleLevel; k++)
       {
              if (ObstaclesCollection[k].x==Xtarget  && ObstaclesCollection[k].y==Ytarget)
                     return false;
       }

       //if (Xtarget>=lumiere.x-1 && Xtarget<=lumiere.x+1 && Ytarget>=lumiere.y-1 && Ytarget<=lumiere.y+1)
       if (Xtarget==lumiere.x && Ytarget==lumiere.y)
       {
              return false;
       }
       else if (Xtarget==joueur.x && Ytarget==joueur.y)
       {
              if (life>0) life--;
              return false;
       }
       else if (Xtarget==BlocRouge->x && Ytarget==BlocRouge->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y-1, HAUT))
                     {
                            BlocRouge->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocRouge->x, BlocRouge->y+1, BAS))
                     {
                            BlocRouge->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocRouge->x-1, BlocRouge->y, GAUCHE))
                     {
                            BlocRouge->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocRouge->x+1, BlocRouge->y, DROITE))
                     {
                            BlocRouge->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocVert->x && Ytarget==BlocVert->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocVert->x, BlocVert->y-1, HAUT))
                     {
                            BlocVert->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocVert->x, BlocVert->y+1, BAS))
                     {
                            BlocVert->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocVert->x-1, BlocVert->y, GAUCHE))
                     {
                            BlocVert->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocVert->x+1, BlocVert->y, DROITE))
                     {
                            BlocVert->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocBleu->x && Ytarget==BlocBleu->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y-1, HAUT))
                     {
                            BlocBleu->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocBleu->x, BlocBleu->y+1, BAS))
                     {
                            BlocBleu->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocBleu->x-1, BlocBleu->y, GAUCHE))
                     {
                            BlocBleu->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocBleu->x+1, BlocBleu->y, DROITE))
                     {
                            BlocBleu->x+=1;
                            return true;
                     }
                     else return false;
              }
       }
       else if (Xtarget==BlocNoir->x && Ytarget==BlocNoir->y)
       {
              if (direction==HAUT)
              {
                     if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y-1, HAUT))
                     {
                            BlocNoir->y-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==BAS)
              {
                     if (isValidMove( ROUGE, BlocNoir->x, BlocNoir->y+1, BAS))
                     {
                            BlocNoir->y+=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==GAUCHE)
              {
                     if (isValidMove( ROUGE, BlocNoir->x-1, BlocNoir->y, GAUCHE))
                     {
                            BlocNoir->x-=1;
                            return true;
                     }
                     else return false;
              }
              else if (direction==DROITE)
              {
                     if (isValidMove( ROUGE, BlocNoir->x+1, BlocNoir->y, DROITE))
                     {
                            BlocNoir->x+=1;
                            return true;
                     }
                     else return false;
              }
       }

       return true;
}

void updateBoss( void )
{
       for( unsigned char k=0; k<NbBossLevel; k++ )
       {
              unsigned int X = BossCollection[k].xcur;
              unsigned int Y = BossCollection[k].ycur;
              unsigned char mini = BossCollection[k].mini;
              unsigned char maxi = BossCollection[k].maxi;
              unsigned int direction = BossCollection[k].direction;
              unsigned int sens = BossCollection[k].sens;

              if (direction==VERTICAL)
              {
                     if (sens==BAS)
                     {
                            if (Y<maxi && checkNextPositionMonster( X, Y+1, BAS)==true)
                            {
                                   BossCollection[k].ycur++;
                                   BossCollection[k].sens=BAS;
                            }
                            else
                            {
                                   BossCollection[k].sens=HAUT;
                            }
                     }
                     else if (sens==HAUT)
                     {
                            if (Y>mini && checkNextPositionMonster( X, Y-1, HAUT)==true)
                            {
                                   BossCollection[k].ycur--;
                                   BossCollection[k].sens=HAUT;
                            }
                            else
                            {
                                   BossCollection[k].sens=BAS;
                            }
                     }
              }
              else if (direction==HORIZONTAL)
              {

                     if (sens==DROITE)
                     {
                            if (X<maxi && checkNextPositionMonster( X+1, Y, DROITE)==true)
                            {
                                   BossCollection[k].xcur++;
                                   BossCollection[k].sens=DROITE;
                            }
                            else
                            {
                                   BossCollection[k].sens=GAUCHE;
                            }
                     }
                     else if (sens==GAUCHE)
                     {
                            if (X>mini && checkNextPositionMonster( X-1, Y, GAUCHE)==true)
                            {
                                   BossCollection[k].xcur--;
                                   BossCollection[k].sens=GAUCHE;
                            }
                            else
                            {
                                   BossCollection[k].sens=DROITE;
                            }
                     }
              }

       }
}

void renderBoss( SDL_Surface *surf )
{

       for( unsigned char k=0; k<NbBossLevel; k++ )
       {
              unsigned int lX = SIZE*BossCollection[k].xcur+OFFSET_X;
              unsigned int lY = SIZE*BossCollection[k].ycur+OFFSET_Y - 2; //Boss are 18 pixel high instead of 16
              unsigned char mini = BossCollection[k].mini;
              unsigned char maxi = BossCollection[k].maxi;
              unsigned int direction = BossCollection[k].direction;
              unsigned int sens = BossCollection[k].sens;
              unsigned int type = BossCollection[k].color;



              SDL_Rect source, dest;



              if (type ==BIGBOSS)
              {
                     lX -= 8;
                     lY -= 14;

                     dest.w = 32;
                     dest.h = 32;
                     source.w = 32;
                     source.h = 32;

                     unsigned int OFFSET_X_MONSTER = 0;

                     if (direction==VERTICAL)
                     {

                            /*
                                              if (true)
                                             {
                                                _lineRGBA( lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                             }
                            */
                            if (sens==BAS && BossCollection[k].ycur<maxi)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX, lY+3, &bigboss, OFFSET_X_MONSTER+32,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+3;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX, lY+5, &bigboss, OFFSET_X_MONSTER+64,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+5;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX, lY+8, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+8;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX, lY+11, &bigboss, OFFSET_X_MONSTER+32,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+11;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX, lY+13, &bigboss, OFFSET_X_MONSTER+64,64,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+13;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 64;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                            }
                            else if (sens==BAS && BossCollection[k].ycur==maxi)
                            {

                                   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,64,32,32,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 64;
                                   SDL_BlitSurface( bigboss, &source, surf, &dest );
                            }
                            else if (sens==HAUT && BossCollection[k].ycur>mini)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,0,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX, lY-3, &bigboss, OFFSET_X_MONSTER+32,0,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-3;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX, lY-5, &bigboss, OFFSET_X_MONSTER+64,0,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-5;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX, lY-8, &bigboss, OFFSET_X_MONSTER,32,0,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-8;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX, lY-11, &bigboss, OFFSET_X_MONSTER+32,0,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-11;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX, lY-13, &bigboss, OFFSET_X_MONSTER+64,0,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-13;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 0;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                            }
                            else if (sens==HAUT && BossCollection[k].ycur==mini)
                            {

                                   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,0,32,32,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 0;
                                   SDL_BlitSurface( bigboss, &source, surf, &dest );
                            }
                     }

                     else if (direction==HORIZONTAL)
                     {

                            /*
                                             if (true)
                                             {
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                                             }
                            */

                            if (sens==GAUCHE && BossCollection[k].xcur>mini)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX-3, lY, &bigboss, OFFSET_X_MONSTER+32,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-3;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX-5, lY, &bigboss, OFFSET_X_MONSTER+64,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-5;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX-8, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-8;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX-11, lY, &bigboss, OFFSET_X_MONSTER+32,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-11;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX-13, lY, &bigboss, OFFSET_X_MONSTER+64,96,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-13;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 96;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                            }
                            else if (sens==GAUCHE && BossCollection[k].xcur==mini)
                            {

                                   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,96,32,32,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 96;
                                   SDL_BlitSurface( bigboss, &source, surf, &dest );
                            }
                            else if (sens==DROITE && BossCollection[k].xcur<maxi)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX+3, lY, &bigboss, OFFSET_X_MONSTER+32,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+3;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX+5, lY, &bigboss, OFFSET_X_MONSTER+64,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+5;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX+8, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+8;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX+11, lY, &bigboss, OFFSET_X_MONSTER+32,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+11;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX+13, lY, &bigboss, OFFSET_X_MONSTER+64,32,32,32,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+13;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+64;
                                          source.y = 32;
                                          SDL_BlitSurface( bigboss, &source, surf, &dest );
                                   }
                            }
                            else if (sens==DROITE && BossCollection[k].xcur==maxi)
                            {
                                   //dsubimage( lX, lY, &bigboss, OFFSET_X_MONSTER,32,32,32,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 32;
                                   SDL_BlitSurface( bigboss, &source, surf, &dest );
                            }
                     }

              }
              else
              {

                     dest.w = 16;
                     dest.h = 18;
                     source.w = 16;
                     source.h = 18;

                     unsigned int OFFSET_X_MONSTER = 48*type;


                     if (direction==VERTICAL)
                     {

                            /*
                                              if (true)
                                             {
                                                _lineRGBA( lX+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( lX+1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX+1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( lX-1+SIZE/2, mini*SIZE+OFFSET_Y+SIZE/2, lX-1+SIZE/2, maxi*SIZE+OFFSET_Y+SIZE/2, 255, 0, 0, 255 );
                                             }
                            */
                            if (sens==BAS && BossCollection[k].ycur<maxi)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX, lY+3, &bossmages, OFFSET_X_MONSTER+16,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+3;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX, lY+5, &bossmages, OFFSET_X_MONSTER+32,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+5;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX, lY+8, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+8;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX, lY+11, &bossmages, OFFSET_X_MONSTER+16,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+11;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX, lY+13, &bossmages, OFFSET_X_MONSTER+32,36,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY+13;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 36;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                            }
                            else if (sens==BAS && BossCollection[k].ycur==maxi)
                            {

                                   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,36,16,18,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 36;
                                   SDL_BlitSurface( bossmages, &source, surf, &dest );
                            }
                            else if (sens==HAUT && BossCollection[k].ycur>mini)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX, lY-3, &bossmages, OFFSET_X_MONSTER+16,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-3;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX, lY-5, &bossmages, OFFSET_X_MONSTER+32,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-5;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX, lY-8, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-8;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX, lY-11, &bossmages, OFFSET_X_MONSTER+16,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-11;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX, lY-13, &bossmages, OFFSET_X_MONSTER+32,0,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY-13;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 0;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                            }
                            else if (sens==HAUT && BossCollection[k].ycur==mini)
                            {
                                   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,0,16,18,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 0;
                                   SDL_BlitSurface( bossmages, &source, surf, &dest );
                            }
                     }

                     else if (direction==HORIZONTAL)
                     {

                            /*
                                             if (true)
                                             {
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY+1+SIZE/2, 255, 0, 0, 255 );
                                                _lineRGBA( mini*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, maxi*SIZE+OFFSET_X+SIZE/2, lY-1+SIZE/2, 255, 0, 0, 255 );
                                             }
                            */

                            if (sens==GAUCHE && BossCollection[k].xcur>mini)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX-3, lY, &bossmages, OFFSET_X_MONSTER+16,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-3;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX-5, lY, &bossmages, OFFSET_X_MONSTER+32,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-5;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX-8, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-8;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX-11, lY, &bossmages, OFFSET_X_MONSTER+16,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-11;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX-13, lY, &bossmages, OFFSET_X_MONSTER+32,54,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX-13;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 54;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                            }
                            else if (sens==GAUCHE && BossCollection[k].xcur==mini)
                            {
                                   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,54,16,18,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 54;
                                   SDL_BlitSurface( bossmages, &source, surf, &dest );
                            }
                            else if (sens==DROITE && BossCollection[k].xcur<maxi)
                            {
                                   if (frame_boss==0)   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==1)   //dsubimage( lX+3, lY, &bossmages, OFFSET_X_MONSTER+16,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+3;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==2)   //dsubimage( lX+5, lY, &bossmages, OFFSET_X_MONSTER+32,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+5;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==3)   //dsubimage( lX+8, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+8;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==4)   //dsubimage( lX+11, lY, &bossmages, OFFSET_X_MONSTER+16,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+11;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+16;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                                   else if (frame_boss==5)   //dsubimage( lX+13, lY, &bossmages, OFFSET_X_MONSTER+32,18,16,18,  DIMAGE_NONE);
                                   {
                                          dest.x = lX+13;
                                          dest.y = lY;
                                          source.x = OFFSET_X_MONSTER+32;
                                          source.y = 18;
                                          SDL_BlitSurface( bossmages, &source, surf, &dest );
                                   }
                            }
                            else if (sens==DROITE && BossCollection[k].xcur==maxi)
                            {
                                   //dsubimage( lX, lY, &bossmages, OFFSET_X_MONSTER,18,16,18,  DIMAGE_NONE);
                                   dest.x = lX;
                                   dest.y = lY;
                                   source.x = OFFSET_X_MONSTER;
                                   source.y = 18;
                                   SDL_BlitSurface( bossmages, &source, surf, &dest );
                            }
                     }
              }
       }

       frame_boss++;
       if (frame_boss==6)
       {
              updateBoss();
              frame_boss=0;
       }
}

/*
void loadLevel( void )
{
       int numLevel = 0;

       LEVELCOLOR = 0;
       SIZE_MAP_X=13;
       SIZE_MAP_Y=11;

       NbReceptorsLevel = 3;
       NbDoorsLevel = 1;
       NbBlocksLevel = 4;
       NbTreasureLevel = 5;
       NbObstacleLevel = 8;
       NbMonsterLevel = 2;
       NbBossLevel = 1;

       BlocksCollection = (Blocks*) malloc( NbBlocksLevel * sizeof( Blocks) );
       ReceptorCollection = (Receptor*) malloc( NbReceptorsLevel * sizeof( Receptor) );
       DoorCollection = (Door*) malloc( NbDoorsLevel * sizeof( Door) );
       TreasureCollection = (Treasure*) malloc( NbTreasureLevel * sizeof( Treasure) );
       ObstaclesCollection = (Obstacle*) malloc( NbObstacleLevel * sizeof( Obstacle) );
       MonstersCollection = (Monster*) malloc( NbMonsterLevel * sizeof( Monster) );
       BossCollection = (Boss*) malloc( NbBossLevel * sizeof( Boss ) );

       Map[numLevel].x=0;
       Map[numLevel].y=0;
       Map[numLevel].R=255;
       Map[numLevel].G=255;
       Map[numLevel].B=255;
       Map[numLevel].A=255;
       Map[numLevel].visited=true;
       Map[numLevel].doorWest=false;
       Map[numLevel].doorNorth=true;
       Map[numLevel].doorEast=false;
       Map[numLevel].doorSouth=false;

       joueur = { 6, 9, HAUT };
       lumiere = { 6, 5, 1,1,1,0, false };

       ObstaclesCollection[0] = {2,2,BLOCK_SUN};
       ObstaclesCollection[1] = {10,2,BLOCK_SUN};
       ObstaclesCollection[2] = {2,8,BLOCK_SUN};
       ObstaclesCollection[3] = {10,8,BLOCK_SUN};
       ObstaclesCollection[4] = {4,5,BLOCK_WATER};
       ObstaclesCollection[5] = {8,5,BLOCK_WATER};
       ObstaclesCollection[6] = {6,3,BLOCK_WATER};
       ObstaclesCollection[7] = {6,7,BLOCK_WATER};

       BlocksCollection[0] = { 8, 3, 255, 0, 0, 127, TRANSPARENT };
       BlocksCollection[1] = { 4, 7, 0, 255, 0, 127, TRANSPARENT };
       BlocksCollection[2] = { 4, 3, 0, 0, 255, 127, TRANSPARENT };
       BlocksCollection[3] = { 8, 7, 0, 0, 0, 255, OPAQUE };

       BlocRouge = &BlocksCollection[0];
       BlocVert = &BlocksCollection[1];
       BlocBleu = &BlocksCollection[2];
       BlocNoir = &BlocksCollection[3];

       ReceptorCollection[0] = { 5, 0, BAS, 255, 0, 0, false };  // Receptor RED
       ReceptorCollection[1] = { 12, 4, GAUCHE, 0, 255, 0, false }; // Receptor GREEN
       ReceptorCollection[2] = { 0, 6, DROITE, 0, 0, 255, false }; // Receptor BLUE
       // ReceptorCollection[3] = { 10, 8, GAUCHE, 0, 0, 0, false }; // Receptor BLACK
       // ReceptorCollection[4] = { 8, 0, BAS, 255, 0, 255, false }; // Receptor PINK
       // ReceptorCollection[5] = { 9, 10, HAUT, 0, 255, 255, false }; // Receptor CYAN
       // ReceptorCollection[6] = { 12, 8, GAUCHE, 255, 255, 0, false }; // Receptor YELLOW
       // ReceptorCollection[7] = { 0, 7, DROITE, 255, 255, 255, false }; // Receptor WHITE

       DoorCollection[0] = { 6,0, HAUT, false, true, 3, { R_ROUGE,R_BLEU, R_VERT }, 2}; // Door NORTH
       //DoorCollection[1] = { 0, 5, GAUCHE, false, true, 1, { R_BLEU, -1, -1 }, 0}; // Door WEST
       //DoorCollection[2] = { 12, 5, DROITE, false, true, 1, { R_VERT, -1, -1 }, 0}; // Door EAST
       //DoorCollection[3] = { 6, 10, BAS, false, false, 0, { -1 -1, -1 }, -1}; // Door SOUTH

       TreasureCollection[0] = {1,1, T_RED, false, WHITE_FRAGMENT, 100, true };
       TreasureCollection[1] = {2,1, T_YELLOW, false, BLACK_FRAGMENT, 100, true };
       TreasureCollection[2] = {3,1, T_GREEN, false, RED_FRAGMENT, 100, true };
       TreasureCollection[3] = {4,1, T_BLUE, false, GREEN_FRAGMENT, 100, true };
       TreasureCollection[4] = {5,1, T_BLUE, false, BLUE_FRAGMENT, 100, true };

       MonstersCollection[0] = { 6, 4, 2, 10, HORIZONTAL, GAUCHE, BLOB };
       MonstersCollection[1] = { 7, 5, 2, 8, VERTICAL, HAUT, SKELETON };

       BossCollection[0] = { 6, 2, 2, 10, HORIZONTAL, GAUCHE, B_RED };

}
*/

void renderObstacles( SDL_Surface *surf, int passnumber )
{
       if (passnumber == LAYER_BELOW_RAYS)
       {

              for( unsigned char k=0; k<NbObstacleLevel; k++ )
              {
                     unsigned int Xb = ObstaclesCollection[k].x;
                     unsigned int Yb = ObstaclesCollection[k].y;
                     unsigned char Tb = ObstaclesCollection[k].type;

                     unsigned int lX = SIZE*Xb+OFFSET_X;
                     unsigned int lY = SIZE*Yb+OFFSET_Y;

                     SDL_Rect source, dest;

                     dest.x = lX;
                     dest.y = lY;
                     dest.w = SIZE * ZOOMFACTOR;
                     dest.h = SIZE * ZOOMFACTOR;

                     if (Tb==BLOCK_WATER)
                     {
                            // dsubimage( lX, lY, &sprites, 64,112,16,16,  DIMAGE_NONE);
                            source.x = 64;
                            source.y = 112;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;
                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
              }
       }
       else if (passnumber == LAYER_ABOVE_RAYS)
       {

              for( unsigned char k=0; k<NbObstacleLevel; k++ )
              {
                     unsigned int Xb = ObstaclesCollection[k].x;
                     unsigned int Yb = ObstaclesCollection[k].y;
                     unsigned char Tb = ObstaclesCollection[k].type;

                     unsigned int lX = SIZE*Xb+OFFSET_X;
                     unsigned int lY = SIZE*Yb+OFFSET_Y;

                     SDL_Rect source, dest;

                     dest.x = lX;
                     dest.y = lY;
                     dest.w = SIZE * ZOOMFACTOR;
                     dest.h = SIZE * ZOOMFACTOR;

                     if (Tb==BLOCK_STONE)
                     {
                            //dsubimage( lX, lY, &sprites, 0,112,16,16,  DIMAGE_NONE);
                            source.x = 0;
                            source.y = 112;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;
                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (Tb==BLOCK_MOON)
                     {
                            //dsubimage( lX, lY, &sprites, 16,112,16,16,  DIMAGE_NONE);
                            source.x = 16;
                            source.y = 112;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;
                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (Tb==BLOCK_WOOD)
                     {
                            //dsubimage( lX, lY, &sprites, 32,112,16,16,  DIMAGE_NONE);
                            source.x = 32;
                            source.y = 112;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;
                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (Tb==BLOCK_SUN)
                     {
                            //dsubimage( lX, lY, &sprites, 48,112,16,16,  DIMAGE_NONE);
                            source.x = 48;
                            source.y = 112;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;
                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }

              }
       }
}

void renderTreasures( SDL_Surface* surf )
{
       for( unsigned char k=0; k<NbTreasureLevel; k++ )
       {
              if (TreasureCollection[k].isvisible)
              {
                     unsigned int Xb = TreasureCollection[k].x;
                     unsigned int Yb = TreasureCollection[k].y;
                     chest Tb = TreasureCollection[k].type;
                     bool Ob = TreasureCollection[k].isopen;

                     unsigned int lX = SIZE*Xb+OFFSET_X;
                     unsigned int lY = SIZE*Yb+OFFSET_Y;

                     SDL_Rect source, dest;

                     dest.x = lX;
                     dest.y = lY;
                     dest.w = SIZE * ZOOMFACTOR;
                     dest.h = SIZE * ZOOMFACTOR;

                     if (Tb==T_RED)
                     {
                            if (Ob==false)
                            {
                                   //dsubimage( lX, lY, &chests, 0,0,16,16,  DIMAGE_NONE);
                                   source.x = 0;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );
                            }
                            else
                            {
                                   //dsubimage( lX, lY, &chests, 0,48,16,16,  DIMAGE_NONE);
                                   source.x = 0;
                                   source.y = 48;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                     }
                     else if (Tb==T_YELLOW)
                     {
                            if (Ob==false)
                            {
                                   //dsubimage( lX, lY, &chests, 16,0,16,16,  DIMAGE_NONE);
                                   source.x = 16;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                            else
                            {
                                   //dsubimage( lX, lY, &chests, 16,48,16,16,  DIMAGE_NONE);
                                   source.x = 16;
                                   source.y = 48;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                     }
                     else if (Tb==T_GREEN)
                     {
                            if (Ob==false)
                            {
                                   //dsubimage( lX, lY, &chests, 32,0,16,16,  DIMAGE_NONE);
                                   source.x = 32;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                            else
                            {
                                   //dsubimage( lX, lY, &chests, 32,48,16,16,  DIMAGE_NONE);
                                   source.x = 32;
                                   source.y = 48;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                     }
                     else if (Tb==T_BLUE)
                     {
                            if (Ob==false)
                            {
                                   //dsubimage( lX, lY, &chests, 48,0,16,16,  DIMAGE_NONE);
                                   source.x = 48;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                            else
                            {
                                   //dsubimage( lX, lY, &chests, 48,48,16,16,  DIMAGE_NONE);
                                   source.x = 48;
                                   source.y = 48;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( chests, &source, surf, &dest );

                            }
                     }
              }
       }

}

void renderMovable( SDL_Surface *surf )
{
       for( unsigned char k=0; k<NbBlocksLevel; k++ )
       {
              unsigned int Xb = BlocksCollection[k].x;
              unsigned int Yb = BlocksCollection[k].y;
              unsigned char Rb = BlocksCollection[k].R;
              unsigned char Gb = BlocksCollection[k].G;
              unsigned char Bb = BlocksCollection[k].B;
              //unsigned char Ab = BlocksCollection[k].A;

              unsigned int lX = SIZE*Xb+OFFSET_X;
              unsigned int lY = SIZE*Yb+OFFSET_Y;

              SDL_Rect source, dest;

              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;


              if (Rb==255 && Gb==0 && Bb==0)
              {
                     //dsubimage( lX, lY, &sprites, 16,0,16,16,  DIMAGE_NONE);
                     source.x = 16;
                     source.y = 0;
                     source.w = SIZE * ZOOMFACTOR;
                     source.h = SIZE * ZOOMFACTOR;
                     SDL_BlitSurface( sprites, &source, surf, &dest );
              }
              else if (Rb==0 && Gb==0 && Bb==255)
              {
                     //dsubimage( lX, lY, &sprites, 32,0,16,16,  DIMAGE_NONE);
                     source.x = 32;
                     source.y = 0;
                     source.w = SIZE * ZOOMFACTOR;
                     source.h = SIZE * ZOOMFACTOR;
                     SDL_BlitSurface( sprites, &source, surf, &dest );
              }
              else if (Rb==0 && Gb==255 && Bb==0)
              {
                     //dsubimage( lX, lY, &sprites, 48,0,16,16,  DIMAGE_NONE);
                     source.x = 48;
                     source.y = 0;
                     source.w = SIZE * ZOOMFACTOR;
                     source.h = SIZE * ZOOMFACTOR;
                     SDL_BlitSurface( sprites, &source, surf, &dest );
              }
              else if (Rb==0 && Gb==0 && Bb==0)
              {
                     // dsubimage( lX, lY, &sprites, 64,0,16,16,  DIMAGE_NONE);
                     source.x = 64;
                     source.y = 0;
                     source.w = SIZE * ZOOMFACTOR;
                     source.h = SIZE * ZOOMFACTOR;
                     SDL_BlitSurface( sprites, &source, surf, &dest );
              }
              else if (Rb==255 && Gb==255 && Bb==255)
              {
                     //dsubimage( lX, lY, &sprites, 0,0,16,16,  DIMAGE_NONE);
                     source.x = 0;
                     source.y = 0;
                     source.w = SIZE * ZOOMFACTOR;
                     source.h = SIZE * ZOOMFACTOR;
                     SDL_BlitSurface( sprites, &source, surf, &dest );
              }

       }

}

void renderPlayer( SDL_Surface *surf )
{
       unsigned int lX = SIZE*joueur.x*ZOOMFACTOR+OFFSET_X;
       unsigned int lY = SIZE*joueur.y*ZOOMFACTOR+OFFSET_Y;

       SDL_Rect source, dest;

       dest.w = SIZE * ZOOMFACTOR;
       dest.h = SIZE * ZOOMFACTOR;
       source.w = SIZE * ZOOMFACTOR;
       source.h = SIZE * ZOOMFACTOR;


       source.x = 0;
       source.y = 0;

       if (direction==BAS)
       {
              if (mouvement==true)
              {
                     if (frame==0)
                     {
                            source.x = 0;
                            source.y = 0;
                            dest.x = lX;
                            dest.y = lY-11;
                     }
                     else if (frame==1)
                     {
                            source.x = 16;
                            source.y = 0;
                            dest.x = lX;
                            dest.y = lY-6;
                     }
                     else if (frame==2)
                     {
                            source.x = 32;
                            source.y = 0;
                            dest.x = lX;
                            dest.y = lY;
                     }
                     frame++;
                     if  (frame==3) mouvement = false;
              }
              else
              {
                     source.x = 0;
                     source.y = 0;
                     frame = 0;
                     dest.x = lX;
                     dest.y = lY;
              }
       }
       else if (direction==DROITE)
       {
              if (mouvement==true)
              {
                     if (frame==0)
                     {
                            source.x = 0;
                            source.y = 16;
                            dest.x = lX-11;
                            dest.y = lY;
                     }
                     else if (frame==1)
                     {
                            source.x = 16;
                            source.y = 16;
                            dest.x = lX-6;
                            dest.y = lY;
                     }
                     else if (frame==2)
                     {
                            source.x = 32;
                            source.y = 16;
                            dest.x = lX;
                            dest.y = lY;
                     }
                     frame++;
                     if  (frame==3) mouvement = false;
              }
              else
              {
                     source.x = 0;
                     source.y = 16;
                     frame = 0;
                     dest.x = lX;
                     dest.y = lY;
              }
       }
       else if (direction==HAUT)
       {
              if (mouvement==true)
              {
                     if (frame==0)
                     {
                            source.x = 0;
                            source.y = 32;
                            dest.x = lX;
                            dest.y = lY+11;
                     }
                     else if (frame==1)
                     {
                            source.x = 16;
                            source.y = 32;
                            dest.x = lX;
                            dest.y = lY+6;
                     }
                     else if (frame==2)
                     {
                            source.x = 32;
                            source.y = 32;
                            dest.x = lX;
                            dest.y = lY;
                     }
                     frame++;
                     if  (frame==3) mouvement = false;
              }
              else
              {
                     source.x = 0;
                     source.y = 32;
                     frame = 0;
                     dest.x = lX;
                     dest.y = lY;
              }
       }
       else if (direction==GAUCHE)
       {
              if (mouvement==true)
              {
                     if (frame==0)
                     {
                            source.x = 0;
                            source.y = 48;
                            dest.x = lX+11;
                            dest.y = lY;
                     }
                     else if (frame==1)
                     {
                            source.x = 16;
                            source.y = 48;
                            dest.x = lX+6;
                            dest.y = lY;
                     }
                     else if (frame==2)
                     {
                            source.x = 32;
                            source.y = 48;
                            dest.x = lX;
                            dest.y = lY;
                     }
                     frame++;
                     if  (frame==3) mouvement = false;
              }
              else
              {
                     source.x = 0;
                     source.y = 48;
                     frame = 0;
                     dest.x = lX;
                     dest.y = lY;
              }
       }

       SDL_BlitSurface( player, &source, surf, &dest );
}

void renderLight( SDL_Surface *surf )
{
       unsigned int lX = SIZE*lumiere.x+OFFSET_X;
       unsigned int lY = SIZE*lumiere.y+OFFSET_Y;

       SDL_Rect source, dest;

       dest.x = lX;
       dest.y = lY;
       dest.w = SIZE * ZOOMFACTOR;
       dest.h = SIZE * ZOOMFACTOR;

       frame_light%=6;

       source.x = 16*frame_light;
       source.y = 0;
       source.w = SIZE * ZOOMFACTOR;
       source.h = SIZE * ZOOMFACTOR;

       frame_light++;

       SDL_BlitSurface( light, &source, surf, &dest );
}

void drawMinimap( SDL_Surface *surf )
{
       for( unsigned char k=0; k< NB_LEVELS; k++ )
       {
              if (Map[k].visited==true)
              {
                     unsigned char R= Map[k].R;
                     unsigned char G= Map[k].G;
                     unsigned char B= Map[k].B;
                     unsigned char A= Map[k].A;

                     unsigned int lX = OFFSET_X_MINIMAP + Map[k].x*(SCALE_X_MINIMAP+2);
                     unsigned int lY = OFFSET_Y_MINIMAP + Map[k].y*(SCALE_Y_MINIMAP+2);

                     if (k==currentLevel)
                            boxRGBA( surf, lX-4, lY-3, lX+4, lY+3, 0, 255, 0, A/2);

                     rectangleRGBA( surf, lX-4, lY-3, lX+4, lY+3, R, G, B, A);

                     if (Map[k].doorWest==true) rectangleRGBA( surf, lX-6, lY-1, lX-4, lY+1, R, G, B, A);
                     if (Map[k].doorEast==true) rectangleRGBA( surf, lX+4, lY-1, lX+6, lY+1, R, G, B, A);
                     if (Map[k].doorNorth==true) rectangleRGBA( surf, lX-1, lY-5, lX+1, lY-3, R, G, B, A);
                     if (Map[k].doorSouth==true) rectangleRGBA( surf, lX-1, lY+3, lX+1, lY+5, R, G, B, A);
              }
       }
}

void drawInterface( SDL_Surface *surf )
{

       unsigned int midX, midY;
       SDL_Rect source, dest;

       //dsubimage( 232, 0, &parchemin, 0,0, 164, 210,  DIMAGE_NONE);

       dest.w = parchemin->w;
       dest.h = parchemin->h;
       source.w = parchemin->w;
       source.h = parchemin->h;
       dest.x = 220;
       dest.y = 0;
       source.x = 0;
       source.y = 0;
       SDL_BlitSurface( parchemin, &source, surf, &dest );

       nSDL_DrawString(surf, fontfantasygrey, 235, 23, "MAGIC Light");
       nSDL_DrawString(surf, fontfantasyblack, 233, 22, "MAGIC Light");

       nSDL_DrawString(surf, fontfantasygrey, 255, 32, "%s", RevShort);
       nSDL_DrawString(surf, fontfantasyblack, 254, 31, "%s", RevShort);

       nSDL_DrawString(surf, fonttinygrey, 230, 47, "S:%d", score);
       nSDL_DrawString(surf, fonttinyblack, 229, 46, "S:%d", score);

       nSDL_DrawString(surf, fonttinygrey, 230, 57, "L:%d", currentLevel);
       nSDL_DrawString(surf, fonttinyblack, 229, 56, "L:%d", currentLevel);

       //nSDL_DrawString(surf, fonttinygrey, 255, 67, "Life : ");
       //nSDL_DrawString(surf, fonttinyblack, 254, 66, "Life : ");

       for( unsigned char k = 0; k< life; k++)
       {
              //dsubimage( 340 + k*8, 57, &hearts, 8, 0, 8, 8,  DIMAGE_NONE);

              dest.w =8;
              dest.h = 8;
              source.w = 8;
              source.h = 8;
              dest.x = 230+k*8;
              dest.y = 67;
              source.x = 8;
              source.y = 0;
              SDL_BlitSurface( hearts, &source, surf, &dest );
       }

       for( unsigned char k = life; k< lifeMax; k++)
       {
              //dsubimage( 340 + k*8, 57, &hearts, 0, 0, 8, 8,  DIMAGE_NONE);

              dest.w =8;
              dest.h = 8;
              source.w = 8;
              source.h = 8;
              dest.x = 230+k*8;
              dest.y = 67;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( hearts, &source, surf, &dest );
       }

       if (WhiteFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 0, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 0;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (BlackFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 32, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 32;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (RedFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 64, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 64;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (GreenFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 96, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 96;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }
       if (BlueFrag==true)
       {
              //dsubimage( 340, 66, &fragments, 128, 0, 32, 32,  DIMAGE_NONE);
              dest.w = 32;
              dest.h = 32;
              source.w = 32;
              source.h = 32;
              dest.x = 280;
              dest.y = 45;
              source.x = 128;
              source.y = 0;
              SDL_BlitSurface( fragments, &source, surf, &dest );
       }

       /*

             dfont( &font_fantasy );

             dprint( 256, 36, C_RGB(150,150,150), "MAGIC Light %s", RevShort);
             dprint( 255, 35, C_BLACK, "MAGIC Light %s", RevShort);

             dfont( &font_tiny );

             dprint( 251, 56, C_RGB(150,150,150), "Score : %d", score );
             dprint( 250, 55, C_BLACK, "Score : %d", score );

             dprint( 251, 66, C_RGB(150,150,150), "Level : %d", currentLevel );
             dprint( 250, 65, C_BLACK, "Level : %d",  currentLevel );

             dprint( 316, 56, C_RGB(150,150,150), "Life :");
             dprint( 315, 55, C_BLACK, "Life :" );

             for( unsigned char k = 0; k< life; k++)
             {
                    dsubimage( 340 + k*8, 57, &hearts, 8, 0, 8, 8,  DIMAGE_NONE);
             }

             for( unsigned char k = life; k< lifeMax; k++)
             {
                    dsubimage( 340 + k*8, 57, &hearts, 0, 0, 8, 8,  DIMAGE_NONE);
             }


             if (WhiteFrag==true) dsubimage( 340, 66, &fragments, 0, 0, 32, 32,  DIMAGE_NONE);
             if (BlackFrag==true) dsubimage( 340, 66, &fragments, 32, 0, 32, 32,  DIMAGE_NONE);
             if (RedFrag==true) dsubimage( 340, 66, &fragments, 64, 0, 32, 32,  DIMAGE_NONE);
             if (GreenFrag==true) dsubimage( 340, 66, &fragments, 96, 0, 32, 32,  DIMAGE_NONE);
             if (BlueFrag==true) dsubimage( 340, 66, &fragments, 128, 0, 32, 32,  DIMAGE_NONE);
       */


       midY=(200+240)/2-8;


       dest.w = SIZE * ZOOMFACTOR;
       dest.h = SIZE * ZOOMFACTOR;
       source.w = SIZE * ZOOMFACTOR;
       source.h = SIZE * ZOOMFACTOR;



       midX=16;
       //dsubimage( midX-8, midY-8, &player, 0,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 0;
       source.y = 0;
       SDL_BlitSurface( player, &source, surf, &dest );


       midX+=55;
       //dsubimage( midX-8, midY-8, &light, 0,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 0;
       source.y = 0;
       SDL_BlitSurface( light, &source, surf, &dest );


       midX+=55;
       //dsubimage( midX-8, midY-8, &sprites, 16,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 16;
       source.y = 0;
       SDL_BlitSurface( sprites, &source, surf, &dest );

       midX+=55;
       //dsubimage( midX-8, midY-8, &sprites, 48,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 48;
       source.y = 0;
       SDL_BlitSurface( sprites, &source, surf, &dest );

       midX+=55;
       //dsubimage( midX-8, midY-8, &sprites, 32,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 32;
       source.y = 0;
       SDL_BlitSurface( sprites, &source, surf, &dest );

       midX+=55;
       //dsubimage( midX-8, midY-8, &sprites, 64,0,16,16,  DIMAGE_NONE);
       dest.x = midX;
       dest.y = midY;
       source.x = 64;
       source.y = 0;
       SDL_BlitSurface( sprites, &source, surf, &dest );


       if (selected >=1 && selected <=6)
       {

              if (frame_cursor==0)
              {
                     //dsubimage( 66*(selected)-33-11, midY-11, &cursor, 0,0,22,22,  DIMAGE_NONE);
                     dest.x = 16+55*(selected-1)-3;
                     dest.y = midY-3;
                     dest.w = 22;
                     dest.h = 22;
                     source.w = 22;
                     source.h = 22;
                     source.x = 0;
                     source.y = 0;
                     SDL_BlitSurface( cursor, &source, surf, &dest );
              }
              else if (frame_cursor==1)
              {
                     //dsubimage( 66*(selected)-33-11, midY-11, &cursor, 22,0,22,22,  DIMAGE_NONE);
                     dest.x = 16+55*(selected-1)-3;
                     dest.y = midY-3;
                     dest.w = 22;
                     dest.h = 22;
                     source.w = 22;
                     source.h = 22;
                     source.x = 22;
                     source.y = 0;
                     SDL_BlitSurface( cursor, &source, surf, &dest );
              }
              else if (frame_cursor==2)
              {
                     //dsubimage( 66*(selected)-33-11, midY-11, &cursor, 44,0,22,22,  DIMAGE_NONE);
                     dest.x = 16+55*(selected-1)-3;
                     dest.y = midY-3;
                     dest.w = 22;
                     dest.h = 22;
                     source.w = 22;
                     source.h = 22;
                     source.x = 44;
                     source.y = 0;
                     SDL_BlitSurface( cursor, &source, surf, &dest );
              }
              else if (frame_cursor==3)
              {
                     //dsubimage( 66*(selected)-33-11, midY-11, &cursor, 22,0,22,22,  DIMAGE_NONE);
                     dest.x = 16+55*(selected-1)-3;
                     dest.y = midY-3;
                     dest.w = 22;
                     dest.h = 22;
                     source.w = 22;
                     source.h = 22;
                     source.x = 22;
                     source.y = 0;
                     SDL_BlitSurface( cursor, &source, surf, &dest );
              }
       }
}

bool isValidMove( selection selected, unsigned char x, unsigned char y, orientations sens )
{
       for(unsigned char k=0; k<NbDoorsLevel; k++)
       {
              if ( x==DoorCollection[k].x && y==DoorCollection[k].y)
              {
                     if (DoorCollection[k].isdoor && DoorCollection[k].isopen)
                     {
                            loadLevel(  DoorCollection[k].nextLevelNumber );
                            joueur = { 11-10*x/12, 9-8*y/10, sens };
                            return false;
                     }
              }
       }

       if (x<1 || x>SIZE_MAP_X-2 || y<1 || y>SIZE_MAP_Y-2)
       {
              return false;
       }
       else if (selected == JOUEUR)
       {
              if (x==lumiere.x && y==lumiere.y) return false;
              else if (x==BlocRouge->x && y==BlocRouge->y) return false;
              else if (x==BlocVert->x && y==BlocVert->y) return false;
              else if (x==BlocBleu->x && y==BlocBleu->y) return false;
              else if (x==BlocNoir->x && y==BlocNoir->y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       else if (selected == LUMIERE)
       {
              if (x==joueur.x && y==joueur.y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       else if (selected == ROUGE)
       {
              if (x==joueur.x && y==joueur.y) return false;
              else if (x==BlocVert->x && y==BlocVert->y) return false;
              else if (x==BlocBleu->x && y==BlocBleu->y) return false;
              else if (x==BlocNoir->x && y==BlocNoir->y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       else if (selected == VERT)
       {
              if (x==joueur.x && y==joueur.y) return false;
              else if (x==BlocRouge->x && y==BlocRouge->y) return false;
              else if (x==BlocBleu->x && y==BlocBleu->y) return false;
              else if (x==BlocNoir->x && y==BlocNoir->y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       else if (selected == BLEU)
       {
              if (x==joueur.x && y==joueur.y) return false;
              else if (x==BlocRouge->x && y==BlocRouge->y) return false;
              else if (x==BlocVert->x && y==BlocVert->y) return false;
              else if (x==BlocNoir->x && y==BlocNoir->y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       else if (selected == NOIR)
       {
              if (x==joueur.x && y==joueur.y) return false;
              else if (x==BlocRouge->x && y==BlocRouge->y) return false;
              else if (x==BlocVert->x && y==BlocVert->y) return false;
              else if (x==BlocBleu->x && y==BlocBleu->y) return false;
              else
              {
                     for (unsigned char k =0; k<NbObstacleLevel; k++)
                            if (ObstaclesCollection[k].x==x && ObstaclesCollection[k].y==y)
                                   return false;
                     return true;
              }
       }
       return false;
}

void gameMechanics(  selection what, orientations touche )
{
       if (touche==GAUCHE)
       {
              if (what==JOUEUR && isValidMove(JOUEUR, joueur.x-1, joueur.y, touche ))
              {
                     joueur.x--;
                     frame=0;
                     direction=GAUCHE;
                     mouvement=true;
              }
              else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x-1, joueur.y, touche))
              {
                     frame=0;
                     direction=GAUCHE;
              }
              else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x-1, lumiere.y, touche))     lumiere.x--;
              else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x-1, BlocRouge->y, touche ))     BlocRouge->x--;
              else if (what==VERT && isValidMove(VERT, BlocVert->x-1, BlocVert->y, touche))     BlocVert->x--;
              else if (what==BLEU && isValidMove(BLEU, BlocBleu->x-1, BlocBleu->y, touche))     BlocBleu->x--;
              else if (what==NOIR && isValidMove(NOIR, BlocNoir->x-1, BlocNoir->y, touche))     BlocNoir->x--;
       }
       if (touche==DROITE)
       {
              if (what==JOUEUR && isValidMove(JOUEUR, joueur.x+1, joueur.y, touche))
              {
                     joueur.x++;
                     frame=0;
                     direction=DROITE;
                     mouvement=true;
              }
              else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x+1, joueur.y, touche))
              {
                     frame=0;
                     direction=DROITE;
              }
              else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x+1, lumiere.y, touche))     lumiere.x++;
              else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x+1, BlocRouge->y, touche))     BlocRouge->x++;
              else if (what==VERT && isValidMove(VERT, BlocVert->x+1, BlocVert->y, touche))     BlocVert->x++;
              else if (what==BLEU && isValidMove(BLEU, BlocBleu->x+1, BlocBleu->y, touche))     BlocBleu->x++;
              else if (what==NOIR && isValidMove(NOIR, BlocNoir->x+1, BlocNoir->y, touche))     BlocNoir->x++;
       }
       if (touche==HAUT)
       {
              if (what==JOUEUR && isValidMove(JOUEUR, joueur.x, joueur.y-1, touche))
              {
                     joueur.y--;
                     frame=0;
                     direction=HAUT;
                     mouvement=true;
              }
              else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x, joueur.y-1, touche))
              {
                     frame=0;
                     direction=HAUT;
              }
              else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x, lumiere.y-1, touche))     lumiere.y--;
              else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x, BlocRouge->y-1, touche))     BlocRouge->y--;
              else if (what==VERT && isValidMove(VERT, BlocVert->x, BlocVert->y-1, touche))     BlocVert->y--;
              else if (what==BLEU && isValidMove(BLEU, BlocBleu->x, BlocBleu->y-1, touche))     BlocBleu->y--;
              else if (what==NOIR && isValidMove(NOIR, BlocNoir->x, BlocNoir->y-1, touche))     BlocNoir->y--;
       }
       if (touche==BAS)
       {
              if (what==JOUEUR && isValidMove(JOUEUR, joueur.x, joueur.y+1, touche))
              {
                     joueur.y++;
                     frame=0;
                     direction=BAS;
                     mouvement=true;
              }
              else if (what==JOUEUR && !isValidMove(JOUEUR, joueur.x, joueur.y+1, touche))
              {
                     frame=0;
                     direction=BAS;
              }
              else if (what==LUMIERE && isValidMove(LUMIERE, lumiere.x, lumiere.y+1, touche))     lumiere.y++;
              else if (what==ROUGE && isValidMove(ROUGE, BlocRouge->x, BlocRouge->y+1, touche))     BlocRouge->y++;
              else if (what==VERT && isValidMove(VERT, BlocVert->x, BlocVert->y+1, touche))     BlocVert->y++;
              else if (what==BLEU && isValidMove(BLEU, BlocBleu->x, BlocBleu->y+1, touche))     BlocBleu->y++;
              else if (what==NOIR && isValidMove(NOIR, BlocNoir->x, BlocNoir->y+1, touche))     BlocNoir->y++;
       }
}

void renderReceptors( SDL_Surface* surf )
{
       for( unsigned char k=0; k<NbReceptorsLevel; k++ )
       {
              unsigned int lX = SIZE*ReceptorCollection[k].x+OFFSET_X+SIZE/2;
              unsigned int lY = SIZE*ReceptorCollection[k].y+OFFSET_Y+SIZE/2;
              unsigned char Rr = ReceptorCollection[k].R;
              unsigned char Gr = ReceptorCollection[k].G;
              unsigned char Br = ReceptorCollection[k].B;
              unsigned int direction = ReceptorCollection[k].direction;
              bool valide = ReceptorCollection[k].isvalide;

              unsigned char Rc,Gc,Bc;

              if (valide==false)
              {
                     Rc=0;
                     Gc=0;
                     Bc=0;
              }
              else
              {
                     Rc=255;
                     Gc=255;
                     Bc=255;
              }

              boxRGBA( surf, lX-4, lY-4, lX+4, lY+4, Rc, Gc, Bc, 255 );
              boxRGBA( surf,  lX-2, lY-2, lX+2, lY+2, Rr, Gr, Br, 255 );


              if (direction==GAUCHE)
              {
                     lineRGBA( surf,  lX-4, lY-1, lX-8, lY-1, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX-4, lY, lX-8, lY, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX-4, lY+1, lX-8, lY+1, Rc, Gc, Bc, 255 );
              }
              else if (direction==DROITE)
              {
                     lineRGBA( surf, lX+4, lY-1, lX+8, lY-1, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX+4, lY, lX+8, lY, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX+4, lY+1, lX+8, lY+1, Rc, Gc, Bc, 255 );
              }
              else if (direction==HAUT)
              {
                     lineRGBA( surf, lX-1, lY-4, lX-1, lY-8, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX, lY-4, lX, lY-8, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX+1, lY-4, lX+1, lY-8, Rc, Gc, Bc, 255 );
              }
              else if (direction==BAS)
              {
                     lineRGBA( surf, lX-1, lY+4, lX-1, lY+8, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX, lY+4, lX, lY+8, Rc, Gc, Bc, 255 );
                     lineRGBA( surf, lX+1, lY+4, lX+1, lY+8, Rc, Gc, Bc, 255 );
              }

       }
}

uint32_t _getpixel(SDL_Surface *surface, int x, int y)
{
       int bpp = surface->format->BytesPerPixel;
       /* Here p is the address to the pixel we want to retrieve */
       uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

       switch(bpp)
       {
       case 1:
              return *p;

       case 2:
              return *(uint16_t *)p;

       case 3:
              if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                     return p[0] << 16 | p[1] << 8 | p[2];
              else
                     return p[0] | p[1] << 8 | p[2] << 16;

       case 4:
              return *(uint32_t *)p;

       default:
              return 0;       /* shouldn't happen, but avoids warnings */
       }
}

void getPixelRGB( SDL_Surface* surf, unsigned int x, unsigned int y, unsigned char *R1, unsigned char* G1, unsigned char *B1 )
{

       Uint8 r,g,b;
       SDL_LockSurface( surf );
       SDL_GetRGB(_getpixel( surf, x, y), surf->format, &r, &g, &b);
       *R1=r;
       *G1=g;
       *B1=b;
       SDL_UnlockSurface( surf );
}


void checkReceptors( SDL_Surface* surf  )
{
       for( unsigned char k=0; k<NbReceptorsLevel; k++ )
       {
              unsigned int lX1=0, lX2=0, lX3=0;
              unsigned int lY1=0, lY2=0, lY3=0;

              unsigned int lX = SIZE*ReceptorCollection[k].x+OFFSET_X+SIZE/2;
              unsigned int lY = SIZE*ReceptorCollection[k].y+OFFSET_Y+SIZE/2;
              unsigned char Rr = ReceptorCollection[k].R;
              unsigned char Gr = ReceptorCollection[k].G;
              unsigned char Br = ReceptorCollection[k].B;
              unsigned int direction = ReceptorCollection[k].direction;

              if (direction==GAUCHE)
              {
                     // all 7 changed to 8
                     lX1=lX-7;
                     lX2=lX-7;
                     lX3=lX-7;
                     lY1=lY-1;
                     lY2=lY;
                     lY3=lY+1;
              }
              else if (direction==DROITE)
              {
                     lX1=lX+7;
                     lX2=lX+7;
                     lX3=lX+7;
                     lY1=lY-1;
                     lY2=lY;
                     lY3=lY+1;
              }
              else if (direction==HAUT)
              {
                     lX1=lX-1;
                     lX2=lX;
                     lX3=lX+1;
                     lY1=lY-7;
                     lY2=lY-7;
                     lY3=lY-7;
              }
              else if (direction==BAS)
              {
                     lX1=lX-1;
                     lX2=lX;
                     lX3=lX+1;
                     lY1=lY+7;
                     lY2=lY+7;
                     lY3=lY+7;
              }

              unsigned char R1, G1, B1;
              unsigned char R2, G2, B2;
              unsigned char R3, G3, B3;
              unsigned char Rm, Gm, Bm;

              getPixelRGB( surf, lX1, lY1, &R1, &G1, &B1 );
              getPixelRGB( surf, lX2, lY2, &R2, &G2, &B2 );
              getPixelRGB( surf, lX3, lY3, &R3, &G3, &B3 );

              Rm = (R1+R2+R3)/3;
              Gm = (G1+G2+G3)/3;
              Bm = (B1+B2+B3)/3;

//              _boxRGBA( 310, 10+k*30, 330, 30+k*30, Rm, Gm, Bm, 255 );
//              _rectangleRGBA( 310, 10+k*30, 330, 30+k*30, 0, 0, 0, 255 );

//              _boxRGBA( 340, 10+k*30, 360, 30+k*30, Rr, Gr, Br, 255 );
//              _rectangleRGBA( 340, 10+k*30, 360, 30+k*30, 0, 0, 0, 255 );


              if (Rm>=Rr-EPSILON && Rm<=Rr+EPSILON && Gm>=Gr-EPSILON && Gm<=Gr+EPSILON && Bm>=Br-EPSILON && Bm<=Br+EPSILON)
              {
                     ReceptorCollection[k].isvalide = true;
              }
              else
              {
                     ReceptorCollection[k].isvalide = false;
              }
       }
}

void renderDoors( SDL_Surface* surf )
{
       for( unsigned char k=0; k<NbDoorsLevel; k++ )
       {
              unsigned int lX = SIZE*DoorCollection[k].x+OFFSET_X;
              unsigned int lY = SIZE*DoorCollection[k].y+OFFSET_Y;

              SDL_Rect source, dest;

              dest.x = lX;
              dest.y = lY;
              dest.w = SIZE * ZOOMFACTOR;
              dest.h = SIZE * ZOOMFACTOR;



              int OFFSET_Y_TILE = 16*LEVELCOLOR;

              if (DoorCollection[k].isdoor==false) // check if it is a door or a wall (draw walls)
              {
                     if (DoorCollection[k].direction == HAUT)
                     {
                            //dsubimage( lX, lY, &sprites, 0,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                            source.x = 0;
                            source.y = OFFSET_Y_TILE+16;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == GAUCHE)
                     {
                            //dsubimage( lX, lY, &sprites, 16,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                            source.x = 16;
                            source.y = OFFSET_Y_TILE+16;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == BAS)
                     {
                            //dsubimage( lX, lY, &sprites, 32,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                            source.x = 32;
                            source.y = OFFSET_Y_TILE+16;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == DROITE)
                     {
                            //dsubimage( lX, lY, &sprites, 48,OFFSET_Y_TILE+16,16,16,  DIMAGE_NONE);
                            source.x = 48;
                            source.y = OFFSET_Y_TILE+16;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
              }
              else if (DoorCollection[k].isopen==false) // check if the door is open (draw stairs)
              {
                     if (DoorCollection[k].direction == HAUT)
                     {
                            //dsubimage( lX, lY, &sprites, 0,80,16,16,  DIMAGE_NONE);
                            source.x = 0;
                            source.y = 80;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == GAUCHE)
                     {
                            //dsubimage( lX, lY, &sprites, 16,80,16,16,  DIMAGE_NONE);
                            source.x = 16;
                            source.y = 80;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == BAS)
                     {
                            //dsubimage( lX, lY, &sprites, 32,80,16,16,  DIMAGE_NONE);
                            source.x = 32;
                            source.y = 80;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == DROITE)
                     {
                            //dsubimage( lX, lY, &sprites, 48,80,16,16,  DIMAGE_NONE);
                            source.x = 48;
                            source.y = 80;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
              }
              else  // check if the door is open (draw doors)
              {
                     if (DoorCollection[k].direction == HAUT)
                     {
                            //dsubimage( lX, lY, &sprites, 0,96,16,16,  DIMAGE_NONE);
                            source.x = 0;
                            source.y = 96;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == GAUCHE)
                     {
                            //dsubimage( lX, lY, &sprites, 16,96,16,16,  DIMAGE_NONE);
                            source.x = 16;
                            source.y = 96;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == BAS)
                     {
                            //dsubimage( lX, lY, &sprites, 32,96,16,16,  DIMAGE_NONE);
                            source.x = 32;
                            source.y = 96;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
                     else if (DoorCollection[k].direction == DROITE)
                     {
                            //dsubimage( lX, lY, &sprites, 48,96,16,16,  DIMAGE_NONE);
                            source.x = 48;
                            source.y = 96;
                            source.w = SIZE * ZOOMFACTOR;
                            source.h = SIZE * ZOOMFACTOR;

                            SDL_BlitSurface( sprites, &source, surf, &dest );
                     }
              }
       }

}

void checkDoors( SDL_Surface* surf )
{
       for( unsigned char k=0; k<NbDoorsLevel; k++ )
       {
              if (DoorCollection[k].isdoor==true) // check if it is a door or a wall
              {
                     bool temp = true;
                     for(int j=0; j<DoorCollection[k].nbreceptor; j++ )
                     {
                            int receptorReference =  DoorCollection[k].receptors[j];
                            bool currentrecept = ReceptorCollection[receptorReference].isvalide;
                            Receptor* tempRecept = &ReceptorCollection[receptorReference];

                            temp=temp && currentrecept;

                            if (currentrecept)
                            {
                                   if(DoorCollection[k].direction==HAUT)
                                   {
                                          int X = DoorCollection[k].x*SIZE+OFFSET_X;
                                          int Y = DoorCollection[k].y*SIZE-SIZE/4+OFFSET_Y;

                                          if (DoorCollection[k].nbreceptor==1)
                                          {
                                                 boxRGBA( surf, X+SIZE/2-1, Y-1, X+SIZE/2+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/2-2, Y-2, X+SIZE/2+2, Y+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==2)
                                          {
                                                 boxRGBA( surf, X+SIZE/3*(j+1)-1, Y-1, X+SIZE/3*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/3*(j+1)-2, Y-2, X+SIZE/3*(j+1)+2, Y+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==3)
                                          {
                                                 boxRGBA( surf, X+SIZE/4*(j+1)-1, Y-1, X+SIZE/4*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/4*(j+1)-2, Y-2, X+SIZE/4*(j+1)+2, Y+2, 150, 150, 150, 255);
                                          }
                                   }
                                   else if(DoorCollection[k].direction==BAS)
                                   {
                                          int X = DoorCollection[k].x*SIZE+OFFSET_X;
                                          int Y = DoorCollection[k].y*SIZE+SIZE+SIZE/4+OFFSET_Y;

                                          if (DoorCollection[k].nbreceptor==1)
                                          {
                                                 boxRGBA( surf, X+SIZE/2-1, Y-1, X+SIZE/2+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/2-2, Y-2, X+SIZE/2+2, Y+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==2)
                                          {
                                                 boxRGBA( surf, X+SIZE/3*(j+1)-1, Y-1, X+SIZE/3*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/3*(j+1)-2, Y-2, X+SIZE/3*(j+1)+2, Y+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==3)
                                          {
                                                 boxRGBA( surf, X+SIZE/4*(j+1)-1, Y-1, X+SIZE/4*(j+1)+1, Y+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X+SIZE/4*(j+1)-2, Y-2, X+SIZE/4*(j+1)+2, Y+2, 150, 150, 150, 255);
                                          }
                                   }
                                   else if(DoorCollection[k].direction==GAUCHE)
                                   {
                                          int X = DoorCollection[k].x*SIZE-SIZE/4+OFFSET_X;
                                          int Y = DoorCollection[k].y*SIZE+OFFSET_Y;

                                          if (DoorCollection[k].nbreceptor==1)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/2-1, X+1, Y+SIZE/2+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/2-2, X+2, Y+SIZE/2+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==2)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/3*(j+1)-1, X+1, Y+SIZE/3*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/3*(j+1)-2, X+2, Y+SIZE/3*(j+1)+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==3)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/4*(j+1)-1, X+1, Y+SIZE/4*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/4*(j+1)-2, X+2, Y+SIZE/4*(j+1)+2, 150, 150, 150, 255);
                                          }
                                   }
                                   else if(DoorCollection[k].direction==DROITE)
                                   {
                                          int X = DoorCollection[k].x*SIZE+SIZE+SIZE/4+OFFSET_X;
                                          int Y = DoorCollection[k].y*SIZE+OFFSET_Y;

                                          if (DoorCollection[k].nbreceptor==1)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/2-1, X+1, Y+SIZE/2+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/2-2, X+2, Y+SIZE/2+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==2)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/3*(j+1)-1, X+1, Y+SIZE/3*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/3*(j+1)-2, X+2, Y+SIZE/3*(j+1)+2, 150, 150, 150, 255);
                                          }
                                          else if (DoorCollection[k].nbreceptor==3)
                                          {
                                                 boxRGBA( surf, X-1, Y+SIZE/4*(j+1)-1, X+1, Y+SIZE/4*(j+1)+1, tempRecept->R, tempRecept->G, tempRecept->B, 255);
                                                 rectangleRGBA( surf, X-2, Y+SIZE/4*(j+1)-2, X+2, Y+SIZE/4*(j+1)+2, 150, 150, 150, 255);
                                          }
                                   }
                            }
                     }
                     DoorCollection[k].isopen=temp;
              }
       }
}


void exitAndFree( void )
{
       free(BlocksCollection);
       free(ReceptorCollection);
       free(DoorCollection);
       free(TreasureCollection);
       free(ObstaclesCollection);
//       free(BossCollection);
//       free(MonstersCollection);
//       free(BulletCollection);

       BlocksCollection=NULL;
       ReceptorCollection=NULL;
       DoorCollection=NULL;
       TreasureCollection=NULL;
       ObstaclesCollection=NULL;
       MonstersCollection=NULL;
       BossCollection=NULL;
       BulletCollection=NULL;


       SDL_FreeSurface( leveltiles );
       SDL_FreeSurface( sprites );
       SDL_FreeSurface( light );
       SDL_FreeSurface( player );
       SDL_FreeSurface( chests );
       SDL_FreeSurface( parchemin );
       SDL_FreeSurface( cursor );
       SDL_FreeSurface( monstres );
       SDL_FreeSurface(bigboss );
       SDL_FreeSurface( bossmages);
       SDL_FreeSurface(hearts);
       SDL_FreeSurface(fragments);
       SDL_FreeSurface(maintitle);
       SDL_FreeSurface(bigparch);

       SDL_FreeSurface( screenCalcul );

       nSDL_FreeFont(fontfantasyblack);
       nSDL_FreeFont(fontfantasygrey);
       nSDL_FreeFont(fontfantasyred);
       nSDL_FreeFont(fonttinyblack);
       nSDL_FreeFont(fonttinygrey);
       nSDL_FreeFont(fonttinywhite);

}


void initWalls( void )
{
       murs[0] = { HORIZONTAL, OFFSET_X+SIZE*1, OFFSET_X+SIZE*(SIZE_MAP_X-1), OFFSET_Y+SIZE };
       murs[1] = { VERTICAL, OFFSET_Y+SIZE*1, OFFSET_Y+SIZE*(SIZE_MAP_Y-1), OFFSET_X+SIZE*(SIZE_MAP_X-1) };
       murs[2] = { HORIZONTAL, OFFSET_X+SIZE*1, OFFSET_X+SIZE*(SIZE_MAP_X-1), OFFSET_Y+SIZE*(SIZE_MAP_Y-1) };
       murs[3] = { VERTICAL, OFFSET_Y+SIZE*1, OFFSET_Y+SIZE*(SIZE_MAP_Y-1), OFFSET_X+SIZE };

//       for(unsigned char k=0; k<SIZE_MAP_X; k++)
//              dprint_opt( OFFSET_X+SIZE*k+SIZE/2, OFFSET_Y-SIZE/2, C_WHITE, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, "%d", k );

//       for( unsigned char k=1; k<SIZE_MAP_Y; k++)
//              dprint_opt( OFFSET_X-SIZE/2, OFFSET_Y+k*SIZE+SIZE/2, C_WHITE, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, "%d", k );
}

/* return the sign of the number given in parameter*/
/* -1 if x<0   0 if x=0 and +1 if x>0 */
int _sgn( int x )
{
       if(x>=0)
              return 1;
       else if(x<0)
              return -1;
       else
              return 0;
}

/* return the absolute value of the number given in parameter*/
int _abs( int x )
{
       if(x>=0)
              return x;
       else
              return -1*x;
}

int computeLightModification( unsigned int currentCellX, unsigned int currentCellY, unsigned char* R, unsigned char* G, unsigned char* B, unsigned char* A )
{
       if (*A!=ALPHA_BLOQUE)
       {
              for( unsigned char k=0; k<NbBlocksLevel; k++ )
              {
                     if (BlocksCollection[k].x == currentCellX)
                            if (BlocksCollection[k].y == currentCellY)
                            {
                                   if (*R!=BlocNoir->R || *G!=BlocNoir->G || *B!=BlocNoir->B || *A!=ALPHA_OPAQUE )      // We check if the light is not already a black ray
                                   {
                                          if (BlocksCollection[k].type == OPAQUE)
                                          {
                                                 if (*R==lumiere.R && *G==lumiere.G && *B==lumiere.B && *A==lumiere.A)
                                                 {
                                                        *R = (*R-lumiere.R) & BlocksCollection[k].R;
                                                        *G = (*G-lumiere.G) & BlocksCollection[k].G;
                                                        *B = (*B-lumiere.B) & BlocksCollection[k].B;
                                                        *A = ALPHA_OPAQUE;
                                                 }
                                                 else
                                                 {
                                                        *R = *R & BlocksCollection[k].R;
                                                        *G = *G & BlocksCollection[k].G;
                                                        *B = *B & BlocksCollection[k].B;
                                                        *A = ALPHA_OPAQUE;
                                                 }
                                          }
                                          if (BlocksCollection[k].type == TRANSPARENT)
                                          {
                                                 if (*R==lumiere.R && *G==lumiere.G && *B==lumiere.B && *A==lumiere.A)
                                                 {
                                                        *R = (*R-lumiere.R) | BlocksCollection[k].R;
                                                        *G = (*G-lumiere.G) | BlocksCollection[k].G;
                                                        *B = (*B-lumiere.B) | BlocksCollection[k].B;
                                                        *A = ALPHA_TRANSPARENT;
                                                 }
                                                 else
                                                 {
                                                        *R = *R | BlocksCollection[k].R;
                                                        *G = *G | BlocksCollection[k].G;
                                                        *B = *B | BlocksCollection[k].B;
                                                        *A = ALPHA_TRANSPARENT;
                                                 }
                                          }
                                   }
                            }
              }

              for( unsigned char k=0; k<NbObstacleLevel; k++ )
              {
                     if (ObstaclesCollection[k].type!=BLOCK_WATER)
                            if (ObstaclesCollection[k].x == currentCellX)
                                   if (ObstaclesCollection[k].y == currentCellY)
                                   {
                                          *A = ALPHA_BLOQUE;
                                   }
              }

              if (*A == ALPHA_BLOQUE) return 1;
       }
       return 0;
}

void rayCast( SDL_Surface *surf, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char* Rs, unsigned char* Gs, unsigned char* Bs, unsigned char* As )
{
       // Cette fonction est un algo type BRESENHAM qui lance un rayon depuis la lumière jusqu'à chaque pixel des murs
       // Elle analyse la présence de blocs de glace de couleurs qui peuvent changer la couleur de la lumière du départ (Rs, Gs, Bs)
       // Elle dessine le point sur le mur de la bonne couleur

       // ATTENTION : toujours mettre x1 et y1 comme étant en premier et correspondant à la source de lumière !!!!


       char currentCellX = -1;
       char currentCellY = -1;

       unsigned int x,y;
       int dx,dy,swp,temp,s1,s2,p,i;

       x=x1;
       y=y1;
       dx=_abs(x2-x1);
       dy=_abs(y2-y1);
       s1=_sgn(x2-x1);
       s2=_sgn(y2-y1);
       swp=0;

       if(dy>dx)
       {
              temp=dx;
              dx=dy;
              dy=temp;
              swp=1;
       }
       p=2*dy-dx;
       for(i=0; i<dx; i+=SPEEDUP_RAY)
       {
              currentCellX = (x-OFFSET_X)/SIZE;
              currentCellY = (y-OFFSET_Y)/SIZE;
              int result = computeLightModification( currentCellX, currentCellY, Rs, Gs, Bs, As );


              if (result==1)
              {
                     if ((*Rs!=lumiere.R || *Gs!=lumiere.G || *Bs!=lumiere.B)  && *As==ALPHA_BLOQUE)
                     {
                            boxRGBA( surf, x-1, y-1, x+1, y+1,  *Rs, *Gs, *Bs, 255 );
                     }
              }
              else if (result==0)
              {
                     pixelRGBA( surf,  x, y, *Rs, *Gs, *Bs, *As );
              }


              while(p>=0)
              {
                     p=p-2*dx;
                     if(swp)
                            x+=s1*SPEEDUP_RAY;
                     else
                            y+=s2*SPEEDUP_RAY;
              }
              p=p+2*dy;
              if(swp)
                     y+=s2*SPEEDUP_RAY;
              else
                     x+=s1*SPEEDUP_RAY;
       }
}

void renderWalls( SDL_Surface *surf )
{
       unsigned char lR = lumiere.R;
       unsigned char lG = lumiere.G;
       unsigned char lB = lumiere.B;
       unsigned char lA = lumiere.A;

       unsigned int lX = SIZE*lumiere.x+OFFSET_X+SIZE/2;
       unsigned int lY = SIZE*lumiere.y+OFFSET_Y+SIZE/2;

       for( unsigned char k=0; k<4; k++ )
       {
              if (murs[k].direction == HORIZONTAL)
                     for( unsigned int z=murs[k].mini; z<=murs[k].maxi; z+=SPEEDUP_WALL)
                     {
                            lR = lumiere.R;
                            lG = lumiere.G;
                            lB = lumiere.B;
                            lA = lumiere.A;

                            rayCast( surf, lX, lY, z, murs[k].fixe,  &lR,&lG,&lB,&lA );

                            if (lR!=lumiere.R || lG!=lumiere.G || lB!=lumiere.B || lA!=lumiere.A )
                            {
                                   if(lA!=ALPHA_BLOQUE)
                                   {
                                          pixelRGBA( surf, z, murs[k].fixe+1,  lR,lG,lB, 255 );
                                          pixelRGBA( surf, z, murs[k].fixe,  lR,lG,lB, 255 );
                                          pixelRGBA( surf, z, murs[k].fixe-1,  lR,lG,lB, 255 );
                                   }
                            }
                     }

              if (murs[k].direction == VERTICAL)
                     for( unsigned int z=murs[k].mini; z<=murs[k].maxi; z+=SPEEDUP_WALL)
                     {
                            lR = lumiere.R;
                            lG = lumiere.G;
                            lB = lumiere.B;
                            lA = lumiere.A;

                            rayCast( surf,  lX, lY, murs[k].fixe, z, &lR,&lG,&lB,&lA );

                            if (lR!=lumiere.R || lG!=lumiere.G || lB!=lumiere.B || lA!=lumiere.A )
                            {
                                   if (lA!=ALPHA_BLOQUE)
                                   {
                                          pixelRGBA( surf,  murs[k].fixe+1, z, lR,lG,lB,255 );
                                          pixelRGBA( surf, murs[k].fixe, z, lR,lG,lB,255 );
                                          pixelRGBA( surf, murs[k].fixe-1, z, lR,lG,lB,255 );
                                   }
                            }
                     }

       }
}


void renderMap( SDL_Surface *surf )
{
       //dimage(OFFSET_X, OFFSET_Y, &mapbackgrd);
       unsigned int OFFSET_X_TILE = 0;

       OFFSET_X_TILE = 64*LEVELCOLOR*ZOOMFACTOR;

       for(unsigned char x=0; x<SIZE_MAP_X; x++)
              for( unsigned char y=0; y<SIZE_MAP_Y; y++)
              {
                     unsigned int lX=OFFSET_X+x*SIZE*ZOOMFACTOR;
                     unsigned int lY=OFFSET_Y+y*SIZE*ZOOMFACTOR;

                     SDL_Rect source, dest;

                     dest.x = lX;
                     dest.y = lY;
                     dest.w = SIZE * ZOOMFACTOR;
                     dest.h = SIZE * ZOOMFACTOR;


                     if(y==0)
                     {
                            if (x==0)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,0,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else if (x==SIZE_MAP_X-1)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,0,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+32,0,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+2*SIZE*ZOOMFACTOR;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                     }

                     else if(y==SIZE_MAP_Y-1)
                     {
                            if (x==0)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,16,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE;
                                   source.y = SIZE * ZOOMFACTOR;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else if (x==SIZE_MAP_X-1)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,16,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                                   source.y = SIZE * ZOOMFACTOR;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+48,16,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+3*SIZE*ZOOMFACTOR;
                                   source.y = SIZE * ZOOMFACTOR;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                     }
                     else
                     {
                            if (x==0)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+48,0,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+3*SIZE*ZOOMFACTOR;
                                   source.y = 0;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else if (x==SIZE_MAP_X-1)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+32,16,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+2*SIZE*ZOOMFACTOR;
                                   source.y = SIZE * ZOOMFACTOR;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else if ((x+y)%2==0)
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+0,32,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE;
                                   source.y = 2*SIZE*ZOOMFACTOR;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                            else
                                   //dsubimage( lX, lY, &leveltiles, OFFSET_X_TILE+16,32,16,16,  DIMAGE_NONE);
                            {
                                   source.x = OFFSET_X_TILE+SIZE*ZOOMFACTOR;
                                   source.y = 2*SIZE * ZOOMFACTOR;;
                                   source.w = SIZE * ZOOMFACTOR;
                                   source.h = SIZE * ZOOMFACTOR;
                                   SDL_BlitSurface( leveltiles, &source, surf, &dest );
                            }
                     }
              }
}


void initMap( void )
{
       for( unsigned char k=0; k < 33; k++) Map[k].visited = false;
       currentLevel = 0;
}


/*
void updateMainScreen( void )
{
       SDL_FillRect( screenCalcul, NULL, 0x00000000);
       //boxRGBA( double_buffer, 10, 10, 20, 20, 255, 0,0, 255);

       //SDL_BlitSurface(leveltiles, NULL, double_buffer, NULL );

       frame_cursor++;
       frame_cursor = frame_cursor % 4;

       drawInterface( screenCalcul );
       drawMinimap( screenCalcul );

       renderMap( screenCalcul );

       checkDoors( screenCalcul);
       renderDoors( screenCalcul);

       renderObstacles( screenCalcul, LAYER_BELOW_RAYS );
       renderObstacles( screenCalcul, LAYER_ABOVE_RAYS );

       renderWalls( screenCalcul );

       checkReceptors( screenCalcul );
       renderReceptors( screenCalcul );

       renderMovable( screenCalcul );

       renderLight( screenCalcul );
       renderTreasures( screenCalcul );

       renderMonsters( screenCalcul );
       renderBoss(screenCalcul );

       renderPlayer( screenCalcul );

       SDL_Flip(screenCalcul);
}
*/


int main(int argc, char **argv)
{
       initSDL();
       initWalls();

       loadResources();

       KeyManager::Initialize();
       MouseManager::Initialize();

       do
       {

              selectStartMenu = 0;
              selectDifficultyMenu = 0;
              selectOptionMenu = 0;
              selectOptionPause = 0;
              selectOptionLoose = 0;
              selectStoryMenu = 0;
              selectEndMenu = 0;
              difficulty = 0;

              doneGame = false;
              doneMain = false;
              doneOption = false;
              donePause = false;
              doneStart = false;
              doneTitle = false;
              doneStory = false;
              doneEnd = false;
              GotoMainMenu=false;

              drawTitle();

              SDL_Delay(200);

              unsigned char choice = drawStartMenu();

              if (choice==0)
              {
                     initMap();
                     loadprogress();

                     synchroniseGame( );
                     loadLevel( currentLevel );
                     adjustLevel( currentLevel );

              }
              else if (choice==1)
              {
                     // We start a new game, so we load Level #000

                     SDL_Delay(200);
                     difficulty = drawDifficultyMenu();

                     BlackFrag = false;
                     WhiteFrag = false;
                     RedFrag = false;
                     GreenFrag = false;
                     BlueFrag = false;
                     score = 0;

                     if (difficulty==0) life=5;
                     else  if (difficulty==1) life=3;
                     else if (difficulty==2) life=1;

                     SDL_Delay(200);

                     drawStoryMenu();

                     initMap();

                     loadLevel( 0 );
              }
              else if (choice==2)
              {
                     exitAndFree();
                     return 0;
              }

              initWalls();

              while (!doneGame)
              {

                     SDL_FillRect( screenCalcul, NULL, 0x00000000);

                     //boxRGBA( double_buffer, 10, 10, 20, 20, 255, 0,0, 255);
                     //SDL_BlitSurface(leveltiles, NULL, double_buffer, NULL );

                     frame_cursor++;
                     frame_cursor = frame_cursor % 4;

                     drawInterface( screenCalcul );
                     drawMinimap( screenCalcul );

                     renderMap( screenCalcul );

                     checkDoors( screenCalcul);
                     renderDoors( screenCalcul);

                     renderObstacles( screenCalcul, LAYER_BELOW_RAYS );
                     renderObstacles( screenCalcul, LAYER_ABOVE_RAYS );

                     renderWalls( screenCalcul );

                     checkReceptors( screenCalcul );
                     renderReceptors( screenCalcul );

                     renderMovable( screenCalcul );

                     renderLight( screenCalcul );
                     renderTreasures( screenCalcul );

                     renderMonsters( screenCalcul );
                     renderBoss(screenCalcul );

                     if (attackboss==false && NbBossLevel!=0 && frame_boss_attack == 0 )
                     {
                            srand( SDL_GetTicks() );
                            unsigned char randomattack = rand() % 101;

                            if (randomattack>=45 && randomattack<=65) launch_Boss_Attack();
                     }
                     if (attackboss==true) launch_Boss_Attack();


                     renderPlayer( screenCalcul );

                     SDL_Flip(screenCalcul);

                     get_inputs();


                     updateCurrentState( );


                     if (life==0) drawLoose();

                     if (selectOptionPause==1 || selectOptionPause==2 || selectOptionLoose==1) doneGame=true;

                     if (life==0 && selectOptionLoose==0)
                     {
                            if (difficulty==0) life=5;
                            else  if (difficulty==1) life=3;
                            else if (difficulty==2) life=1;

                            loadLevel( currentLevel );
                     }

                     if (WhiteFrag && BlackFrag && RedFrag && GreenFrag && BlueFrag )
                     {
                            the_end();
                            doneGame=true;
                            delete_save();
                            initMap();
                     }



              }

              //updateCurrentState( );

              if (doneEnd==false) saveprogress();

       }
       while (GotoMainMenu);


       KeyManager::Close();
       MouseManager::Close();

       exitAndFree( );

       return 0;
}



void initSDL(void)
{
       if(SDL_Init(SDL_INIT_VIDEO) == -1)
       {
              printf("Couldn't initialize SDL: %s\n", SDL_GetError());
              exit(EXIT_FAILURE);
       }

       screenCalcul = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);

       if(screenCalcul == NULL)
       {
              printf("Couldn't initialize display: %s\n", SDL_GetError());
              SDL_Quit();
              exit(EXIT_FAILURE);
       }
}
