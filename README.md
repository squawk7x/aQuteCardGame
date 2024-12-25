
---
<center><span style="font-size: 36px; font-weight: bold;">a Qute Card Game!</span></center>

--- 

This card game is played with 36 cards, consisting of 4 suits {"♦", "♠", "♥", "♣"} and 9 ranks {"6", "7", "8", "9", "10", "J", "Q", "K", "A"}.
The game can be played by 2 or 3 players. Player 1 competes against 1 or 2 AI players. (The default configuration is a 3-player game.)

Below is a description of the gameboard layout, detailed rules, game controls, and compile/run instructions.

---

## Gameboard Layout 

1. **Top Row**: Displays the 2 opponent AI players (Player 2 and Player 3). When playing against only 1 opponent, the panel for player 3 is hidden.
2. **Second Row**: Contains the **Monitor** (shows cards of the same rank played in sequence), various choosers (**Qute Chooser**, **Eights Chooser**, **Jpoints Chooser**, and **Round Chooser**), and panels for 'Cards drawn for 7 or 8' and for 'Cards drawn for 8'. 

   - *Cards drawn for 7s or 8s*: Cards are shown **'enabled'** if the active player drew cards due to one or more sevens played by the previous player. Cards are shown **'disabled'** if they were drawn for eights, indicating players' turns were skipped.
   - *Cards drawn for 8s*: Cards are shown **disabled** because they were drawn for eights, indicating players' turns were skipped. This panel is hidden when playing against only one opponent.

3. **Third Row**: Includes the **Shuffles counter** (indicates how often the blind was refilled and shuffled), the **Blind** (shows the top card of the draw pile), the **JsuitChooser** (to choose which suit follows a 'J'), and the **Stack** (recently played cards).
4. **Fourth Row**: Indicators for cards drawn (except cards drawn for 7s and 8s) (Drawn), playable cards that fit the actual card on the stack (Playable), cards played by the current player (Played).
   
   This row shows an indicator for the rounds played, and the player scores.
   Furthermore, there are radiobuttons for the number of players (Players) and how to sort the hand deck (Sort Cards by) as well as checkboxes for some sound effects and whether or not the card faces of the opponent's hand deck are visible (Settings).

5. **Bottom Row**: Contains your own hand of cards (Player 1). When it is your turn, just click on a playable card to put it on the stack.

---

<div style="page-break-before: always;"></div>

---

### Layout with card faces of opponent players visible (3 players):

![layout](res/layout/table_layout_3_players.png)

---

<div style="page-break-before: always;"></div>

---

### Layout with card faces of opponent players visible (2 players):

![layout](res/layout/table_layout_2_players.png)

---

<div style="page-break-before: always;"></div>

---

### Layout with card faces of opponent players visible (2 players) after counting:

- **Player 1** holds Q=10 + A=15 + J=20 => 45 points.
- **Player 2** finished the round by playing 2 J and set Jpoints Chooser to '-' => 2x (-20) = -40 points.

![layout](res/layout/table_layout_2_players_after_counting.png)


---

<div style="page-break-before: always;"></div>

---

## Game Rules

### Starting the Game
- **Initial Setup**: Each player starts with 5 cards.
- **First Move**: Player 1 plays a card onto the stack and can add more cards of the same rank.
  The next player must play a card of the same suit or rank and may add more cards of the same rank.

---

### Playing a Turn
- **Using Hand Cards**: Players must use their hand cards and play at least one card.
- **Drawing from the Blind**: If no suitable card is in hand, draw one card from the blind. This card can be played if it matches the card on the stack; otherwise, the turn passes to the next player.

---

### Special Conditions
- **Drawing Cards**: Only one card can be drawn from the blind, except when covering a '6'.
- **Covering a '6'**: Must be covered by a different rank. If no suitable card is in hand, draw cards from the blind until a non-'6' card can be played. The 'Qute' condition (four cards of the same rank played in sequence) overrides this rule, allowing the player to either end the round or cover the fourth '6' with another rank.

---

### Special Cards
- **6**: Must be covered by a different rank. Draw cards until this is possible.
- **7**: The next player must draw one card from the blind.
- **8**: The next player must draw 2 cards and is skipped. Multiple 8s can force the next player to draw 2 cards for each '8' played or distribute the draws among the following players.
  If multiple 8s are distributed among several players, these players will also be skipped. This is decided using the 'Eights Chooser'.
- **J**: Can be played on any suit, and the player chooses the suit to follow using the 'JsuitChooser'.
- **A**: The next player is skipped. Multiple Aces skip sequential players.

---

### 'Qute' Condition
When four cards of the same rank are played in sequence (by different players), the player who plays the fourth card can choose to end the round or continue. Four '6's {"6♦", "6♠", "6♥", "6♣"} and 'Qute' enabled override the 'a 6 must be covered' rule.

---

## Scoring
- The game consists of several rounds.
- The end of a round is reached when a player has no more cards or a 'Qute' is announced.
- At the end of the round, scoring occurs, and points will be added to the previous scores.
- The game ends when a player reaches a score higher than 125 points. The winner is the player with the lowest score.
- The player with the highest score starts the next round.

---

### Card Values:
- **6, 7, 8, 9**: 0 points
- **10, Q, K**: 10 points
- **J**: 20 points (-20 for the player or +20 for the opponent players when finishing a round and a 'J' is on top of the stack.)
- **A**: 15 points

---

### Special Scoring Rules:
- **125 Points**: A player's score resets to 0 if they reach exactly 125 points.
- **Finishing with 'J'**: The player can choose to either reduce their score by 20 points per 'J' or increase opponent scores by 20 points per 'J', decided via the 'Jpoints Chooser'.
- **Refilled Blind**: Each refill of the blind multiplies the end-of-round points (doubling, tripling, etc.), including the +/-20 points rule when finishing with 'J'.

---

## Game Control

### Mouse Controls
- **Left-click on card**: Play the card onto the stack if it matches.
- **Left-click on choosers**: Toggle decisions of the different choosers to desired decision on 'multiple 8s', 'Jsuit', 'Jpoints', and 'Qute'.
- **Left-click**: End the current player's turn and make the next player play.

---

<div style="page-break-before: always;"></div>

---

### Choosers
- **Multiple 8s**: Decide how the draw cards rule for 'multiple 8s' will apply. 
  Click on this chooser to toggle between 'all 8s for the next player' or 'share the 8s among the following players'.  
  *(The toggling is disabled when playing against 1 opponent only.)*
  
  ![multiple 8](res/layout/chooser_eights_n.png)  
  ![multiple 8](res/layout/chooser_eights_a.png)

- **J Suit**: Choose the suit to follow a 'J'. Click on this chooser to toggle to the next suit (order of toggling is {"♦", "♠", "♥", "♣"}).
  The initial 'Jsuit' is set to the most common suit in the player's hand cards.  
  ![jsuit](res/layout/chooser_jsuit_of_hearts.png)

- **J Points**: Choose between increasing the scores of opponent players or reducing the score of the active player.  
  Click on this chooser to toggle between '+20 points' and '-20 points' when scoring.  
  ![jpoints](res/layout/chooser_jpoints_p.png)  
  ![jpoints](res/layout/chooser_jpoints_m.png)

- **'Qute'**: Decide whether to finish or continue the round.  
  Click on this chooser to toggle between 'Qute' (finishing the round) and 'Continue this round'.  
  Special case: You can finish a round if 4 times 6 is indicated in the 'Monitor' (6♦ 6♠ 6♥ 6♣) and 'Qute' enabled.  
  ![qute](res/layout/chooser_qute_y.png)  
  ![qute](res/layout/chooser_qute_n.png)

---

<div style="page-break-before: always;"></div>

---

### More Controls and Settings
![controls](res/layout/indicator_and_control_switches.png)

- **Players**: You can play against 1 or 2 AI players.
- **Sort Cards by**: Arrange hand deck cards by suit or rank. (It resets to 'Unsorted' when a card is drawn from the blind.)
- **Sound**: Enables fancy sound effects.
- **Visible**: Enables the card faces of the opponent players to be shown. (The checkbox is unset again when a new round is started.)

---

### Special Keys for Testing
- **CTRL + 6, 7, 8, 9, T, J, Q, K, A**: Add corresponding cards (given rank for all suits) to the active player's hand.

---

### How to Install
1. **Try a precompiled version** (/build/.../aQuteCardGame). (Ubuntu 22.04LTS)
2. **OR:**
   - **Install git** (if not installed yet).
   - **Clone the repository**: `git clone https://github.com/squawk7x/aQuteCardGame.git`
   - **Install Qt Creator** (if not already installed) to compile the program for your specific operating system.
   - **In Qt Creator**: Open File or Project (Ctrl-O), change into the directory 'aQuteCardGame' and double-click on `CMakeLists.txt`.
   - **In Qt Creator**: Run the program by pressing (Ctrl-R) or by clicking on the green arrow.

---

<center><span style="font-size: 36px; font-weight: bold;">Enjoy The Game!</span></center>

---
