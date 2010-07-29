/* 
 +----------------------------------------------------------------------------+
 | php_opengl - The PHP OpenGL bindings                                       |
 +----------------------------------------------------------------------------+
 | Copyright (c) 2001 Brad Lafountain                                         |
 +----------------------------------------------------------------------------+
 | This file is part of php_opengl.                                           |
 |                                                                            |
 | php_opengl is free software; you can redistribute it and/or modify         |
 | it under the terms of the GNU General Public License as published by       |
 | the Free Software Foundation; either version 2 of the License, or          |
 | (at your option) any later version.                                        |
 |                                                                            |
 | php_opengl is distributed in the hope that it will be useful,              |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of             |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              |
 | GNU General Public License for more details.                               |
 |                                                                            |
 | You should have received a copy of the GNU General Public License          |
 | along with php_opengl; if not, write to the Free Software                  |
 | Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  |
 +----------------------------------------------------------------------------+
 | Authors: Brad Lafountain <rodif_bl@yahoo.com>                              |
 +----------------------------------------------------------------------------+
*/

/* $Id: php_convert.c,v 1.3 2001/09/02 16:25:52 drbrain Exp $ */

#ifdef PHP_WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "php.h"
#include "php_convert.h"

void call_user_callback(HashTable *call_backs,int call_type,int num_params,zval **params)
{
	zval retval;
	zval *function_name;
	if(zend_hash_index_find(call_backs,call_type,(void **)&function_name) == SUCCESS)
	{
		TSRMLS_FETCH();
		if(call_user_function(CG(function_table), NULL, function_name, &retval, num_params, params TSRMLS_CC) != SUCCESS)
		{
			zend_error(E_ERROR, "Function call failed");
		}
	}
}

void c_array_to_php_array(void *c_array,int num,zval *php_array,int type)
{
	zval *val;
	HashTable *ht;
	int i;

	convert_to_array(php_array);
	ht = Z_ARRVAL_P(php_array);
	for(i = 0;i < num;i++)
	{
		MAKE_STD_ZVAL(val);
		switch(type)
		{
		case C_INT_TO_PHP_LONG:
			ZVAL_LONG(val,(int)((int*)c_array)[i]);
			break;
		case C_UINT_TO_PHP_LONG:
			ZVAL_LONG(val,(unsigned int)((unsigned int*)c_array)[i]);
			break;
		case C_LONG_TO_PHP_LONG:
			ZVAL_LONG(val,((long*)c_array)[i]);
			break;
		case C_FLOAT_TO_PHP_DOUBLE:
			ZVAL_DOUBLE(val,(float)((float*)c_array)[i]);
			break;
		case C_DOUBLE_TO_PHP_DOUBLE:
			ZVAL_DOUBLE(val,(double)((double*)c_array)[i]);
			break;
		case C_BOOLEAN_TO_PHP_BOOLEAN:
			ZVAL_BOOL(val,(unsigned char)((unsigned char*)c_array)[i]);
			break;
		case C_CHAR_TO_PHP_LONG:
			ZVAL_LONG(val,(char)((char*)c_array)[i]);
			break;
		case C_USHORT_TO_PHP_LONG:
			ZVAL_LONG(val,(unsigned short)((unsigned short*)c_array)[i]);
			break;
		}
		zend_hash_next_index_insert(ht,&val,sizeof(zval*),NULL);
	}
}

void *php_array_to_c_array(zval *param,int type,int size,int *array_size)
{
	HashTable *param_ht = param->value.ht;
	zval **cur;
	void *params;
	int i,tmp_size = zend_hash_num_elements(param_ht);

	zend_hash_internal_pointer_reset(param_ht);
	params = (void *)emalloc(size * tmp_size);

	i = 0;
	while(zend_hash_get_current_data(param_ht,(void **)&cur) == SUCCESS)
	{
		if((*cur)->type == IS_ARRAY)
		{
			int new_array_size;
			void *array = php_array_to_c_array(*cur,type,size,&new_array_size);
			params = erealloc(params, (tmp_size + new_array_size) * size);
			memcpy(&((char*)params)[i*size],array,new_array_size * size);
			i += (new_array_size - 1);
			efree(array);
		}
		else
		{
			switch(type)
			{
			case TO_C_FLOAT:
				convert_to_double(*cur);
				((float*)params)[i] = (float)Z_DVAL_P(*cur);
				break;
			case TO_C_DOUBLE:
				convert_to_double(*cur);
				((double*)params)[i] = Z_DVAL_P(*cur);
				break;
			case TO_C_INT:
				convert_to_long(*cur);
				((int*)params)[i] = (int)Z_LVAL_P(*cur);
				break;
			case TO_C_LONG:
				convert_to_long(*cur);
				((long*)params)[i] = Z_LVAL_P(*cur);
				break;
			case TO_C_UCHAR:
				convert_to_long(*cur);
				((unsigned char*)params)[i] = (unsigned char)Z_LVAL_P(*cur);
				break;
			case TO_C_SCHAR:
				convert_to_long(*cur);
				((signed char*)params)[i] = (signed char)Z_LVAL_P(*cur);
				break;
			case TO_C_USHORT:
				convert_to_long(*cur);
				((unsigned short*)params)[i] = (unsigned short)Z_LVAL_P(*cur);
				break;
			case TO_C_SHORT:
				convert_to_long(*cur);
				((short*)params)[i] = (short)Z_LVAL_P(*cur);
				break;
			case TO_C_UINT:
				convert_to_long(*cur);
				((unsigned int*)params)[i] = (unsigned int)Z_LVAL_P(*cur);
				break;
			case TO_C_STRING:
				convert_to_string(*cur);
				((char **)params)[i] = estrdup(Z_STRVAL_P(*cur));
			}
		}
		zend_hash_move_forward(param_ht);
		i++;
	}
	if(array_size != NULL)
		*array_size = i;
	return (void *)params;
}

int gl_pixel_size(GLenum format)
{
	switch(format)
	{
	case GL_COLOR_INDEX:
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
	case GL_ALPHA:
	case GL_STENCIL_INDEX:
	case GL_DEPTH_COMPONENT:
	case GL_LUMINANCE:
		return 1;

	case GL_LUMINANCE_ALPHA:
		return 2;

	case GL_RGB:
#ifdef GL_BGR_EXT
	case GL_BGR_EXT:
#endif
		return 3;

	case GL_RGBA:
#ifdef GL_BGRA_EXT
	case GL_BGRA_EXT:
#endif
#ifdef GL_ABGR_EXT
	case GL_ABGR_EXT:
#endif
		return 4;
        case 1:
        case 2:
        case 3:
        case 4:
                return format;
	default:
		return -1;
	}
}

int gl_type_size(GLenum type)
{
	switch(type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return sizeof(GLbyte);

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		return sizeof(GLshort);

	case GL_INT:
	case GL_UNSIGNED_INT:
		return sizeof(GLint);
	
	case GL_FLOAT:
		return sizeof(GLfloat);

	case GL_BITMAP:
		return sizeof(GLbitfield);
	
	default:
		return 1;
	}
}
