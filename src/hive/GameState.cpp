#include "GameState.hpp"

namespace Hive {
    GameState::GameState() : currentPlayer(Player::PlayerColor::Red), pausedPlayer(Player::PlayerColor::Blue) {
        turn = 0;
    }

    GameState::GameState(GameState& gameState) : board(gameState.board), currentPlayer(gameState.GetCurrentPlayer()), pausedPlayer(gameState.GetPausedPlayer()), performedMoves(gameState.performedMoves) {
        turn = gameState.turn;
    }

    const Player::HivePlayer& GameState::GetCurrentPlayer() const {
        return currentPlayer;
    }

    const Player::HivePlayer& GameState::GetPausedPlayer() const {
        return pausedPlayer;
    }

    std::vector<Move::HiveMove> GameState::GetPossibleMoves() const {
    }

    bool GameState::IsMoveValid(const Move::HiveMove& move) const {
    }

    //Private

    std::vector<Move::HiveMove> GameState::GetPossibleDeployMoves() const {
        std::vector<Move::HiveMove> possibleDeployMoves;

        if (turn == 0) {
            std::vector<AxialPosition> emptyAxialPositions = board.GetEmptyAxialPositionsOnBoard();
            for (Piece::GamePiece undeployedGamePiece : currentPlayer.GetUndeployedGamePieces()) {
                for (AxialPosition emptyAxialPosition : emptyAxialPositions) {
                    possibleDeployMoves.push_back(Move::HiveMove(Move::MoveType::DeployMove, AxialPosition(0, 0), emptyAxialPosition));
                }
            }
        } else if (turn == 1) {
            std::vector<Piece::GamePieceStack> gameSpieceStacks = board.GetGamePieceStacks();
            std::vector<AxialPosition> neighbouringEmptyPositions = board.GetNeighbouringEmptyAxialPositions(gameSpieceStacks[0].GetAxialPosition());

            for (Piece::GamePiece undeployedGamePiece : currentPlayer.GetUndeployedGamePieces()) {
                for (AxialPosition neighbouringEmptyPosition : neighbouringEmptyPositions) {
                    possibleDeployMoves.push_back(Move::HiveMove(Move::MoveType::DeployMove, AxialPosition(0, 0), neighbouringEmptyPosition));
                }
            }
        } else {
            std::vector<AxialPosition> deployablePositions;

            std::vector<Piece::GamePieceStack> gameSpieceStacksOfCurrentPlayer = board.GetGamePieceStacksByColor(currentPlayer.GetPlayerColor());
            for(Piece::GamePieceStack gameSpieceStack : gameSpieceStacksOfCurrentPlayer) {
                std::vector<AxialPosition> neighbouringEmptyPositions = board.GetNeighbouringEmptyAxialPositions(gameSpieceStack.GetAxialPosition());
                for(AxialPosition neighbouringEmptyPosition : neighbouringEmptyPositions) {
                    std::vector<Piece::GamePieceStack> neighbouringStacks = board.GetNeighbouringGamePieceStacks(neighbouringEmptyPosition);
                    bool enemyStackNeighbouring = false;
                    for(Piece::GamePieceStack neighbouringStack : neighbouringStacks) {
                        if(neighbouringStack.GetGamePieceOnTop().GetCorrespondingPlayerColor() != currentPlayer.GetPlayerColor()) {
                            enemyStackNeighbouring = true;
                            break;
                        }
                    }
                    if(!enemyStackNeighbouring && std::find(deployablePositions.begin(), deployablePositions.end(), neighbouringEmptyPosition) == deployablePositions.end()) {
                        deployablePositions.push_back(neighbouringEmptyPosition);
                    }
                }
            }

            for(AxialPosition deployablePosition : deployablePositions) {
                possibleDeployMoves.push_back(Move::HiveMove(Move::MoveType::DeployMove, AxialPosition(0, 0), deployablePosition));
            }
        }
        return possibleDeployMoves;
    }
}  // namespace Hive