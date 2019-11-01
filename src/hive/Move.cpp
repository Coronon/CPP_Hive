#include "Move.hpp"

namespace Hive {
    Move::Move(const MoveType &moveType, const AxialPosition &startPosition, const AxialPosition &destinationPosition, const PieceType &movedPieceType) : startPosition(startPosition), destinationPosition(destinationPosition) {
        this->moveType = moveType;
        this->movedPieceType = movedPieceType;
    }

    MoveType Move::GetMoveType() const {
        return moveType;
    }

    PieceType Move::GetMovedPieceType() const {
        return movedPieceType;
    }

    AxialPosition Move::GetStartPosition() const {
        return startPosition;
    }

    AxialPosition Move::GetDestinationPosition() const {
        return destinationPosition;
    }
}  // namespace Hive