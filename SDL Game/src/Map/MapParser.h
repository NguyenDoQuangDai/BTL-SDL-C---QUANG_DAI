
#ifndef MAPPARSER_H
#define MAPPARSER_H

#include "GameMap.h"
#include "TileLayer.h"
#include "tinyxml.h"

#include<string>
#include<map>

class MapParser
{
    public:
        bool Load(std::string LevelX); //load map
        void Clean();

        inline GameMap* GetMap(std::string id) {return m_MapDict[id];}
        inline static MapParser* GetInstance() {return s_Instance = (s_Instance != nullptr)? s_Instance: new MapParser();}

    private:
        bool Parse(std::string id, std::string source); //muon phan tich cu phap cua map(parse map) phai huong anh xa vao map id va file name(source)
        Tileset ParseTileset(TiXmlElement* xmlTileset); //ptich cu phap cac tilesets, truyen vao ham cac thong tin tileset
                                                        //vd: <tileset firstgid="1" name="Jungle_terrain" tilewidth="32" tileheight="32" tilecount="420" columns="28">
        TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount);
        //ptich cu phap tilelayers


    private:
        //khoi tao cac static members
        MapParser() {}; //->private constructor
        static MapParser* s_Instance;
        std::map<std::string, GameMap*> m_MapDict;  // anh xa map
                                                    // string -> Id cua map
};

#endif // MAPPARSER_H
