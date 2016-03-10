#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

/* arguments : -nb de penguins par player
               -configuration de plateau (possibles :full-hexa
                                                     octo-tetra
                                                     full-tetra)
               -largeur plateau
               -longueur plateau
               -strategies*/


typedef enum gameState {PLACING, PLAYING, FINISHED} gameState;
gameState state;

// Play one placement or move
// returns 0 in case of error, 1 else
int nextStep(struct move* prev_moves)
{
    int result = -1;
    switch(state)
    {
    case PLACING:
        if ((result = place_penguins(prev_moves)) == 2)
            state = PLAYING;
        break;
    case PLAYING:
        if ((result = play_game(prev_moves)) == 2) {
            state = FINISHED;
        }
        break;
    case FINISHED:
        break;
    }

    if(result == 0)
        state = FINISHED;

    return result;
}

// play and display the whole game
// returns 0 in case of error
// 1 else
int play_n_display(char* layout)
{

    struct move prev_moves[2 * get_nb_players()];
    state = PLACING;

#ifdef NO_GRAPHICS

    while(state != FINISHED)
        nextStep(prev_moves);

    return 1;


#else
    int quit = 0;
    if(display_init())
    {
        graphics_init(srv->g, layout);


        while(!quit && state != FINISHED )
        {
            if(handleEvents(&quit))
                nextStep(prev_moves);

            synchronize(srv->g);
            draw();
            updateScreen();
	    //SDL_Delay(20);
        }

        graphics_free();
        display_free();
    }

    return !quit;

#endif
}




int main(int argc, char **argv)
{

    // Initializing server
    printf("INITIALIZING SERVER\n");
    if (!server_init(argc, argv))
        return EXIT_FAILURE;

    printf("INITIALIZING STRATEGIES\n");
    // Initializating strategies
    if (strategies_init() == 0) {
        free_srv();
        return EXIT_FAILURE;
    }

    printf("LET THE GAME BEGIN\n");
    // Playing and displaying the whole game
    if (play_n_display(argv[2])) {
	// End game and declare winner
        end_game();
    }


    return EXIT_SUCCESS;
}
