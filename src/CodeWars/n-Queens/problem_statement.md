# [n-Queens problem](https://www.codewars.com/kata/5985ea20695be6079e000003/cpp)
###### Site Difficulty: 1 kyu / Perceived difficulty: Very Hard

The eight queens puzzle is the problem of placing eight chess queens on an ```8 × 8``` chessboard so that no two queens threaten each other. Thus, a solution requires that no two queens share the same row, column or diagonal. The eight queens puzzle is an example of the more general ```n``` queens problem of placing ```n``` non-attacking queens on an ```n × n``` chessboard. You can read about the problem on its Wikipedia page: [Eight queens puzzle](https://en.wikipedia.org/wiki/Eight_queens_puzzle).

You will receive a (**possibly large**) number ```n``` and have to place ```n``` queens on a ```n × n``` chessboard, so that no two queens attack each other. This requires that no two queens share the same row, column or diagonal. You will also receive the mandatory position of one queen. This position is given 0-based with ```0 <= row < n``` and ```0 <= col < n``` ```{row, col}```. The coordinates ```{0, 0}``` are in the top left corner of the board. For many given parameters multiple solutions are possible. You have to find one of the possible solutions, all that fit the requirements will be accepted.

You have to return the solution board as a string, indicating empty fields with ```'.'``` (period) and Queens with ```'Q'``` (uppercase Q), ending each row with ```'\n'```.

If no solution is possible for the given parameters, return ```""``` (empty string).

input parameters are: 
- ```int n``` 
- ```std::pair<int mandatory_row, int mandatory_column>```

### Tests
- there are 8 tests for very small boards $(n \leq 10)$
- there are 8 tests for cases without solution
- there are 5 tests for small boards $(10 < n \leq 50)$
- there are 5 tests for medium boards $(100 < n \leq 500)$
- there are 5 tests for large boards $(500 < n \leq 1000)$

### Example

For input of ```size = 8```, mandatory ```column = 3``` and mandatory ```row = 0```, your solution could return:

```"...Q....\n......Q.\n..Q.....\n.......Q\n.Q......\n....Q...\nQ.......\n.....Q..\n"```

giving the following board:
```
. . . Q . . . .
. . . . . . Q .
. . Q . . . . .
. . . . . . . Q
. Q . . . . . .
. . . . Q . . .
Q . . . . . . .
. . . . . Q . .
```
(Other solutions to this example are possible and accepted. The mandatory queen has to be in its position, in the example in the first row at ```col=3```, ```row=0```.)

## Solution
Find my full solution in ```./solution.pdf```