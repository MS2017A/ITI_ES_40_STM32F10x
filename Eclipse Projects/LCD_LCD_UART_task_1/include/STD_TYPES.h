/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * Author : Mohamed Adel Anis Ahmed				   *
 * date   : 20 Feb 2020                            *
 * version: 1.0                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

/*******************************Unsigned types******************************/

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef	unsigned long int	uint32_t;
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;


/*******************************Signed types******************************/

typedef signed char			int8_t;
typedef	signed short int	int16_t;
typedef	signed long int		int32_t;
typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;

/*******************************Float numbers types******************************/

typedef	float			float32_t;
typedef double			float64_t;
typedef long double		float96_t;
typedef float32_t		f32;
typedef float64_t		f64;
typedef float96_t		f96;


/*******************************Error types******************************/

typedef uint8_t	ErrorStatus_t	;
#define ERROR_OK	1
#define ERROR_NOK	0

typedef enum STD_ERROR
{
  OK,
  NOK,
  NULL_POINTER,
  NOT_VALID_INPUTS,
  BUSY

}STD_ERROR;

/*******************************Miscellaneous******************************/

#define	  NULL		((void*)0)




#endif
