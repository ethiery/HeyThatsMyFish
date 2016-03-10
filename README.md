# hey-thats-my-fish

Hey that's my fish is a board game. You can learn more about it [here](https://boardgamegeek.com/boardgame/8203/hey-s-my-fish).

As part of an academic project, 3 friends and I developed a video game version of it. It is not possible to play it directly, as the goal was to create and confront AIs! This repository contains the sources of this video game, 3 AIs and an abstract (in french).

## Install & compile

- First, clone the repository wherever you want
- Then install a few dependencies : libsdl2-dev, libsdl2-image-dev, libsdl2-ttf-dev
- Finally, compile the full project : from directory src, "make all"

## Launch a game

The exectuable is called banquise. Each AI is compiled as a dynamic library lib_strat_*.so.

In order to launch banquise, you have to type:
```bash
./banquise [nb_penguin_by_player] [map_shape] [map_height] [map_width] ./strategies(.so) ... 
```

`nb_penguin_by player`    sets players' initial number of penguins; must be a positive integer

`map_shape`               sets map's type; can be of three types:
- `full-tetra`      map fully made of squares
- `full-hexa`       map fully made of hexagones
- `octo-tetra`      map made of octogones and squares

`map_height`              sets map's height; must be a positive integer

`map_width`               sets map's width; must be a positive integer

`strategies(.so)`         each strategy defines a player; must be at least two strategies

If you want to confront an AI to itself, you need to duplicate the library first. This can be done by executing :
```bash
make double
```
