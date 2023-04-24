#ifndef LAYER_H
#define LAYER_H


class Layer
{
    public:
        virtual void Render() = 0; //render layer..
        virtual void Update() = 0; //update layer map moi, trang thai moi..
};

#endif // LAYER_H
