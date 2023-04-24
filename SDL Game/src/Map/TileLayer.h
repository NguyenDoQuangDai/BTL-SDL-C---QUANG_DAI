#ifndef TILELAYER_H
#define TILELAYER_H

#include "Layer.h"

#include<vector>
#include<string>

struct Tileset{
    int FirstID, LastID; //first, last Id cua tileset
    int RowCount, ColCount; //so hang(row), cot(column)
    int TileCount, TileSize; //tilecount: so tile ma tileset co = FirstId - LastId + 1, tilesize: 32x32 hay 16x16
    std::string Name, Source; //ten va ten file cua tileset
};

using TilesetList = std::vector<Tileset>; //mang tileset
using TileMap = std::vector<std::vector<int> >; //mang 2 chieu lay cac gia tri tileId trong ma tran

class TileLayer: public Layer
{
    public:
        TileLayer(int tilesize, int rowcount, int colcount, TileMap tilemap, TilesetList tilesets); //khoi tao cac tham so cho chg tr con

        virtual void Render();
        virtual void Update();
        //ham ao la ham thanh vien dc khai bao trong lop co so va dc dinh nghia lai(ghi de) boi lop dan xuat,
        //dam bao rang ham chinh xac dc goicho moi doi tg Object bat ke loai tham chieu hoac con tro dc sd cho lenh goi ham

        inline TileMap GetTileMap() {return m_Tilemap;} //ham tra ve mang 2c cac tileID

    private:
        int m_TileSize;
        int m_RowCount, m_ColCount;

        TileMap m_Tilemap;
        TilesetList m_Tilesets;
};


#endif // TILELAYER_H
