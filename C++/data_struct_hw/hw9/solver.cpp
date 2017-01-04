 private void solver(
          char[][] board,
          boolean[][] tracker,
          Trie lexicon,
          String word,
          int x, int y,
          Set<String> result)
  {
 
    if (lexicon.isWord(word)) { result.add(word); }
 
    if (!lexicon.isPrefix(word)) { return; }
 
    boolean[][] tmp = deepCopy(tracker);
    tmp[x][y] = true;
 
    //upper left
    if (0 <= x - 1 && 0 <= y - 1 && !tmp[x - 1][y - 1]){
      solver(board,tmp,lexicon, word+board[x-1][y-1], x-1, y-1, result);
    }
 
    //up
    if (0 <= y - 1 && !tmp[x][y - 1]){
      solver(board, tmp, lexicon, word + board[x][y-1], x, y-1, result);
    }
 
    //upper right
    if (x + 1 < board.length && 0 <= y - 1 && !tmp[x + 1][y - 1]){
      solver(board,tmp,lexicon, word+board[x+1][y-1], x+1, y-1, result);
    }
 
    //right
    if (x + 1 < board.length && !tmp[x + 1][y]){
      solver(board, tmp, lexicon, word + board[x+1][y], x+1, y, result);
    }
 
    //lower right
    if (x+1 < board.length && y+1 < board[0].length && !tmp[x+1][y+1]){
      solver(board,tmp,lexicon, word+board[x+1][y+1], x+1, y+1, result);
    }
 
    //down
    if (y + 1 < board[0].length && !tmp[x][y + 1]){
      solver(board, tmp, lexicon, word + board[x][y+1], x, y+1, result);
    }
 
    //lower left
    if (0 <= x - 1 && y + 1 < board[0].length && !tmp[x - 1][y + 1]){
      solver(board,tmp,lexicon, word+board[x-1][y+1], x-1, y+1, result);
    }
 
    //left
    if (0 <= x - 1 && !tmp[x - 1][y]){
      solver(board, tmp, lexicon, word + board[x-1][y], x-1, y, result);
    }
  }
}