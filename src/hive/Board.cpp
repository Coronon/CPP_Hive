#include "Board.hpp"

namespace Hive {
    //Public Functions

    Board::Board() {
        pieceStacks = std::unordered_map<int, PieceStack>();
        //PlaceObstacles();
    }

    bool Board::PieceStackExists(const AxialPosition& position) const {
        //For unordered_map
        int hashValue = position.GetHashValue();
        if (pieceStacks.find(hashValue) == pieceStacks.end()) {
            return false;
        } else {
            return true;
        }
    }

    bool Board::PieceStackExists(int x, int y) const {
        //For unordered_map
        AxialPosition position(x, y);
        return PieceStackExists(position);
    }

    PieceStack Board::GetPieceStack(const AxialPosition& position) const {
        PieceStack pieceStack = pieceStacks.at(position.GetHashValue());
        return pieceStack;
    }

    PieceStack Board::GetPieceStack(int x, int y) const {
        AxialPosition position(x, y);
        return GetPieceStack(position);
    }

    std::vector<PieceStack> Board::GetPieceStacks() const {
        std::vector<PieceStack> stacks;
        for (std::pair<int, PieceStack> stackPair : pieceStacks) {
            stacks.push_back(stackPair.second);
        }
        return stacks;
    }

    std::vector<PieceStack> Board::GetPieceStacksWithoutObstacles() const {
        std::vector<PieceStack> stacks;
        for (std::pair<int, PieceStack> stackPair : pieceStacks) {
            if(stackPair.second.GetPieceOnTop().GetType() != PieceType::Obstacle) {
                stacks.push_back(stackPair.second);
            }
        }
        return stacks;
    }

    std::vector<PieceStack> Board::GetPieceStacksByColor(Color color) const {
        std::vector<PieceStack> stacks;
        for (std::pair<int, PieceStack> stackPair : pieceStacks) {
            if (stackPair.second.GetPieceOnTop().GetColor() == color) {
                stacks.push_back(stackPair.second);
            }
        }
        return stacks;
    }

    std::vector<PieceStack> Board::GetPieceStacksByColorAndType(Color color, PieceType pieceType) const {
        std::vector<PieceStack> stacks;
        for (std::pair<int, PieceStack> stackPair : pieceStacks) {
            if (stackPair.second.GetPieceOnTop().GetColor() == color && stackPair.second.GetPieceOnTop().GetType() == pieceType) {
                stacks.push_back(stackPair.second);
            }
        }
        return stacks;
    }

    Piece Board::GetPiece(const AxialPosition& position, int layer) const {
        //For unordered_map
        return GetPieceStack(position).GetPieceByLayer(layer);
    }

    Piece Board::GetPiece(int x, int y, int layer) const {
        AxialPosition position(x, y);
        return GetPiece(position, layer);
    }

    std::vector<PieceStack> Board::GetNeighbouringPieceStacks(const AxialPosition& position) const {
        //For unordered_map
        std::vector<PieceStack> neighbouringPieceStacks;
        std::vector<AxialPosition> neighbouringPositions = position.GetNeighbouringPositions();

        for (AxialPosition neighbouringPosition : neighbouringPositions) {
            if (PieceStackExists(neighbouringPosition)) {
                neighbouringPieceStacks.push_back(GetPieceStack(neighbouringPosition));
            }
        }
        return neighbouringPieceStacks;
    }

    std::vector<PieceStack> Board::GetNeighbouringPieceStacksExceptObstacles(const AxialPosition& position) const {
        //For unordered_map
        std::vector<PieceStack> neighbouringPieceStacks;
        std::vector<AxialPosition> neighbouringPositions = position.GetNeighbouringPositions();

        for (AxialPosition neighbouringPosition : neighbouringPositions) {
            if (PieceStackExists(neighbouringPosition) && GetPieceStack(neighbouringPosition).GetPieceOnTop().GetType() != PieceType::Obstacle) {
                neighbouringPieceStacks.push_back(GetPieceStack(neighbouringPosition));
            }
        }
        return neighbouringPieceStacks;
    }

    std::vector<AxialPosition> Board::GetEmptyNeighbouringAxialPositions(const AxialPosition& position) const {
        std::vector<AxialPosition> neighbouringPositions;
        neighbouringPositions = position.GetNeighbouringPositions();
        for (int i = 0; i < neighbouringPositions.size(); i++) {
            if (PieceStackExists(neighbouringPositions[i])) {
                neighbouringPositions.erase(neighbouringPositions.begin() + i);
            }
        }
        return neighbouringPositions;
    }

    std::vector<AxialPosition> Board::GetEmptySlideableNeighbouringAxialPositions(const AxialPosition& position) const {
        std::vector<AxialPosition> emptyNeighbouringPositions = GetEmptyNeighbouringAxialPositions(position);
        for (int i = 0; i < emptyNeighbouringPositions.size(); i++) {
            if (!CanSlide(position, emptyNeighbouringPositions[i])) {
                emptyNeighbouringPositions.erase(emptyNeighbouringPositions.begin() + i);
            }
        }
        return emptyNeighbouringPositions;
    }

    std::vector<AxialPosition> Board::GetEmptySlideableNeighbouringAxialPositionsExcept(const AxialPosition& position, const std::vector<AxialPosition>& ignoredPositions) const {
        std::vector<AxialPosition> emptyNeighbouringSlideablePosition = GetEmptySlideableNeighbouringAxialPositions(position);
        for (int i = 0; i < emptyNeighbouringSlideablePosition.size(); i++) {
            for (AxialPosition ignoredPosition : ignoredPositions) {
                if (emptyNeighbouringSlideablePosition[i] == ignoredPosition) {
                    emptyNeighbouringSlideablePosition.erase(emptyNeighbouringSlideablePosition.begin() + i);
                }
            }
        }
        return emptyNeighbouringSlideablePosition;
    }

    void Board::AddPieceOnTop(Piece& piece, AxialPosition& position) {
        if (PieceStackExists(position)) {
            GetPieceStack(position).AddPieceOnTop(piece);
        } else {
            PieceStack newPieceStack = PieceStack(position);
            newPieceStack.AddPieceOnTop(piece);
            pieceStacks.insert({position.GetHashValue(), newPieceStack});
        }
    }

    void Board::RemoveUpmostPiece(AxialPosition& position) {
        GetPieceStack(position).RemovePieceOnTop();
    }

    bool Board::IsAxialPositionAtBorderOfBoard(AxialPosition& position) const {
        int z = 0 - position.x - position.y;
        if (std::abs(position.x) + std::abs(position.y) + std::abs(z) == 10) {
            return true;
        } else {
            return false;
        }
    }

    bool Board::IsAxialPositionOnBoard(AxialPosition& position) const {
        int z = 0 - position.x - position.y;
        if (std::abs(position.x) + std::abs(position.y) + std::abs(z) <= 10) {
            return true;
        } else {
            return false;
        }
    }

    std::vector<AxialPosition> Board::GetEmptyAxialPositionsOnBoard() const {
        std::vector<AxialPosition> emptyAxialPositions;
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                AxialPosition possiblyEmptyPositionOnBoard = AxialPosition(x, y);
                if (IsAxialPositionOnBoard(possiblyEmptyPositionOnBoard) && !PieceStackExists(possiblyEmptyPositionOnBoard)) {
                    emptyAxialPositions.push_back(possiblyEmptyPositionOnBoard);
                }
            }
        }
        //std::cout << emptyAxialPositions.size() << std::endl;
        return emptyAxialPositions;
    }

    bool Board::IsHiveCoherentIfPieceMovesFromPosition(const AxialPosition& position) const {
        int hiveSize = GetCoherentHiveSize(position);
        if (GetCoherentHiveSize(position) == pieceStacks.size() - 1 - 3) {
            return true;
        } else {
            return false;
        }
    }

    bool Board::CanSlide(const AxialPosition& slideStartPos, const AxialPosition& slideEndPos) const {
        if (!slideStartPos.IsNeighbourTo(slideEndPos)) {
            return false;
        }

        int slideDirection = GetDirectionOfNeighbouringPositions(slideStartPos, slideEndPos);
        std::vector<PieceStack> neighbouringPieceStacks = GetNeighbouringPieceStacks(slideStartPos);

        bool slideBlockedFromRight = false;
        bool slideBlockedFromLeft = false;
        for (PieceStack neighbouringPieceStack : neighbouringPieceStacks) {
            int directionToNeighbour = GetDirectionOfNeighbouringPositions(slideStartPos, neighbouringPieceStack.GetAxialPosition());
            if (directionToNeighbour == slideDirection - 1 || (slideDirection == 0 && directionToNeighbour == 6)) {
                slideBlockedFromLeft = true;
            } else if (directionToNeighbour == slideDirection + 1 || (slideDirection == 6 && directionToNeighbour == 0)) {
                slideBlockedFromRight = true;
            }
        }

        if (slideBlockedFromLeft && slideBlockedFromRight) {
            return false;
        } else {
            std::vector<PieceStack> neighbouringPieceStacksAfterSlide = GetNeighbouringPieceStacks(slideEndPos);
            bool startAndEndHaveCommonHiveNeighbour = false;

            for (PieceStack neighbouringPieceStack : neighbouringPieceStacks) {
                for (PieceStack neighbouringPieceStackAfterSlide : neighbouringPieceStacksAfterSlide) {
                    if (neighbouringPieceStack.GetAxialPosition() == neighbouringPieceStackAfterSlide.GetAxialPosition() && neighbouringPieceStack.GetPieceOnTop().GetType() != PieceType::Obstacle) {
                        startAndEndHaveCommonHiveNeighbour = true;
                    }
                }
            }
            if (startAndEndHaveCommonHiveNeighbour) {
                return true;
            } else {
                return false;
            }
        }
    }

    //Private Functions

    void Board::PlaceObstacles() {
        Util::RandomNumberGenerator randomNumberGenerator;
        std::vector<AxialPosition> obstaclePositions;

        while (obstaclePositions.size() < 3) {
            int randXPos = randomNumberGenerator.GetRandomInt(-5, 5);
            int randYPos = randomNumberGenerator.GetRandomInt(-5, 5);
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

    int Board::GetCoherentHiveSize() const {
        std::unordered_map<int, PieceStack> hive;
        std::vector<PieceStack> pieceStacks = GetPieceStacksWithoutObstacles();

        if (pieceStacks.size() == 0) {
            return 0;
        }

        PieceStack startStack = pieceStacks[0];
        std::vector<PieceStack> pieceStacksToSearch;
        pieceStacksToSearch.push_back(startStack);

        while (!pieceStacksToSearch.empty()) {
            std::vector<PieceStack> neighbouringPieceStacks = GetNeighbouringPieceStacks(pieceStacksToSearch[0].GetAxialPosition());
            for (PieceStack neighbouringPieceStack : neighbouringPieceStacks) {
                if (hive.find(neighbouringPieceStack.GetAxialPosition().GetHashValue()) == hive.end()) {
                    hive.insert({neighbouringPieceStack.GetAxialPosition().GetHashValue(), neighbouringPieceStack});
                    pieceStacksToSearch.push_back(neighbouringPieceStack);
                }
            }
            pieceStacksToSearch.erase(pieceStacksToSearch.begin());
        }
        return hive.size();
    }

    int Board::GetCoherentHiveSize(const AxialPosition& ignorePosition) const {
        std::unordered_map<int, PieceStack> hive;
        std::vector<PieceStack> pieceStacks = GetPieceStacksWithoutObstacles();

        if (pieceStacks.size() == 0) {
            return 0;
        }

        PieceStack startStack = pieceStacks[0];
        if(startStack.GetAxialPosition() == ignorePosition) {
            startStack = pieceStacks[1];
        }
        std::deque<PieceStack> pieceStacksToSearch;
        pieceStacksToSearch.push_back(startStack);

        while (!pieceStacksToSearch.empty()) {
            std::vector<PieceStack> neighbouringPieceStacks = GetNeighbouringPieceStacksExceptObstacles(pieceStacksToSearch[0].GetAxialPosition());
            for (PieceStack neighbouringPieceStack : neighbouringPieceStacks) {
                if (neighbouringPieceStack.GetAxialPosition() == ignorePosition) {
                    continue;
                }
                if (hive.find(neighbouringPieceStack.GetAxialPosition().GetHashValue()) == hive.end()) {
                    hive.insert({neighbouringPieceStack.GetAxialPosition().GetHashValue(), neighbouringPieceStack});
                    pieceStacksToSearch.push_back(neighbouringPieceStack);
                }
            }
            pieceStacksToSearch.pop_front();
        }
        return hive.size();
    }

    int Board::GetDirectionOfNeighbouringPositions(const AxialPosition& startPos, const AxialPosition& destinationPos) const {
        if (startPos.GetDistanceTo(destinationPos) != 1) {
            throw "Error. Unable to get direction between two positions that are not neighbours";
        }
        AxialPosition axialDirection = destinationPos.Subtract(startPos);
        if (AxialPosition(1, -1) == axialDirection) {
            return 0;
        } else if (AxialPosition(1, 0) == axialDirection) {
            return 1;
        } else if (AxialPosition(0, 1) == axialDirection) {
            return 2;
        } else if (AxialPosition(-1, 1) == axialDirection) {
            return 3;
        } else if (AxialPosition(-1, 0) == axialDirection) {
            return 4;
        } else if (AxialPosition(0, -1) == axialDirection) {
            return 5;
        } else {
            return -1;
        }
    }
}  // namespace Hive
