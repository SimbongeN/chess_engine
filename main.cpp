#include <iostream>
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

// ------------------------------------------------- GLOBAL VARIABLES -------------------------------------------------

//global board array
uint8_t board[64];
constexpr uint8_t EMPTY = 0;
constexpr int GetType = 7; //number to do bitwise and to get the piece type
constexpr int GetColor = 1; //number to do bitwise and after shifting bits 3 to the right
constexpr int file = 8;


//--------------------------------------------- PIECE RELATED FUNCTIONS AND INFORMATION ----------------------------------------------
enum PIECES : uint8_t {
    PAWN = 0b00000001,
    KNIGHT = 0b00000010,
    BISHOP = 0b00000011,
    ROOK = 0b00000100,
    QUEEN = 0b00000101,
    KING = 0b00000110
};

uint8_t decodePiece(const uint8_t piece) {
    return piece & GetType;
}

int decodeColorPiece(const uint8_t piece) {
    return (piece >> 3) & GetColor;
}

bool isBlack(const int color) {
    return color == 1;
}

bool hasMoved(const uint8_t piece) {
    return ((piece >> 4) & 1) == 1;
}

uint8_t setHasMoved(const uint8_t piece) {
    return piece | 16;
}

//create piece method
uint8_t createPiece(uint8_t piece,const bool isBlack, const bool hasMoved) {

    if (isBlack) {
        piece = piece | 1 << 3;
        return hasMoved ? piece | (1 << 4): piece;
    }

    return hasMoved ? piece | (1 << 4): piece;
}

//---------------------------------------------------------- PIECE MOVEMENT -----------------------------------------------
inline int getFileFromPosition(const int position){return position % file;}

bool checkWrap(const int currentPosition, const int nextPosition, const int maxDiff) {
    return abs(getFileFromPosition(currentPosition) - getFileFromPosition(nextPosition)) > maxDiff;
}

bool addPieceInVector(const int currentSquare, const int next_possible_square, vector<uint8_t> &possible_squares) {
    //decode the next possible square piece if it is a king
    const bool isKing = decodePiece(board[next_possible_square]) == KING;

    if (board[next_possible_square] == 0)
        possible_squares.push_back(next_possible_square);
    else {
        const int currentPieceColor = decodeColorPiece(board[currentSquare]);
        const int nextPositionPiece = decodeColorPiece(board[next_possible_square]);
        if (currentPieceColor != nextPositionPiece && !isKing) {
            possible_squares.push_back(next_possible_square);
            return false;
        }
        return false;
    }

    return true;
}


//get all valid moves for knight
//the knight has 8 possible plays
//1 file = 8
//possible_moves = [+2 file, -2 file] [+1 rank, -1 rank]  or [1 file, -1 file] [+3 rank, -3 rank]
vector<uint8_t> getAllPossibleKnightMoves(const int currentSquare) {
    vector<uint8_t> possible_squares;
    //first two condition is for [+2 file, -2 file] [+1 rank, -1 rank]
    int next_possible_square = (currentSquare + (2 * file)) + 1;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {

        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    next_possible_square = (currentSquare + (2 * file)) - 1;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    //condition 2 [-2 file]
    next_possible_square = (currentSquare + (-2 * file)) + 1;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    next_possible_square = (currentSquare + (-2 * file)) - 1;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    //condition for [+1 file, -1 file] [+2 rank, -2 rank]
    next_possible_square = (currentSquare + (1 * file)) + 2;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    next_possible_square = (currentSquare + (1 * file)) - 2;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    //condition for [-1]
    next_possible_square = (currentSquare + (-1 * file)) + 2;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }

    next_possible_square = (currentSquare + (-1 * file)) - 2 ;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addPieceInVector(currentSquare, next_possible_square,possible_squares);
    }
    return possible_squares;
}

vector<uint8_t> getAllPossibleBishopMoves(const int currentSquare) {

    bool firstQuadrantComplete = false;
    bool SecondQuadrantComplete = false;
    bool ThirdQuadrantComplete = false;
    bool fourthQuadrantComplete = false;

    vector<uint8_t> possible_squares;
    int Nextfile = 1;
    int Nextrank = 1;

    while (true) {

        //check first quadrant
        if (!firstQuadrantComplete) {
            int next_possible_square_first_quad = (currentSquare + (Nextfile * file)) + Nextrank;
            if (!(next_possible_square_first_quad > 63 || next_possible_square_first_quad < 0) && !checkWrap(currentSquare, next_possible_square_first_quad, 1)) {
                bool addResult = addPieceInVector(currentSquare, next_possible_square_first_quad,possible_squares);
                if (!addResult)
                    firstQuadrantComplete = true;
            }else {
                firstQuadrantComplete = true;
            }
        }

        //check the second quadrant
        if (!SecondQuadrantComplete) {
            int next_possible_square_Second_quad = (currentSquare + (Nextfile * file)) - Nextrank;
            if (!(next_possible_square_Second_quad > 63 || next_possible_square_Second_quad < 0) && !checkWrap(currentSquare, next_possible_square_Second_quad, 1)) {
                bool addResult = addPieceInVector(currentSquare, next_possible_square_Second_quad,possible_squares);
                if (!addResult)
                    SecondQuadrantComplete = true;
            }else {
                SecondQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!ThirdQuadrantComplete) {
            int next_possible_square_third_quad = (currentSquare + (-1 * Nextfile * file)) - Nextrank;
            if (!(next_possible_square_third_quad > 63 || next_possible_square_third_quad < 0) && !checkWrap(currentSquare, next_possible_square_third_quad, 1)) {
                bool addResult = addPieceInVector(currentSquare, next_possible_square_third_quad,possible_squares);
                if (!addResult)
                    ThirdQuadrantComplete = true;
            }else {
                ThirdQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!fourthQuadrantComplete) {
            int next_possible_square_fourth_quad = (currentSquare + (-1 * Nextfile * file)) + Nextrank;
            if (!(next_possible_square_fourth_quad > 63 || next_possible_square_fourth_quad < 0) && !checkWrap(currentSquare, next_possible_square_fourth_quad, 1)) {
                bool addResult = addPieceInVector(currentSquare, next_possible_square_fourth_quad,possible_squares);
                if (!addResult)
                    fourthQuadrantComplete = true;
            }else {
                fourthQuadrantComplete = true;
            }
        }

        if ( firstQuadrantComplete && SecondQuadrantComplete && ThirdQuadrantComplete && fourthQuadrantComplete)
            break;

        Nextfile++;
        Nextrank++;
    }

    return possible_squares;

}


//-------------------------------------------------------- BOARD RELATED FUNCTIONS -----------------------------------------

//initializing board
void clearBoard()
{
    for(int i = 0; i < 64; i++)
        board[i] = EMPTY;
}

//resetting pieces to starting pieces
void resetBoard() {
    //white pieces 0 - 7
    board[0] = createPiece(ROOK, false, false);
    board[1] = createPiece(KNIGHT, false, false);
    board[2] = createPiece(BISHOP, false, false);
    board[3] = createPiece(KING, false, false);
    board[4] = createPiece(QUEEN, false, false);
    board[5] = createPiece(BISHOP, false, false);
    board[6] = createPiece(KNIGHT, false, false);
    board[7] = createPiece(ROOK, false, false);

    //white pawn pieces
    for (int i = 8 ; i < 16; i++ )
        board[i] = createPiece(PAWN, false,false);

    // black pieces
    board[56] = createPiece(ROOK, true, false);
    board[57] = createPiece(KNIGHT, true, false);
    board[58] = createPiece(BISHOP, true, false);
    board[59] = createPiece(KING, true, false);
    board[60] = createPiece(QUEEN, true, false);
    board[61] = createPiece(BISHOP, true, false);
    board[62] = createPiece(KNIGHT, true, false);
    board[63] = createPiece(ROOK, true, false);

    //white pawn pieces
    for (int i = 48 ; i < 56; i++ )
        board[i] = createPiece(PAWN, true, false);

}


//piece decoding to char version for testing
char pieceChar(uint8_t piece) {
    if (piece == EMPTY) return '.';

    int type = piece & 7;
    int color = (piece >> 3) & 1;

    char c = 0;

    switch(type)
    {
        case PAWN:   c='p'; break;
        case KNIGHT: c='n'; break;
        case BISHOP: c='b'; break;
        case ROOK:   c='r'; break;
        case QUEEN:  c='q'; break;
        case KING:   c='k'; break;
    }

    if (color == 0) c = static_cast<char>(toupper(c));

    return c;
}


void printBoard()
{
    for(int rank = 7; rank >= 0; rank--)
    {
        for(int file = 0; file < 8; file++)
        {
            int sq = rank*8 + file;
            std::cout << pieceChar(board[sq]) << " ";
        }
        std::cout << std::endl;
    }
}

void movePiece(const int from, const int to) {

    board[to] = setHasMoved(board[from]);
    board[from] = EMPTY;
}

bool isValidMove(const int from, const int to) {
    if (board[from] == EMPTY) {
        cout << "No piece on square " << from << "\n";
        return false;
    }

    uint8_t pieceType = decodePiece(board[from]);
    vector<uint8_t> possibleMoves;

    switch (pieceType) {
        case KNIGHT: possibleMoves = getAllPossibleKnightMoves(from); break;
        case BISHOP: possibleMoves = getAllPossibleBishopMoves(from); break;
        default:
            cout << "Move generation not implemented for this piece yet.\n";
        return false;
    }

    // cout << "possible moves are: ";
    // for (const int sq : possibleMoves)
    //     cout << sq << "-";
    //
    // cout << "\n";

    for (int sq : possibleMoves)
        if (sq == to) return true;

    return false;
}


void gameLoop() {
    string input;
    int from, to;

    cout << "Chess engine started. Type 'quit' to exit.\n";
    cout << "Enter moves as: <from> <to>  (e.g. '1 18')\n\n";

    while (true) {
        printBoard();
        cout << "\n> ";

        // Read the whole line so 'quit' is caught cleanly
        if (!getline(cin, input)) break;

        if (input == "quit") {
            cout << "Goodbye.\n";
            break;
        }

        // Parse two integers from the input line
        istringstream ss(input);
        if (!(ss >> from >> to)) {
            cout << "Invalid input. Enter two square numbers or 'quit'.\n";
            continue;
        }

        if (from < 0 || from > 63 || to < 0 || to > 63) {
            cout << "Squares must be between 0 and 63.\n";
            continue;
        }

        if (!isValidMove(from, to)) {
            cout << "Illegal move.\n";
            continue;
        }

        movePiece(from, to);
    }
}

int main() {
    clearBoard();
    resetBoard();
    //printBoard();
    gameLoop();
}

