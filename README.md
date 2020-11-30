# Assigment: Programming of a Match-3-Line-Drawer

## Rough Gameplay

There is a game field consisting of hex fields containing blocks. The player needs to connect adjacent blocks which belong to the same type in order to get points and remove them. Therefore, the player needs to draw a connected line which can contain each block only once. After the player selected at least 3 blocks and releases the mouse button, the selected blocks will be destroyed and all other fields slide column by column downwards to fill up the empty regions. The upper ones which then stay empty will be filled up with random blocks. Depending on the amount of destroy blocks, the player gets different amount of points. After a predefined amount of moves the game is over.

## Requirements

[x] Gamefield with hex fields of appr. 7x6 blocks (randomly generated)
[x] At least 4 different block colors or motives
[x] Adjacent blocks can be selected if they have the same color
[x] Blocks shall be removed if at least 3 were selected, blocks above will &quot;fall down&quot;, when some upmost field is empty a random block will be generated
[x[ Clear highlight when drawing backwards
[x] Mouse controls

## Ideas for more features, expansions..

[ ] Score (+ UI)
[ ] Game ends after x moves
[ ] Possibility to restart the game after game over
[ ] Special blocks with special effects
[ ] Combos
[ ] …

## Conditions

- Engine: Unreal Engine 4
- Language: C++, no blueprint scripting
- Only own code, no plugins/ third party code

## Criteria

- Clean structure, expandable
- Commented and/or understandable code
- Needs to work
- Performant
- Graphical quality is irrelevant

## Examples

| ![Caveman Story](https://user-images.githubusercontent.com/4397989/99886820-ddc26b00-2c3f-11eb-8961-6adce0d6cd68.jpg) | ![Poko Pang](https://user-images.githubusercontent.com/4397989/99886821-df8c2e80-2c3f-11eb-913c-714258de75de.png) |
| :-------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------: |
|                                                    _Caveman Story_                                                    |                                                    _Poko Pang_                                                    |
