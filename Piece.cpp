#include "Board.h"

Board::Piece::Piece(Color c, Type t)
	:	_color(c), _type(t)
{}

Board::Piece::Piece()
	: _color(Color::White), _type(Type::Empty)
{}

Board::Piece::Color Board::Piece::getColor() const {
	return _color;
}

Board::Piece::Type Board::Piece::getType() const {
	return _type;
}
