# SNAKE Arcade Game
2016 BYU CS 224 Lab 9


### Description 
"Write a snake game program in C that scores points by eating randomly placed food on the display. There are four levels of play, each with increasing difficulty. The game ends when the snake runs into an obstacle, itself, or time expires. The direction of the snake's head is turned to horizontal and vertical paths using the push buttons and the body of the snake follows in the head's path. The snake is always moving and the snake moves faster at each level. The tail grows by one segment every time the snake eats a food."

### Learn
_ Use techniques of real-time programming to implement time critical algorithms.
_ Implement an event driven programming model.
_ Decompose a problem into even service routines that respond to asynchronous events.
_ Program a finite state machine (FSM or automata) in C.
_ Use C struct's and function pointers to implement C objects.
_ Dynamically allocate and recover memory from the system heap using malloc and free.


### History of Snake
Snake is a video game that originated during the late 1970s in arcades and has maintained popularity since then, becoming something of a classic. After it became the standard pre-loaded game on Nokia phones in 1998, Snake found a massive audience.

The Snake variety of games dates back to the arcade game Blockade, developed and published by Gremlin in 1976. In 1978, Atari, Inc. released, as an unofficial port, an early home console version of the Blockade concept, titled Surround. Surround was one of the nine Atari 2600 (VCS) launch titles, and was also sold by Sears under the name Chase. That same year, a similar game was launched for the Bally Astrocade as Checkmate.

The first known personal computer version of Snake, titled Worm, was programmed in 1978 by Peter Trefonas of the US on the TRS-80 computer, and published by CLOAD magazine in the same year. This was followed shortly afterwards with versions from the same author for the Commodore PET and Apple II computers. A microcomputer port of Hustle was first written by Peter Trefonas in 1979 and published by CLOAD. This was later released by Milton Bradley for the TI-99/4A in 1980.

Some better-known versions include the Neopets example, which is known as Meerca Chase. Its revised version is known as Meerca Chase II. A variant called Nibbles was included with MS-DOS for a period of time as a QBasic sample program. An analog joystick-controlled variant of Snake, called Anaconda, was included as a hidden minigame in TimeSplitters 2, which featured free rotation instead of a fixed 4-direction system, and multiple types of food.

http://en.wikipedia.org/wiki/Snake_(video_game)

### Rules and Levels of Play
Rules
- The player controls a long, continuous line resembling a snake, which roams around a bordered plane, eating foods.
- The game ends when the snake's head hits its own body or the "walls" that surround the playing area.
- Each time the snake eats a piece of food, its tail grows longer.
- The user controls the direction of the snake's head (up, down, left, or right) using switches 4 thru 1, respectively.
- The snake's body follows the path of the head.
- The player cannot stop the snake from moving while the game is in progress.
- The player cannot make the snake go in reverse.
- New foods are not placed on top of existing foods or the snake body.
- When moving to the next level, continue the game score and snake length and direction.

Levels
##### Level 1
- "Cyclical" play area (enter a wall and appear on opposite side).
- 10 foods each worth 1 point.
- Always 10 foods - reappear in random position as eaten.
- Slow speed (250 ms/move).
- 30 second time limit to eat 10 foods.
##### Level 2
- 4 non-electric perimeter walls - snake makes a 90 degree direction change.
- 10 foods appear at beginning of level but disappear as eaten.
- Each food eaten is worth 2 points.
- Obstacles (rocks) are randomly placed on display - snake dies if head runs into a rock.
- Medium speed (125 ms/move).
- 30 second time limit to eat all 10 foods.
##### Level 3
- 4 electric perimeter walls - snake dies if the head runs into a wall.
- Only 1 food at a time on the game board - a new food randomly appears after a food is eaten.
- Each food eaten is worth 3 points.
- Obstacles (rocks) are randomly placed on display - snake dies if head runs into a rock.
- Medium fast speed (62 ms/move).
- 45 second time limit to eat 10 foods.
##### Level 4
- 4 electric perimeter walls - snake dies if the head runs into a wall.
- Only 1 food at a time on the game board - a new food randomly appears after a food is eaten.
- Each food eaten is worth 4 points.
- Obstacles (rocks) are randomly placed on display - snake dies if head runs into a rock.
- Fast speed (31 ms/move).
- 1 minute time limit to eat 10 foods.
##### King Snake!
- If a player completes the 4th level, they are crowned the grand champion having achieved the maximum score of 100 points!


### Specs 
- 1 point	Your snake program is written in C using an event driven model of asynchronous events and event service routines. Your files contain header comments that include your name and a declaration that the completed assignment is your own work. The MSP430 processor clock runs at 12 Mhz and sleeps while waiting to process an event. The green LED blinks on and off at 1 second intervals. A snake.h C header file is included in all snake snake files and only contains comments, enum's, #define's, and function prototypes (no code!)
- 1 point	Your snake moves up/down, left/right according to switches 1-4.
- 1 point	The current score, level, and remaining time are displayed in real-time (when any value changes) during the game. The final score is displayed at the end of the game. The game then waits for any switch to be pressed before restarting the game.
- 1 point	Your snake grows and the score is updated as food is eaten.
- 1 point	A short beep tone is played and the LCD backlight blinks each time a food is consumed. A congratulatory ditty is played when moving from one level to the next and a raspberry sound is played when the game ends.
- 1 point	Malloc, free, union, and function pointers are correctly used in your game implementation. (No memory leaks!)
- 2 points	Your program follows all the rules and requirements listed above for level 1.
- 2 points	Your program follows all the rules and requirements listed above for level 2.
