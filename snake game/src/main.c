/*
A snake game project in work, the project comes to demonstrate 
the use of libraries as well as working with pointers and 
concepts of computer science. 
*/
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"
#include <stdbool.h>



TTF_Font* font = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* gif_frames [MAX_FRAMES];
SDL_Event event;
int game_is_running = FALSE;
int total_frames = 0;
int last_frame_time = 0;
int score = 0;

int initilize_window(void);
void render_menue();
void setup();
void process_input();
void update();
void render();
void render_game_over();
void destroy_window();
void load_frames(char *);
void display_startup_gif();
void close_gif();
void display_intro();


struct snake{
    int x[MAX_SNAKE_LENGTH]; // X position on the segments.
    int y[MAX_SNAKE_LENGTH]; // Y position on the segments.
    int length; //Current length of the snake.
    int direction; // 0 = Up, 1 = Right, 2 = Down, 3 = Left. 
}snake;

struct food{
    int x;
    int y;
    int size;
}food;

typedef enum{
    INTRO,
    MENU,
    GAME,
    GAME_OVER,
    QUIT
}GameState;

GameState current_state = INTRO; 



int main()
{
    game_is_running = initilize_window();
    setup();

    IMG_Init(IMG_INIT_PNG);
    

    while(game_is_running)
    {
        process_input();

        if(current_state == INTRO)
        {
            load_frames("start/frame%d.png");
            display_intro();
        }

        if(current_state == MENU)
        {
            render_menue(); // render the menue.
        }
        else if(current_state == GAME)
        {
            update(); //Update the game
            render();//Render the game.
        }
        else if(current_state == GAME_OVER)
        {
            render_game_over();
            score = 0;
        }
        else if(current_state == QUIT)
        {
            game_is_running = FALSE;//Quit the game.
        }
        
    }

    destroy_window();

    return 0;
}


int initilize_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
            NULL, 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT, 
            0
            );
    if(!window)
    {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        return FALSE;
    }

    return TRUE;
}

void render_menue()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//Black background.
    SDL_RenderClear(renderer);

    //Render the "Start Game" and "Quit" options.
    SDL_Color white = {255, 255, 255, 255};//White color for text.
    char *start_text = "Press ENTER to start Game.";
    char *quit_text = "Press ESC to Quit";

    //Creat textures for the text.
    SDL_Surface* surface = TTF_RenderText_Solid(font, start_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {WINDOW_WIDTH/2 - surface->w/2, WINDOW_HEIGHT/3, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(font, quit_text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.y+=40;//Move the second option by 40 pixels downward (the quit text).
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void setup()
{
    snake.length = 3;
    snake.x[0] = 400; snake.y[0] = 400; // head of the snake.
    snake.x[1] = 390; snake.y[1] = 400; // body segment 1.
    snake.x[2] = 380; snake.y[2] = 400; // body segment 2.
    snake.direction = 1; //Initialize direction to Right.

    //Initialize food position.
    food.x = rand() % (WINDOW_WIDTH/FOOD_SIZE)*FOOD_SIZE;
    food.y = rand() % (WINDOW_HEIGHT/FOOD_SIZE)*FOOD_SIZE;

    font = TTF_OpenFont("font/Raleway-VariableFont_wght.ttf", 30);
    if(!font)
    {
        fprintf(stderr, "Error loaing font: %s\n", TTF_GetError());
        game_is_running = FALSE;
    }
}

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    if(event.type == SDL_QUIT)
    {
        current_state = QUIT;
    }

    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.scancode)
        {
            case SDL_SCANCODE_RETURN: // Enter key.
                if(current_state == MENU )
                {
                    current_state = GAME;//Restart Game.
                    setup();
                }
                if(current_state == GAME_OVER || current_state == INTRO)
                {
                    current_state = MENU;
                }
                break;
            case SDL_SCANCODE_ESCAPE: // ESCAPE key.
                if(current_state == MENU || current_state == GAME_OVER)
                {
                    //QUIT the game directly from the menu.
                    current_state = QUIT;
                }
                break;
            default:
                break;
        }

        if(current_state == GAME)
        {
        // keyboard API for key pressed
        switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
                if (snake.direction != 2) snake.direction = 0; // Up
                break;
            case SDL_SCANCODE_RIGHT:
                if (snake.direction != 3) snake.direction = 1; // Right
                break;
            case SDL_SCANCODE_DOWN:
                if (snake.direction != 0) snake.direction = 2; // Down
                break;
            case SDL_SCANCODE_LEFT:
                if (snake.direction != 1) snake.direction = 3; // Left
                break;
            case SDL_SCANCODE_ESCAPE:
                game_is_running = FALSE;
                break;
            default:
                break;
				}
        }
    }
}


void update()
{
    // Move the snake's body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // Move the head in the current direction
    if (snake.direction == 0) snake.y[0] -= SNAKE_SEGMENT_SIZE; // Up
    if (snake.direction == 1) snake.x[0] += SNAKE_SEGMENT_SIZE; // Right
    if (snake.direction == 2) snake.y[0] += SNAKE_SEGMENT_SIZE; // Down
    if (snake.direction == 3) snake.x[0] -= SNAKE_SEGMENT_SIZE; // Left

    // Check for collisions with walls
    if (snake.x[0] < 0 || snake.x[0] >= WINDOW_WIDTH ||
        snake.y[0] < 0 || snake.y[0] >= WINDOW_HEIGHT) {
        current_state = GAME_OVER; 
    }

    // Check for collisions with itself
    for (int i = 1; i < snake.length; i++) {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
            current_state = GAME_OVER; 
        }
    }

    // Check if the snake eats the food
    if (snake.x[0] < food.x + FOOD_SIZE && 
    snake.x[0] + SNAKE_SEGMENT_SIZE > food.x &&
    snake.y[0] < food.y + FOOD_SIZE && 
    snake.y[0] + SNAKE_SEGMENT_SIZE > food.y) 
    {
        snake.length++; // Increase the length of the snake.
        score+=1;

        // Initialize the new segment's position
        snake.x[snake.length - 1] = snake.x[snake.length - 2];
        snake.y[snake.length - 1] = snake.y[snake.length - 2];

        // Reposition the food
        food.x = rand() % (WINDOW_WIDTH / FOOD_SIZE) *  FOOD_SIZE;
        food.y = rand() % (WINDOW_HEIGHT /  FOOD_SIZE) *  FOOD_SIZE;
    }

    // Frame rate control
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    last_frame_time = SDL_GetTicks();
}



void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);// Black background.
    SDL_RenderClear(renderer);

    //Draw a snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//Green snake.
    for(int i=0; i<snake.length; i++)
    {
        SDL_Rect snake_rect = {snake.x[i], snake.y[i], SNAKE_SEGMENT_SIZE, SNAKE_SEGMENT_SIZE};
        SDL_RenderFillRect(renderer, &snake_rect);
    }

    // Draw food.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);// red food.
    SDL_Rect food_rect = {food.x, food.y, FOOD_SIZE, FOOD_SIZE};
    SDL_RenderFillRect(renderer, &food_rect);

    //render the score.
    SDL_Color white = {255, 255, 255, 255};// White color.
    char score_text[50];
    sprintf(score_text, "Score: %d", score);

    SDL_Surface* surface = TTF_RenderText_Solid(font, score_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect score_rect = {10, 10, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &score_rect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void render_game_over()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render the "Game Over" text
    SDL_Color white = {255, 255, 255, 255}; // White color for text
    char *game_over_text = "Game Over! Press ENTER to Restart or ESC to Quit";

    // Create textures for the text
    SDL_Surface* surface = TTF_RenderText_Solid(font, game_over_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {WINDOW_WIDTH / 2 - surface->w / 2, WINDOW_HEIGHT / 3, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void destroy_window()
{
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void load_frames(char *frames)
{
    total_frames = 0;
    char frame_path[ MAX_FRAMES];
    for(int i=0; i<MAX_FRAMES; i++)
    {
        snprintf(frame_path, sizeof(frame_path), frames, i);
        SDL_Surface* temp_surface = IMG_Load(frame_path);
        if(!temp_surface)
        {
            printf("Failed to load frame: %s\n", frame_path);
            break;
        }
        gif_frames[i] = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_FreeSurface(temp_surface);
        total_frames++;
    }
    
}


void close_gif()
{
    for(int i = 0; i < total_frames; i++)
    {
        if(gif_frames[i])
        {
            SDL_DestroyTexture(gif_frames[i]);
        }
    }
}

void display_startup_gif() {
    int current_frame = 0;
    Uint32 last_frame_time = 0;
    const Uint32 frame_delay = 80; // Delay in milliseconds

    bool quit = false;

        SDL_Color black = {0, 0, 0, 255}; // Black color for text
        char *game_over_text = "Game Over! Press ENTER to Restart or ESC to Quit";

       
         // Create textures for the text
        SDL_Surface* surface = TTF_RenderText_Solid(font, game_over_text, black);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {WINDOW_WIDTH / 2 - surface->w / 2, 1, surface->w, surface->h};
        SDL_FreeSurface(surface);    

    while (!quit) 
    {

        process_input();

        Uint32 current_time = SDL_GetTicks();
        if (current_time > (last_frame_time + frame_delay) -1) {
            current_frame = (current_frame + 1) % total_frames; // Cycle through frames
            last_frame_time = current_time;

            if (current_frame == 0) {
                quit = true; // Exit after one full GIF loop
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        SDL_Rect gif_rect = {0, 0, 1000, 800}; // Position and size of the GIF
        SDL_RenderCopy(renderer, gif_frames[current_frame], NULL, &gif_rect);

        // Render the "Game Over!" text (this will be on top of the GIF)    
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
}


void display_intro()
{
    int current_frame = 0;
    Uint32 last_frame_time = 0;
    const Uint32 frame_delay = 30; // Delay in milliseconds

    SDL_Color black = {255, 255, 255, 255}; // Black color for text
    char *game_over_text = "D-Game LTD";

    // Create textures for the text
    SDL_Surface* surface = TTF_RenderText_Solid(font, game_over_text, black);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {WINDOW_WIDTH /2  - surface->w / 2, WINDOW_HEIGHT/2, surface->w, surface->h};
    SDL_FreeSurface(surface);

    bool gif_finished = false; // Flag to determine if the GIF has finished

    while (true) 
    {
        Uint32 current_time = SDL_GetTicks();
        if (!gif_finished && current_time > last_frame_time + frame_delay) {
            // Advance to the next frame
            current_frame++;
            last_frame_time = current_time;

            // If the last frame is reached, mark the GIF as finished
            if (current_frame >= total_frames) {
                gif_finished = true;
                current_frame = total_frames - 1; // Stick to the last frame
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the current frame of the GIF
        SDL_Rect gif_rect = {0, 0, 1000, 800}; // Position and size of the GIF
        SDL_RenderCopy(renderer, gif_frames[current_frame], NULL, &gif_rect);

        

        // Render the text on top of the last frame
        if (gif_finished) {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            
        }
        
        // Present the frame
        SDL_RenderPresent(renderer);

        if (gif_finished) {
            SDL_Delay(1500);
            current_state = MENU;
            break;
            
        }
    }

    // Clean up the text texture
    SDL_DestroyTexture(texture);
}



