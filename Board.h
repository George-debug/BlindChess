#pragma once

#include <vector>
#include <string>

	//I know its not 100% OOP
	//! Chess Board Class
	class Board {
	public:

		//! Board Status
		enum class Status : size_t {
			WhiteWon,
			BlackWon,
			PromotionNeeded,
			StaleMate,
			Draw,
			Live
		};

		//! Square Struct
		//! 
		//! Ex:
		//! a8 = Square(0, 0)
		//! h1 = Square(7, 7)
		struct Square {
			size_t row, column;

			Square(size_t r, size_t c);

			bool operator==(const Square& b);
		};

		//! Move Struct
		struct Move {
			Square from, to;

			Move(const Square& f, const Square& t);
		};

		class Piece {

		public:
			enum class Color : bool {
				White = true,
				Black = false
			};

			enum class Type : char {
				Knight = 'N',
				Rook = 'R',
				Pawn = 'P',
				Queen = 'Q',
				King = 'K',
				Bishop = 'B',
				Empty = '.'
			};

		private:
			Color _color;
			Type _type;

		public:
			Piece();
			Piece(Color c, Type t);

			Color getColor() const;
			Type getType() const;
		};

	private:
		Piece _board[8][8];
		Status _status;
		Piece::Color _turnColor;

		//If you dont need them in any translation unit :) just move them to Board.cpp (all private static)
		[[nodiscard]] static bool isValidSquare(size_t r, size_t c);
		[[nodiscard]] static bool isValidSquare(const Square& s);

		[[nodiscard]] Move translateChessNotation(const std::string& move) const;
		[[nodiscard]] static Move translateGoFromGoToNotation(const std::string& goFrom, const std::string& goTo); //a2 to g8

		[[nodiscard]] std::vector<Square> possibleAttackMovesFrom(const Square& s) const;
		//[[nodiscard]] std::vector<Square> possibleNonAttackMovesFrom(const Square& s) const;
		
		[[nodiscard]] std::vector<Square> possibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type) ) const;

		//PiecePossibleMovesFromIf
		//Imagine is a Piece "p" on Square "s", return all possibleMovesFromIf
		[[nodiscard]] std::vector<Square> pawnPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const;
		[[nodiscard]] std::vector<Square> rookPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const;
		[[nodiscard]] std::vector<Square> bishopPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const;
		[[nodiscard]] std::vector<Square> queenPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const;
		[[nodiscard]] std::vector<Square> knightPossibleMovesFromIf(const Square& s, bool(*addIf)(Piece::Type)) const;

	public:
		Board();
		[[nodiscard]] std::vector<Square> possibleMovesFrom(const Square& s) const;
		[[nodiscard]] std::vector<Square> possibleMovesFrom(size_t r, size_t c) const;
		bool move(size_t r1, size_t c1, size_t r2, size_t c2);
		bool move(const Square& from, const Square& to);
		bool move(const Move& m);
		bool move(const std::string& move); //chess Notation
		bool move(const std::string& from, const std::string& to); //ex: "a2", "g8"
		bool promoteTo(Piece::Type t);
		Piece getPiece(const Square& s) const;
		Piece getPiece(size_t row, size_t column) const;

		[[nodiscard]] Piece::Color getTurnColor() const;
		[[nodiscard]] Status getStatus() const;
		[[nodiscard]] std::vector< std::vector<Piece>> getBoard() const;
		[[nodiscard]] bool isFinished() const;
	};