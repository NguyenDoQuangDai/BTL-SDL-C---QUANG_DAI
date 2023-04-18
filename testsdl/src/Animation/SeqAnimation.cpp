#include "SeqAnimation.h"
#include "TextureManager.h"
#include "tinyxml.h"

#include<iostream>

SeqAnimation::SeqAnimation(bool repeat): Animation(repeat) {};

void SeqAnimation::DrawFrame(float x, float y, float xScale, float yScale, SDL_RendererFlip flip)
{
    TextureManager::GetInstance()->Draw(m_CurrentSeq.TextureIDs[m_CurrentFrame], x, y, m_CurrentSeq.Width, m_CurrentSeq.Height, xScale, yScale, flip);
    //ve nhan vat voi cac tham so dc truyen
}

void SeqAnimation::Update(float dt)
{
    if(m_Repeat || !m_IsEnded) {
        m_IsEnded = false;
        m_CurrentFrame = (SDL_GetTicks()/m_CurrentSeq.Speed) % m_CurrentSeq.FrameCount;
    //Frame Sequence hien tai = (thoi gian tu dau / SPFS) % so Sequences cua sheet -> khi draw se thay doi repeat lien tuc
    //Speed la thoi gian Frame Sequence duoc xuat hien(Second Per FSequence)
    }

    if(!m_Repeat && m_CurrentFrame == (m_CurrentSeq.FrameCount - 1)) { //neu Frame hien tai laf frame cuoi
        m_IsEnded = true;
        m_CurrentFrame = (m_CurrentSeq.FrameCount - 1); // Do Frame Sequence bat dau tu 0
    }
}

void SeqAnimation::SetCurrentSeq(std::string seqID)
{
    if(m_SeqMap.count(seqID) > 0) {
        m_CurrentSeq = m_SeqMap[seqID];
        //check neu sequence co ton tai. Neu co thi gan vao CurrentSeq, neu ko thi chay else
    }
    else {
        std::cout << "The given sequence animation is not matching: " << seqID << std::endl;
    }
}

void SeqAnimation::Parse(std::string source)
{
    TiXmlDocument xml;
    xml.LoadFile(source); //load mo file xml
    if(xml.Error()) {
        std::cout << " Failed to load animation file: " << source << std::endl;  //check neu mo file loi
    }

/* VD:
    <animations>
    <sequence id="boss1_idle" frameCount="4" speed="100" width="128" height="96">
        <frame textureID ="idle-1"/>
        <frame textureID ="idle-2"/>
        ...
*/
    TiXmlElement* root = xml.RootElement();//lay root Element <animations>
    for(TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        //vong lap lay element cua child Element "sequence":
    if(e->Value() == std::string("sequence")) {
        Sequence seq; //su dung cac bien gia tri ben duoi de tao cac sequences
        std::string seqID = e->Attribute("id"); //lay thuoc tinh "id"
        e->Attribute("speed", &seq.Speed);//...
        e->Attribute("width", &seq.Width);
        e->Attribute("height", &seq.Height);
        e->Attribute("frameCount", &seq.FrameCount);//...

        for(TiXmlElement* frame = e->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement()) {
            seq.TextureIDs.push_back(frame->Attribute("textureID"));
            //vong lap lay cac element frame cua child Element "textureID" va day vao mang vector TextureIDs
            }
        m_SeqMap[seqID] = seq; //vd: "idle-1", "idle-2"...
        }
    }
}


