#include "Ek_i.h"

Ek::Ek()
{
}

Ek::~Ek()
{
}

void Ek::clear(){
    int pMaxN = size();
    for (int i = -pMaxN; i<=pMaxN; i++)
        for (int j = -pMaxN; j<=pMaxN; j++)
            for (int k = -pMaxN; k<=pMaxN; k++)
            {
                (*this)[i][j][k]=dcomp(0,0);
            }
}

int Ek::size(int pMaxN){
    dcomp3D::resize(pMaxN);
    for (int i = -pMaxN; i<=pMaxN; i++)
    {
        dcomp3D::operator[](i).resize(pMaxN);
        for (int j = -pMaxN; j<=pMaxN; j++)
        {
            dcomp3D::operator[](i)[j].resize(pMaxN);
        }
    }

    return pMaxN;
}

int Ek::size()
{
    return dcomp3D::size();
}

void Ek::print(std::ostream *ost){
    int eksize = dcomp3D::size();
    for(int i=-eksize; i<=eksize; i++)
        for(int j=-eksize; j<=eksize; j++)
            for(int k=-eksize; k<=eksize; k++)
                *ost << "Ek["<<i<<"]["<<j<<"]["<<k<<"]="<<flush<<(*this)[i][j][k]<<endl;
}
