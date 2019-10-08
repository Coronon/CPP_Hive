#pragma once

#include <vector>

#include "Piece.hpp"
#include "AxialPosition.hpp"

namespace Hive {
    class PieceStack {
    private:
        std::vector<Piece> pieces;
        AxialPosition position;
    public:
        PieceStack(AxialPosition& position);

        const AxialPosition& GetAxialPosition() const;
        void AddPieceOnTop(Piece& piece);
        void RemovePieceOnTop();
        Piece& GetPieceByLayer(int layer);
        Piece& GetPieceOnTop();
        std::vector<Piece>& GetPieces();
    };
}  // namespace Hive::GamingPiece