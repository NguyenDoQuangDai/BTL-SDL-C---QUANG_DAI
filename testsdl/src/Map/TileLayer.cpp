#include "TileLayer.h"
#include "TextureManager.h"
#include "Camera.h"

TileLayer::TileLayer(int tilesize, int rowcount, int colcount, TileMap tilemap, TilesetList tilesets) :
m_TileSize(tilesize), m_ColCount(colcount), m_RowCount(rowcount), m_Tilemap(tilemap), m_Tilesets(tilesets) //khoi tao cac thanh phan
{
        for (unsigned int i = 0; i < m_Tilesets.size(); i++) {
            TextureManager::GetInstance()->Load(m_Tilesets[i].Name, "assets/maps/" + m_Tilesets[i].Source);
             //loop goi ham load trong TextureManager voi name id cua tileset va filename tileset
        }
}

void TileLayer::Render()
{
	for(unsigned int i = 0; i < m_RowCount; i++) {
		for(unsigned int j = 0; j < m_ColCount; j++) {
			int tileID = m_Tilemap[i][j]; //lay tileID trong tile map layer
			int temp = tileID;

			if(tileID == 0) {
				continue; //continue vong loop khi ko co gi
			}
			else {
				int index = NULL;
				if(m_Tilesets.size() > 1) {
					for(unsigned int k = 1; k < m_Tilesets.size(); k++) {
						if(tileID >= m_Tilesets[k].FirstID && tileID <= m_Tilesets[k].LastID) { //check id co nam trong tileset thu k ko
							//chuyen gia tri tileID trong file map ve gia tri tile id trong tileset thu k goc
							tileID = tileID + m_Tilesets[k].TileCount - m_Tilesets[k].LastID;
							// = tileID - m_Tilesets[k].FirstID + 1;
							index = k;
							break;

						}
					}
				}

                Tileset ts = m_Tilesets[index]; //gan ts = tileset thu k
                int tileRow = tileID / ts.ColCount; //co x dong(row), y cot(col) -> dong thu 0 -> x-1 co id tu (x*y)-y den x*y-1
                int tileCol = tileID - (tileRow * ts.ColCount) - 1; //id - x*y - 1 = cot thu 0 -> y-1 cua id

                //neu tile o cot cuoi cung
                if(tileID % ts.ColCount == 0) {
                    tileRow--;
                    tileCol = ts.ColCount - 1; //chuyen so row x, so col y ve row thu x-1, col thu y-1
                }
                TextureManager::GetInstance()->DrawTile(ts.Name, ts.TileSize, j * ts.TileSize, i * ts.TileSize, tileRow, tileCol);
                //goi ham TextureManager drawtile voi cac thong so da tinh cua ts
                //-> ve tile voi tileRow va tileCol trong tileset vao  hcn vi tri (j*32,i*32) nhu id trong ma tran tilemap
			}
		}
	}
}


void TileLayer::Update()
{

}
