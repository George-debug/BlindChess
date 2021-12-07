#include "Board.h"

#include <iostream>
#include <algorithm>

using Type = Board::Piece::Type;
using Color = Board::Piece::Color;
using Status = Board::Status;

static constexpr Type firstLine[8] = {
	Type::Rook,
	Type::Knight,
	Type::Bishop,
	Type::Queen,
	Type::King,
	Type::Bishop,
	Type::Knight,
	Type::Rook
};

Color Board::getTurnColor() const {
	return _turnColor;
}

Status Board::getStatus() const {
	return _status;
}

std::vector< std::vector<Board::Piece>> Board::getBoard() const {
	std::vector< std::vector<Board::Piece>> output;
	output.reserve(8);

	for (size_t i = 0; i < 8; ++i) {
		output.emplace_back(std::begin(_board[i]), std::end(_board[i]));
	}

	return output;
}

Board::Board() : _status(Status::Live), _turnColor(Color::White) {
	for (size_t i = 0; i < 8; ++i) {
		_board[6][i] = Piece(Color::White, Type::Pawn);
		_board[7][i] = Piece(Color::White, firstLine[i]);

		_board[1][i] = Piece(Color::Black, Type::Pawn);
		_board[0][i] = Piece(Color::Black, firstLine[i]);
	}
}

bool Board::isValidSquare(size_t r, size_t c) {
	return r < 8 && c < 8;
}

bool Board::isValidSquare(const Square& s) {
	return isValidSquare(s.row, s.column);
}

Board::Square translateRowNumber(const std::string& s) {
	return Board::Square(8 - (s[1] - '0'), s[0] - 'a');
}

Board::Move Board::translateGoFromGoToNotation(const std::string& goFrom, const std::string& goTo) {
	return Move(
		translateRowNumber(goFrom),
		translateRowNumber(goTo)
	);
}

std::vector<Board::Square> Board::possibleMovesFromIf(const Board::Square& s, bool(*addIf)(Type) ) const {
	std::vector<Board::Square> moves;
	Piece p = _board[s.row][s.column];

	switch (p.getType())
	{
	case Type::Pawn:
		return pawnPossibleMovesFromIf(s, addIf);

	//case Type::King:
	//	break;
	case Type::Queen:
		return queenPossibleMovesFromIf(s, addIf);
	case Type::Rook:
		return rookPossibleMovesFromIf(s, addIf);
	//case Type::Knight:
	//	break;
	case Type::Bishop:
		return bishopPossibleMovesFromIf(s, addIf);
	}
	return moves;
}

bool allowAll(Type t) {
	return true;
}

std::vector<Board::Square> Board::possibleMovesFrom(const Square& s) const {
	return possibleMovesFromIf(s, allowAll);
}

std::vector<Board::Square> Board::possibleMovesFrom(size_t r, size_t c) const {
	return possibleMovesFrom(Square(r, c));
}

bool Board::move(size_t r1, size_t c1, size_t r2, size_t c2) {
	return move(Square(r1, c1), Square(r2, c2));
}

bool Board::move(const Square& from, const Square& to) {

	std::cout << "Board: I try (" << from.row << ", " << from.column << ") to (" << to.row << ", " << to.column << ")" << std::endl;

	if ( !(isValidSquare(from) && isValidSquare(to)) )
		return false;

	Piece p1 = _board[from.row][from.column], p2 = _board[to.row][to.column];

	if (p1.getType() == Type::Empty || (p1.getColor() == p2.getColor() && p2.getType() != Type::Empty) || p1.getColor() != _turnColor)
		return false;

	auto moves = possibleMovesFrom(from);

	if(std::find(moves.begin(), moves.end(), to) == moves.end())
		return false;

	_board[to.row][to.column] = _board[from.row][from.column];
	_board[from.row][from.column] = Piece(Color::White, Type::Empty);

	// _turnColor = !_turnColor; nu merge pentru ca "nu e convetibil catre null"
	_turnColor = _turnColor == Piece::Color::White ? Piece::Color::Black : Piece::Color::White ;
	return true;
}

bool Board::move(const std::string& from, const std::string& to) {
	auto m = translateGoFromGoToNotation(from, to);

	return move(m);
}

bool Board::move(const Move& m) {
	return move(m.from, m.to);
}

bool Board::isFinished() const {
	return _status != Status::Live && _status != Status::PromotionNeeded;
}

Board::Piece Board::getPiece(size_t row, size_t column) const {
	return _board[row][column];
}


Board::Piece Board::getPiece(const Square& s) const {
	return getPiece(s.row, s.column);
}
