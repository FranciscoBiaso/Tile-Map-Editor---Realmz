#pragma once
#include <string>
#include <vector>
#include "Definitions.h"
#include "Vec2.h"

namespace data {
	/*!
		ImgObj class
		============

		This class represents an image object that can contain n image references where n is equal to 1, 2 or 4 in the following sizes: 32x32,32x64,64x32 or 64x64.
	 */
	class ImgObj
	{
	private:
		def::IMG_SIZE size;
		math::Vec2<int> imgs[IMGOBJ_MAX_IMGS];

	public:
		/**
		 * constructor. 
		 * @param size internal img size (32x32, 32x64, 64x32 or 64x64).
		 */
		ImgObj(def::IMG_SIZE size, const std::vector<math::Vec2<int>>& references);

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a std::string.
		 */
		std::string getSizeAsString() const;

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a int.
		 */
		int getSizeAsInt();
	};

}

