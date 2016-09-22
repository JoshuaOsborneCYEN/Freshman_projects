import java.util.Random;
/**
 * This class starts up a simple game of Tic Tac Toe with positions stored in arrays.
 * Though the game itself is simple the implementation was much more difficult than expected.
 * 
 * @author Joshua Osborne 
 * @version CSC/CYEN 120-002 2-12-16
 */
public class TicTacToeGame
{
    private String[][] field;
    private TicTacToeView view;    
    
    /**
     * Constructor for objects of class TicTacToeGame
     * Initializes field class and creates new instance
     * of TicTacToeView.
     */
    public TicTacToeGame()
    {
        field = new String[3][3]; //x,y
        this.reset();
        view = new TicTacToeView(this);
    }

    /**
     * Clears all the squares of the field.
     */
    public void reset() {
        for (int i = 0; i < field.length; i++) {            
            for (int j = 0; j < field.length; j++) {
                field[i][j] = "";
            }
        }            
    }
    
    /**
     * Records that X or O has claimed the square indicated by the first parameter
     * ONLY if the location is valid and not occupied.
     * @param String location of mark (expected values from TicTacToeView)
     * @param String X or O 
     * @return boolean whether mark was successful or not
     */
    
    public boolean markSquare(String location, String mark) {
        boolean successful;
        switch (location) { //assuming grid is laid out like XY plane
            case "UPPER LEFT": successful = placeMark(0, 0, mark);
            break;
            case "UPPER CENTER": successful = placeMark(1, 0, mark);
            break;
            case "UPPER RIGHT": successful = placeMark(2, 0, mark);
            break;
            
            case "CENTER LEFT": successful = placeMark(0, 1, mark);
            break;
            case "CENTER CENTER": successful = placeMark(1, 1, mark);
            break;
            case "CENTER RIGHT": successful = placeMark(2, 1, mark);
            break;
        
            case "LOWER LEFT": successful = placeMark(0, 2, mark);
            break;
            case "LOWER CENTER": successful = placeMark(1, 2, mark);
            break;
            case "LOWER RIGHT": successful = placeMark(2, 2, mark);
            break;
            
            default: System.out.println("Please select a valid location. Valid locations are: "+
                        "UPPER LEFT, UPPER CENTER, UPPER LEFT, " +
                        "CENTER LEFT, CENTER CENTER, CENTER RIGHT, " +
                        "LOWER LEFT, LOWER CENTER, LOWER RIGHT");
                     successful = false;
 
        }
        
        return successful;
    }
    
    /**
     * Internal method for TicTacToeGame that markSquare uses for its logic
     * and dirty work to place tic-tac-toe tiles.
     * @param int corresponding to x coordinate of location 
     * @param int corresponding to y coordinate of location
     * @param String mark to be placed (accepts "X" or "O")
     */
    private boolean placeMark(int x, int y, String mark) {
        if (!mark.equals("X") && !mark.equals("O")) {
            System.out.println("Please use capital 'X' or capital 'O'");
            return false;
        } else if (field[x][y].equals("")) {
            field[x][y] = mark;
            return true;
        } else {
            System.out.println("There is already a character there; please use a different location.");
            return false;
        }
    }
    
    /**
     * Checks to see if anybody has won the game.
     * @return boolean whether there is a winner
     */
    public boolean checkWinner() {
        /**
         * There is a win if any of the following triples are the same letter:
         * 00 10 20 row1 
         * 01 11 21 row2 
         * 02 12 22 row3 
         * 00 01 02 col1 
         * 10 11 12 col2 
         * 20 21 22 col3 
         * 00 11 22 diag 
         * 02 11 20 diag 
         * Choosing 3 arbitrary points, the upper left, center, and lower right,
         * any winning configuration must include at least one of these points.
         * If someone has one of these points we can check adjacent points.
         */
        String letter; //stores letter of field position being checked.
        if (!field[0][0].equals("")) { //Checking upper left
            letter = field[0][0];
            if (field[1][0].equals(letter)) {
                if (field[2][0].equals(letter)) {
                    return true;
                }
            }  
            
            if (field[1][1].equals(letter)) {
                if (field[2][2].equals(letter)) {
                    return true;
                }
            } 
            
            if (field[0][1].equals(letter)) {
                if (field[0][2].equals(letter)) {
                    return true;
                }
            }
        } 
        
        if (!field[1][1].equals("")) { //Checking center
            letter = field[1][1];
            if (field[0][1].equals(letter)) {
                if (field[2][1].equals(letter)) {
                    return true;
                }
            } 
            
            if (field[1][0].equals(letter)) {
                if (field[1][2].equals(letter)) {
                    return true;
                }
            } 
            
            if (field[0][2].equals(letter)) {
                if (field[2][0].equals(letter)) {
                    return true;
                }
            }
        } 
        
        if (!field[2][2].equals("")) { //Checking bottom right
            letter = field[2][2];
            if (field[0][2].equals(letter)) {
                if (field[1][2].equals(letter)) {
                    return true;
                }
            } 
            
            if (field[2][0].equals(letter)) {
                if (field[2][1].equals(letter)) {
                    return true;
                }
            }
        }
        
        //No matches?? Alright, no winner.
        
        return false;
            
    }
    
    /**
     * Checks to see if the board is full and there are no winners.
     * @return boolean if cat got the game.
     */
    public boolean catGame() {
        for (int i = 0; i < field.length; i++) {
            
            for (int j = 0; j < field.length; j++) {
                
                if (field[i][j].equals("")) {
                    
                    return false;
                }
            }
        }
            
        
       //field has been checked and hasn't returned false, therefore there are no more blank spaces
        return true;
    }
    
    /**
     * Invokes method from TicTacToeView to let the computer make a turn.
     * It will keep selecting random numbers until some spot is valid.
     * The logic could be improved here similar to the checkWinner logic,
     * but that's really not necessary since Tic-Tac-Toe is a solved game,
     * and proper logic can make the computer unbeatable. And that's no fun.
     */
    public void computerTurn() {
        Random r = new Random();
        int x;
        int y;
        boolean stillLooking = true;
        while (stillLooking) {
            x = r.nextInt(3);
            y = r.nextInt(3);
            stillLooking = !view.nextMove(x,y);
        }
    }
}