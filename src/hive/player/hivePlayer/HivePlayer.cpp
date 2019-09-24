#include "HivePlayer.hpp"

namespace Hive::Player {
    HivePlayer::HivePlayer(PlayerColor playerColor) {
        this->playerColor = playerColor;
        this->undeployedGamingPieces = std::vector<Piece::GamePiece>();
        InitializeUndeployedGamePieces();
    }

    Piece::PieceColor HivePlayer::GetPieceColorFromPlayerColor() {
        if(playerColor == PlayerColor::Red) {
            return Piece::PieceColor::Red;
        } else {
            return Piece::PieceColor::Blue;
        }
    }

    void HivePlayer::InitializeUndeployedGamePieces() {
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::QueenBee, GetPieceColorFromPlayerColor()));

        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Spider, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Spider, GetPieceColorFromPlayerColor()));

        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Beetle, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Beetle, GetPieceColorFromPlayerColor()));

        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Grasshopper, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Grasshopper, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Grasshopper, GetPieceColorFromPlayerColor()));

        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Ant, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Ant, GetPieceColorFromPlayerColor()));
        undeployedGamingPieces.push_back(Piece::GamePiece(Piece::PieceType::Ant, GetPieceColorFromPlayerColor()));
    }
}  // namespace Hive