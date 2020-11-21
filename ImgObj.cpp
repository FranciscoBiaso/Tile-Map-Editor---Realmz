#include "ImgObj.h"

data::ImgObj::ImgObj(std::string name, def::IMG_SIZE size, const std::vector<math::Vec2<int>> references) : name(name), size(size)
{
	for (int i = 0; i < references.size(); i++)
		imgs[i] = references[i];
}

def::IMG_SIZE data::ImgObj::getSize() const
{
	return size;
}

std::string data::ImgObj::getSizeAsString() const
{
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32: return "32x32";
		break;
	case def::IMG_SIZE::IMG_SIZE_32X64: return "32x64";
		break;
	case def::IMG_SIZE::IMG_SIZE_64X32: return "64x32";
		break;
	case def::IMG_SIZE::IMG_SIZE_64X64: return "64x64";
		break;
	default: return "getSizeAsString() unknown type!";
		break;
	}
}

int data::ImgObj::getSizeAsInt() const
{
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32: return 1;
		break;
	case def::IMG_SIZE::IMG_SIZE_32X64: // return 2 //
	case def::IMG_SIZE::IMG_SIZE_64X32: return 2;
		break;
	case def::IMG_SIZE::IMG_SIZE_64X64: return 4;
		break;
	default: return 1;
		break;
	}
}

void data::ImgObj::setImgRefs(const std::vector<math::Vec2<int>> references)
{
	for (int i = 0; i < references.size(); i++)
		imgs[i] = references[i];
}

void data::ImgObj::setName(std::string name)
{
	this->name = name;
}