# TexasHoldem

By Cindy Le, Yanni Yang and Adrien Xie

CSE 332S: Object-Oriented Software Development Laboratory

Washington University in St. Louis

Fall 2016

http://classes.cec.wustl.edu/~cse332/

## How to Run

To compile, use Visual Studio. To run the executable program, enter the command line as follows separated by space:

> Game Name, Player 1’s Name, Player 2’s Name, More Players’ Names (optional)

**Game name**: FiveCardDraw, SevenCardStud, or TexasHoldEm.

**Players names**: combinations of alphanumeric characters that represent players. 
If followed by a star, the player will discard cards and leave the game automatically. 
“NO” “No” “nO” “no, ” either followed or not followed by a star, are not allowed.

## How to Play

**FiveCardDraw** has two betting phases, one before discarding cards and another after. 
The program will first print out five cards that a player has and ask whether to discard or not. 
When entering the card indices to discard, all integers should be in the same line. 
The program will then print out player information twice, the first one in order of the time by 
which they join the game while the second one by their hands’ rank.

**SevenCardStud** has five turns of dealing cards and five betting phases after each one of them. 
Cards that are dealt facing down will only be visible until the end of round. 
Before that, a player can see his/her own cards but not others’.

**TexasHoldEm** has four turns of dealing cards and four betting phases after each one of them. 
Unlike the previous games, it has five cards shared by all players. 

After a round, we ask if any player wants to leave or join. 
The input should be a player’s name per line until a “no. ”
If a player runs out of chips, this is the time the program asks whether to reset it to 20. 
Then player data are stored as local files with four lines that contains name, wins, losses, and
chips respectively. 

##Error Code

**0** Successful run. 

**1** An unknown error has occurred. 

**2** No Sufficient command line arguments are given. 

**3** Too many command line arguments are given. 

**4** More than five cards are in the hand. 

**5** The deck has nothing to pop up. 

**6** Hand's index is out of range.

**7** The hand contains less than five cards.

**8** The function only applies to five cards.

**9** The hand contains two cards of same suit.

**10** Instance is not available! 

**11** Game is already started! 

**12** Unknown game! 

**13** No game is currently in process! 

**14** There is an issue with the pot. 

**15** The player is already playing! 

**16** No player is unfolded. 

**17** No player wins this round. 

**18** No enough players are currently in this round. 

**19** The decks have no cards to deal. 

##Sample Cases

Please see ReadMe.txt.
