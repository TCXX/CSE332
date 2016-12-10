CSE 332 Lab  4: Multiple Card Games
-------------------------------------
Team members: Cindy Le, Yanni Yang and Adrien Xie
=====================================
INSTRUCTION TO PLAY

To run the test, enter the command line as follows separated by space:

[Game Name] [Player 1’s Name] [Player 2’s Name] [More Players’ Names (optional)]

Game name : FiveCardDraw or SevenCardStud
Players names: combinations of alphanumeric characters that represent players. 
If followed by a star (*), the player will discard cards and leave the game automatically. 
“NO” “No” “nO” “no, ” either followed or not followed by “*”, are not allowed.

FiveCardDraw has two betting phases, one before discarding cards and another after. 
The program will first print out five cards that a player has and ask whether to discard or not. 
When entering the card indices to discard, all integers should be in the same line. 
The program will then print out player information twice, the first one in order of the time by 
which they join the game while the second one by their hands’ rank.

SevenCardStud has five turns of dealing cards and five betting phases after each one of them. 
Cards that are dealt facing down will only be visible until the end of round. 
Before that, a player can see his/her own cards but not others’.

After a round, we ask if any player wants to leave or join. 
The input should be a player’s name per line until a “no. ”
If a player runs out of chips, this is the time the program asks whether to reset it to 20. 
Then player data are stored as local files with four lines that contains name, wins, losses, and
chips respectively. 

Note: In this lab, we ignore autoplayers that we did for extra credit of last time.

=====================================

In this lab we implemented a new game, SevenCardStud, which shares many features with 
FiveCardDraw. We created an intermediate class PokerGame deriving from Game class and
moved many functions from FiveCardDraw to PokerGame to keep consistency of the subclasses. 

We realized that before_round() and round() were different from game to game, so we made
them virtual in PokerGame and depend on implementation in subclasses. In after_round(), the
program sorts players by findMaxHash() in Hand.cpp, a function picking the five cards that gives
a highest rank. Because it applies to any players with five or more cards, the two games can
share the same function in PokerGame. We also renamed before_turn(), turn(), and after_turn() in
PokerGame based on their functional effects.

Besides structural adjustment, we added betting phases to the games by using bet_in_turn(). It
reads from command line input the number of chips and records it in member variable “bet” of
each player. At the end of the round, these chips are either returned to players who win the game,
or added to the pot that will then be shared by winners of the round.

As for hiding card contents, we defined an enum of visibility for each cards. When asking the
program to print cards, we will specify access mode of enum that tells toString() what to hide from
outstream.

Header/Source Files Newly Created:
PokerGame.h            PokerGame.cpp
SevenCardStud.h      SevenCardStud.cpp

Header/Source Files with Significant Modifications: 
FiveCardDraw.h        FiveCardDraw.cpp
Game.h                     Game.cpp
Hand.h                      Hand.cpp

For more information, please see comments in the .h and .cpp files.

-------------------------------------

While building the file, we got the following errors:

C4018: '>=' : signed/unsigned mismatch
Caused by comparing an int with a size_t
Fixed by changing both sides of the equation to variables of size_t

Once the program was built successfully, we got the following errors:

After entered a new game name, the program continues to execute the old one.
Caused by not properly resetting the game pointer to the new game.
Fixed by adding reset() for the pointer.

The member variable “pot” in the class PokerGame got crazily big ( e. g. 42972579 )
Caused by the subtracting an unsigned int variable out of boundary. 
Fixed by correcting the function betChip().

User are suggested all-in unnecessarily.
Caused by incorrect checking on the player bet and the number of chips needed for call.
Fixed by correcting the check statement

Then the program runs and outputs as expected without errors or warnings. 
Some trial results are appended at the end of this file. 

=====================================
Error Code

0 Successful run. 
1 An unknown error has occurred. 
2 No Sufficient command line arguments are given. 
3 Too many command line arguments are given. 
4 More than five cards are in the hand. 
5 The deck has nothing to pop up. 
6 Hand's index is out of range.
7 The hand contains less than five cards.
8 The function only applies to five cards.
9 The hand contains two cards of same suit.
10 Instance is not available! 
11 Game is already started!
12 Unknown game! 
13 No game is currently in process! 
14 The player is already playing! 
15 No player is unfolded.
16 No player wins this round.
17 No enough players are currently in this round.
18 The decks have no cards to deal.

=======================================
Extra Credit

We defined a new class TexasHoldEm for a new game that has four turns of dealing cards and
four betting phases after each one of them. Unlike the previous games, it has five cards shared
by all players. 

To make this work, we defined an extra function in PokerGame that pops up a card the proper
deck and then adds it to every player’s hand. Then we changed the Game class so that it could
recognize “TexasHoldEm” as a game name.

============================================================================

Test Case Part I: FiveCardDraw and SevenCardStud

============================================================================

Input 1: 
Command Line Arguments: lab3.exe 

Output 1:
Program's name: C:\Users\xie.z_000\documents\visual studio 2015\Projects\lab4.exe\Debug\lab4.exe
Error 2: No Sufficient command line arguments are given.

--------------------------------------

Input 2: 
Command Line Arguments: lab4.exe SevenCardStud

Output 2:
Program's name: C:\Users\xie.z_000\documents\visual studio 2015\Projects\lab4.exe\Debug\lab4.exe
Error 2: No Sufficient command line arguments are giving.

--------------------------------------

Input 3: 
Command Line Arguments: lab4.exe SevenCardStud

Output 3:
Program's name: C:\Users\xie.z_000\documents\visual studio 2015\Projects\lab4.exe\Debug\lab4.exe
Error 2: No Sufficient command line arguments are giving.


--------------------------------------

Input 4: 
Command Line Arguments: lab4.exe SevenCardStud

Output 4:
Program's name: C:\Users\xie.z_000\documents\visual studio 2015\Projects\lab4.exe\Debug\lab4.exe
Error 2: No Sufficient command line arguments are giving.

--------------------------------------

Input 5: 
Command Line Arguments: lab4.exe SevenCardStud a b c

Output 5:
Player b(17): 5D  8D  9H  jC
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
5

Player c(16): 3D  6C  6S  jD
Please enter 5 for CALL, '6' or '7' for RAISE, or 'f' for FOLD:
7

Player a(15): 5C  8S  jH  qD
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
9

Player b(14): 5D  8D  9H  jC
Please enter 9 for CALL, '10' or '11' for RAISE, or 'f' for FOLD:
11

Player c(12): 3D  6C  6S  jD
Please enter 11 for CALL, '12' or '13' for RAISE, or 'f' for FOLD:
13

Player a(10): 5C  8S  jH  qD
Please enter 13 for CALL, '14' or '15' for RAISE, or 'f' for FOLD:
15

Player b(8): 5D  8D  9H  jC
Please enter 15 for CALL, '16' or '17' for RAISE, or 'f' for FOLD:
16

Player c(6): 3D  6C  6S  jD
Please enter 16 for CALL, '17' or '18' for RAISE, or 'f' for FOLD:
17

Player a(4): 5C  8S  jH  qD
Please enter 17 for CALL, '18' or '19' for RAISE, or 'f' for FOLD:
19

Player b(3): 5D  8D  9H  jC
Please enter '19' for ALL-IN, or 'f' for FOLD:
19

Player c(2): 3D  6C  6S  jD
Please enter '19' for ALL-IN, or 'f' for FOLD:
19


Player a(0): 5C  8S  **  **
Player b(0): 5D  9H  **  **
Player c(0): 3D  6C  **  **

Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Run out of chips!

Player b(0): 2H  5D  8D  9H  jC     (No Rank)
Run out of chips!

Player c(0): 3D  6C  6S  jD  qS     (One Pair)
Run out of chips!


Player a(0): 5C  8S  kS  **  **
Player b(0): 2H  5D  9H  **  **
Player c(0): 3D  6C  qS  **  **

Player a(0): 4C  5C  8S  jH  qD  kS     (No Rank)
Run out of chips!

Player b(0): 2H  3S  5D  8D  9H  jC     (No Rank)
Run out of chips!

Player c(0): 3D  5S  6C  6S  jD  qS     (One Pair)
Run out of chips!


Player a(0): 4C  5C  8S  kS  **  **
Player b(0): 2H  3S  5D  9H  **  **
Player c(0): 3D  5S  6C  qS  **  **

Player a(0): 4C  5C  8S  10C  jH  qD  kS     (No Rank)
Run out of chips!

Player b(0): 2H  3S  5D  8D  9H  jC  aS     (No Rank)
Run out of chips!

Player c(0): 2D  3D  5S  6C  6S  jD  qS     (One Pair)
Run out of chips!


Player a(0): 4C  5C  8S  kS  **  **  **
Player b(0): 2H  3S  5D  9H  **  **  **
Player c(0): 3D  5S  6C  qS  **  **  **


Player a(0): 4C  5C  8S  10C  jH  qD  kS     (No Rank)
Player b(0): 2H  3S  5D  8D  9H  jC  aS     (No Rank)
Player c(0): 2D  3D  5S  6C  6S  jD  qS     (One Pair)

Player c(0): 2D  3D  5S  6C  6S  jD  qS     (One Pair)
Player b(0): 2H  3S  5D  8D  9H  jC  aS     (No Rank)
Player a(0): 4C  5C  8S  10C  jH  qD  kS     (No Rank)

1 winner(s) sharing 60 chips:
c
Player b has no chips. Reset? (yes/no)
yes
Player a has no chips. Reset? (yes/no)
yes

Any player leaving? Enter 'No' for nobody.
Player's name:
adrien
The player adrien is not currently playing!

Any player leaving? Enter 'No' for nobody.
Player's name:
yanni
The player yanni is not currently playing!

Any player leaving? Enter 'No' for nobody.
Player's name:
cindy
The player cindy is not currently playing!

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
adrien

Any player joining? Enter 'No' for nobody.
Player's name:
yanni

Any player joining? Enter 'No' for nobody.
Player's name:
cindy

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game continues with 6 players.
Please enter a game name.
SevenCardStud


Player a(19): 4C  5C  jH
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player b(19): 2H  8D  9H
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
1

Player c(59): 3D  6S  qS
Please enter 1 for CALL, '2' or '3' for RAISE, or 'f' for FOLD:
3

Player adrien(19): 8S  qD  kS
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
3

Player yanni(19): 3S  5D  jC
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
3

Player cindy(19): 5S  6C  jD
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
3

Player a(19): 4C  5C  jH
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
3

Player b(18): 2H  8D  9H
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
3


Player a(16): 4C  **  **
Player b(16): 2H  **  **
Player c(56): qS  **  **
Player adrien(16): kS  **  **
Player yanni(16): 3S  **  **
Player cindy(16): 5S  **  **

Player a(16): 4C  5C  7H  jH
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
f

Player b(16): 2H  8D  9H  aS
Please enter 3 for CALL, '4' or '5' for RAISE, or 'f' for FOLD:
4

Player c(56): 2D  3D  6S  qS
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
5

Player adrien(16): 8S  10C  qD  kS
Please enter 5 for CALL, '6' or '7' for RAISE, or 'f' for FOLD:
5

Player yanni(16): 3S  5D  8C  jC
Please enter 5 for CALL, '6' or '7' for RAISE, or 'f' for FOLD:
7

Player cindy(16): 3H  5S  6C  jD
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
7

Player b(15): 2H  8D  9H  aS
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
7

Player c(54): 2D  3D  6S  qS
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
f

Player adrien(14): 8S  10C  qD  kS
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
f


Player a(13): **  **  **  **
Player b(12): 2H  aS  **  **
Player c(52): **  **  **  **
Player adrien(12): **  **  **  **
Player yanni(12): 3S  8C  **  **
Player cindy(12): 3H  5S  **  **

Player b(12): 2H  7C  8D  9H  aS     (No Rank)
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
f

Player yanni(12): 3S  5D  7S  8C  jC     (No Rank)
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
f


Player a(13): **  **  **  **  **
Player b(5): **  **  **  **  **
Player c(52): **  **  **  **  **
Player adrien(12): **  **  **  **  **
Player yanni(5): **  **  **  **  **
Player cindy(12): 3H  5S  10H  **  **


Player a(13): **  **  **  **  **
Player b(5): **  **  **  **  **
Player c(52): **  **  **  **  **
Player adrien(12): **  **  **  **  **
Player yanni(5): **  **  **  **  **
Player cindy(12): 3H  5S  6C  10H  jD     (No Rank)

Player cindy(12): 3H  5S  6C  10H  jD     (No Rank)
Player a(13): **  **  **  **  **
Player b(5): **  **  **  **  **
Player c(52): **  **  **  **  **
Player adrien(12): **  **  **  **  **
Player yanni(5): **  **  **  **  **

1 winner(s) sharing 61 chips:
cindy

Any player leaving? Enter 'No' for nobody.
Player's name:
adrien

Any player leaving? Enter 'No' for nobody.
Player's name:
yanni

Any player leaving? Enter 'No' for nobody.
Player's name:
cindy

Any player leaving? Enter 'No' for nobody.
Player's name:
a

Any player leaving? Enter 'No' for nobody.
Player's name:
b

Any player leaving? Enter 'No' for nobody.
Player's name:
c

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game ends with 0 players.

--------------------------------------

Input 6: 
Command Line Arguments: lab4.exe FiveCardDraw a b c

Output 6:

C:\Users\xie.z_000\Documents\Visual Studio 2015\Projects\lab4\Debug>lab4.exe Fiv
eCardDraw a b c

Player a(12): 5C  8S  jH  qD  kS     (No Rank)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player b(4): 2H  5D  8D  9H  jC     (No Rank)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
1

Player c(51): 3D  6C  6S  jD  qS     (One Pair)
Please enter 1 for CALL, '2' or '3' for RAISE, or 'f' for FOLD:
2

Player a(12): 5C  8S  jH  qD  kS     (No Rank)
Please enter 2 for CALL, '3' or '4' for RAISE, or 'f' for FOLD:
3

Player b(3): 2H  5D  8D  9H  jC     (No Rank)
Please enter 3 for CALL, '4 for RAISE, or 'f' for FOLD:
4

Player c(49): 3D  6C  6S  jD  qS     (One Pair)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
4

Player a(9): 5C  8S  jH  qD  kS     (No Rank)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
4


Player a(8): 5C  8S  jH  qD  kS     (No Rank)
Card to discard? Enter the indices separated by space in a line.
1

Player b(0): 2H  5D  8D  9H  jC     (No Rank)
Card to discard? Enter the indices separated by space in a line.
2

Player c(47): 3D  6C  6S  jD  qS     (One Pair)
Card to discard? Enter the indices separated by space in a line.
5555

Player a(8): 3S  8S  jH  qD  kS     (No Rank)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
4

Player b(0): 2H  5S  8D  9H  jC     (No Rank)
Run out of chips!

Player c(47): 3D  6C  6S  jD  qS     (One Pair)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
4


Player a(4): 3S  8S  jH  qD  kS     (No Rank)
Player b(0): 2H  5S  8D  9H  jC     (No Rank)
Player c(43): 3D  6C  6S  jD  qS     (One Pair)

Player c(43): 3D  6C  6S  jD  qS     (One Pair)
Player a(4): 3S  8S  jH  qD  kS     (No Rank)
Player b(0): 2H  5S  8D  9H  jC     (No Rank)

1 winner(s) sharing 23 chips:
c
Player b has no chips. Reset? (yes/no)
yes

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game continues with 3 players.
Please enter a game name.
FiveCardDraw


Player a(3): 5C  8S  jH  qD  kS     (No Rank)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
2

Player b(19): 2H  5D  8D  9H  jC     (No Rank)
Please enter 2 for CALL, '3' or '4' for RAISE, or 'f' for FOLD:
4

Player c(65): 3D  6C  6S  jD  qS     (One Pair)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
6

Player a(1): 5C  8S  jH  qD  kS     (No Rank)
Please enter '3' for ALL-IN, or 'f' for FOLD:
3

Player b(15): 2H  5D  8D  9H  jC     (No Rank)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
f


Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Card to discard? Enter the indices separated by space in a line.
0

Player c(59): 3D  6C  6S  jD  qS     (One Pair)
Card to discard? Enter the indices separated by space in a line.
000000 3

Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Run out of chips!

Player c(59): 3D  3S  6C  jD  qS     (One Pair)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
8

Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Run out of chips!


Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Player b(13): **  **  **  **  **
Player c(57): 3D  3S  6C  jD  qS     (One Pair)

Player c(57): 3D  3S  6C  jD  qS     (One Pair)
Player a(0): 5C  8S  jH  qD  kS     (No Rank)
Player b(13): **  **  **  **  **

1 winner(s) sharing 20 chips:
c
Player a has no chips. Reset? (yes/no)
yes

Any player leaving? Enter 'No' for nobody.
Player's name:
a

Any player leaving? Enter 'No' for nobody.
Player's name:
a
The player a is not currently playing!

Any player leaving? Enter 'No' for nobody.
Player's name:
b

Any player leaving? Enter 'No' for nobody.
Player's name:
b
The player b is not currently playing!

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
b

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game continues with 2 players.
Please enter a game name.
FiveCardDraw


Player c(76): 3D  5D  6S  jC  jH     (One Pair)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
2

Player b(12): 8D  8S  9H  jD  qD     (One Pair)
Please enter 2 for CALL, '3' or '4' for RAISE, or 'f' for FOLD:
4

Player c(74): 3D  5D  6S  jC  jH     (One Pair)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
6

Player b(8): 8D  8S  9H  jD  qD     (One Pair)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
6


Player c(70): 3D  5D  6S  jC  jH     (One Pair)
Card to discard? Enter the indices separated by space in a line.
1

Player b(6): 8D  8S  9H  jD  qD     (One Pair)
Card to discard? Enter the indices separated by space in a line.
5

Player c(70): 5D  6C  6S  jC  jH     (Two Pairs)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
6

Player b(6): 5C  8D  8S  9H  jD     (One Pair)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
0
0
0
6


Player c(64): 5D  6C  6S  jC  jH     (Two Pairs)
Player b(0): 5C  8D  8S  9H  jD     (One Pair)

Player c(64): 5D  6C  6S  jC  jH     (Two Pairs)
Player b(0): 5C  8D  8S  9H  jD     (One Pair)

1 winner(s) sharing 26 chips:
c
Player b has no chips. Reset? (yes/no)
no

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game ends with 1 players.

============================================================================

Test Case Part II: TexasHoldEm (Extra Credit)

============================================================================

Input 1:
Command Line Arguments: lab4.exe texasHoldEm

Ouput 1: 

C:\Users\xie.z_000\Documents\Visual Studio 2015\Projects\lab4\Debug>lab4.exe Tex
asHoldEm
Program's name: lab4.exe
Error 2: No Sufficient command line arguments are given.

Input 2: 
Command Line Arguments: lab4.exe TexasHoldEm a b c

Output 2:
Player b(2): 3D  8D  8S  9H  jC     (One Pair)
Please enter '12' for ALL-IN, or 'f' for FOLD:
12

Player c(77): 3D  6S  8S  9H  jD     (No Rank)
Please enter 14 for CALL, '15' or '16' for RAISE, or 'f' for FOLD:
14


Player a(5): 3D  8S  9H  **  **
Player b(0): 3D  8S  9H  **  **
Player c(75): 3D  8S  9H  **  **

Player a(5): 3D  5D  8S  9H  jH  qD     (No Rank)
Please enter 14 for CALL, '15' or '16' for RAISE, or 'f' for FOLD:
14

Player b(0): 3D  5D  8D  8S  9H  jC     (One Pair)
Run out of chips!

Player c(75): 3D  5D  6S  8S  9H  jD     (No Rank)
Please enter 14 for CALL, '15' or '16' for RAISE, or 'f' for FOLD:
2
16

Player a(0): 3D  5D  8S  9H  jH  qD     (No Rank)
Run out of chips!

Player b(0): 3D  5D  8D  8S  9H  jC     (One Pair)
Run out of chips!


Player a(0): 3D  5D  8S  9H  **  **
Player b(0): 3D  5D  8S  9H  **  **
Player c(73): 3D  5D  8S  9H  **  **


Player a(0): 3D  5D  8S  9H  jH  qD     (No Rank)
Player b(0): 3D  5D  8D  8S  9H  jC     (One Pair)
Player c(73): 3D  5D  6S  8S  9H  jD     (No Rank)

Player b(0): 3D  5D  8D  8S  9H  jC     (One Pair)
Player a(0): 3D  5D  8S  9H  jH  qD     (No Rank)
Player c(73): 3D  5D  6S  8S  9H  jD     (No Rank)

1 winner(s) sharing 59 chips:
b
Player a has no chips. Reset? (yes/no)
yes

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
adrien

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game continues with 4 players.
Please enter a game name.
TexasHoldEm


Player a(19): 3D  jC
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player b(58): 8D  jD
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player c(72): 6S  jH
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player adrien(11): 9H  qD
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0


Player a(19): **  **
Player b(58): **  **
Player c(72): **  **
Player adrien(11): **  **

Player a(19): 3D  5D  6C  8S  jC     (No Rank)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player b(58): 5D  6C  8D  8S  jD     (One Pair)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player c(72): 5D  6C  6S  8S  jH     (One Pair)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player adrien(11): 5D  6C  8S  9H  qD     (No Rank)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0


Player a(19): 5D  6C  8S  **  **
Player b(58): 5D  6C  8S  **  **
Player c(72): 5D  6C  8S  **  **
Player adrien(11): 5D  6C  8S  **  **

Player a(19): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player b(58): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player c(72): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
0

Player adrien(11): 5C  5D  6C  8S  9H  qD     (One Pair)
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
2

Player a(19): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter 2 for CALL, '3' or '4' for RAISE, or 'f' for FOLD:
4

Player b(58): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 4 for CALL, '5' or '6' for RAISE, or 'f' for FOLD:
5

Player c(72): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter 5 for CALL, '6' or '7' for RAISE, or 'f' for FOLD:
6

Player adrien(9): 5C  5D  6C  8S  9H  qD     (One Pair)
Please enter 6 for CALL, '7' or '8' for RAISE, or 'f' for FOLD:
7

Player a(15): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter 7 for CALL, '8' or '9' for RAISE, or 'f' for FOLD:
8

Player b(53): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 8 for CALL, '9' or '10' for RAISE, or 'f' for FOLD:
9

Player c(66): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter 9 for CALL, '10' or '11' for RAISE, or 'f' for FOLD:
10

Player adrien(4): 5C  5D  6C  8S  9H  qD     (One Pair)
Please enter 10 for CALL, '11 for RAISE, or 'f' for FOLD:
11

Player a(11): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter 11 for CALL, '12' or '13' for RAISE, or 'f' for FOLD:
122
12

Player b(49): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 12 for CALL, '13' or '14' for RAISE, or 'f' for FOLD:
13

Player c(62): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter 13 for CALL, '14' or '15' for RAISE, or 'f' for FOLD:
14

Player adrien(0): 5C  5D  6C  8S  9H  qD     (One Pair)
Run out of chips!

Player a(7): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter 14 for CALL, '15' or '16' for RAISE, or 'f' for FOLD:
15

Player b(45): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 15 for CALL, '16' or '17' for RAISE, or 'f' for FOLD:
16

Player c(58): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter 16 for CALL, '17' or '18' for RAISE, or 'f' for FOLD:
17

Player adrien(0): 5C  5D  6C  8S  9H  qD     (One Pair)
Run out of chips!

Player a(4): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter 17 for CALL, '18' or '19' for RAISE, or 'f' for FOLD:
18

Player b(42): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 18 for CALL, '19' or '20' for RAISE, or 'f' for FOLD:
19

Player c(55): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Please enter 19 for CALL, '20' or '21' for RAISE, or 'f' for FOLD:
20

Player adrien(0): 5C  5D  6C  8S  9H  qD     (One Pair)
Run out of chips!

Player a(1): 3D  5C  5D  6C  8S  jC     (One Pair)
Please enter '19' for ALL-IN, or 'f' for FOLD:
21
219
19

Player b(39): 5C  5D  6C  8D  8S  jD     (Two Pairs)
Please enter 20 for CALL, '21' or '22' for RAISE, or 'f' for FOLD:
f


Player a(0): 5C  5D  6C  8S  **  **
Player b(38): **  **  **  **  **  **
Player c(52): 5C  5D  6C  8S  **  **
Player adrien(0): 5C  5D  6C  8S  **  **


Player a(0): 3D  5C  5D  6C  8S  jC     (One Pair)
Player b(38): **  **  **  **  **  **
Player c(52): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Player adrien(0): 5C  5D  6C  8S  9H  qD     (One Pair)

Player c(52): 5C  5D  6C  6S  8S  jH     (Two Pairs)
Player adrien(0): 5C  5D  6C  8S  9H  qD     (One Pair)
Player a(0): 3D  5C  5D  6C  8S  jC     (One Pair)
Player b(38): **  **  **  **  **  **

1 winner(s) sharing 74 chips:
c
Player adrien has no chips. Reset? (yes/no)
yes
Player a has no chips. Reset? (yes/no)
yes

Any player leaving? Enter 'No' for nobody.
Player's name:
adrien

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game continues with 3 players.
Please enter a game name.
TexasHoldEm


Player a(19): jH  qD
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
f

Player b(37): 8D  jC
Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD:
f


Player a(19): **  **
Player b(37): **  **
Player c(125): **  **


Player a(19): **  **
Player b(37): **  **
Player c(125): 6S  jD

Player c(125): 6S  jD
Player a(19): **  **
Player b(37): **  **

1 winner(s) sharing 3 chips:
c

Any player leaving? Enter 'No' for nobody.
Player's name:
a

Any player leaving? Enter 'No' for nobody.
Player's name:
c

Any player leaving? Enter 'No' for nobody.
Player's name:
no

Any player joining? Enter 'No' for nobody.
Player's name:
no

Game ends with 1 players.

