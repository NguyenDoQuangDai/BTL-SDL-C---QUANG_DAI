#ifndef IOBJECT_H
#define IOBJECT_H


class IObject   //interface class cho phep cac objects khac nhau chia se chung cn(function) nhung co the xu ly cn theo cach khac biet neu can
                //moi classes thua ke tu IObject can dinh nghia(define) hoac thuc thi cac phuong phap(method)
{
    public:
          //ham ao(virtual) la ham thanh vien dc khai bao trong lop co so va dc dinh nghia lai(ghi de) boi lop dan xuat,
            //dam bao rang ham chinh xac dc goicho moi doi tg Object bat ke loai tham chieu hoac con tro dc sd cho lenh goi ham

        //3 fuction ma moi object duoc ve tren man hinh can
        virtual void Draw()=0;
        virtual void Update(float dt)=0;// ve/don object
        virtual void Clean()=0;//
};

#endif // IOBJECT_H
