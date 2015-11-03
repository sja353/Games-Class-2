#ifndef SHEETPOSITION_HEADER
#define SHEETPOSITION_HEADER

class Sheetposition {
public:
	Sheetposition & operator=(const Sheetposition rhs);
	Sheetposition(){};
	Sheetposition(float u, float v, float width, float height, float size, float sheet_width, float sheet_length);
	float u;
	float v;
	float width;
	float height;
	float size;
};
#endif