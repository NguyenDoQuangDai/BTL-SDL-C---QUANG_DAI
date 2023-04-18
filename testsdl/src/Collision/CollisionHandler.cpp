#include "CollisionHandler.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler()
{
    m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetLayers().back(); //lay layer va cham
    m_CollisionTilemap = m_CollisionLayer->GetTileMap(); //load lay tile map trong layer va cham cho tilemap va cham
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b)
{
    bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
    bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);

    //check neu 2 object rectangle chong len tren/xuong duoi(nam tren/nam duoi), neu co thi return

    return (x_overlaps && y_overlaps);
}

bool CollisionHandler::MapCollision(SDL_Rect a) //check neu object chong de len tren/o duoi map
{
    int tileSize = 32;
    int RowCount = 20;
    int ColCount = 100;

    int left_tile = a.x / tileSize;
    int right_tile = (a.x + a.w) / tileSize;

    int top_tile = a.y / tileSize;
    int bottom_tile = (a.y + a.h) / tileSize;
    // lay  gia tri trc, sau, tren, duoi  tile

    if (left_tile < 0) {left_tile = 0;}
    if (right_tile > ColCount) {right_tile = ColCount;}

    if (top_tile < 0) {top_tile = 0;}
    if (bottom_tile > RowCount) {bottom_tile = RowCount;}
    //check dang trc, sau, tren, duoi  tile ko vuot qua (0,0) va (ColCount,RowCount)
    //bo di cung dc vi test chua thay loi nao ca

    for (int i = left_tile; i <= right_tile; i++) {
        for (int j = top_tile; j <= bottom_tile; j++) {
            if (m_CollisionTilemap[j][i] > 0) {
                return true; //neu tileid >0 thi nhan vat co the va cham voi tilemap vc
            }
        }
    }

    return false;
}

