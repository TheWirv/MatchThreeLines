# Assigment: Programming of a Match-3-Line-Drawer

## Rough Gameplay

There is a game field consisting of hex fields containing blocks. The player needs to connect adjacent blocks which belong to the same type in order to get points and remove them. Therefore, the player needs to draw a connected line which can contain each block only once. After the player selected at least 3 blocks and releases the mouse button, the selected blocks will be destroyed and all other fields slide column by column downwards to fill up the empty regions. The upper ones which then stay empty will be filled up with random blocks. Depending on the amount of destroy blocks, the player gets different amount of points. After a predefined amount of moves the game is over.

## Requirements

- Gamefield with hex fields of appr. 7x6 blocks (randomly generated)
- At least 4 different block colors or motives
- Adjacent blocks can be selected if they have the same color
- Blocks shall be removed if at least 3 were selected, blocks above will &quot;fall down&quot;, when some upmost field is empty a random block will be generated
- Clear highlight when drawing backwards
- Mouse controls

## Ideas for more features, expansions..

- Score (+ UI)
- Game ends after x moves
- Possibility to restart the game after game over
- Special blocks with special effects
- Combos
- …

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

![](RackMultipart20201121-4-1ozx9de_html_89ec818b3137dacf.jpg) ![](RackMultipart20201121-4-1ozx9de_html_9efd953a6c59dea7.png)
