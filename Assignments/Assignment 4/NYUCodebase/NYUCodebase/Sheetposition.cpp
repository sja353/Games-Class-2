#include "Sheetposition.h"

Sheetposition::Sheetposition(float u, float v, float width, float height, float size, float sheet_width, float sheet_length){
	this->u = u / sheet_width;
	this->v = v / sheet_length;
	this->width = width / sheet_width;
	this->height = height / sheet_length;
	this->size = size;
}

Sheetposition& Sheetposition:: operator=(const Sheetposition rhs){
	this->u = rhs.u;
	this->v = rhs.v;
	this->width = rhs.width;
	this->height = rhs.height;
	this->size = rhs.size;

	return *this;
}