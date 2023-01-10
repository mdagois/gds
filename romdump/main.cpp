#include <cassert>
#include <iomanip>
#include <iostream>

using namespace std;

static bool loadFile(uint8_t*& out_data, uint32_t& out_data_size, const char* filename)
{
    assert(filename != nullptr);

    FILE* file = fopen(filename, "rb");
    if(file == nullptr)
    {
        cout << "Could not open file [" << filename << "] for read" << endl;
        return false;
    }

    fseek(file, 0, SEEK_END);
    const long int length = ftell(file);
    if(length <= 0)
    {
        cout << "File size if zero [" << filename << "]" << endl;
        fclose(file);
        return false;
    }
    fseek(file, 0, SEEK_SET);

    uint8_t* data = new uint8_t[length];
    if(data == nullptr)
    {
        cout << "Could not allocate enough memory [" << length << "] for [" << filename << "]" << endl;
        fclose(file);
        return false;
    }

    size_t n = fread(data, length, 1, file);
    fclose(file);
    if(n != 1)
    {
        cout << "Could not read the data for file [" << filename << "]" << endl;
        delete [] data;
        return false;
    }

    out_data = data;
    out_data_size = length;
    return true;
}

int main(int argc, const char** argv)
{
    if(argc < 2)
    {
        cout << "No ROM file specified" << endl;
        return 1;
    }

    const char* filename = argv[1];
    uint8_t* file_data;
    uint32_t file_data_size = 0;
    if(!loadFile(file_data, file_data_size, filename))
    {
        return 1;
    }

    cout << hex << setfill('0');
    constexpr uint8_t default_value = 0;
    for(uint32_t i = 0; i < file_data_size; ++i)
    {
        if(file_data[i] == default_value)
        {
            continue;
        }

        cout << "\t\tu16:0x" << setw(4) << i << " => u8:0x" << static_cast<uint32_t>(file_data[i]) << "," << endl;
    }
    return 0;
}

