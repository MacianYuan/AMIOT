/**
* \file mal.h
* \author WooShang <wooshang@126.com>
* \date 2015/07/09
*
* This file interfaces of Middle layout.
* Copyright (C) 2015 WooShang.
*
*/

#ifndef MAL_H
#define MAL_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

int Mal_SYS_Init();

int Mal_SYS_Exit();

int Mal_VO_SetLayout(int layout);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __MAL_H__*/
