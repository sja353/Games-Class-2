#ifndef SHEETPOSITION_HEADER
#define SHEETPOSITION_HEADER
// small class for making the passing of values refering to a certain position on a spritesheet more convenient
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