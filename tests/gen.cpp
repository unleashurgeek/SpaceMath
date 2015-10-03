#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct vProxy
{
    vProxy() : vType("vec2<T>"), A(-1), B(-1), C(-1), D(-1) { }
    std::string vType;
    int A;
    int B;
    int C;
    int D;
};

std::vector<vProxy> vecGen(int size)
{
    std::vector<vProxy> output;
    // xy, rg, st
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            vProxy temp;
            temp.vType = "vec2<T>";
            temp.A = i;
            temp.B = j;
            output.push_back(temp);
        }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
            {
                vProxy temp;
                temp.vType = "vec3<T>";
                temp.A = i;
                temp.B = j;
                temp.C = k;
                output.push_back(temp);
            }
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
                for (int l = 0; l < size; l++)
                {
                    vProxy temp;
                    temp.vType = "vec4<T>";
                    temp.A = i;
                    temp.B = j;
                    temp.C = k;
                    temp.D = l;
                    output.push_back(temp);
                }
    return output;
}

void printVProxys(std::vector<vProxy> data)
{
    std::string xyza = "xyzw";
    std::string rgba = "rgba";
    std::string stpq = "stpq";
    std::ofstream o("log.txt");

    for (int i = 0; i != data.size(); i++)
    {
        o << "vProxy<" << data[i].vType << ",T," << data[i].A << ","
            << data[i].B << "," << data[i].C << ","
            << data[i].D << "> " <<
            ((data[i].A != -1) ? xyza[data[i].A] : (char)0) <<
            ((data[i].B != -1) ? xyza[data[i].B] : (char)0) <<
            ((data[i].C != -1) ? xyza[data[i].C] : (char)0) <<
            ((data[i].D != -1) ? xyza[data[i].D] : (char)0) <<
            ", " <<
            ((data[i].A != -1) ? rgba[data[i].A] : (char)0) <<
            ((data[i].B != -1) ? rgba[data[i].B] : (char)0) <<
            ((data[i].C != -1) ? rgba[data[i].C] : (char)0) <<
            ((data[i].D != -1) ? rgba[data[i].D] : (char)0) <<
            ", " <<
            ((data[i].A != -1) ? stpq[data[i].A] : (char)0) <<
            ((data[i].B != -1) ? stpq[data[i].B] : (char)0) <<
            ((data[i].C != -1) ? stpq[data[i].C] : (char)0) <<
            ((data[i].D != -1) ? stpq[data[i].D] : (char)0) <<
            ";" << std::endl;
    }
}

int main()
{
    
    printVProxys(vecGen(4));
    std::cin.get();
    return 0;
}