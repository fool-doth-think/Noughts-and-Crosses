//Using SDL and standard IO
#include <stdio.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <string>
#include <iostream>
#include <SDL2\SDL_mixer.h>
#include <cmath>

//0 is nought and 1 is cross, 

class LTexture{
    public:
        LTexture();
        ~LTexture();

        bool loadFromFile(std::string path);
        void free();

        void render(int x, int y);

        int getWidth();
        int getHeight();
    
    private:
        SDL_Texture* mTexture; 
        int mWidth;
        int mHeight;
}; 

class Square{
    public:
        Square();


        int getXPos();
        int getYPos();

        void setPos(int x, int y); 

        int getSymbol();
        void setSymbol(int sym); 

        void handleEvent(SDL_Event* e);

    private:
        int mXPos;
        int mYPos;
        int mSymbol;
};

const int SCREEN_WIDTH = 600, SCREEN_HEIGHT = 600;
int gTurn = 1; 

bool init();
bool loadMedia();
void close(); 
SDL_Surface* LoadSurface(std::string path); 

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gTest = NULL; 
SDL_Renderer* gRenderer = NULL;


LTexture gCrossTexture;
LTexture gBackgroundTexture;
LTexture gNoughtTexture;

bool init(){
    bool success = true;
    if( SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDLcould not initalise! SDL Error : %s\n", SDL_GetError());
        success = false; 
    } else {
        gWindow = SDL_CreateWindow("Noughts_and_Crosses.exe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
        if(gWindow == NULL){
            printf("Window could not be created, SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            int imgflags = IMG_INIT_PNG;
            if(!(IMG_Init(imgflags) & imgflags)){
                printf("SDL_Image could not initialise %s\n", IMG_GetError()); 
                success = false;
            } 

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                printf("Renderer could not be created, SDL Error : %s\n", SDL_GetError()); 
                success = false; 
            }
        }

    }

    return success;

};






LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){


    SDL_Texture* newTexture = NULL;


    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL){
        printf("Unable to load image %s, SDL Error : %s\n", path.c_str(), SDL_GetError());

    } else {

        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if(newTexture == NULL){
            printf("Unable to create texture from %s , SDL Error : %s\n", path.c_str(), SDL_GetError());
        } else {

            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h; 

        }

        SDL_FreeSurface(loadedSurface); 
    }

    mTexture = newTexture; 
    return mTexture != NULL; 
    

}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y){
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);  
}



int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight; 
}



Square::Square(){
    mXPos = 0;
    mYPos = 0;
    mSymbol = -1; 
}

int Square::getXPos(){
    return mXPos;
}

int Square::getYPos(){
    return mYPos;
}

void Square::setPos(int x, int y){
    mXPos = x;
    mYPos = y;
}

int Square::getSymbol(){
    return mSymbol; 
}

void Square::setSymbol(int sym){
    mSymbol = sym; 
}

void Square::handleEvent(SDL_Event* e){
    int x, y;
    if(e->type = SDL_MOUSEBUTTONUP){
        SDL_GetMouseState(&x, &y);
        if((x > mXPos && x < mXPos + 200 && y > mYPos && y < mYPos + 200) && mSymbol == -1){
            mSymbol = gTurn; 
            if(gTurn == 0){
                gTurn = 1;
            } else { 
                gTurn = 0;
            }
        }
    }
}

int checkWin(Square gridArray[]){
    int win = -1;
    if(gridArray[4].getSymbol() == 0){
        if(gridArray[0].getSymbol() == 0 && gridArray[8].getSymbol() == 0){
            win = 0; 
        }
        if(gridArray[1].getSymbol() == 0 && gridArray[7].getSymbol() == 0){
            win = 0; 
        }
        if(gridArray[2].getSymbol() == 0 && gridArray[6].getSymbol() == 0){
            win = 0; 
        }
        if(gridArray[3].getSymbol() == 0 && gridArray[5].getSymbol() == 0){
            win = 0; 
        }
    }
    if(gridArray[0].getSymbol() == 0){
        if(gridArray[1].getSymbol() == 0 && gridArray[2].getSymbol() == 0){
            win = 0; 
        }
        if(gridArray[3].getSymbol() == 0 && gridArray[6].getSymbol() == 0){
            win = 0; 
        }
    }
    if(gridArray[8].getSymbol() == 0){
        if(gridArray[5].getSymbol() == 0 && gridArray[2].getSymbol() == 0){
            win = 0; 
        }
        if(gridArray[7].getSymbol() == 0 && gridArray[6].getSymbol() == 0){
            win = 0; 
        }
    }
    



    if(gridArray[4].getSymbol() == 1){
        if(gridArray[0].getSymbol() == 1 && gridArray[8].getSymbol() == 1){
            win = 1; 
        }
        if(gridArray[1].getSymbol() == 1 && gridArray[7].getSymbol() == 1){
            win = 1; 
        }
        if(gridArray[2].getSymbol() == 1 && gridArray[6].getSymbol() == 1){
            win = 1; 
        }
        if(gridArray[3].getSymbol() == 1 && gridArray[5].getSymbol() == 1){
            win = 1; 
        }
    }
    if(gridArray[0].getSymbol() == 1){
        if(gridArray[1].getSymbol() == 1 && gridArray[2].getSymbol() == 1){
            win = 1; 
        }
        if(gridArray[3].getSymbol() == 1 && gridArray[6].getSymbol() == 1){
            win = 1; 
        }
    }
    if(gridArray[8].getSymbol() == 1){
        if(gridArray[5].getSymbol() == 1 && gridArray[2].getSymbol() == 1){
            win = 1; 
        }
        if(gridArray[7].getSymbol() == 1 && gridArray[6].getSymbol() == 1){
            win = 1; 
        }
    }
        

    return win;
}

bool loadMedia(){
    bool success = true;
    if(!gCrossTexture.loadFromFile("textures/x.png")){
        printf("Failed to load media");
        success = false;
    }
    if(!gBackgroundTexture.loadFromFile("textures/XandOgrid.png")){
        printf("Failed to load media");
        success = false; 
    }
    if(!gNoughtTexture.loadFromFile("textures/o.png")){
        printf("Failed to load media");
        success = false; 
    }
    
        
    

    return success; 

}

void reset(Square gridArray[]){
    for(int i = 0; i < 9; i++){
        gridArray[i].setSymbol(-1); 
    }
    gTurn = 1; 

}

void close(){
    gBackgroundTexture.free();
    gCrossTexture.free();

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL; 
    SDL_DestroyWindow(gWindow);
    gWindow = NULL; 

    IMG_Quit(); 
    SDL_Quit(); 
}


int main(int argc, char *argv[]){

    
    if(!init()){
        printf("Failed to initalise\n");
    } else if(!loadMedia()){
        printf("Failed to load media\n");
    } 

    Square gridArray[9];
    gridArray[0].setPos(0, 0);
    gridArray[1].setPos(200, 0);
    gridArray[2].setPos(400, 0);
    gridArray[3].setPos(10, 200);
    gridArray[4].setPos(200, 200);
    gridArray[5].setPos(400, 200);
    gridArray[6].setPos(0, 400);
    gridArray[7].setPos(200, 400);
    gridArray[8].setPos(400, 400); 

    int win;


    SDL_Event e;
    
    while(true){
        if( SDL_PollEvent(&e)){
            if(SDL_QUIT == e.type){
                break;
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                for(int i = 0; i < 9; i++){
                    gridArray[i].handleEvent(&e); 
                }
            }
            if(e.type == SDL_KEYDOWN){
                reset(gridArray);
            }
            
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); 
        SDL_RenderClear(gRenderer);
   
        gBackgroundTexture.render(0, 0);

        for(int i = 0; i < 9; i++){
            if(gridArray[i].getSymbol() == 0){
                gNoughtTexture.render(gridArray[i].getXPos(), gridArray[i].getYPos());
            }
            if(gridArray[i].getSymbol() == 1){
                gCrossTexture.render(gridArray[i].getXPos(), gridArray[i].getYPos());
            }
        }
        
        int win = checkWin(gridArray);

        if(win == 1){
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
            SDL_RenderClear(gRenderer);
        }
        if(win == 0){
            SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);
            SDL_RenderClear(gRenderer);
        }


        SDL_RenderPresent( gRenderer );

    }


    close(); 
    SDL_Quit();

    return 0;
}