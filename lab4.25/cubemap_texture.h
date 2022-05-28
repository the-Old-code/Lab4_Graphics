#ifndef CUBEMAP_H
#define	CUBEMAP_H

#include <string>
#include <GL/glew.h>
#include <Magick++.h>

using namespace std;

/*Этот класс включает в себя реализацию кубической текстуры и предоставляет простой интерфейс для ее загрузки и использования.
  Конструктор принимает директорию и 6 имен файлов, которые содержат стороны куба.*/

class CubemapTexture
{
public:
    
    CubemapTexture(const string& Directory,
                   const string& PosXFilename,
                   const string& NegXFilename,
                   const string& PosYFilename,
                   const string& NegYFilename,
                   const string& PosZFilename,
                   const string& NegZFilename);

    ~CubemapTexture();
    
    bool Load();

    void Bind(GLenum TextureUnit);

private:
   
    string m_fileNames[6];
    GLuint m_textureObj;
};

#endif	/* CUBEMAP_H */

