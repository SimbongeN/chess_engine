#include <iostream>
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

// ------------------------------------------------- GLOBAL VARIABLES -------------------------------------------------

enum GLOBAL_VARIABLES : int {
    GetType = 7, //number to do bitwise and to get the piece type
    GetColor = 1, //number to do bitwise and after shifting bits 3 to the right
    file = 8,
    Rank = 8
};


//--------------------------------------------- PIECE RELATED FUNCTIONS AND INFORMATION ----------------------------------------------
enum PIECES : uint8_t {
    PAWN = 0b00000001,
    KNIGHT = 0b00000010,
    BISHOP = 0b00000011,
    ROOK = 0b00000100,
    QUEEN = 0b00000101,
    KING = 0b00000110,
    EMPTY = 0
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

//--------------------------------------------- PIECE MOVEMENT -----------------------------------------------
inline int getFileFromPosition(const int position){return position % file;}

bool checkWrap(const int currentPosition, const int nextPosition, const int maxDiff) {
    return abs(getFileFromPosition(currentPosition) - getFileFromPosition(nextPosition)) > maxDiff;
}

bool addMoveInVector(const int currentSquare, const int next_possible_square, vector<uint8_t> &possible_squares,uint8_t board[]) {
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
//possible_moves = [+2 rank, -2 rank] [+1 file, -1 file]  or [1 rank, -1 rank] [+3 file, -3 file]
vector<uint8_t> getAllPossibleKnightMoves(const int currentSquare, uint8_t board[]) {
    vector<uint8_t> possible_squares;
    //first two condition is for [+2 rank, -2 rank] [+1 file, -1 file]
    int next_possible_square = (currentSquare + (2 * Rank)) + 1;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {

        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    next_possible_square = (currentSquare + (2 * Rank)) - 1;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares,board);
    }

    //condition 2 [-2 Rank]
    next_possible_square = (currentSquare + (-2 * Rank)) + 1;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    next_possible_square = (currentSquare + (-2 * Rank)) - 1;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 1)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    //condition for [+1 Rank, -1 Rank] [+2 file, -2 file]
    next_possible_square = (currentSquare + (1 * Rank)) + 2;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    next_possible_square = (currentSquare + (1 * Rank)) - 2;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    //condition for [-1]
    next_possible_square = (currentSquare + (-1 * Rank)) + 2;

    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }

    next_possible_square = (currentSquare + (-1 * Rank)) - 2 ;
    if (!(next_possible_square > 63 || next_possible_square < 0) && !checkWrap(currentSquare, next_possible_square, 2)) {
        addMoveInVector(currentSquare, next_possible_square,possible_squares, board);
    }
    return possible_squares;
}

vector<uint8_t> getAllPossibleBishopMoves(const int currentSquare, uint8_t board[]) {
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
            int next_possible_square_first_quad = (currentSquare + (Nextrank * Rank)) + Nextfile;
            if (!(next_possible_square_first_quad > 63 || next_possible_square_first_quad < 0) && !checkWrap(currentSquare, next_possible_square_first_quad, Nextfile % 8)) {
                bool addResult = addMoveInVector(currentSquare, next_possible_square_first_quad,possible_squares,board);
                if (!addResult)
                    firstQuadrantComplete = true;
            }else {
                firstQuadrantComplete = true;
            }
        }

        //check the second quadrant
        if (!SecondQuadrantComplete) {
            int next_possible_square_Second_quad = (currentSquare + (Nextrank * Rank)) - Nextfile;
            if (!(next_possible_square_Second_quad > 63 || next_possible_square_Second_quad < 0) && !checkWrap(currentSquare, next_possible_square_Second_quad, Nextfile % 8)) {
                bool addResult = addMoveInVector(currentSquare, next_possible_square_Second_quad,possible_squares,board);
                if (!addResult)
                    SecondQuadrantComplete = true;
            }else {
                SecondQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!ThirdQuadrantComplete) {
            int next_possible_square_third_quad = (currentSquare + (-1 * Nextrank * Rank)) - Nextfile;
            if (!(next_possible_square_third_quad > 63 || next_possible_square_third_quad < 0) && !checkWrap(currentSquare, next_possible_square_third_quad, Nextfile % 8)) {
                bool addResult = addMoveInVector(currentSquare, next_possible_square_third_quad,possible_squares,board);
                if (!addResult)
                    ThirdQuadrantComplete = true;
            }else {
                ThirdQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!fourthQuadrantComplete) {
            int next_possible_square_fourth_quad = (currentSquare + (-1 * Nextrank * Rank)) + Nextfile;
            if (!(next_possible_square_fourth_quad > 63 || next_possible_square_fourth_quad < 0) && !checkWrap(currentSquare, next_possible_square_fourth_quad, Nextfile % 8)) {
                bool addResult = addMoveInVector(currentSquare, next_possible_square_fourth_quad,possible_squares,board);
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

vector<uint8_t> getAllPossibleRookMoves(const int currentSquare, uint8_t board[]) {
    vector<uint8_t> possible_squares;
    bool rightDirectionComplete = false;
    bool leftDirectionComplete = false;
    bool upDirectionComplete = false;
    bool downDirectionComplete = false;

    int NextFile = 1;
    int NextRank = 1;
    while (true) {
        if (!rightDirectionComplete) {
            int next_square = currentSquare + (NextFile);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, NextFile * 8)) {
                bool addResult = addMoveInVector(currentSquare, next_square,possible_squares,board);
                if (!addResult)
                    rightDirectionComplete = true;
            }else {
                rightDirectionComplete = true;
            }
        }

        if (!leftDirectionComplete) {
            int next_square = currentSquare + (-1 * NextFile);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, NextFile % 8)) {
                bool addResult = addMoveInVector(currentSquare, next_square,possible_squares,board);
                if (!addResult)
                    leftDirectionComplete = true;
            }else {
                leftDirectionComplete = true;
            }
        }

        if (!upDirectionComplete) {
            int next_square = currentSquare + (NextRank * 8);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 0 )) {
                bool addResult = addMoveInVector(currentSquare, next_square,possible_squares,board);
                if (!addResult)
                    upDirectionComplete = true;
            }else {
                upDirectionComplete = true;
            }
        }

        if (!downDirectionComplete) {
            int next_square = currentSquare + (-1 * NextRank * 8);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 0)) {
                bool addResult = addMoveInVector(currentSquare, next_square,possible_squares,board);
                if (!addResult)
                    downDirectionComplete = true;
            }else {
                downDirectionComplete = true;
            }
        }

        if (rightDirectionComplete && leftDirectionComplete && upDirectionComplete && downDirectionComplete)
            break;

        NextFile++;
        NextRank++;
    }

    return possible_squares;

}

vector<uint8_t> getAllPossiblePawnMoves(const int currentSquare,uint8_t board[]) {
    vector<uint8_t> possible_squares;
    const bool isMoved = hasMoved(board[currentSquare]);
    if (!isBlack(decodeColorPiece(board[currentSquare]))) {
        if (!isMoved) {
            const int next_square = currentSquare + (2 * Rank);
            addMoveInVector(currentSquare, next_square,possible_squares,board);
        }

        int next_square = currentSquare + (1 * Rank) + 1;
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1) && board[next_square] != 0) {
            if (decodeColorPiece(board[currentSquare]) != decodeColorPiece(board[next_square]))
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }

        next_square = currentSquare + (1 * Rank) - 1;
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1) && board[next_square] != 0) {
            if (decodeColorPiece(board[currentSquare]) != decodeColorPiece(board[next_square]))
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }

        next_square = currentSquare + (1 * Rank);
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
            if (board[next_square] == 0)
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }
    }else {
        if (!isMoved) {
            const int next_square = currentSquare + (-2 * Rank);
            addMoveInVector(currentSquare, next_square,possible_squares,board);
        }

        int next_square = currentSquare + (-1 * Rank) + 1;
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1) && board[next_square] != 0) {
            if (decodeColorPiece(board[currentSquare]) != decodeColorPiece(board[next_square]))
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }

        next_square = currentSquare + (-1 * Rank) - 1;
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1) && board[next_square] != 0) {
            if (decodeColorPiece(board[currentSquare]) != decodeColorPiece(board[next_square]))
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }

        next_square = currentSquare + (-1 * Rank);
        if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
            if (board[next_square] == 0)
                addMoveInVector(currentSquare, next_square, possible_squares,board);
        }
    }
    return possible_squares;
}

vector<uint8_t> getAllPossibleQueenMoves(const int currentSquare, uint8_t board[]) {
    vector<uint8_t> getAllRookMoves = getAllPossibleRookMoves(currentSquare,board);
    vector<uint8_t> getAllBishopMoves = getAllPossibleBishopMoves(currentSquare,board);
    for (const uint8_t bishopMoves: getAllBishopMoves)
        getAllRookMoves.push_back(bishopMoves);

    return getAllRookMoves;

}

bool isKingSideCastingPossible(const int currentSquare, uint8_t board[]) {
    if (hasMoved(board[currentSquare]))
        return false;

    if (decodePiece(board[currentSquare + 3]) != ROOK)
        return false;

    for (int i = 1; i <= 3; i++)
        if (board[currentSquare + i] != EMPTY)
            return false;
    if (hasMoved(board[currentSquare + 3]))
        return false;

    return true;

}

bool isQueenSideCastingPossible(const int currentSquare, uint8_t board[]) {
    if (hasMoved(board[currentSquare]))
        return false;

    if (decodePiece(board[currentSquare - 4]) != ROOK)
        return false;

    for (int i = 1; i <= 4; i++)
        if (board[currentSquare + i] != EMPTY)
            return false;
    if (hasMoved(board[currentSquare - 4]))
        return false;

    return true;

}

vector<uint8_t> getAllPossibleKingMoves(const int currentSquare, uint8_t board[], bool kingInCheck) {
    vector<uint8_t> possible_squares;
    int next_square = currentSquare + 1;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
       addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = currentSquare - 1;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = currentSquare + Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = currentSquare - Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square =  (currentSquare + 1) + Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = (currentSquare -  1)+ Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = (currentSquare + 1) - Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    next_square = (currentSquare - 1 ) - Rank;
    if (!(next_square > 63 || next_square < 0) && !checkWrap(currentSquare, next_square, 1)) {
        addMoveInVector(currentSquare, next_square,possible_squares,board);
    }

    if (isKingSideCastingPossible(currentSquare, board) && !kingInCheck) {
        addMoveInVector(currentSquare, 2, possible_squares, board);
    }

    if (isQueenSideCastingPossible(currentSquare, board) && !kingInCheck) {
        addMoveInVector(currentSquare, 2, possible_squares, board);
    }

    return possible_squares;
}

bool checkKnightCastingRays(const int kingsPosition, uint8_t board[]) {
    //first two condition is for [+2 rank, -2 rank] [+1 file, -1 file]
    int knightCastRay = (kingsPosition + (2 * Rank)) + 1;

    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 1)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    knightCastRay = (kingsPosition + (2 * Rank)) - 1;
    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 1)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    //condition 2 [-2 Rank]
    knightCastRay = (kingsPosition + (-2 * Rank)) + 1;

    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 1)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    knightCastRay = (kingsPosition + (-2 * Rank)) - 1;
    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 1)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    //condition for [+1 Rank, -1 Rank] [+2 file, -2 file]
    knightCastRay = (kingsPosition + (1 * Rank)) + 2;

    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 2)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    knightCastRay = (kingsPosition + (1 * Rank)) - 2;
    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 2)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    //condition for [-1]
    knightCastRay = (kingsPosition + (-1 * Rank)) + 2;

    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 2)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    knightCastRay = (kingsPosition + (-1 * Rank)) - 2 ;
    if (!(knightCastRay > 63 || knightCastRay < 0) && !checkWrap(kingsPosition, knightCastRay, 2)) {
        if (decodePiece(board[knightCastRay]) == KNIGHT && decodeColorPiece(board[knightCastRay]) != decodeColorPiece(board[kingsPosition]))
            return true;
    }

    return false;
}

bool checkBishopAndQueenCastingRays(const int kingsPosition, uint8_t board[]) {
    bool firstQuadrantComplete = false;
    bool SecondQuadrantComplete = false;
    bool ThirdQuadrantComplete = false;
    bool fourthQuadrantComplete = false;

    int Nextfile = 1;
    int Nextrank = 1;

    while (true) {

        //check first quadrant
        if (!firstQuadrantComplete) {
            int next_possible_square_first_quad = (kingsPosition + (Nextrank * Rank)) + Nextfile;
            if (!(next_possible_square_first_quad > 63 || next_possible_square_first_quad < 0) && !checkWrap(kingsPosition, next_possible_square_first_quad, Nextfile % 8)) {
                if (decodePiece(board[next_possible_square_first_quad]) == BISHOP || decodePiece(board[next_possible_square_first_quad]) ==  QUEEN && decodeColorPiece(board[next_possible_square_first_quad]) != decodeColorPiece(board[kingsPosition]))
                    return true;
                if (board[next_possible_square_first_quad] != EMPTY)
                    firstQuadrantComplete = true;
            }else {
                firstQuadrantComplete = true;
            }
        }

        //check the second quadrant
        if (!SecondQuadrantComplete) {
            int next_possible_square_Second_quad = (kingsPosition + (Nextrank * Rank)) - Nextfile;
            if (!(next_possible_square_Second_quad > 63 || next_possible_square_Second_quad < 0) && !checkWrap(kingsPosition, next_possible_square_Second_quad, Nextfile % 8)) {
                if (decodePiece(board[next_possible_square_Second_quad]) == BISHOP || decodePiece(board[next_possible_square_Second_quad]) ==  QUEEN && decodeColorPiece(board[next_possible_square_Second_quad]) != decodeColorPiece(board[kingsPosition]))
                    return true;
                if (board[next_possible_square_Second_quad] != EMPTY)
                    SecondQuadrantComplete = true;
            }else {
                SecondQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!ThirdQuadrantComplete) {
            int next_possible_square_third_quad = (kingsPosition + (-1 * Nextrank * Rank)) - Nextfile;
            if (!(next_possible_square_third_quad > 63 || next_possible_square_third_quad < 0) && !checkWrap(kingsPosition, next_possible_square_third_quad, Nextfile % 8)) {
                if (decodePiece(board[next_possible_square_third_quad]) == BISHOP || decodePiece(board[next_possible_square_third_quad]) ==  QUEEN && decodeColorPiece(board[next_possible_square_third_quad]) != decodeColorPiece(board[kingsPosition]))
                    return true;
                if (board[next_possible_square_third_quad] != EMPTY)
                    ThirdQuadrantComplete = true;
            }else {
                ThirdQuadrantComplete = true;
            }
        }

        //check the third quadrant
        if (!fourthQuadrantComplete) {
            int next_possible_square_fourth_quad = (kingsPosition + (-1 * Nextrank * Rank)) + Nextfile;
            if (!(next_possible_square_fourth_quad > 63 || next_possible_square_fourth_quad < 0) && !checkWrap(kingsPosition, next_possible_square_fourth_quad, Nextfile % 8)) {
                if ((decodePiece(board[next_possible_square_fourth_quad]) == BISHOP || decodePiece(board[next_possible_square_fourth_quad]) ==  QUEEN) && decodeColorPiece(board[next_possible_square_fourth_quad]) != decodeColorPiece(board[kingsPosition]))
                    return true;
                if (board[next_possible_square_fourth_quad] != EMPTY)
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

    return false;
}

bool checkRookAndQueenCastingRays(const int kingPosition, uint8_t board[]) {
    bool rightDirectionComplete = false;
    bool leftDirectionComplete = false;
    bool upDirectionComplete = false;
    bool downDirectionComplete = false;

    int NextFile = 1;
    int NextRank = 1;
    while (true) {
        if (!rightDirectionComplete) {
            int next_square = kingPosition + (NextFile);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(kingPosition, next_square, NextFile % 8)) {
                if (decodePiece(board[next_square]) == ROOK || decodePiece(board[next_square]) ==  QUEEN && decodeColorPiece(board[next_square]) != decodeColorPiece(board[kingPosition]))
                    return true;
                if (board[next_square] != EMPTY)
                    rightDirectionComplete = true;
            }else {
                rightDirectionComplete = true;
            }
        }

        if (!leftDirectionComplete) {
            int next_square = kingPosition + (-1 * NextFile);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(kingPosition, next_square, NextFile)) {
                if (decodePiece(board[next_square]) == ROOK || decodePiece(board[next_square]) ==  QUEEN && decodeColorPiece(board[next_square]) != decodeColorPiece(board[kingPosition]))
                    return true;
                if (board[next_square] != EMPTY)
                    leftDirectionComplete = true;
            }else {
                leftDirectionComplete = true;
            }
        }

        if (!upDirectionComplete) {
            int next_square = kingPosition + (NextRank * 8);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(kingPosition, next_square, 0)) {
                if (decodePiece(board[next_square]) == ROOK || decodePiece(board[next_square]) ==  QUEEN && decodeColorPiece(board[next_square]) != decodeColorPiece(board[kingPosition]))
                    return true;
                if (board[next_square] != EMPTY)
                    upDirectionComplete = true;
            }else {
                upDirectionComplete = true;
            }
        }

        if (!downDirectionComplete) {
            int next_square = kingPosition + (-1 * NextRank * 8);
            if (!(next_square > 63 || next_square < 0) && !checkWrap(kingPosition, next_square, 0)) {
                if (decodePiece(board[next_square]) == ROOK || decodePiece(board[next_square]) ==  QUEEN && decodeColorPiece(board[next_square]) != decodeColorPiece(board[kingPosition]))
                    return true;
                if (board[next_square] != EMPTY)
                    downDirectionComplete = true;
            }else {
                downDirectionComplete = true;
            }
        }

        if (rightDirectionComplete && leftDirectionComplete && upDirectionComplete && downDirectionComplete)
            break;

        NextFile++;
        NextRank++;
    }

    return false;

}

bool isKingInCheck(const int kingsPosition, uint8_t board[]) {
    if (checkKnightCastingRays(kingsPosition, board))
        return true;

    if (checkBishopAndQueenCastingRays(kingsPosition, board))
        return true;

    if (checkRookAndQueenCastingRays(kingsPosition, board))
        return true;

    //TODO: add pawn check casting rays

    return false;
}




//--------------------------------------------------- BOARD RELATED FUNCTIONS -----------------------------------------

struct GameState {
    uint8_t board[64]{};
    uint8_t unpasentSquare = 0;
    uint8_t colorsTurn = 0; //0 indicates whites turns
    bool kingInCheck = false;
    uint8_t blackKingsCurrentSquare = 60;
    uint8_t whiteKingsCurrentSquare = 4;
    bool checkMate = isKingInCheck(whiteKingsCurrentSquare, board);


};

bool isKingInCheck(vector<uint8_t>& pieceLineOfSight, uint8_t kingsSquare) {
    for (uint8_t square: pieceLineOfSight)
        if (square == kingsSquare)
            return true;
    return false;
}

//initializing board
GameState clearBoard(GameState currentSate)
{
    for(int i = 0; i < 64; i++)
        currentSate.board[i] = EMPTY;

    return currentSate;
}

//resetting pieces to starting pieces
GameState resetBoard(GameState currentSate) {
    //white pieces 0 - 7
    currentSate.board[0] = createPiece(ROOK, false, false);
    currentSate.board[1] = createPiece(KNIGHT, false, false);
    currentSate.board[2] = createPiece(BISHOP, false, false);
    currentSate.board[3] = createPiece(QUEEN, false, false);
    currentSate.board[4] = createPiece(KING, false, false);
    currentSate.board[5] = createPiece(BISHOP, false, false);
    currentSate.board[6] = createPiece(KNIGHT, false, false);
    currentSate.board[7] = createPiece(ROOK, false, false);

    //white pawn pieces
    for (int i = 8 ; i < 16; i++ )
        currentSate.board[i] = createPiece(PAWN, false,false);

    // black pieces
    currentSate.board[56] = createPiece(ROOK, true, false);
    currentSate.board[57] = createPiece(KNIGHT, true, false);
    currentSate.board[58] = createPiece(BISHOP, true, false);
    currentSate.board[59] = createPiece(QUEEN, true, false);
    currentSate.board[60] = createPiece(KING, true, false);
    currentSate.board[61] = createPiece(BISHOP, true, false);
    currentSate.board[62] = createPiece(KNIGHT, true, false);
    currentSate.board[63] = createPiece(ROOK, true, false);

    //white pawn pieces
    for (int i = 48 ; i < 56; i++ )
        currentSate.board[i] = createPiece(PAWN, true, false);

    return currentSate;

}


//piece decoding to char version for testing
char pieceChar(const uint8_t piece) {
    if (piece == EMPTY) return '.';

    const int type = decodePiece(piece);
    const int color = decodeColorPiece(piece);

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


void printBoard(uint8_t board[])
{
    for(int rank = 7; rank >= 0; rank--)
    {
        for(int file = 0; file < 8; file++)
        {
            const int sq = rank*8 + file;
            std::cout << pieceChar(board[sq]) << " ";
        }
        std::cout << std::endl;
    }
}

GameState movePiece(const int from, const int to, GameState currentState) {

    currentState.board[to] = setHasMoved(currentState.board[from]);
    currentState.board[from] = EMPTY;
    if (currentState.colorsTurn == 1) {
        if (decodePiece(currentState.board[to]) == KING)
            currentState.blackKingsCurrentSquare = to;
        currentState.colorsTurn = 0;
    }
    else {
        if (decodePiece(currentState.board[to]) == KING)
            currentState.whiteKingsCurrentSquare = to;

        currentState.colorsTurn = 1;
    }

    //TODO: do en peasant squares

    return currentState;
}

bool isValidMove(const int from, const int to,GameState currentState) {

    if (decodeColorPiece(currentState.board[from]) != currentState.colorsTurn) {
        cout<< "Wrong piece colors turn \n";
        return false;
    }


    if (currentState.board[from] == EMPTY) {
        cout << "No piece on square " << from << "\n";
        return false;
    }

    const uint8_t pieceType = decodePiece(currentState.board[from]);
    vector<uint8_t> possibleMoves;

    switch (pieceType) {
        case KNIGHT: possibleMoves = getAllPossibleKnightMoves(from, currentState.board); break;
        case BISHOP: possibleMoves = getAllPossibleBishopMoves(from, currentState.board); break;
        case ROOK: possibleMoves = getAllPossibleRookMoves(from, currentState.board); break;
        case PAWN: possibleMoves = getAllPossiblePawnMoves(from, currentState.board); break;
        case QUEEN: possibleMoves = getAllPossibleQueenMoves(from, currentState.board); break;
        case KING: possibleMoves =  getAllPossibleKingMoves(from, currentState.board, currentState.kingInCheck); break;
        default:
            cout << "Move generation not implemented for this piece yet.\n";
        return false;
    }

    bool movePossible = false;
    for (const int sq : possibleMoves)
        if (sq == to){ movePossible =  true; break;}

    cout<<"Possible moves: ";
    for (const int sq : possibleMoves)
        cout<<sq << " ";
    cout<<endl;

    //simulate move and check if king is in check
    if (movePossible) {
        GameState tempState = movePiece(from, to, currentState);
        if (currentState.colorsTurn == 1) {
            tempState.kingInCheck = isKingInCheck(tempState.blackKingsCurrentSquare, tempState.board);
        }else {
            tempState.kingInCheck = isKingInCheck(tempState.whiteKingsCurrentSquare, tempState.board);
        }

        if (tempState.kingInCheck) {
            cout<<"King is check invalid move \n";
            movePossible = false;
        }
        else
            movePossible = true;
    }

    return movePossible;
}


void gameLoop(const GameState startState) {
    string input;
    int from, to;

    cout << "Chess engine started. Type 'quit' to exit.\n";
    cout << "Enter moves as: <from> <to>  (e.g. '1 18')\n\n";
    GameState state = startState;
    while (true) {
        printBoard(state.board);
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

        if (!isValidMove(from, to,state)) {
            cout << "Illegal move.\n";
            continue;
        }

        state = movePiece(from, to,state);
    }
}


int main() {
    GameState startState;
    clearBoard(startState);
    startState = resetBoard(startState);
    gameLoop(startState);
}

