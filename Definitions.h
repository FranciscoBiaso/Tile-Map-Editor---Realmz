#pragma once

/**
*  GRID SIZE
*/
#define REALMZ_GRID_SIZE 32
#define TEXTURE_ATLAS_MAX_WIDTH 200 

namespace def{
    /**
     * ENUM IMG SIZES.
     */
    enum class IMG_SIZE {
        IMG_SIZE_32X32, /**< enum IMG_SIZE_32X32  */
        IMG_SIZE_32X64, /**< enum IMG_SIZE_32X64  */
        IMG_SIZE_64X32, /**< enum IMG_SIZE_64X32  */
        IMG_SIZE_64X64, /**< enum IMG_SIZE_64X64  */
    };

    enum ReturnMsg {
        FILE_OK,
        FILE_NOT_SELECTED,
        IMAGE_EMPTY,
        NOT_ENOUGH_MEMORY,
        PIXELBUF_INVALID_SIZE
    };
}