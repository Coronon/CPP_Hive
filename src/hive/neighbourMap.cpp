#include "neighbourMap.hpp"

namespace Hive {
    NeighbourMap::NeighbourMap() {
        neighbourMap = std::vector<std::vector<std::vector<AxialPosition>>>();
        for (int i = 0; i < 11; i++) {
            neighbourMap.push_back(std::vector<std::vector<AxialPosition>>());
            for (int l = 0; l < 11; l++) {
                neighbourMap[i].push_back(std::vector<AxialPosition>());
            }
        }

        for (int x = -5; x < 6; x++) {
            for (int y = -5; y < 6; y++) {
                AxialPosition pos(x,y);
                neighbourMap[x + 5][y + 5] = pos.GetNeighbouringPositions();
            }
        }
    }

    std::vector<AxialPosition*> NeighbourMap::GetNeighbouringPositions(const AxialPosition& position) {
        std::vector<AxialPosition*> neighbouringPositions;
        neighbouringPositions.reserve(4);
        for(int i = 0; i < neighbourMap[position.x + 5][position.y + 5].size(); i++) {
            neighbouringPositions.push_back(&neighbourMap[position.x + 5][position.y + 5].at(i));
        }
        return neighbouringPositions;
    }

    std::vector<AxialPosition>& NeighbourMap::GetNeighbouringPositionsRef(const AxialPosition& position) {
        return neighbourMap[position.x + 5][position.y + 5];
    }
}  // namespace Hive