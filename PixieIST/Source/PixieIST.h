//*** PixieIST.h ***

#ifndef __PixieIST_H__
#define __PixieIST_H__

class PixieIST
{
public:
    static const char* version;

    PixieIST();
    ~PixieIST();

    void Main(const char* commandLineString);

private:

    void DisplayProgramUsage();

};

#endif /* __PixieIST_H__ */
