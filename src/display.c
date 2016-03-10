#include "display.h"

#ifndef NO_GRAPHICS

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

int display_init() {
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return 0;
    }
    else
    {
        //Initialize window and renderer, and check for NULL
        //SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &sdlWindow, &sdlRenderer);
        sdlWindow = SDL_CreateWindow("Penguins ! ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

        if( sdlWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return 0;
        }

        SDL_SetWindowIcon(sdlWindow, IMG_Load("assets/icon.png"));
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC);
        if(sdlRenderer == NULL)
        {
            printf("Renderer could not be created! SDL_Error: %s\n",  SDL_GetError());
            return 0;
        }

        previousTime = currentTime = SDL_GetTicks();
    }

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return 0;
    }

    return 1;

}

void display_free()
{

    // /!\ Do not forget to free all textures !

    //Destroy window
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow( sdlWindow );
    sdlWindow = NULL;
    sdlRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

int graphics_init(struct graph* g, char* layout)
{
    int nb_tiles = g->nb_tiles;

    graphics =  malloc(sizeof(struct gameGraphics));
    initColors();

    graphics->nb_tiles = nb_tiles;
    graphics->tiles = malloc(nb_tiles * sizeof(graphicElement*));
    for (int i = 0; i < nb_tiles; ++i) {
        graphics->tiles[i] = calloc(1, sizeof(graphicElement));
        graphics->tiles[i]->color = (SDL_Color) {
            255,255,255,255
        };
    }

    graphics->penguin_sprite = loadTexture("assets/tux.png");
    graphics->nb_penguins = g->nb_penguins * g->nb_players;
    graphics->penguins = malloc(graphics->nb_penguins * sizeof(graphicElement*));
    for (int i = 0; i < graphics->nb_penguins; ++i) {
        graphics->penguins[i] = calloc(1, sizeof(graphicElement));
        graphics->penguins[i]->sprite = graphics->penguin_sprite;
        graphics->penguins[i]->color  = graphics->colors[penguin_owner(i)];
        graphics->penguins[i]->w = 0.8;
        compute_size(graphics->penguins[i]->sprite, &(graphics->penguins[i]->w), &(graphics->penguins[i]->h));
        graphics->penguins[i]->x = -graphics->penguins[i]->w;
    }


    graphics->background = loadTexture("assets/ocean.jpg");

    if(strcmp(layout, "full-hexa") == 0) {
        graphics->nb_sprites = 3;
        graphics->tiles_sprites = malloc(3 * sizeof(SDL_Texture*));
        graphics->tiles_sprites[0] = loadTexture("assets/tile-hexa-1.png");
        graphics->tiles_sprites[1] = loadTexture("assets/tile-hexa-2.png");
        graphics->tiles_sprites[2] = loadTexture("assets/tile-hexa-3.png");

    }
    else if(strcmp(layout, "full-tetra") == 0) {
        graphics->nb_sprites = 3;
        graphics->tiles_sprites = malloc(3 * sizeof(SDL_Texture*));
        graphics->tiles_sprites[0] = loadTexture("assets/tile-tetra-1.png");
        graphics->tiles_sprites[1] = loadTexture("assets/tile-tetra-2.png");
        graphics->tiles_sprites[2] = loadTexture("assets/tile-tetra-3.png");
    }
    else if(strcmp(layout, "octo-tetra") == 0) {
        graphics->nb_sprites = 6;
        graphics->tiles_sprites = malloc(6 * sizeof(SDL_Texture*));
        graphics->tiles_sprites[0] = loadTexture("assets/tile-tetra-1.png");
        graphics->tiles_sprites[1] = loadTexture("assets/tile-tetra-2.png");
        graphics->tiles_sprites[2] = loadTexture("assets/tile-tetra-3.png");
        graphics->tiles_sprites[3] = loadTexture("assets/tile-octo-1.png");
        graphics->tiles_sprites[4] = loadTexture("assets/tile-octo-2.png");
        graphics->tiles_sprites[5] = loadTexture("assets/tile-octo-3.png");
    }


    int* visited = calloc(nb_tiles, sizeof(int));
    compute_geometry(g, 0, visited, 0.0,0.0, layout);
    free(visited);
    align_n_resize();

    sdlFont = TTF_OpenFont("assets/font.ttf", sdlFontSize);
    if(sdlFont == NULL)
        printf("Error opening font ! SDL_error : %s\n", TTF_GetError());


    return 1;
}

void graphics_free()
{
    for (int i = 0; i < graphics->nb_tiles; ++i)
    {
        if(graphics->tiles[i] != NULL) {
            if(graphics->tiles[i]->sprite != NULL)
                SDL_DestroyTexture(graphics->tiles[i]->sprite);

            free(graphics->tiles[i]);
        }
    }
    free(graphics->tiles);

    for (int i = 0; i < graphics->nb_penguins; ++i)
    {
        if(graphics->penguins[i] != NULL) {
            if(graphics->penguins[i]->sprite!= NULL)
                SDL_DestroyTexture(graphics->penguins[i]->sprite);

            free(graphics->penguins[i]);
        }


    }
    free(graphics->penguins);

    SDL_DestroyTexture(graphics->background);
    SDL_DestroyTexture(graphics->penguin_sprite);

    for (int i = 0; i < graphics->nb_sprites; ++i)
        SDL_DestroyTexture(graphics->tiles_sprites[i]);

    free(graphics->tiles_sprites);

    TTF_CloseFont(sdlFont);
}

void initColors()
{
    graphics->colors[0] = (SDL_Color) {
        255,100,100,255
    };
    graphics->colors[1] = (SDL_Color) {
        100,100,255,255
    };
    graphics->colors[2] = (SDL_Color) {
        100,255,255,255
    };
    graphics->colors[3] = (SDL_Color) {
        100,255,100,255
    };
    graphics->colors[4] = (SDL_Color) {
        255,255,100,255
    };
    graphics->colors[5] = (SDL_Color) {
        255,100,255,255
    };
}

SDL_Texture* loadTexture(char* filepath)
{
    SDL_Texture *texture = IMG_LoadTexture(sdlRenderer, filepath);
    if (texture == NULL) {
        printf("Texture %s could not be loaded! SDL_Error : %s \n", filepath, SDL_GetError());
    }
    return texture;
}

SDL_Texture* loadTextTexture(char* text, SDL_Color color, int size)
{
    if(size != sdlFontSize && size != 0){
        TTF_CloseFont(sdlFont);
        sdlFontSize = size;
        sdlFont = TTF_OpenFont("assets/font.ttf", sdlFontSize);
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(sdlFont, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}

void compute_size(SDL_Texture* texture, float* w, float* h)
{
    int trueW, trueH;
    SDL_QueryTexture(texture, NULL, NULL, &trueW, &trueH);
    if(*w != 0)
    {
        *h = trueH * (*w/trueW);
    }
    else if(*h != 0)
    {
        *w = trueW * (*h/trueH);
    }
    else
    {
        *w = trueW;
        *h = trueH;
    }
}

void renderTexture(SDL_Texture* texture, float x, float y, float w, float h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    //If width or height not specified, query the dimensions of the texture itself
    if(w == 0 || h == 0)
        compute_size(texture, &w, &h);


    rect.w = w;
    rect.h = h;


    //render the texture on the screen
    SDL_RenderCopy(sdlRenderer, texture, NULL, &rect);

}

int handleEvents(int* quit)
{
    SDL_Event e;
    int result = 0;
    static int autoSkip = 0;

    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case SDL_QUIT:
            *quit = 1;
            break;
        case SDL_WINDOWEVENT:
            switch(e.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                *quit = 1;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SCREEN_WIDTH = e.window.data1;
                SCREEN_HEIGHT = e.window.data2;
                align_n_resize();
                break;
            }

            break;
        case SDL_MOUSEBUTTONDOWN:
            result = 1;
            break;
	case SDL_KEYDOWN:
	    switch(e.key.keysym.sym) {
	    case SDLK_ESCAPE:
	    case SDLK_q:
		*quit = 1;
		break;
	    case SDLK_RETURN:
		autoSkip = !autoSkip;
		break;
	    default:
		result = 1;
		break;
	    }
        }
    }
    
    if(autoSkip)
	return 1;
    else
	return result;
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    SDL_Rect rect = {x1, x2, x2 - x1, y2 - y1};
    SDL_RenderFillRect(sdlRenderer, &rect);
}

void drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(sdlRenderer, x1, y1, x2, y2);
}

void drawHexagon(int x, int y, int size)
{
    SDL_Point points[6];

    int i = 0;
    for(float angle = 0; angle <= 2 * PI; angle += PI/3.0, i++)
    {
        points[i].x = cos(angle)*size + x;
        points[i].y = sin(angle)*size + y;
    }

    SDL_RenderDrawPoints(sdlRenderer, points, 6);
    SDL_RenderDrawLines(sdlRenderer, points, 7);
}

void drawElement(graphicElement* elem)
{
    if(elem->sprite != NULL) {
        SDL_SetTextureAlphaMod(elem->sprite, elem->color.a);
        SDL_SetTextureColorMod(elem->sprite, elem->color.r, elem->color.g, elem->color.b);
        renderTexture(elem->sprite, elem->x, elem->y, elem->w, elem->h);
        SDL_SetTextureColorMod(elem->sprite, 255,255,255);
        SDL_SetTextureAlphaMod(elem->sprite, 255);
    }
}

SDL_Point get_ElementCenter(graphicElement* elem)
{
    SDL_Point center;
    center.x = elem->x + elem->w/2;
    center.y = elem->y + elem->h/2;

    return center;
}

void drawGraphics()
{
    renderTexture(graphics->background, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    for (int i = 0; i < graphics->nb_tiles; ++i)
        if(graphics->tiles[i] != NULL)
            drawElement(graphics->tiles[i]);


    for (int i = 0; i < graphics->nb_penguins; ++i)
        if(graphics->penguins[i] != NULL)
            drawElement(graphics->penguins[i]);
}

void drawEdges(struct graph* g) {

    SDL_SetRenderDrawColor(sdlRenderer, 0,0,0,255);
    SDL_Point centers[2];
    for (int i = 0; i < g->nb_tiles; ++i)
    {
        if(graphics->tiles[i] != NULL)
        {
            centers[0] = get_ElementCenter(graphics->tiles[i]);

            for (int j = 0; j < g->tiles[i].nb_neighbours; ++j)
            {
                if(g->tiles[i].neighbours[j] != NO_TILE) {
                    centers[1] = get_ElementCenter(graphics->tiles[g->tiles[i].neighbours[j]]);
                    SDL_RenderDrawLines(sdlRenderer, centers, 2);
                }


            }
        }

    }

}

void drawIds()
{
    SDL_Texture* texture;
    SDL_Color color = {0,0,0,255};
    char text[100];
    for (int i = 0; i < graphics->nb_tiles; ++i)
    {
        if(graphics->tiles[i] != NULL)
        {
            SDL_Point center = get_ElementCenter(graphics->tiles[i]);
            sprintf(text, "%d", i);
            texture = loadTextTexture(text, color, 25);
            renderTexture(texture, center.x, center.y, 0,0);
            SDL_DestroyTexture(texture);
        }
    }
}

void drawScores()
{
    char text[100];
    SDL_Texture* texture;
    for(int player = 0; player < get_nb_players(); ++player)
    {
        sprintf(text, "%d", get_score(player));
        texture = loadTextTexture(text, graphics->colors[player], 50);

        renderTexture(texture, 1, player * 50, 0,0);
        SDL_DestroyTexture(texture);
    }
}

void draw()
{
    if((currentTime = SDL_GetTicks()) - previousTime >= TICKS_PER_FRAME)
    {
        SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
        SDL_RenderClear(sdlRenderer);
        drawGraphics();
        //drawIds();
        drawScores();
        previousTime = currentTime;
    }

}

void synchronize(struct graph* g)
{
    for (int i = 0; i < g->nb_tiles; ++i)
    {
        if(g->tiles[i].penguin >= 0) {
            SDL_Point center = get_ElementCenter(graphics->tiles[i]);
            if((graphics->penguins[g->tiles[i].penguin]->x != center.x - graphics->penguins[g->tiles[i].penguin]->w/2 )&&
                    (graphics->penguins[g->tiles[i].penguin]->y != center.y - graphics->penguins[g->tiles[i].penguin]->h/2))
            {
                graphics->penguins[g->tiles[i].penguin]->x += (center.x - graphics->penguins[g->tiles[i].penguin]->w/2 - graphics->penguins[g->tiles[i].penguin]->x)/10;
                graphics->penguins[g->tiles[i].penguin]->y += (center.y - graphics->penguins[g->tiles[i].penguin]->h/2 - graphics->penguins[g->tiles[i].penguin]->y)/10;
            }
        }

        if(graphics->tiles[i] != NULL)
        {
            if(g->tiles[i].penguin == NO_TILE)
            {
                if(graphics->tiles[i]->color.a > 8)
                    graphics->tiles[i]->color.a -= 8;
                else {
                    graphics->tiles[i]->sprite = NULL;
                    free(graphics->tiles[i]);
                    graphics->tiles[i] = NULL;
                }
            }
        }
    }
}

void compute_geometry_tetra(struct graph* g, int tile_id, int* visited, float current_x, float current_y)
{
    if(tile_id != NO_TILE && !visited[tile_id] && graphics->tiles[tile_id] != NULL) {
        graphics->tiles[tile_id]->x = current_x;
        graphics->tiles[tile_id]->y = current_y;
        graphics->tiles[tile_id]->h = 1;
        graphics->tiles[tile_id]->sprite = graphics->tiles_sprites[g->tiles[tile_id].nb_fish - 1];
        visited[tile_id] = 1;

        compute_geometry_tetra(g, g->tiles[tile_id].neighbours[0], visited, current_x, current_y -1);
        compute_geometry_tetra(g, g->tiles[tile_id].neighbours[1], visited, current_x + 1, current_y);
        compute_geometry_tetra(g, g->tiles[tile_id].neighbours[2], visited, current_x, current_y + 1);
        compute_geometry_tetra(g, g->tiles[tile_id].neighbours[3], visited, current_x -1, current_y);
    }
}

void compute_geometry_hexa(struct graph* g, int tile_id, int* visited, float current_x, float current_y)
{
    if(tile_id != NO_TILE && !visited[tile_id] && graphics->tiles[tile_id] != NULL) {
        graphics->tiles[tile_id]->x = current_x;
        graphics->tiles[tile_id]->y = current_y;
        graphics->tiles[tile_id]->h = 1;
        graphics->tiles[tile_id]->sprite = graphics->tiles_sprites[g->tiles[tile_id].nb_fish - 1];
        visited[tile_id] = 1;

        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[0], visited, current_x , current_y - 1);
        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[1], visited, current_x + sqrt(3)/2, current_y - 0.5);
        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[2], visited, current_x + sqrt(3)/2, current_y + 0.5 );
        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[3], visited, current_x, current_y + 1);
        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[4], visited, current_x - sqrt(3)/2, current_y + 0.5);
        compute_geometry_hexa(g, g->tiles[tile_id].neighbours[5], visited, current_x - sqrt(3)/2, current_y - 0.5);
    }
}

void compute_geometry_octotetra(struct graph* g, int tile_id, int* visited, float current_x, float current_y)
{
    if(tile_id != NO_TILE && !visited[tile_id] && graphics->tiles[tile_id] != NULL) {
        graphics->tiles[tile_id]->x = current_x;
        graphics->tiles[tile_id]->y = current_y;
        visited[tile_id] = 1;

        switch(g->tiles[tile_id].nb_neighbours)
        {
        case 4:
            graphics->tiles[tile_id]->h = 1;
            graphics->tiles[tile_id]->sprite = graphics->tiles_sprites[g->tiles[tile_id].nb_fish - 1];
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[0], visited, current_x - (1/sqrt(2)), current_y - (1 + sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[1], visited, current_x + 1, current_y - (1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[2], visited, current_x - (1/sqrt(2)), current_y + 1);
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[3], visited, current_x - (1 + sqrt(2)), current_y - (1/sqrt(2)));
            break;
        case 8:
            graphics->tiles[tile_id]->h = 1 * (1 + sqrt(2));
            graphics->tiles[tile_id]->sprite = graphics->tiles_sprites[3 + g->tiles[tile_id].nb_fish - 1];
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[0], visited, current_x + (1/sqrt(2)), current_y - 1);
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[1], visited, current_x + ( 1 + 1/sqrt(2)), current_y - ( 1 + 1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[2], visited, current_x + ( 1 + sqrt(2)), current_y + ( 1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[3], visited, current_x + ( 1 + 1/sqrt(2)), current_y + ( 1 + 1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[4], visited, current_x + (1/sqrt(2)), current_y + ( 1 + sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[5], visited, current_x - (1 + 1/sqrt(2)), current_y + ( 1 + 1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[6], visited, current_x - 1, current_y + (1/sqrt(2)));
            compute_geometry_octotetra(g, g->tiles[tile_id].neighbours[7], visited, current_x - (1 + 1/sqrt(2)), current_y - (1 + 1/sqrt(2)));
            break;
        default:
            printf("Cas pas encore géré, tile %d, nb_neighbours %zu\n", tile_id, g->tiles[tile_id].nb_neighbours);
            break;
        }
    }
}

void updateScreen()
{
    SDL_RenderPresent(sdlRenderer);
}


void compute_geometry(struct graph* g, int tile_id, int* visited, float current_x, float current_y, char* layout)
{
    if(strcmp(layout, "full-tetra") == 0)
        compute_geometry_tetra(g, tile_id, visited, current_x, current_y);
    else if(strcmp(layout, "full-hexa") == 0)
        compute_geometry_hexa(g, tile_id, visited, current_x, current_y);
    else if(strcmp(layout, "octo-tetra") == 0)
        compute_geometry_octotetra(g, tile_id, visited, current_x, current_y);

}

void max_coord(float* max_x, float* max_y, float* min_x, float* min_y)
{
    *max_x = 0;
    *max_y = 0;
    *min_x = 10000;
    *min_y = 10000;

    for (int i = 0; i < graphics->nb_tiles; ++i)
    {
    	if(graphics->tiles[i] != NULL){
	        compute_size(graphics->tiles[i]->sprite, &(graphics->tiles[i]->w), &(graphics->tiles[i]->h));
	        if(graphics->tiles[i]->x + graphics->tiles[i]->w > *max_x)
	            *max_x = graphics->tiles[i]->x + graphics->tiles[i]->w ;
	        else if(graphics->tiles[i]->x  < *min_x)
	            *min_x = graphics->tiles[i]->x ;

	        if(graphics->tiles[i]->y + graphics->tiles[i]->h > *max_y)
	            *max_y = graphics->tiles[i]->y + graphics->tiles[i]->h;
	        else if(graphics->tiles[i]->y  < *min_y)
	            *min_y = graphics->tiles[i]->y ;
    	}
    }

}

void align_n_resize() {
    float max_x, max_y, min_x, min_y;
    max_coord(&max_x, &max_y, &min_x, &min_y);

    float ratio_x = (SCREEN_WIDTH - 30)/((max_x));
    float ratio_y = (SCREEN_HEIGHT - 30)/((max_y ));
    float ratio = (ratio_x > ratio_y)?ratio_y:ratio_x;

    int new_x = SCREEN_WIDTH/2  - ratio * (max_x - min_x)/2;
    int new_y = SCREEN_HEIGHT/2 - ratio * (max_y - min_y)/2;

    int offset_x = new_x - ratio * min_x;
    int offset_y = new_y - ratio * min_y;


    for (int i = 0; i < graphics->nb_tiles; ++i)
    {
    	if(graphics->tiles[i] != NULL){
	        graphics->tiles[i]->w *= ratio;
	        graphics->tiles[i]->h *= ratio;
	        graphics->tiles[i]->x *= ratio;
	        graphics->tiles[i]->x += offset_x;
	        graphics->tiles[i]->y *= ratio;
	        graphics->tiles[i]->y += offset_y;
    	}
    }

    for (int i = 0; i < graphics->nb_penguins; ++i)
    {
        graphics->penguins[i]->w *= ratio;
        graphics->penguins[i]->h *= ratio;
        graphics->penguins[i]->x *= ratio;
        graphics->penguins[i]->y *= ratio;
        if(graphics->penguins[i]->x > 0)
            graphics->penguins[i]->x += offset_x;
        if(graphics->penguins[i]->y > 0)
            graphics->penguins[i]->y += offset_y;
    }
}

#endif // for no_graphics
