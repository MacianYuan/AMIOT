#ifndef STREAM_BLOCK_H
#define STREAM_BLOCK_H

#include <stdint.h> 

#define BLOCK_FLAG_TYPE_I               0x0002      ///< I帧
#define BLOCK_FLAG_TYPE_P               0x0004      ///< P帧
#define BLOCK_FLAG_TYPE_B               0x0008      ///< B帧
#define BLOCK_FLAG_TYPE_PB              0x0010      ///< P/B帧
#define BLOCK_FLAG_HEADER               0x0020      ///< 含有头部信息


#define MakeFourCC( a, b, c, d ) \
    ( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) \
    | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )

#define AV_FOURCC_NULL  MakeFourCC('N', 'U', 'L', 'L')
#define AV_FOURCC_AAC   MakeFourCC('m', 'p', '4', 'a')
#define AV_FOURCC_JPEG  MakeFourCC('J', 'P', 'E', 'G')
#define AV_FOURCC_H264  MakeFourCC('a', 'v', 'c', '1')

typedef struct _Mal_StreamBlock
{
    uint8_t*        p_buffer;       ///< 负载数据起始位置
    uint32_t        i_buffer;       ///< 负载数据长度

    uint32_t        i_flags;        ///< 数据包类型，是I帧，P帧
    uint32_t        i_nb_samples;   ///< 音频样本数量
    uint32_t        i_track;        ///< 数据轨道
    uint32_t        i_codec;        ///< 编码类型FOURCC('a','b','c','d')

    int64_t         i_pts;          ///< 数据包时戳
    int64_t         i_dts;          ///< 解码时戳
    int64_t		    i_length;       ///< 持续时长
}Mal_StreamBlock;



#endif //
