#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <iterator> 

#include "Components.h"

class ModelDataClass {
public:
    std::string path;
    float scale;
    float x, y, z;
    float rx, ry, rz;
    CollisionType colType = CollisionType::NoCollision;
};

class FileLoader {
public:
    static std::vector<ModelDataClass*> ReadModelData(string path) {
        std::vector<ModelDataClass*> list;

        std::string line;
        std::ifstream myfile(path);
        if (myfile.is_open())
        {
            while (getline(myfile, line))
            {
                if (line.rfind("//", 0) != 0 && line.rfind("#", 0) != 0) {
                    ModelDataClass* modelData = new ModelDataClass();
                    line = line += ":0";
                    std::vector<std::string> splited1 = Split(line, ":");
                    for (std::string str : splited1) {
                        str = str += "_0";

                        std::vector<std::string> splited2 = Split(str, "_");
                        std::string func = splited2.at(0);
                        float offset = -90.f;

                        if (func == "offset") { offset = std::stof(splited2.at(1)); }
                        else if (func == "path") {
                            modelData->path = splited2.at(1);
                        }
                        else if (func == "scale") {
                            modelData->scale = std::stof(splited2.at(1));
                        }
                        else if (func == "position") {
                            modelData->x = std::stof(splited2.at(1));
                            modelData->y = std::stof(splited2.at(3));
                            modelData->z = -std::stof(splited2.at(2));
                        }
                        else if (func == "rotation") {
                            modelData->rx = std::stof(splited2.at(1)) + offset;
                            modelData->ry = std::stof(splited2.at(3));
                            modelData->rz = -std::stof(splited2.at(2));
                        }
                        else if (func == "collision") {
                            if (splited2.at(splited2.size() - 1) == "RayCastingCollision") {
                                modelData->colType = CollisionType::RayCastingCollision;
                            } 
                            else if (splited2.at(splited2.size() - 1) == "SolidCollision") {
                                modelData->colType = CollisionType::SolidCollision;
                            }
                            else if (splited2.at(splited2.size() - 1) == "WorldCollision") {
                                modelData->colType = CollisionType::WorldCollision;
                            }
                            else {
                                modelData->colType = CollisionType::NoCollision;
                            }
                        }
                        else {
                            std::cout << func << std::endl;
                        }
                    }
                    list.push_back(modelData);
                }
            }
            myfile.close();
        }
        return list;
    }

    static vector<string> Split(string s, string delimiter) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> splited = vector<std::string>();

        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            splited.push_back(token);
            s.erase(0, pos + delimiter.length());
        }

        return splited;
    }
};