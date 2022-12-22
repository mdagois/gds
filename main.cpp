#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <iostream>
#include <map>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

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

#define RECORD_ID(record_type, data_type) (record_type << 8 | data_type)

enum RecordId : uint16_t
{
	ID_HEADER = RECORD_ID(HEADER, 0x02),
	ID_BGNLIB = RECORD_ID(BGNLIB, 0x02),
	ID_LIBNAME = RECORD_ID(LIBNAME, 0x06),
	ID_UNITS = RECORD_ID(UNITS, 0x05),
	ID_ENDLIB = RECORD_ID(ENDLIB, 0x00),
	ID_BGNSTR = RECORD_ID(BGNSTR, 0x02),
	ID_STRNAME = RECORD_ID(STRNAME, 0x06),
	ID_ENDSTR = RECORD_ID(ENDSTR, 0x00),
	ID_BOUNDARY = RECORD_ID(BOUNDARY, 0x00),
	ID_PATH = RECORD_ID(PATH, 0x00),
	ID_SREF = RECORD_ID(SREF, 0x00),
	ID_AREF = RECORD_ID(AREF, 0x00),
	ID_TEXT = RECORD_ID(TEXT, 0x00),
	ID_LAYER = RECORD_ID(LAYER, 0x02),
	ID_DATATYPE = RECORD_ID(DATATYPE, 0x02),
	ID_WIDTH = RECORD_ID(WIDTH, 0x03),
	ID_XY = RECORD_ID(XY, 0x03),
	ID_ENDEL = RECORD_ID(ENDEL, 0x00),
	ID_SNAME = RECORD_ID(SNAME, 0x06),
	ID_COLROW = RECORD_ID(COLROW, 0x02),
	ID_TEXTNODE = RECORD_ID(TEXTNODE, 0x00),
	ID_NODE = RECORD_ID(NODE, 0x00),
	ID_TEXTTYPE = RECORD_ID(TEXTTYPE, 0x02),
	ID_PRESENTATION = RECORD_ID(PRESENTATION, 0x01),
	ID_STRING = RECORD_ID(STRING, 0x06),
	ID_STRANS = RECORD_ID(STRANS, 0x01),
	ID_MAG = RECORD_ID(MAG, 0x05),
	ID_ANGLE = RECORD_ID(ANGLE, 0x05),
	ID_REFLIBS = RECORD_ID(REFLIBS, 0x06),
	ID_FONTS = RECORD_ID(FONTS, 0x06),
	ID_PATHTYPE = RECORD_ID(PATHTYPE, 0x02),
	ID_GENERATIONS = RECORD_ID(GENERATIONS, 0x02),
	ID_ATTRTABLE = RECORD_ID(ATTRTABLE, 0x06),
	ID_ELFLAGS = RECORD_ID(ELFLAGS, 0x01),
	ID_ELKEY = RECORD_ID(ELKEY, 0x03),
	ID_NODETYPE = RECORD_ID(NODETYPE, 0x02),
	ID_PROPATTR = RECORD_ID(PROPATTR, 0x02),
	ID_PROPVALUE = RECORD_ID(PROPVALUE, 0x06),
	ID_BOX = RECORD_ID(BOX, 0x00),
	ID_BOXTYPE = RECORD_ID(BOXTYPE, 0x02),
	ID_PLEX = RECORD_ID(PLEX, 0x03),
	ID_BGNEXTN = RECORD_ID(BGNEXTN, 0x03),
	ID_ENDEXTN = RECORD_ID(ENDEXTN, 0x03),
	ID_TAPENUM = RECORD_ID(TAPENUM, 0x02),
	ID_TAPECODE = RECORD_ID(TAPECODE, 0x02),
	ID_STRCLASS = RECORD_ID(STRCLASS, 0x01),
	ID_RESERVED = RECORD_ID(RESERVED, 0x03),
	ID_FORMAT = RECORD_ID(FORMAT, 0x02),
	ID_MASK = RECORD_ID(MASK, 0x06),
	ID_ENDMASKS = RECORD_ID(ENDMASKS, 0x00),
	ID_LIBDIRSIZE = RECORD_ID(LIBDIRSIZE, 0x02),
	ID_SFRNAME = RECORD_ID(SFRNAME, 0x06),
	ID_LIBSECUR = RECORD_ID(LIBSECUR, 0x02),
};

struct Record
{
public:
	uint16_t getSize() const;
	uint16_t getDataSize() const;
	DataType getDataType() const;
	RecordType getRecordType() const;
	RecordId getRecordId() const;
	const char* getName() const;

	uint16_t getBitArray() const;
	int16_t getTwoByteSignedInteger() const;
	int32_t getFourByteSignedInteger() const;
	double getFourByteReal() const;
	double getEightByteReal() const;
	const char* getAsciiString(std::string& out_buffer) const;

private:
	uint8_t size[2];
	RecordType record_type;
	DataType data_type;
	const uint8_t value[1];
};

uint16_t Record::getSize() const
{
	return size[0] << 8 | size[1];
}

uint16_t Record::getDataSize() const
{
	return getSize() - 4;
}

DataType Record::getDataType() const
{
	return data_type;
}

RecordType Record::getRecordType() const
{
	return record_type;
}

RecordId Record::getRecordId() const
{
	return static_cast<RecordId>(RECORD_ID(record_type, data_type));
}

const char* Record::getName() const
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

uint16_t Record::getBitArray() const
{
	return static_cast<uint16_t>(value[0] << 8 | value[1]);
}

int16_t Record::getTwoByteSignedInteger() const
{
	return static_cast<int16_t>(value[0] << 8 | value[1]);
}

int32_t Record::getFourByteSignedInteger() const
{
	return static_cast<int32_t>(value[0] << 24 | value[1] << 16 | value[2] << 8 | value[3]);
}

double Record::getFourByteReal() const
{
	const double sign = value[0] & 0x80 ? -1.0 : 1.0;
	const int32_t exponent = (value[0] & 0x7F) - 64;
	const uint64_t mantissa = value[1] << 16 | value[2] << 8 | value[3];
	return sign * mantissa * pow(2, 4 * exponent - 24);
}

double Record::getEightByteReal() const
{
	const double sign = value[0] & 0x80 ? -1.0 : 1.0;
	const int32_t exponent = (value[0] & 0x7F) - 64;
	const uint64_t mantissa_high = value[1] << 16 | value[2] << 8 | value[3];
	const uint64_t mantissa_low = value[4] << 24 | value[5] << 16 | value[6] << 8 | value[7];
	const uint64_t mantissa = mantissa_high << 32 | mantissa_low;
	return sign * mantissa * pow(2, 4 * exponent - 56);
}

const char* Record::getAsciiString(std::string& out_buffer) const
{
	out_buffer.clear();
	out_buffer.append(reinterpret_cast<const char*>(value), getDataSize());
	out_buffer.push_back('\0');
	return out_buffer.c_str();
}

ostream& operator<<(ostream& os, const Record& record)
{
    os << record.getName() << '(';
	switch(record.getDataType())
	{
		case NO_DATA:
			os << "null";
			break;
		case BIT_ARRAY:
			os << "0x" << hex << record.getBitArray() << dec;
			break;
		case TWO_BYTE_SIGNED_INTEGER:
			os << record.getTwoByteSignedInteger();
			break;
		case FOUR_BYTE_SIGNED_INTEGER:
			os << record.getFourByteSignedInteger();
			break; 
		case FOUR_BYTE_REAL:
			os << record.getFourByteReal();
			break;
		case EIGHT_BYTE_REAL:
			os << record.getEightByteReal();
			break;
		case ASCII_STRING:
		{
			string buffer;
			os << record.getAsciiString(buffer);
			break;
		}
		default:
			assert(false);
			os << "unknown";
			break;
	}
	os << ')';
	return os;
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
	if(!loadFile(file_data, file_data_size, filename))
	{
		return 1;
	}

	map<const char*, uint32_t> elemCount;
	uint32_t total_size = 0;
	while(total_size < file_data_size)
	{
		Record* record = reinterpret_cast<Record*>(file_data + total_size);
		const char* elemName = record->getName();
		auto elemIt = elemCount.find(elemName);
		if(elemIt == elemCount.end())
		{
			elemCount.insert(pair(elemName, 1));
		}
		else
		{
			++elemIt->second;
		}
#if 0
		if(record->getRecordId() == ID_STRNAME)
		{
			cout << *record << endl;
		}
		if (record->getRecordId() == ID_SNAME)
		{
			cout << *record << endl;
		}
#endif
		total_size += record->getSize();
	}
	for(auto it = elemCount.begin(); it != elemCount.end(); ++it)
	{
		cout << it->first << " = " << it->second << endl;
	}

	return 0;
}

#if 0
HEADER							1
BGNLIB							1
	LIBDIRSIZE					1, ignore, optional
	SFRNAME						1, ignore, optional	
	LIBSECUR					1, ignore, optional
	LIBNAME						1
	REFLIBS						1, unused, optional
	FONTS						1, unused, optional
	ATTRTABLE					1, unused, optional
	GENERATIONS					1, unused, optional
	FORMAT						1, ignore
		MASK					*, ignore, optional
	ENDMASKS					1, ignore, optional
	UNITS						1
	BGNSTR						1
		STRNAME					1
		STRCLASS				1, ignore, optional
		BOUNDARY				*
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			LAYER				1
			DATATYPE			1
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		PATH					1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			LAYER				1
			DATATYPE			1
			PATHTYPE			1, optional
			WIDTH				1, optional
			BGNEXTN				1, ignore, optional
			ENDEXTN				1, ignore, optional
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		SREF					1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			SNAME				1
			STRANS				1, optional
				MAG				1, optional
				ANGLE			1, optional
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		AREF					1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			SNAME				1
			STRANS				1, optional
				MAG				1, optional
				ANGLE			1, optional
			COLROW				1
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		TEXT					1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			LAYER				1
			TEXTTYPE			1
				PRESENTATION	1, ignore, optional
				PATHTYPE		1, optional
				WIDTH			1, optional
				STRANS			1, optional
					MAG			1, optional
					ANGLE		1, optional
				XY				1
				STRING			1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		NODE					1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			LAYER				1
			NODETYPE			1
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
		BOX						1
			ELFLAGS				1, ignore, optional
			PLEX				1, ignore, optional
			LAYER				1
			BOXTYPE				1
			XY					1
			PROPATTR			*
			PROPVALUE			*
		ENDEL					1
	ENDSTR						1
ENDLIB							1
#endif

