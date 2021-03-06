#include "board.hpp"

namespace Hive {
    Board::Board() {
        for (int x = -5; x < 6; x++) {
            for (int y = -5; y < 6; y++) {
                pieceStacks[(x + 5) * 11 + (y + 5)] = PieceStack(AxialPosition(x, y));
            }
        }
        emptySet = std::bitset<121>();
    }

    Board::Board(const Board& board) : pieceStacks(board.pieceStacks), emptySet(board.emptySet) {}

    bool Board::PieceStackExists(const AxialPosition& position) {
        //return !pieceStacks[position.GetID()].IsStackEmpty();
        return emptySet[position.GetID()] == 1;
    }

    bool Board::PieceStackExists(int x, int y) {
        //return !pieceStacks[(x + 5) * 11 + (y + 5)].IsStackEmpty();
        return emptySet[(x + 5) * 11 + (y + 5)] == 1;
    }

    PieceStack& Board::GetPieceStack(const AxialPosition& position) {
        return pieceStacks[position.GetID()];
    }

    PieceStack& Board::GetPieceStack(int x, int y) {
        return pieceStacks[(x + 5) * 11 + (y + 5)];
    }

    std::vector<PieceStack*> Board::GetPieceStacks() {
        std::vector<PieceStack*> stacks;
        stacks.reserve(25);
        for (int i = 0; i < 121; i++) {
            if (emptySet[i] == 1) {
                stacks.emplace_back(&pieceStacks[i]);
            }
        }
        return stacks;
    }

    std::vector<PieceStack*> Board::GetPieceStacksWithoutObstacles() {
        std::vector<PieceStack*> stacks;
        stacks.reserve(22);
        for (int i = 0; i < 121; i++) {
            if (emptySet[i] == 1 && pieceStacks[i].GetPieceOnTop().GetType() != PieceType::Obstacle) {
                stacks.emplace_back(&pieceStacks[i]);
            }
        }
        return stacks;
    }

    std::vector<PieceStack*> Board::GetPieceStacksByColor(Color color) {
        std::vector<PieceStack*> stacks;
        stacks.reserve(11);
        for (int i = 0; i < 121; i++) {
            if (emptySet[i] == 1 && pieceStacks[i].GetPieceOnTop().GetColor() == color) {
                stacks.emplace_back(&pieceStacks[i]);
            }
        }
        return stacks;
    }

    std::vector<PieceStack*> Board::GetPieceStacksByColorAndType(Color color, PieceType pieceType) {
        std::vector<PieceStack*> stacks;
        stacks.reserve(3);
        for (int i = 0; i < 121; i++) {
            if (emptySet[i] == 1 && pieceStacks[i].GetPieceOnTop().GetColor() == color && pieceStacks[i].GetPieceOnTop().GetType() == pieceType) {
                stacks.emplace_back(&pieceStacks[i]);
            }
        }
        return stacks;
    }

    std::vector<PieceStack*> Board::GetPieceStacksContainingPieceByColorAndType(Color color, PieceType pieceType) {
        std::vector<PieceStack*> stacks;
        stacks.reserve(3);
        for (int i = 0; i < 121; i++) {
            for (Piece piece : pieceStacks[i].GetPieces()) {
                if (piece.GetColor() == color && piece.GetType() == pieceType) {
                    stacks.emplace_back(&pieceStacks[i]);
                    break;  //Not counting PieceStacks twice.
                }
            }
        }
        return stacks;
    }

    std::vector<PieceStack*> Board::GetNeighbouringPieceStacks(const AxialPosition& position) {
        std::vector<PieceStack*> neighbouringPieceStacks;
        neighbouringPieceStacks.reserve(6);

        for (AxialPosition neighbouringPosition : neighbourMap.GetNeighbouringPositionsRef(position)) {
            if (emptySet[neighbouringPosition.GetID()] == 1) {
                neighbouringPieceStacks.emplace_back(&GetPieceStack(neighbouringPosition));
            }
        }
        return neighbouringPieceStacks;
    }

    std::vector<PieceStack*> Board::GetNeighbouringPieceStacksExceptObstacles(const AxialPosition& position) {
        std::vector<PieceStack*> neighbouringPieceStacks;
        neighbouringPieceStacks.reserve(6);

        for (AxialPosition neighbouringPosition : neighbourMap.GetNeighbouringPositionsRef(position)) {
            if (PieceStackExists(neighbouringPosition) && GetPieceStack(neighbouringPosition).GetPieceOnTop().GetType() != PieceType::Obstacle) {
                neighbouringPieceStacks.emplace_back(&GetPieceStack(neighbouringPosition));
            }
        }
        return neighbouringPieceStacks;
    }

    std::vector<AxialPosition*> Board::GetEmptyNeighbouringAxialPositions(const AxialPosition& position) {
        std::vector<AxialPosition*> neighbouringPositions = neighbourMap.GetNeighbouringPositions(position);
        for (int i = neighbouringPositions.size() - 1; i >= 0; i--) {
            if (PieceStackExists(*neighbouringPositions[i])) {
                neighbouringPositions.erase(neighbouringPositions.begin() + i);
            }
        }
        return neighbouringPositions;
    }

    std::vector<AxialPosition*> Board::GetEmptySlideableNeighbouringAxialPositions(const AxialPosition& position) {
        std::vector<AxialPosition*> emptyNeighbouringPositions = GetEmptyNeighbouringAxialPositions(position);
        for (int i = emptyNeighbouringPositions.size() - 1; i >= 0; i--) {
            if (!CanSlide(position, *emptyNeighbouringPositions[i])) {
                emptyNeighbouringPositions.erase(emptyNeighbouringPositions.begin() + i);
            }
        }
        return emptyNeighbouringPositions;
    }

    std::vector<AxialPosition*> Board::GetEmptySlideableNeighbouringAxialPositionsExcept(const AxialPosition& position, const std::vector<AxialPosition>& ignoredPositions) {
        std::vector<AxialPosition*> emptyNeighbouringSlideablePositions = GetEmptySlideableNeighbouringAxialPositions(position);
        for (int i = emptyNeighbouringSlideablePositions.size() - 1; i >= 0; i--) {
            for (AxialPosition ignoredPosition : ignoredPositions) {
                auto iterator = std::find_if(emptyNeighbouringSlideablePositions.begin(), emptyNeighbouringSlideablePositions.end(), [&](AxialPosition* pos) { return pos->GetID() == ignoredPosition.GetID(); });
                if (iterator != emptyNeighbouringSlideablePositions.end()) {
                    emptyNeighbouringSlideablePositions.erase(iterator);
                }
            }
        }

        return emptyNeighbouringSlideablePositions;
    }

    void Board::AddPieceOnTop(const Piece& piece, const AxialPosition& position) {
        GetPieceStack(position).AddPieceOnTop(piece);
        emptySet[position.id] = 1;
    }

    void Board::RemoveUpmostPiece(const AxialPosition& position) {
        GetPieceStack(position).RemovePieceOnTop();
        if(GetPieceStack(position).IsStackEmpty()) {
            emptySet[position.id] = 0;
        }
    }

    std::vector<AxialPosition> Board::GetEmptyAxialPositionsOnBoard() {
        std::vector<AxialPosition> emptyAxialPositions;
        emptyAxialPositions.reserve(80);
        for (PieceStack pieceStack : pieceStacks) {
            if (pieceStack.IsStackEmpty() && pieceStack.GetAxialPosition().IsOnBoard()) {
                emptyAxialPositions.push_back(pieceStack.GetAxialPosition());
            }
        }
        return emptyAxialPositions;
    }

    bool Board::IsHiveCoherentIfPieceMovesFromPosition(const AxialPosition& position) {
        std::vector<Piece> pieces = GetPieceStack(position).GetPieces();
        if (pieces.size() > 1) {
            return true;
        }

        std::vector<Piece> obstacle = {Piece(PieceType::Obstacle, Color::Undefined)};
        GetPieceStack(position).SetPieces(obstacle);

        std::vector<PieceStack*> pieceStacks = GetPieceStacksWithoutObstacles();
        if (pieceStacks.size() == 1 || pieceStacks.size() == 0) {
            GetPieceStack(position).SetPieces(pieces);
            return true;
        }

        std::vector<PieceStack*> hive;
        hive.reserve(22);

        PieceStack* startStack = pieceStacks[0];

        hive.push_back(startStack);
        std::deque<PieceStack*> pieceStacksToSearch;
        pieceStacksToSearch.push_back(startStack);

        while (!pieceStacksToSearch.empty()) {
            std::vector<PieceStack*> neighbouringPieceStacks = GetNeighbouringPieceStacksExceptObstacles(pieceStacksToSearch[0]->GetAxialPosition());
            for (PieceStack* neighbouringPieceStack : neighbouringPieceStacks) {
                if (std::find(hive.begin(), hive.end(), neighbouringPieceStack) == hive.end()) {
                    hive.push_back(neighbouringPieceStack);
                    pieceStacksToSearch.push_back(neighbouringPieceStack);
                }
            }
            pieceStacksToSearch.pop_front();
        }

        GetPieceStack(position).SetPieces(pieces);
        if (hive.size() == pieceStacks.size()) {
            return true;
        }
        return false;
    }

    bool Board::CanSlide(const AxialPosition& slideStartPos, const AxialPosition& slideEndPos) {
        if (!slideStartPos.IsNeighbourTo(slideEndPos)) {
            return false;
        }

        std::vector<PieceStack*> neighbouringPieceStacksAtStart = GetNeighbouringPieceStacks(slideStartPos);
        std::vector<PieceStack*> neighbouringPieceStacksAtEnd = GetNeighbouringPieceStacks(slideEndPos);

        int commonNeighbourCount = 0;
        int commonNeighbourCountExcludingObstacles = 0;

        for (PieceStack* neighbouringPieceStackAtStart : neighbouringPieceStacksAtStart) {
            for (PieceStack* neighbouringPieceStackAtEnd : neighbouringPieceStacksAtEnd) {
                if (neighbouringPieceStackAtStart->GetAxialPosition() == neighbouringPieceStackAtEnd->GetAxialPosition() && neighbouringPieceStackAtEnd->GetAxialPosition() != slideStartPos && neighbouringPieceStackAtEnd->GetPieceOnTop().GetType() != PieceType::Undefined) {
                    commonNeighbourCount++;
                    if (neighbouringPieceStackAtStart->GetPieceOnTop().GetType() != PieceType::Obstacle) {
                        commonNeighbourCountExcludingObstacles++;
                    }
                }
            }
        }

        if (commonNeighbourCount < 2 && commonNeighbourCountExcludingObstacles > 0) {
            return true;
        } else {
            return false;
        }
    }

    void Board::PlaceObstacles() {
        srand(time(NULL));
        std::vector<AxialPosition> obstaclePositions;

        while (obstaclePositions.size() < 3) {
            int randXPos = rand() % 11 - 5;
            int randYPos = rand() % 11 - 5;
            AxialPosition obstaclePosition = AxialPosition(randXPos, randYPos);

            if (std::find(obstaclePositions.begin(), obstaclePositions.end(), obstaclePosition) == obstaclePositions.end()) {
                obstaclePositions.push_back(obstaclePosition);
            }
        }

        for (int i = 0; i < 3; i++) {
            Piece obstaclePiece(PieceType::Obstacle, Color::Undefined);
            AddPieceOnTop(obstaclePiece, obstaclePositions[i]);
        }
    }
}  // namespace Hive
