#include "OBJLoader.h"

using namespace std;

vector<Point *> OBJLoader::GetPoints()
{
    printf("PEGA PONTOS\N");
    return polyPoints;
}

OBJLoader::OBJLoader()
{
    ifstream infile("mk4.obj");

    string line;

    float x = 0;
    float y = 0;
    float z = 0;
    std::string::size_type sz;

    while (std::getline(infile, line))
    {
        string buf;
        stringstream ss(line);

        ss >> buf;

        if (buf == "v")
        {
            //std::cout << buf << "\n";
            char *end;
            ss >> buf;

            //buf.replace(1,1,",");
            for (int i = 0; i < buf.length(); ++i)
            {
                if (buf[i] == '.')
                {
                    buf[i] = ',';
                }
            }

            x = stof(buf);
            ss >> buf;
            y = stof(buf);
            ss >> buf;
            z = stof(buf);
            polyPoints.push_back(new Point(x, y, z));
        }
    }

}

OBJLoader::~OBJLoader()
{

}

