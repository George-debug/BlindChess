#include "Board.h"

Board::Move::Move(const Square& f, const Square& t)
	: from(f), to(t)
{}


Board::Square::Square(size_t r, size_t c)
	: row(r), column(c)
{}

bool Board::Square::operator==(const Square& b) {
	return row == b.row && column == b.column;
}