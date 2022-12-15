#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

static bool LoadFile(uint8_t*& out_data, uint32_t& out_data_size, const char* filename)
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

////////////////////////////////////////////////////////////////////////////////

enum DataType : uint8_t
{
	NO_DATA,
	BIT_ARRAY,
	TWO_BYTE_SIGNED_INTEGER,
	FOUR_BYTE_SIGNED_INTEGER,
	FOUR_BYTE_REAL,
	EIGHT_BYTE_REAL,
	ASCII_STRING,

	DATA_TYPE_COUNT,
};

enum RecordType : uint8_t
{
	HEADER,
	BGNLIB,
	LIBNAME,
	UNITS,
	ENDLIB,
	BGNSTR,
	STRNAME,
	ENDSTR,
	BOUNDARY,
	PATH,
	SREF,
	AREF,
	TEXT,
	LAYER,
	DATATYPE,
	WIDTH,
	XY,
	ENDEL,
	SNAME,
	COLROW,
	TEXTNODE,
	NODE,
	TEXTTYPE,
	PRESENTATION,
	SPACING,
	STRING,
	STRANS,
	MAG,
	ANGLE,
	UINTEGER,
	USTRING,
	REFLIBS,
	FONTS,
	PATHTYPE,
	GENERATIONS,
	ATTRTABLE,
	STYPTABLE,
	STRTYPE,
	ELFLAGS,
	ELKEY,
	LINKTYPE,
	LINKKEYS,
	NODETYPE,
	PROPATTR,
	PROPVALUE,
	BOX,
	BOXTYPE,
	PLEX,
	BGNEXTN,
	ENDEXTN,
	TAPENUM,
	TAPECODE,
	STRCLASS,
	RESERVED,
	FORMAT,
	MASK,
	ENDMASKS,
	LIBDIRSIZE,
	SFRNAME,
	LIBSECUR,

	RECORD_TYPE_COUNT,
};

#define ID(record_type, data_type) (record_type << 8 | data_type)

enum RecordId : uint16_t
{
	ID_HEADER = ID(HEADER, 0x02),
	ID_BGNLIB = ID(BGNLIB, 0x02),
	ID_LIBNAME = ID(LIBNAME, 0x06),
	ID_UNITS = ID(UNITS, 0x05),
	ID_ENDLIB = ID(ENDLIB, 0x00),
	ID_BGNSTR = ID(BGNSTR, 0x02),
	ID_STRNAME = ID(STRNAME, 0x06),
	ID_ENDSTR = ID(ENDSTR, 0x00),
	ID_BOUNDARY = ID(BOUNDARY, 0x00),
	ID_PATH = ID(PATH, 0x00),
	ID_SREF = ID(SREF, 0x00),
	ID_AREF = ID(AREF, 0x00),
	ID_TEXT = ID(TEXT, 0x00),
	ID_LAYER = ID(LAYER, 0x02),
	ID_DATATYPE = ID(DATATYPE, 0x02),
	ID_WIDTH = ID(WIDTH, 0x03),
	ID_XY = ID(XY, 0x03),
	ID_ENDEL = ID(ENDEL, 0x00),
	ID_SNAME = ID(SNAME, 0x06),
	ID_COLROW = ID(COLROW, 0x02),
	ID_TEXTNODE = ID(TEXTNODE, 0x00),
	ID_NODE = ID(NODE, 0x00),
	ID_TEXTTYPE = ID(TEXTTYPE, 0x02),
	ID_PRESENTATION = ID(PRESENTATION, 0x01),
	ID_STRING = ID(STRING, 0x06),
	ID_STRANS = ID(STRANS, 0x01),
	ID_MAG = ID(MAG, 0x05),
	ID_ANGLE = ID(ANGLE, 0x05),
	ID_REFLIBS = ID(REFLIBS, 0x06),
	ID_FONTS = ID(FONTS, 0x06),
	ID_PATHTYPE = ID(PATHTYPE, 0x02),
	ID_GENERATIONS = ID(GENERATIONS, 0x02),
	ID_ATTRTABLE = ID(ATTRTABLE, 0x06),
	ID_ELFLAGS = ID(ELFLAGS, 0x01),
	ID_ELKEY = ID(ELKEY, 0x03),
	ID_NODETYPE = ID(NODETYPE, 0x02),
	ID_PROPATTR = ID(PROPATTR, 0x02),
	ID_PROPVALUE = ID(PROPVALUE, 0x06),
	ID_BOX = ID(BOX, 0x00),
	ID_BOXTYPE = ID(BOXTYPE, 0x02),
	ID_PLEX = ID(PLEX, 0x03),
	ID_BGNEXTN = ID(BGNEXTN, 0x03),
	ID_ENDEXTN = ID(ENDEXTN, 0x03),
	ID_TAPENUM = ID(TAPENUM, 0x02),
	ID_TAPECODE = ID(TAPECODE, 0x02),
	ID_STRCLASS = ID(STRCLASS, 0x01),
	ID_RESERVED = ID(RESERVED, 0x03),
	ID_FORMAT = ID(FORMAT, 0x02),
	ID_MASK = ID(MASK, 0x06),
	ID_ENDMASKS = ID(ENDMASKS, 0x00),
	ID_LIBDIRSIZE = ID(LIBDIRSIZE, 0x02),
	ID_SFRNAME = ID(SFRNAME, 0x06),
	ID_LIBSECUR = ID(LIBSECUR, 0x02),
};

struct RecordHeader
{
public:
	uint16_t getSize() const;
	const char* getName() const;

private:
	uint8_t size[2];
	union
	{
		struct
		{
			RecordType record_type;
			DataType data_type;
		};
		RecordId record_id;
	};
};

uint16_t RecordHeader::getSize() const
{
	return size[0] << 8 | size[1];
}

const char* RecordHeader::getName() const
{
	switch(record_type)
	{
		case HEADER: return "HEADER";
		case BGNLIB: return "BGNLIB";
		case LIBNAME: return "LIBNAME";
		case UNITS: return "UNITS";
		case ENDLIB: return "ENDLIB";
		case BGNSTR: return "BGNSTR";
		case STRNAME: return "STRNAME";
		case ENDSTR: return "ENDSTR";
		case BOUNDARY: return "BOUNDARY";
		case PATH: return "PATH";
		case SREF: return "SREF";
		case AREF: return "AREF";
		case TEXT: return "TEXT";
		case LAYER: return "LAYER";
		case DATATYPE: return "DATATYPE";
		case WIDTH: return "WIDTH";
		case XY: return "XY";
		case ENDEL: return "ENDEL";
		case SNAME: return "SNAME";
		case COLROW: return "COLROW";
		case TEXTNODE: return "TEXTNODE";
		case NODE: return "NODE";
		case TEXTTYPE: return "TEXTTYPE";
		case PRESENTATION: return "PRESENTATION";
		case SPACING: return "SPACING";
		case STRING: return "STRING";
		case STRANS: return "STRANS";
		case MAG: return "MAG";
		case ANGLE: return "ANGLE";
		case UINTEGER: return "UINTEGER";
		case USTRING: return "USTRING";
		case REFLIBS: return "REFLIBS";
		case FONTS: return "FONTS";
		case PATHTYPE: return "PATHTYPE";
		case GENERATIONS: return "GENERATIONS";
		case ATTRTABLE: return "ATTRTABLE";
		case STYPTABLE: return "STYPTABLE";
		case STRTYPE: return "STRTYPE";
		case ELFLAGS: return "ELFLAGS";
		case ELKEY: return "ELKEY";
		case LINKTYPE: return "LINKTYPE";
		case LINKKEYS: return "LINKKEYS";
		case NODETYPE: return "NODETYPE";
		case PROPATTR: return "PROPATTR";
		case PROPVALUE: return "PROPVALUE";
		case BOX: return "BOX";
		case BOXTYPE: return "BOXTYPE";
		case PLEX: return "PLEX";
		case BGNEXTN: return "BGNEXTN";
		case ENDEXTN: return "ENDEXTN";
		case TAPENUM: return "TAPENUM";
		case TAPECODE: return "TAPECODE";
		case STRCLASS: return "STRCLASS";
		case RESERVED: return "RESERVED";
		case FORMAT: return "FORMAT";
		case MASK: return "MASK";
		case ENDMASKS: return "ENDMASKS";
		case LIBDIRSIZE: return "LIBDIRSIZE";
		case SFRNAME: return "SFRNAME";
		case LIBSECUR: return "LIBSECUR";
		default:
			break;
	}
	assert(false);
	return "<UNKNOWN>";
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char** argv)
{
	if(argc < 2)
	{
		cout << "No GDS file specified" << endl;
		return 1;
	}

	const char* filename = argv[1];
	uint8_t* file_data = nullptr;
	uint32_t file_data_size = 0;
	if(!LoadFile(file_data, file_data_size, filename))
	{
		return 1;
	}

	uint32_t total_size = 0;
	while(total_size < file_data_size)
	{
		RecordHeader* header = reinterpret_cast<RecordHeader*>(file_data + total_size);
		cout << "Element [" << header->getName() << "]" << endl;
		total_size += header->getSize();
	}

	return 0;
}

