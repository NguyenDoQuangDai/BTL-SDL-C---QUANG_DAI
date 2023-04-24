#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Layer.h"

#include<vector>

class GameMap
{
    public:
        GameMap() {};

        void Render() {
            for(unsigned int i=0; i < m_MapLayers.size(); i++)
                m_MapLayers[i]->Render(); //render moi layers
        }

        void Update() {
            for(unsigned int i=0; i < m_MapLayers.size(); i++)
                m_MapLayers[i]->Update(); //update tat ca layer
        }

        std::vector<Layer*> GetLayers() {
            return m_MapLayers; //lay MapLayers can thiet
        }

    private:
        friend class MapParser;
        //class dc khai la friend class co the truy cap private vs protected members cua class khac ma trong class do no dc khai
        std::vector<Layer*> m_MapLayers; //dinh nghia khai bao member_MapLayers
};

#endif // GAMEMAP_H
