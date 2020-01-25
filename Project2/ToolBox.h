#pragma once
#include <string>
#include "MovementC.h"

class EnumUtil {
public:
#pragma region MovementType
	static string MovementTypeToString(int type) {
		if (type == MovementType::Auto) {
			return "Auto";
		}
		else if (type == MovementType::Keyboard) {
			return "Keyboard";
		}

		return "None";
	}
	static MovementType StringToMovementType(std::string enumName) {
		if ((enumName == "Auto") == 1) {
			return MovementType::Auto;
		}
		else if ((enumName == "Keyboard") == 1) {
			return MovementType::Keyboard;
		}

		return MovementType::None;
	}
#pragma endregion

};

class ToolBox {
public:
	static std::vector<std::string> split(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
};
