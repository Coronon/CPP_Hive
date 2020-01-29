#pragma once

#include <string>

namespace Hive {
    /**
     * @brief All different types of Pieces are defined here.
     * 
     */
    enum class PieceType {
        QueenBee,
        Spider,
        Beetle,
        Grasshopper,
        Ant,
        Obstacle,
        Undefined,
    };

    /**
     * @brief An array that contains the values of the PieceType enum as strings.
     * 
     */
    static const std::string pieceTypeStrings[] = {"BEE", "SPIDER", "BEETLE", "GRASSHOPPER", "ANT", "OBSTACLE", "UNDEFINED"};

    /**
     * @brief Converts a PieceType to its string representation.
     * 
     * @param type The PieceType to convert.
     * @return The string representation of the PieceType.
     */
    std::string PieceTypeToString(Hive::PieceType type);

    /**
     * @brief Converts a string to a PieceType.
     * 
     * @param pieceTypeAsString The PieceType in its string representation.
     * @return The PieceType that corresponds to the string representation.
     */
    PieceType PieceTypeFromString(std::string pieceTypeAsString);
}  // namespace Hive