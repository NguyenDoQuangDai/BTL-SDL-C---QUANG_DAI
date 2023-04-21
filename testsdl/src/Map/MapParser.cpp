#include "MapParser.h"
#include<iostream>

MapParser* MapParser::s_Instance = nullptr; //khoi tao instance, gan nullptr cho bien con tro static

bool MapParser::Load(std::string LevelX)
{   if(LevelX == "Level1")
        return Parse("Level1", "assets/maps/map3.xml"); //map3 -> id level1 se duoc them vao ham Parse trong MapDictionary (xem header file)
    else if(LevelX == "Level2")
        return Parse("Level2", "assets/maps/map2-extended.xml");
}

bool MapParser::Parse(std::string id, std::string source) //ptich tileset
{
    TiXmlDocument xml;
    xml.LoadFile(source); //tao file xml document va load no

    if(xml.Error()) {
        std::cerr << "Failed to load: " << source << std::endl; //neu load file xml gap loi
        return false;
    }

    //<map version="1.10" tiledversion="1.10.0" orientation="orthogonal" renderorder="right-down" width="100" height="20" tilewidth="32" tileheight="32" infinite="0" nextlayerid="6" nextobjectid="1">
    TiXmlElement* root = xml.RootElement(); //root Element la Element dau tien cua xml file. Vd: "map"

    int rowcount, colcount, tilesize = 0;
    root->Attribute("width", &colcount); //lay thuoc tinh(attribute) "width" trong root element map. Vd: width="100"
    root->Attribute("height", &rowcount); //...
    root->Attribute("tilewidth", &tilesize); //...  ko lay tileheight vi kich thuoc tile dang dung la 32x32, tilewidth=tileheight=32

    //Parse Tilesets
    //Vd:  <tileset firstgid="1" name="Jungle_terrain" tilewidth="32" tileheight="32" tilecount="420" columns="28">
    TilesetList tilesets; //tao thanh phan tileset (component)
    for(TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("tileset")) {
            tilesets.push_back(ParseTileset(e)); //loop check child element co la "tileset" -> push return ParseTileset vao list cac tilesets
        }
    }

    //Parse Layers: giong parse Tilesets
    //Vd: <layer id="5" name="Tile Layer 5" width="100" height="20">
    GameMap* gamemap = new GameMap(); //map layers
    for(TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("layer")) {
            TileLayer* tilelayer = ParseTileLayer(e, tilesets, tilesize, rowcount, colcount); //tao tilelayer va goi ham ParseTileLayer
            gamemap->m_MapLayers.push_back(tilelayer); //pushback de pass tilelayers vao map va cung cap gia tri cua m_MapLayers cho gamemap
        }
    }

    m_MapDict[id] = gamemap;
    return true;
}

Tileset MapParser::ParseTileset(TiXmlElement* xmlTileset)
{
    Tileset tileset; //tao tileset de lay gia tri value tu xml

    //thuoc tinh(attribute) cua xml.element(yeu to)
    //Attribute cho phep nha cung cap compiler xac dinh cac attributes(dc xac dinh trong tieu chuan) trong 1 khong gian ten danh rieng cho nha cung cap

    //vd: <tileset firstgid="1" name="Jungle_terrain" tilewidth="32" tileheight="32" tilecount="420" columns="28">

    tileset.Name = xmlTileset->Attribute("name"); //lay name, vd: "Jungle_terrain" dang string
    xmlTileset->Attribute("firstgid", &tileset.FirstID); //lay thuoc tinh(attribute) "firstgid" vd: 1 dang so khac dang string

    xmlTileset->Attribute("tilecount", &tileset.TileCount); //lay thuoc tinh "tilecount", vd: 420
    tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1; //Id dau + so tile cua tileset - 1 = Id cuoi //xem int TileCount trong Tilelayer.h

    xmlTileset->Attribute("columns", &tileset.ColCount);
    tileset.RowCount = tileset.TileCount / tileset.ColCount; //...
    xmlTileset->Attribute("tilewidth", &tileset.TileSize);


    //vd: <image source="Jungle_terrain.png" width="896" height="480"/>

    TiXmlElement* image = xmlTileset->FirstChildElement(); //tao thanh phan image hay yeu to image xml
    tileset.Source = image->Attribute("source"); //lay thanh phan image, vd: "Jungle_terrain.png"
    return tileset; //tra ve tileset

}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount)
{
    TiXmlElement* data = nullptr; //tao xmlElement data
    // <data encoding="csv">
    for(TiXmlElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("data")) {
        //loop check neu ten cua thanh phan ma e dang giu bang voi data" -> lay Element do va gan cho "data"
            data = e;
            break;
        }
    }
    std::string matrix(data->GetText()); //lay matrix tra ve ket qua dang string bang sd ham GetText()
    std::stringstream iss(matrix); //stringstream iss(): bien chuoi line thanh input stream
    std::string id; //gia tri value that su cua tile tren map

    TileMap tilemap(rowcount, std::vector<int>(colcount, 0)); //tao ma tran mang 2 chieu tilemap

    for(int row = 0; row < rowcount; row++) {
        for(int col = 0; col < colcount; col++) {
            std::getline(iss, id, ','); //getline: sd de doc 1 line tu input stream iss. Moi khi gap dau ',' thi check gia tri va dat vao "id"
            std::stringstream convertor(id); //lay gia tri value tu "id" va chuyen doi
            convertor >> tilemap[row][col]; //gan value dc chuyen doi ve dang int vao tilemap int (file TileLayer.h)

            if(!iss.good()) {
                break; //neu den het file -> break
            }
        }
    }
    return(new TileLayer(tilesize, rowcount, colcount, tilemap, tilesets)); //return TileLayer voi du cac thong so can thiet da lay
}

void MapParser::Clean()
{
	std::map<std::string, GameMap*>::iterator it;
    //iterator la doi tuong co the tro den dia chi bo nho cua mang map ko quan tam trat tu
	for(it = m_MapDict.begin(); it != m_MapDict.end(); it++) {
		it->second = nullptr; //loop de xoa moi gia tri
	}

	m_MapDict.clear();
    // clear:xoa moi gia tri value va bien size ve 0

    std::cout << "Map cleared!" << std::endl;
}
