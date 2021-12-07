#include "Board.h"

#include <iostream>
#include <algorithm>

using Type = Board::Piece::Type;
using Color = Board::Piece::Color;
using Status = Board::Status;

std::vector<Board::Square> Board::pawnPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const {
	std::vector<Square> moves;

	Piece p = getPiece(s);
	int direction = p.getColor() == Color::White ? -1 : 1;
	
	size_t nextRow = s.row + direction;
	if (nextRow > 7) return moves;

	{
		Type nextSquareType = _board[nextRow][s.column].getType();

		if (nextSquareType == Type::Empty && addIf(nextSquareType))
			moves.emplace_back(nextRow, s.column);
	}

	if (
		(direction == -1 && s.row == 6 && getPiece(5, s.column).getType() == Type::Empty)
		||
		(direction == 1 && s.row == 1 && getPiece(2, s.column).getType() == Type::Empty)
		) {
		size_t superRow = s.row + 2*direction;
		Type nextSquareType = _board[superRow][s.column].getType();

		if (nextSquareType == Type::Empty && addIf(nextSquareType))
			moves.emplace_back(superRow, s.column);
	}

	{
		size_t nextColumn = s.column - 1;

		if (isValidSquare(nextRow, nextColumn)) {
			Piece nextPiece = _board[nextRow][nextColumn];
			if (nextPiece.getType() != Type::Empty && nextPiece.getColor() != p.getColor() && addIf(nextPiece.getType()))
				moves.emplace_back(nextRow, nextColumn);
		}
	}
	{
		size_t nextColumn = s.column + 1;

		if (isValidSquare(nextRow, nextColumn)) {
			Piece nextPiece = _board[nextRow][nextColumn];
			if (nextPiece.getType() != Type::Empty && nextPiece.getColor() != p.getColor() && addIf(nextPiece.getType()))
				moves.emplace_back(nextRow, nextColumn);
		}
	}


	return moves;
}

int rookRow[4] = { 0, 0, -1, 1 }, rookColumn[4] = {-1, 1, 0, 0};

std::vector<Board::Square> Board::rookPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const {
	std::vector<Square> moves;

	Color rookColor = getPiece(s).getColor();

	for(size_t k = 0 ; k < 4 ; ++k)
		for (size_t i = 1; true ; ++i) {
			Square nextSquare (s.row + rookRow[k] * i, s.column + rookColumn[k]*i);
			if (!isValidSquare(nextSquare)) break;
			Piece nextPiece = getPiece(nextSquare);
		
			if (nextPiece.getType() != Type::Empty) {
				if (nextPiece.getColor() != rookColor)
					moves.push_back(nextSquare);
				break;
			}
			moves.push_back(nextSquare);
		}

	return moves;
}

int bishopRow[4] = { 1, 1, -1, -1 }, bishopColumn[4] = { -1, 1, -1, 1 };

std::vector<Board::Square> Board::bishopPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const {
	std::vector<Square> moves;

	Color bishopColor = getPiece(s).getColor();

	for (size_t k = 0; k < 4; ++k)
		for (size_t i = 1; true; ++i) {
			Square nextSquare(s.row + bishopRow[k] * i, s.column + bishopColumn[k] * i);
			if (!isValidSquare(nextSquare)) break;
			Piece nextPiece = getPiece(nextSquare);

			if (nextPiece.getType() != Type::Empty) {
				if (nextPiece.getColor() != bishopColor)
					moves.push_back(nextSquare);
				break;
			}
			moves.push_back(nextSquare);
		}

	return moves;
}

std::vector<Board::Square> Board::queenPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const {
	std::vector<Square> rookMoves = rookPossibleMovesFromIf(s, addIf), bishopMoves = bishopPossibleMovesFromIf(s, addIf);
	rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());
	return rookMoves;
}


