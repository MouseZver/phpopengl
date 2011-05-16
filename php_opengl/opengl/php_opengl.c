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

/* $Id: php_opengl.c,v 1.17 2001/10/18 21:10:35 drbrain Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "php.h"
#include "php_opengl.h"
#include "php_glu.h"
#include "php_convert.h"

#ifdef PHP_OPENGL_OSMESA
#include "php_osmesa.h"
#endif

char *glget_mask = NULL;
unsigned int glget_max = 0;


// Used for glSelectBuffer
GLuint *select_buffer = NULL;
unsigned long select_buffer_size = 0;

/* yoya customize */
/* {{{ arginfo */
#ifdef ZEND_ENGINE_2
ZEND_BEGIN_ARG_INFO_EX(force_ref_first_arg, 1, 0, 1)
    ZEND_ARG_INFO(1, arg1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(force_ref_second_arg, 1, 0, 2)
    ZEND_ARG_INFO(0, arg1)
    ZEND_ARG_INFO(1, arg2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(force_ref_third_arg, 1, 0, 3)
    ZEND_ARG_INFO(0, arg1)
    ZEND_ARG_INFO(0, arg2)
    ZEND_ARG_INFO(1, arg3)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(force_ref_fourth_arg, 1, 0, 4)
    ZEND_ARG_INFO(0, arg1)
    ZEND_ARG_INFO(0, arg2)
    ZEND_ARG_INFO(0, arg3)
    ZEND_ARG_INFO(1, arg4)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(force_ref_seventh_arg, 1, 0, 7)
    ZEND_ARG_INFO(0, arg1)
    ZEND_ARG_INFO(0, arg2)
    ZEND_ARG_INFO(0, arg3)
    ZEND_ARG_INFO(0, arg4)
    ZEND_ARG_INFO(0, arg5)
    ZEND_ARG_INFO(0, arg6)
    ZEND_ARG_INFO(1, arg7)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(force_ref_ninth_arg, 1, 0, 9)
    ZEND_ARG_INFO(0, arg1)
    ZEND_ARG_INFO(0, arg2)
    ZEND_ARG_INFO(0, arg3)
    ZEND_ARG_INFO(0, arg4)
    ZEND_ARG_INFO(0, arg5)
    ZEND_ARG_INFO(0, arg6)
    ZEND_ARG_INFO(0, arg7)
    ZEND_ARG_INFO(0, arg8)
    ZEND_ARG_INFO(1, arg9)
ZEND_END_ARG_INFO()
#else
static unsigned char force_ref_first_arg[] =
{1, BYREF_FORCE};

static unsigned char force_ref_second_arg[] =
{2, BYREF_NONE, BYREF_FORCE};

static unsigned char force_ref_third_arg[] =
{3, BYREF_NONE, BYREF_NONE, BYREF_FORCE};

static unsigned char force_ref_fourth_arg[] =
{4, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_FORCE};

static unsigned char force_ref_seventh_arg[] =
{7, BYREF_NONE,  BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_FORCE};
static unsigned char force_ref_ninth_arg[] =
{9, BYREF_NONE,  BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_NONE, BYREF_FORCE};
#endif
/* }}} */

function_entry opengl_functions[] = {
	// GL Functions
	PHP_FE(glaccum,NULL)
	PHP_FE(glalphafunc,NULL)
	PHP_FE(glaretexturesresident,NULL)
	PHP_FE(glarrayelement,NULL)
	PHP_FE(glbegin,NULL)
	PHP_FE(glbindtexture,NULL)
	PHP_FE(glbitmap,NULL)
	PHP_FE(glblendfunc,NULL)
	PHP_FE(glcalllist,NULL)
	PHP_FE(glcalllists,NULL)
	PHP_FE(glclear,NULL)
	PHP_FE(glclearaccum,NULL)
	PHP_FE(glclearcolor,NULL)
	PHP_FE(glcleardepth,NULL)
	PHP_FE(glclearindex,NULL)
	PHP_FE(glclearstencil,NULL)
	PHP_FE(glclipplane,NULL)
	PHP_FE(glcolor3b,NULL)
	PHP_FE(glcolor3bv,NULL)
	PHP_FE(glcolor3d,NULL)
	PHP_FE(glcolor3dv,NULL)
	PHP_FE(glcolor3f,NULL)
	PHP_FE(glcolor3fv,NULL)
	PHP_FE(glcolor3i,NULL)
	PHP_FE(glcolor3iv,NULL)
	PHP_FE(glcolor3s,NULL)
	PHP_FE(glcolor3sv,NULL)
	PHP_FE(glcolor3ub,NULL)
	PHP_FE(glcolor3ubv,NULL)
	PHP_FE(glcolor3ui,NULL)
	PHP_FE(glcolor3uiv,NULL)
	PHP_FE(glcolor3us,NULL)
	PHP_FE(glcolor3usv,NULL)
	PHP_FE(glcolor4b,NULL)
	PHP_FE(glcolor4bv,NULL)
	PHP_FE(glcolor4d,NULL)
	PHP_FE(glcolor4dv,NULL)
	PHP_FE(glcolor4f,NULL)
	PHP_FE(glcolor4fv,NULL)
	PHP_FE(glcolor4i,NULL)
	PHP_FE(glcolor4iv,NULL)
	PHP_FE(glcolor4s,NULL)
	PHP_FE(glcolor4sv,NULL)
	PHP_FE(glcolor4ub,NULL)
	PHP_FE(glcolor4ubv,NULL)
	PHP_FE(glcolor4ui,NULL)
	PHP_FE(glcolor4uiv,NULL)
	PHP_FE(glcolor4us,NULL)
	PHP_FE(glcolor4usv,NULL)
	PHP_FE(glcolormask,NULL)
	PHP_FE(glcolormaterial,NULL)
	PHP_FE(glcolorpointer,NULL)
	PHP_FE(glcopypixels,NULL)
	PHP_FE(glcopyteximage1d,NULL)
	PHP_FE(glcopyteximage2d,NULL)
	PHP_FE(glcopytexsubimage1d,NULL)
	PHP_FE(glcopytexsubimage2d,NULL)
	PHP_FE(glcullface,NULL)
	PHP_FE(gldeletelists,NULL)
	PHP_FE(gldeletetextures,NULL)
	PHP_FE(gldepthfunc,NULL)
	PHP_FE(gldepthmask,NULL)
	PHP_FE(gldepthrange,NULL)
	PHP_FE(gldisable,NULL)
	PHP_FE(gldisableclientstate,NULL)
	PHP_FE(gldrawarrays,NULL)
	PHP_FE(gldrawbuffer,NULL)
	PHP_FE(gldrawelements,NULL)
	PHP_FE(gldrawpixels,NULL)
	PHP_FE(gledgeflag,NULL)
	PHP_FE(gledgeflagpointer,NULL)
	PHP_FE(gledgeflagv,NULL)
	PHP_FE(glenable,NULL)
	PHP_FE(glenableclientstate,NULL)
	PHP_FE(glend,NULL)
	PHP_FE(glendlist,NULL)
	PHP_FE(glevalcoord1d,NULL)
	PHP_FE(glevalcoord1dv,NULL)
	PHP_FE(glevalcoord1f,NULL)
	PHP_FE(glevalcoord1fv,NULL)
	PHP_FE(glevalcoord2d,NULL)
	PHP_FE(glevalcoord2dv,NULL)
	PHP_FE(glevalcoord2f,NULL)
	PHP_FE(glevalcoord2fv,NULL)
	PHP_FE(glevalmesh1,NULL)
	PHP_FE(glevalmesh2,NULL)
	PHP_FE(glevalpoint1,NULL)
	PHP_FE(glevalpoint2,NULL)
	PHP_FE(glfeedbackbuffer,NULL)
	PHP_FE(glfinish,NULL)
	PHP_FE(glflush,NULL)
	PHP_FE(glfogf,NULL)
	PHP_FE(glfogfv,NULL)
	PHP_FE(glfogi,NULL)
	PHP_FE(glfogiv,NULL)
	PHP_FE(glfrontface,NULL)
	PHP_FE(glfrustum,NULL)
	PHP_FE(glgenlists,NULL)
	PHP_FE(glgentextures,NULL)
	PHP_FE(glgetbooleanv, force_ref_second_arg)
	PHP_FE(glgetclipplane, force_ref_second_arg)
	PHP_FE(glgetdoublev, force_ref_second_arg)
	PHP_FE(glgeterror,NULL)
	PHP_FE(glgetfloatv, force_ref_second_arg)
	PHP_FE(glgetintegerv, force_ref_second_arg)
	PHP_FE(glgetlightfv, force_ref_third_arg)
	PHP_FE(glgetlightiv, force_ref_third_arg)
	PHP_FE(glgetmapdv, force_ref_second_arg)
	PHP_FE(glgetmapfv, force_ref_second_arg)
	PHP_FE(glgetmapiv, force_ref_second_arg)
	PHP_FE(glgetmaterialfv, force_ref_third_arg)
	PHP_FE(glgetmaterialiv, force_ref_third_arg)
	PHP_FE(glgetpixelmapfv,NULL)
	PHP_FE(glgetpixelmapuiv,NULL)
	PHP_FE(glgetpixelmapusv,NULL)
	PHP_FE(glgetpointerv,NULL)
	PHP_FE(glgetpolygonstipple, force_ref_first_arg)
	PHP_FE(glgetstring,NULL)
	PHP_FE(glgettexenvfv, force_ref_third_arg)
	PHP_FE(glgettexenviv, force_ref_third_arg)
	PHP_FE(glgettexgendv, force_ref_third_arg)
	PHP_FE(glgettexgenfv, force_ref_third_arg)
	PHP_FE(glgettexgeniv, force_ref_third_arg)
	PHP_FE(glgetteximage,NULL)
	PHP_FE(glgettexlevelparameterfv, force_ref_fourth_arg)
	PHP_FE(glgettexlevelparameteriv, force_ref_fourth_arg)
	PHP_FE(glgettexparameterfv, force_ref_third_arg)
	PHP_FE(glgettexparameteriv, force_ref_third_arg)
	PHP_FE(glhint,NULL)
	PHP_FE(glindexmask,NULL)
	PHP_FE(glindexpointer,NULL)
	PHP_FE(glindexd,NULL)
	PHP_FE(glindexdv,NULL)
	PHP_FE(glindexf,NULL)
	PHP_FE(glindexfv,NULL)
	PHP_FE(glindexi,NULL)
	PHP_FE(glindexiv,NULL)
	PHP_FE(glindexs,NULL)
	PHP_FE(glindexsv,NULL)
	PHP_FE(glindexub,NULL)
	PHP_FE(glindexubv,NULL)
	PHP_FE(glinitnames,NULL)
	PHP_FE(glinterleavedarrays,NULL)
	PHP_FE(glisenabled,NULL)
	PHP_FE(glislist,NULL)
	PHP_FE(glistexture,NULL)
	PHP_FE(gllightmodelf,NULL)
	PHP_FE(gllightmodelfv,NULL)
	PHP_FE(gllightmodeli,NULL)
	PHP_FE(gllightmodeliv,NULL)
	PHP_FE(gllightf,NULL)
	PHP_FE(gllightfv,NULL)
	PHP_FE(gllighti,NULL)
	PHP_FE(gllightiv,NULL)
	PHP_FE(gllinestipple,NULL)
	PHP_FE(gllinewidth,NULL)
	PHP_FE(gllistbase,NULL)
	PHP_FE(glloadidentity,NULL)
	PHP_FE(glloadmatrixd,NULL)
	PHP_FE(glloadmatrixf,NULL)
	PHP_FE(glloadname,NULL)
	PHP_FE(gllogicop,NULL)
	PHP_FE(glmap1d,NULL)
	PHP_FE(glmap1f,NULL)
	PHP_FE(glmap2d,NULL)
	PHP_FE(glmap2f,NULL)
	PHP_FE(glmapgrid1d,NULL)
	PHP_FE(glmapgrid1f,NULL)
	PHP_FE(glmapgrid2d,NULL)
	PHP_FE(glmapgrid2f,NULL)
	PHP_FE(glmaterialf,NULL)
	PHP_FE(glmaterialfv,NULL)
	PHP_FE(glmateriali,NULL)
	PHP_FE(glmaterialiv,NULL)
	PHP_FE(glmatrixmode,NULL)
	PHP_FE(glmultmatrixd,NULL)
	PHP_FE(glmultmatrixf,NULL)
	PHP_FE(glnewlist,NULL)
	PHP_FE(glnormal3b,NULL)
	PHP_FE(glnormal3bv,NULL)
	PHP_FE(glnormal3d,NULL)
	PHP_FE(glnormal3dv,NULL)
	PHP_FE(glnormal3f,NULL)
	PHP_FE(glnormal3fv,NULL)
	PHP_FE(glnormal3i,NULL)
	PHP_FE(glnormal3iv,NULL)
	PHP_FE(glnormal3s,NULL)
	PHP_FE(glnormal3sv,NULL)
	PHP_FE(glnormalpointer,NULL)
	PHP_FE(glortho,NULL)
	PHP_FE(glpassthrough,NULL)
	PHP_FE(glpixelmapfv,NULL)
	PHP_FE(glpixelmapuiv,NULL)
	PHP_FE(glpixelmapusv,NULL)
	PHP_FE(glpixelstoref,NULL)
	PHP_FE(glpixelstorei,NULL)
	PHP_FE(glpixeltransferf,NULL)
	PHP_FE(glpixeltransferi,NULL)
	PHP_FE(glpixelzoom,NULL)
	PHP_FE(glpointsize,NULL)
	PHP_FE(glPointParameterfv,NULL) // yoya
	PHP_FE(glpolygonmode,NULL)
	PHP_FE(glpolygonoffset,NULL)
	PHP_FE(glpolygonstipple,NULL)
	PHP_FE(glpopattrib,NULL)
	PHP_FE(glpopclientattrib,NULL)
	PHP_FE(glpopmatrix,NULL)
	PHP_FE(glpopname,NULL)
	PHP_FE(glprioritizetextures,NULL)
	PHP_FE(glpushattrib,NULL)
	PHP_FE(glpushclientattrib,NULL)
	PHP_FE(glpushmatrix,NULL)
	PHP_FE(glpushname,NULL)
	PHP_FE(glrasterpos2d,NULL)
	PHP_FE(glrasterpos2dv,NULL)
	PHP_FE(glrasterpos2f,NULL)
	PHP_FE(glrasterpos2fv,NULL)
	PHP_FE(glrasterpos2i,NULL)
	PHP_FE(glrasterpos2iv,NULL)
	PHP_FE(glrasterpos2s,NULL)
	PHP_FE(glrasterpos2sv,NULL)
	PHP_FE(glrasterpos3d,NULL)
	PHP_FE(glrasterpos3dv,NULL)
	PHP_FE(glrasterpos3f,NULL)
	PHP_FE(glrasterpos3fv,NULL)
	PHP_FE(glrasterpos3i,NULL)
	PHP_FE(glrasterpos3iv,NULL)
	PHP_FE(glrasterpos3s,NULL)
	PHP_FE(glrasterpos3sv,NULL)
	PHP_FE(glrasterpos4d,NULL)
	PHP_FE(glrasterpos4dv,NULL)
	PHP_FE(glrasterpos4f,NULL)
	PHP_FE(glrasterpos4fv,NULL)
	PHP_FE(glrasterpos4i,NULL)
	PHP_FE(glrasterpos4iv,NULL)
	PHP_FE(glrasterpos4s,NULL)
	PHP_FE(glrasterpos4sv,NULL)
	PHP_FE(glreadbuffer,NULL)
	PHP_FE(glreadpixels,force_ref_seventh_arg)
	PHP_FE(glreadpixels_yoya,NULL)
	PHP_FE(glrectd,NULL)
	PHP_FE(glrectdv,NULL)
	PHP_FE(glrectf,NULL)
	PHP_FE(glrectfv,NULL)
	PHP_FE(glrecti,NULL)
	PHP_FE(glrectiv,NULL)
	PHP_FE(glrects,NULL)
	PHP_FE(glrectsv,NULL)
	PHP_FE(glrendermode,NULL)
	PHP_FE(glrotated,NULL)
	PHP_FE(glrotatef,NULL)
	PHP_FE(glscaled,NULL)
	PHP_FE(glscalef,NULL)
	PHP_FE(glscissor,NULL)
	PHP_FE(glselectbuffer,NULL)
	PHP_FE(glshademodel,NULL)
	PHP_FE(glstencilfunc,NULL)
	PHP_FE(glstencilmask,NULL)
	PHP_FE(glstencilop,NULL)
	PHP_FE(gltexcoord1d,NULL)
	PHP_FE(gltexcoord1dv,NULL)
	PHP_FE(gltexcoord1f,NULL)
	PHP_FE(gltexcoord1fv,NULL)
	PHP_FE(gltexcoord1i,NULL)
	PHP_FE(gltexcoord1iv,NULL)
	PHP_FE(gltexcoord1s,NULL)
	PHP_FE(gltexcoord1sv,NULL)
	PHP_FE(gltexcoord2d,NULL)
	PHP_FE(gltexcoord2dv,NULL)
	PHP_FE(gltexcoord2f,NULL)
	PHP_FE(gltexcoord2fv,NULL)
	PHP_FE(gltexcoord2i,NULL)
	PHP_FE(gltexcoord2iv,NULL)
	PHP_FE(gltexcoord2s,NULL)
	PHP_FE(gltexcoord2sv,NULL)
	PHP_FE(gltexcoord3d,NULL)
	PHP_FE(gltexcoord3dv,NULL)
	PHP_FE(gltexcoord3f,NULL)
	PHP_FE(gltexcoord3fv,NULL)
	PHP_FE(gltexcoord3i,NULL)
	PHP_FE(gltexcoord3iv,NULL)
	PHP_FE(gltexcoord3s,NULL)
	PHP_FE(gltexcoord3sv,NULL)
	PHP_FE(gltexcoord4d,NULL)
	PHP_FE(gltexcoord4dv,NULL)
	PHP_FE(gltexcoord4f,NULL)
	PHP_FE(gltexcoord4fv,NULL)
	PHP_FE(gltexcoord4i,NULL)
	PHP_FE(gltexcoord4iv,NULL)
	PHP_FE(gltexcoord4s,NULL)
	PHP_FE(gltexcoord4sv,NULL)
	PHP_FE(gltexcoordpointer,NULL)
	PHP_FE(gltexenvf,NULL)
	PHP_FE(gltexenvfv,NULL)
	PHP_FE(gltexenvi,NULL)
	PHP_FE(gltexenviv,NULL)
	PHP_FE(gltexgend,NULL)
	PHP_FE(gltexgendv,NULL)
	PHP_FE(gltexgenf,NULL)
	PHP_FE(gltexgenfv,NULL)
	PHP_FE(gltexgeni,NULL)
	PHP_FE(gltexgeniv,NULL)
	PHP_FE(glteximage1d,NULL)
	PHP_FE(glteximage2d,NULL)
	PHP_FE(gltexparameterf,NULL)
	PHP_FE(gltexparameterfv,NULL)
	PHP_FE(gltexparameteri,NULL)
	PHP_FE(gltexparameteriv,NULL)
	PHP_FE(gltexsubimage1d,NULL)
	PHP_FE(gltexsubimage2d,NULL)
	PHP_FE(gltranslated,NULL)
	PHP_FE(gltranslatef,NULL)
	PHP_FE(glvertex2d,NULL)
	PHP_FE(glvertex2dv,NULL)
	PHP_FE(glvertex2f,NULL)
	PHP_FE(glvertex2fv,NULL)
	PHP_FE(glvertex2i,NULL)
	PHP_FE(glvertex2iv,NULL)
	PHP_FE(glvertex2s,NULL)
	PHP_FE(glvertex2sv,NULL)
	PHP_FE(glvertex3d,NULL)
	PHP_FE(glvertex3dv,NULL)
	PHP_FE(glvertex3f,NULL)
	PHP_FE(glvertex3fv,NULL)
	PHP_FE(glvertex3i,NULL)
	PHP_FE(glvertex3iv,NULL)
	PHP_FE(glvertex3s,NULL)
	PHP_FE(glvertex3sv,NULL)
	PHP_FE(glvertex4d,NULL)
	PHP_FE(glvertex4dv,NULL)
	PHP_FE(glvertex4f,NULL)
	PHP_FE(glvertex4fv,NULL)
	PHP_FE(glvertex4i,NULL)
	PHP_FE(glvertex4iv,NULL)
	PHP_FE(glvertex4s,NULL)
	PHP_FE(glvertex4sv,NULL)
	PHP_FE(glvertexpointer,NULL)
	PHP_FE(glviewport,NULL)

	// GLU Functions
	PHP_FE(gluerrorstring,NULL)
	PHP_FE(gluerrorunicodestringext,NULL)
	PHP_FE(glugetstring,NULL)
	PHP_FE(gluortho2d,NULL)
	PHP_FE(gluperspective,NULL)
	PHP_FE(glupickmatrix,NULL)
	PHP_FE(gluproject,NULL)
	PHP_FE(glunuproject,NULL)
	PHP_FE(gluscaleimage,force_ref_ninth_arg)
	PHP_FE(glulookat,NULL)
	PHP_FE(glubuild1dmipmaps,NULL)
	PHP_FE(glubuild2dmipmaps,NULL)
	PHP_FE(glunewquadric,NULL)
	PHP_FE(gludeletequadric,NULL)
	PHP_FE(gluquadricnormals,NULL)
	PHP_FE(gluquadrictexture,NULL)
	PHP_FE(gluquadricorientation,NULL)
	PHP_FE(gluquadricdrawstyle,NULL)
	PHP_FE(glucylinder,NULL)
	PHP_FE(gludisk,NULL)
	PHP_FE(glupartialdisk,NULL)
	PHP_FE(glusphere,NULL)
	PHP_FE(glunewtess,NULL)
	PHP_FE(gludeletetess,NULL)
	PHP_FE(glutessbeginpolygon,NULL)
	PHP_FE(glutessbegincontour,NULL)
	PHP_FE(glutessvertex,NULL)
	PHP_FE(glutessendcontour,NULL)
	PHP_FE(glutessendpolygon,NULL)
	PHP_FE(glutessproperty,NULL)
	PHP_FE(glutessnormal,NULL)
	PHP_FE(glugettessproperty,NULL)
	PHP_FE(glunewnurbsrenderer,NULL)
	PHP_FE(gludeletenurbsrenderer,NULL)
	PHP_FE(glubeginsurface,NULL)
	PHP_FE(glubegincurve,NULL)
	PHP_FE(gluendcurve,NULL)
	PHP_FE(gluendsurface,NULL)
	PHP_FE(glubegintrim,NULL)
	PHP_FE(gluendtrim,NULL)
	PHP_FE(glupwlcurve,NULL)
	PHP_FE(glunurbscurve,NULL)
	PHP_FE(glunurbssurface,NULL)
	PHP_FE(gluloadsamplingmatrices,NULL)
	PHP_FE(glunurbsproperty,NULL)
	PHP_FE(glugetnurbsproperty,force_ref_second_arg)
	PHP_FE(glutesscallback,NULL)

	// Provide access to glSelectBuffer
	PHP_FE(glgetselectbuffer,NULL)

	// OSMesa functions
#ifdef PHP_OPENGL_OSMESA
	PHP_FE(osmesacreatecontext,NULL)
	PHP_FE(osmesacreatecontextext,NULL)
	PHP_FE(osmesadestroycontext,NULL)
	PHP_FE(osmesamakecurrent,NULL)
	PHP_FE(osmesagetcurrentcontext,NULL)
	PHP_FE(osmesapixelstore,NULL)
	PHP_FE(osmesagetintegerv, force_ref_second_arg)
	PHP_FE(osmesagetdepthbuffer,NULL)
	PHP_FE(osmesagetcolorbuffer,NULL)
	// php_osmesa only
	PHP_FE(osmesagetbuffer,NULL)
#endif

	{NULL, NULL, NULL}
};

zend_module_entry opengl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"OpenGL functions", 
	opengl_functions, 
	PHP_MINIT(opengl), 
	PHP_MSHUTDOWN(opengl), 
	PHP_RINIT(opengl),
	PHP_RSHUTDOWN(opengl),
	PHP_MINFO(opengl),
#if ZEND_MODULE_API_NO >= 20010901
	NO_VERSION_YET,
#endif
	STANDARD_MODULE_PROPERTIES,
};

#ifdef COMPILE_DL_OPENGL
ZEND_GET_MODULE(opengl)
#endif

PHP_MINFO_FUNCTION(opengl)
{
	php_printf("OpenGL support enabled");
}

// {{{ void glaccum(long op, double value)
PHP_FUNCTION(glaccum)
{
	zval *op, *value;
	TWO_PARAM(op, value);
	convert_to_long(op);
	convert_to_double(value);
	glAccum((int)Z_LVAL_P(op),(float)Z_DVAL_P(value));
}
// }}}

// {{{ void glalphafunc(long func, double ref)
PHP_FUNCTION(glalphafunc)
{
	zval *func, *ref;
	TWO_PARAM(func, ref);
	convert_to_long(func);
	convert_to_double(ref);
	glAlphaFunc((int)Z_LVAL_P(func),(float)Z_DVAL_P(ref));
}
// }}}

// {{{ bool glaretexturesresident(long n, array textures, array residences)
PHP_FUNCTION(glaretexturesresident)
{
	zval *n, *textures, *residences;
	GLboolean return_int;
	GLuint *v_textures;
	GLboolean *v_residences;
	THREE_PARAM(n, textures, residences);
	convert_to_long(n);
	convert_to_array(textures);
	convert_to_array(residences);
	v_textures = php_array_to_uint_array(textures);
	v_residences = (GLboolean *)emalloc(sizeof(GLboolean) * Z_LVAL_P(n));
	return_int = glAreTexturesResident((int)Z_LVAL_P(n),v_textures,v_residences);
	boolean_array_to_php_array(v_residences,Z_LVAL_P(n),residences);
	efree(v_residences);
	RETURN_BOOL(return_int);
}
// }}}

// {{{ void glarrayelement(long i)
PHP_FUNCTION(glarrayelement)
{
	zval *i;
	ONE_PARAM(i);
	convert_to_long(i);
	glArrayElement((int)Z_LVAL_P(i));
}
// }}}

// {{{ void glbegin(long mode)
PHP_FUNCTION(glbegin)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glBegin((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glbindtexture(long target, long texture)
PHP_FUNCTION(glbindtexture)
{
	zval *target, *texture;
	TWO_PARAM(target, texture);
	convert_to_long(target);
	convert_to_long(texture);
	glBindTexture((int)Z_LVAL_P(target),(unsigned int)Z_LVAL_P(texture));
}
// }}}

// {{{ void glbitmap(long width, long height, double xorig, double yorig, double xmove, double ymove, string bitmap)
PHP_FUNCTION(glbitmap)
{
	zval *width, *height, *xorig, *yorig, *xmove, *ymove, *bitmap;
	SEVEN_PARAM(width, height, xorig, yorig, xmove, ymove, bitmap);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_double(xorig);
	convert_to_double(yorig);
	convert_to_double(xmove);
	convert_to_double(ymove);
	convert_to_string(bitmap);
	glBitmap((int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(float)Z_DVAL_P(xorig),(float)Z_DVAL_P(yorig),(float)Z_DVAL_P(xmove),(float)Z_DVAL_P(ymove),Z_STRVAL_P(bitmap));
}
// }}}

// {{{ void glblendfunc(long sfactor, long dfactor)
PHP_FUNCTION(glblendfunc)
{
	zval *sfactor, *dfactor;
	TWO_PARAM(sfactor, dfactor);
	convert_to_long(sfactor);
	convert_to_long(dfactor);
	glBlendFunc((int)Z_LVAL_P(sfactor),(int)Z_LVAL_P(dfactor));
}
// }}}

// {{{ void glcalllist(long list)
PHP_FUNCTION(glcalllist)
{
	zval *list;
	ONE_PARAM(list);
	convert_to_long(list);
	glCallList((unsigned int)Z_LVAL_P(list));
}
// }}}

// {{{ null glcalllists(long n, long type, array lists)
PHP_FUNCTION(glcalllists)
{
	zval *n, *type, *lists;
	GLvoid *v_lists;
	THREE_PARAM(n, type, lists);
	convert_to_long(n);
	convert_to_long(type);
	convert_to_array(lists);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_lists = php_array_to_short_array(lists);
		break;
	case GL_UNSIGNED_SHORT:
		v_lists = php_array_to_ushort_array(lists);
		break;
	case GL_INT:
		v_lists = php_array_to_int_array(lists);
		break;
	case GL_UNSIGNED_INT:
		v_lists = php_array_to_uint_array(lists);
		break;
	case GL_FLOAT:
		v_lists = php_array_to_float_array(lists);
		break;
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_2_BYTES:
	case GL_3_BYTES:
	case GL_4_BYTES:
		zend_error(E_ERROR,"These types are not supported GL_BYTE, GL_UNSIGNED_BYTE, GL_2_BYTES, GL_3_BYTES or GL_4_BYTES");
		RETURN_NULL();
		break;
	}
	glCallLists((int)Z_LVAL_P(n),(int)Z_LVAL_P(type),v_lists);
	efree(v_lists);
}
// }}}

// {{{ void glclear(long mask)
PHP_FUNCTION(glclear)
{
	zval *mask;
	ONE_PARAM(mask);
	convert_to_long(mask);
	glClear(Z_LVAL_P(mask));
}
// }}}

// {{{ void glclearaccum(double red, double green, double blue, double alpha)
PHP_FUNCTION(glclearaccum)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glClearAccum((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
}
// }}}

// {{{ void glclearcolor(double red, double green, double blue, double alpha)
PHP_FUNCTION(glclearcolor)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glClearColor((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
}
// }}}

// {{{ void glcleardepth(double depth)
PHP_FUNCTION(glcleardepth)
{
	zval *depth;
	ONE_PARAM(depth);
	convert_to_double(depth);
	glClearDepth(Z_DVAL_P(depth));
}
// }}}

// {{{ void glclearindex(double c)
PHP_FUNCTION(glclearindex)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_double(c);
	glClearIndex((float)Z_DVAL_P(c));
}
// }}}

// {{{ void glclearstencil(long s)
PHP_FUNCTION(glclearstencil)
{
	zval *s;
	ONE_PARAM(s);
	convert_to_long(s);
	glClearStencil((int)Z_LVAL_P(s));
}
// }}}

// {{{ void glclipplane(long plane, array equation)
PHP_FUNCTION(glclipplane)
{
	zval *plane, *equation;
	GLdouble *v_equation;
	TWO_PARAM(plane, equation);
	convert_to_long(plane);
	convert_to_array(equation);
	v_equation = php_array_to_double_array(equation);
	glClipPlane((int)Z_LVAL_P(plane),v_equation);
	efree(v_equation);
}
// }}}

// {{{ void glcolor3b(long red, long green, long blue)
PHP_FUNCTION(glcolor3b)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3b((signed char)Z_LVAL_P(red),(signed char)Z_LVAL_P(green),(signed char)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3bv(array v)
PHP_FUNCTION(glcolor3bv)
{
	zval *v;
	GLbyte *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_schar_array(v);
	glColor3bv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3d(double red, double green, double blue)
PHP_FUNCTION(glcolor3d)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	glColor3d(Z_DVAL_P(red),Z_DVAL_P(green),Z_DVAL_P(blue));
}
// }}}

// {{{ void glcolor3dv(array v)
PHP_FUNCTION(glcolor3dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glColor3dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3f(double red, double green, double blue)
PHP_FUNCTION(glcolor3f)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	glColor3f((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue));
}
// }}}

// {{{ void glcolor3fv(array v)
PHP_FUNCTION(glcolor3fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glColor3fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3i(long red, long green, long blue)
PHP_FUNCTION(glcolor3i)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3i((int)Z_LVAL_P(red),(int)Z_LVAL_P(green),(int)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3iv(array v)
PHP_FUNCTION(glcolor3iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glColor3iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3s(long red, long green, long blue)
PHP_FUNCTION(glcolor3s)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3s((short)Z_LVAL_P(red),(short)Z_LVAL_P(green),(short)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3sv(array v)
PHP_FUNCTION(glcolor3sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glColor3sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3ub(long red, long green, long blue)
PHP_FUNCTION(glcolor3ub)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3ub((unsigned char)Z_LVAL_P(red),(unsigned char)Z_LVAL_P(green),(unsigned char)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3ubv(array v)
PHP_FUNCTION(glcolor3ubv)
{
	zval *v;
	GLubyte *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_uchar_array(v);
	glColor3ubv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3ui(long red, long green, long blue)
PHP_FUNCTION(glcolor3ui)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3ui((unsigned int)Z_LVAL_P(red),(unsigned int)Z_LVAL_P(green),(unsigned int)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3uiv(array v)
PHP_FUNCTION(glcolor3uiv)
{
	zval *v;
	GLuint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_uint_array(v);
	glColor3uiv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor3us(long red, long green, long blue)
PHP_FUNCTION(glcolor3us)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3us((unsigned short)Z_LVAL_P(red),(unsigned short)Z_LVAL_P(green),(unsigned short)Z_LVAL_P(blue));
}
// }}}

// {{{ void glcolor3usv(array v)
PHP_FUNCTION(glcolor3usv)
{
	zval *v;
	GLushort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_ushort_array(v);
	glColor3usv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4b(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4b)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4b((signed char)Z_LVAL_P(red),(signed char)Z_LVAL_P(green),(signed char)Z_LVAL_P(blue),(signed char)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4bv(array v)
PHP_FUNCTION(glcolor4bv)
{
	zval *v;
	GLbyte *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_schar_array(v);
	glColor4bv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4d(double red, double green, double blue, double alpha)
PHP_FUNCTION(glcolor4d)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glColor4d(Z_DVAL_P(red),Z_DVAL_P(green),Z_DVAL_P(blue),Z_DVAL_P(alpha));
}
// }}}

// {{{ void glcolor4dv(array v)
PHP_FUNCTION(glcolor4dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glColor4dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4f(double red, double green, double blue, double alpha)
PHP_FUNCTION(glcolor4f)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glColor4f((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
}
// }}}

// {{{ void glcolor4fv(array v)
PHP_FUNCTION(glcolor4fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glColor4fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4i(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4i)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4i((int)Z_LVAL_P(red),(int)Z_LVAL_P(green),(int)Z_LVAL_P(blue),(int)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4iv(array v)
PHP_FUNCTION(glcolor4iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glColor4iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4s(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4s)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4s((short)Z_LVAL_P(red),(short)Z_LVAL_P(green),(short)Z_LVAL_P(blue),(short)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4sv(array v)
PHP_FUNCTION(glcolor4sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glColor4sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4ub(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4ub)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4ub((unsigned char)Z_LVAL_P(red),(unsigned char)Z_LVAL_P(green),(unsigned char)Z_LVAL_P(blue),(unsigned char)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4ubv(array v)
PHP_FUNCTION(glcolor4ubv)
{
	zval *v;
	GLubyte *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_uchar_array(v);
	glColor4ubv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4ui(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4ui)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4ui((unsigned int)Z_LVAL_P(red),(unsigned int)Z_LVAL_P(green),(unsigned int)Z_LVAL_P(blue),(unsigned int)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4uiv(array v)
PHP_FUNCTION(glcolor4uiv)
{
	zval *v;
	GLuint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_uint_array(v);
	glColor4uiv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolor4us(long red, long green, long blue, long alpha)
PHP_FUNCTION(glcolor4us)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4us((unsigned short)Z_LVAL_P(red),(unsigned short)Z_LVAL_P(green),(unsigned short)Z_LVAL_P(blue),(unsigned short)Z_LVAL_P(alpha));
}
// }}}

// {{{ void glcolor4usv(array v)
PHP_FUNCTION(glcolor4usv)
{
	zval *v;
	GLushort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_ushort_array(v);
	glColor4usv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glcolormask(boolean red, boolean green, boolean blue, boolean alpha)
PHP_FUNCTION(glcolormask)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_boolean(red);
	convert_to_boolean(green);
	convert_to_boolean(blue);
	convert_to_boolean(alpha);
	glColorMask(Z_BVAL_P(red),Z_BVAL_P(green),Z_BVAL_P(blue),Z_BVAL_P(alpha));
}
// }}}

// {{{ void glcolormaterial(long face, long mode)
PHP_FUNCTION(glcolormaterial)
{
	zval *face, *mode;
	TWO_PARAM(face, mode);
	convert_to_long(face);
	convert_to_long(mode);
	glColorMaterial((int)Z_LVAL_P(face),(int)Z_LVAL_P(mode));
}
// }}}

// {{{ null glcolorpointer(long size, long type, long stride, array pointer)
PHP_FUNCTION(glcolorpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);

	switch(Z_LVAL_P(type))
	{
	case GL_BYTE:
		v_pointer = php_array_to_byte_array(pointer);
		break;
	case GL_UNSIGNED_BYTE:
		v_pointer = php_array_to_ubyte_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_UNSIGNED_SHORT:
		v_pointer = php_array_to_ushort_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_UNSIGNED_INT:
		v_pointer = php_array_to_uint_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	default:
		zend_error(E_ERROR,"Wrong data type");
		RETURN_NULL();
		break;
	}
	glColorPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer); // yoya
}
// }}}

// {{{ void glcopypixels(long x, long y, long width, long height, long type)
PHP_FUNCTION(glcopypixels)
{
	zval *x, *y, *width, *height, *type;
	FIVE_PARAM(x, y, width, height, type);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(type);
	glCopyPixels((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(type));
}
// }}}

// {{{ void glcopyteximage1d(long target, long level, long internalFormat, long x, long y, long width, long border)
PHP_FUNCTION(glcopyteximage1d)
{
	zval *target, *level, *internalFormat, *x, *y, *width, *border;
	SEVEN_PARAM(target, level, internalFormat, x, y, width, border);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalFormat);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(border);
	glCopyTexImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalFormat),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(border));
}
// }}}

// {{{ void glcopyteximage2d(long target, long level, long internalFormat, long x, long y, long width, long height, long border)
PHP_FUNCTION(glcopyteximage2d)
{
	zval *target, *level, *internalFormat, *x, *y, *width, *height, *border;
	EIGHT_PARAM(target, level, internalFormat, x, y, width, height, border);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalFormat);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(border);
	glCopyTexImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalFormat),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(border));
}
// }}}

// {{{ void glcopytexsubimage1d(long target, long level, long xoffset, long x, long y, long width)
PHP_FUNCTION(glcopytexsubimage1d)
{
	zval *target, *level, *xoffset, *x, *y, *width;
	SIX_PARAM(target, level, xoffset, x, y, width);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	glCopyTexSubImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width));
}
// }}}

// {{{ void glcopytexsubimage2d(long target, long level, long xoffset, long yoffset, long x, long y, long width, long height)
PHP_FUNCTION(glcopytexsubimage2d)
{
	zval *target, *level, *xoffset, *yoffset, *x, *y, *width, *height;
	EIGHT_PARAM(target, level, xoffset, yoffset, x, y, width, height);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(yoffset);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glCopyTexSubImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(yoffset),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height));
}
// }}}

// {{{ void glcullface(long mode)
PHP_FUNCTION(glcullface)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glCullFace((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void gldeletelists(long list, long range)
PHP_FUNCTION(gldeletelists)
{
	zval *list, *range;
	TWO_PARAM(list, range);
	convert_to_long(list);
	convert_to_long(range);
	glDeleteLists((unsigned int)Z_LVAL_P(list),(int)Z_LVAL_P(range));
}
// }}}

// {{{ void gldeletetextures(long n, array textures)
PHP_FUNCTION(gldeletetextures)
{
	zval *n, *textures;
	GLuint *v_textures;
	TWO_PARAM(n, textures);
	convert_to_long(n);
	convert_to_array(textures);
	v_textures = php_array_to_uint_array(textures);
	glDeleteTextures((int)Z_LVAL_P(n),v_textures);
	efree(v_textures);
}
// }}}

// {{{ void gldepthfunc(long func)
PHP_FUNCTION(gldepthfunc)
{
	zval *func;
	ONE_PARAM(func);
	convert_to_long(func);
	glDepthFunc((int)Z_LVAL_P(func));
}
// }}}

// {{{ void gldepthmask(boolean flag)
PHP_FUNCTION(gldepthmask)
{
	zval *flag;
	ONE_PARAM(flag);
	convert_to_boolean(flag);
	glDepthMask(Z_BVAL_P(flag));
}
// }}}

// {{{ void gldepthrange(double zNear, double zFar)
PHP_FUNCTION(gldepthrange)
{
	zval *zNear, *zFar;
	TWO_PARAM(zNear, zFar);
	convert_to_double(zNear);
	convert_to_double(zFar);
	glDepthRange(Z_DVAL_P(zNear),Z_DVAL_P(zFar));
}
// }}}

// {{{ void gldisable(long cap)
PHP_FUNCTION(gldisable)
{
	zval *cap;
	ONE_PARAM(cap);
	convert_to_long(cap);
	glDisable((int)Z_LVAL_P(cap));
}
// }}}

// {{{ void gldisableclientstate(long array)
PHP_FUNCTION(gldisableclientstate)
{
	zval *array;
	ONE_PARAM(array);
	convert_to_long(array);
	glDisableClientState((int)Z_LVAL_P(array));
}
// }}}

// {{{ void gldrawarrays(long mode, long first, long count)
PHP_FUNCTION(gldrawarrays)
{
	zval *mode, *first, *count;
	THREE_PARAM(mode, first, count);
	convert_to_long(mode);
	convert_to_long(first);
	convert_to_long(count);
	glDrawArrays((int)Z_LVAL_P(mode),(int)Z_LVAL_P(first),(int)Z_LVAL_P(count));
}
// }}}

// {{{ void gldrawbuffer(long mode)
PHP_FUNCTION(gldrawbuffer)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glDrawBuffer((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void gldrawelements(long mode, long count, long type, array indices)
PHP_FUNCTION(gldrawelements)
{
	zval *mode, *count, *type, *indices;
	GLvoid *v_indices;
	FOUR_PARAM(mode, count, type, indices);
	convert_to_long(mode);
	convert_to_long(count);
	convert_to_long(type);
	convert_to_array(indices);
	switch(Z_LVAL_P(type))
	{
	case GL_UNSIGNED_BYTE:
		v_indices = php_array_to_ubyte_array(indices);
		break;
	case GL_UNSIGNED_SHORT:
		v_indices = php_array_to_ushort_array(indices);
		break;
	case GL_UNSIGNED_INT:
		v_indices = php_array_to_uint_array(indices);
		break;
	default:
		zend_error(E_ERROR,"Wrong data type");
		RETURN_NULL();
		break;
	}
	glDrawElements((int)Z_LVAL_P(mode),(int)Z_LVAL_P(count),(int)Z_LVAL_P(type),v_indices);
	efree(v_indices); // yoya
}
// }}}

// {{{ void gldrawpixels(long width, long height, long format, long type, array pixels)
PHP_FUNCTION(gldrawpixels)
{
	zval *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	FIVE_PARAM(width, height, format, type, pixels);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glDrawPixels((int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	efree(v_pixels);
}
// }}}

// {{{ void gledgeflag(boolean flag)
PHP_FUNCTION(gledgeflag)
{
	zval *flag;
	ONE_PARAM(flag);
	convert_to_boolean(flag);
	glEdgeFlag(Z_BVAL_P(flag));
}
// }}}

// {{{ void gledgeflagpointer(long stride, array pointer)
PHP_FUNCTION(gledgeflagpointer)
{
	zval *stride, *pointer;
	GLvoid *v_pointer;
	TWO_PARAM(stride, pointer);
	convert_to_long(stride);
	convert_to_array(pointer);
	v_pointer = php_array_to_uchar_array(pointer);
	glEdgeFlagPointer((int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer);
}
// }}}

// {{{ void gledgeflagv(array flag)
PHP_FUNCTION(gledgeflagv)
{
	zval *flag;
	GLboolean *v_flag;
	ONE_PARAM(flag);
	convert_to_array(flag);
	v_flag = php_array_to_uchar_array(flag);
	glEdgeFlagv(v_flag);
	efree(v_flag);
}
// }}}

// {{{ void glenable(long cap)
PHP_FUNCTION(glenable)
{
	zval *cap;
	ONE_PARAM(cap);
	convert_to_long(cap);
	glEnable((int)Z_LVAL_P(cap));
}
// }}}

// {{{ void glenableclientstate(long array)
PHP_FUNCTION(glenableclientstate)
{
	zval *array;
	ONE_PARAM(array);
	convert_to_long(array);
	glEnableClientState((int)Z_LVAL_P(array));
}
// }}}

// {{{ void glend()
PHP_FUNCTION(glend)
{
	
	ZERO_PARAM();
	glEnd();
}
// }}}

// {{{ void glendlist()
PHP_FUNCTION(glendlist)
{
	
	ZERO_PARAM();
	glEndList();
}
// }}}

// {{{ void glevalcoord1d(double u)
PHP_FUNCTION(glevalcoord1d)
{
	zval *u;
	ONE_PARAM(u);
	convert_to_double(u);
	glEvalCoord1d(Z_DVAL_P(u));
}
// }}}

// {{{ void glevalcoord1dv(array u)
PHP_FUNCTION(glevalcoord1dv)
{
	zval *u;
	GLdouble *v_u;
	ONE_PARAM(u);
	convert_to_array(u);
	v_u = php_array_to_double_array(u);
	glEvalCoord1dv(v_u);
	efre(v_u);
}
// }}}

// {{{ void glevalcoord1f(double u)
PHP_FUNCTION(glevalcoord1f)
{
	zval *u;
	ONE_PARAM(u);
	convert_to_double(u);
	glEvalCoord1f((float)Z_DVAL_P(u));
}
// }}}

// {{{ void glevalcoord1fv(array u)
PHP_FUNCTION(glevalcoord1fv)
{
	zval *u;
	GLfloat *v_u;
	ONE_PARAM(u);
	convert_to_array(u);
	v_u = php_array_to_float_array(u);
	glEvalCoord1fv(v_u);
	efree(v_u);
}
// }}}

// {{{ void glevalcoord2d(double u, double v)
PHP_FUNCTION(glevalcoord2d)
{
	zval *u, *v;
	TWO_PARAM(u, v);
	convert_to_double(u);
	convert_to_double(v);
	glEvalCoord2d(Z_DVAL_P(u),Z_DVAL_P(v));
}
// }}}

// {{{ void glevalcoord2dv(array u)
PHP_FUNCTION(glevalcoord2dv)
{
	zval *u;
	GLdouble *v_u;
	ONE_PARAM(u);
	convert_to_array(u);
	v_u = php_array_to_double_array(u);
	glEvalCoord2dv(v_u);
	efree(v_u);
}
// }}}

// {{{ void glevalcoord2f(double u, double v)
PHP_FUNCTION(glevalcoord2f)
{
	zval *u, *v;
	TWO_PARAM(u, v);
	convert_to_double(u);
	convert_to_double(v);
	glEvalCoord2f((float)Z_DVAL_P(u),(float)Z_DVAL_P(v));
}
// }}}

// {{{ void glevalcoord2fv(array u)
PHP_FUNCTION(glevalcoord2fv)
{
	zval *u;
	GLfloat *v_u;
	ONE_PARAM(u);
	convert_to_array(u);
	v_u = php_array_to_float_array(u);
	glEvalCoord2fv(v_u);
	efree(v_u);
}
// }}}

// {{{ void glevalmesh1(long mode, long i1, long i2)
PHP_FUNCTION(glevalmesh1)
{
	zval *mode, *i1, *i2;
	THREE_PARAM(mode, i1, i2);
	convert_to_long(mode);
	convert_to_long(i1);
	convert_to_long(i2);
	glEvalMesh1((int)Z_LVAL_P(mode),(int)Z_LVAL_P(i1),(int)Z_LVAL_P(i2));
}
// }}}

// {{{ void glevalmesh2(long mode, long i1, long i2, long j1, long j2)
PHP_FUNCTION(glevalmesh2)
{
	zval *mode, *i1, *i2, *j1, *j2;
	FIVE_PARAM(mode, i1, i2, j1, j2);
	convert_to_long(mode);
	convert_to_long(i1);
	convert_to_long(i2);
	convert_to_long(j1);
	convert_to_long(j2);
	glEvalMesh2((int)Z_LVAL_P(mode),(int)Z_LVAL_P(i1),(int)Z_LVAL_P(i2),(int)Z_LVAL_P(j1),(int)Z_LVAL_P(j2));
}
// }}}

// {{{ void glevalpoint1(long i)
PHP_FUNCTION(glevalpoint1)
{
	zval *i;
	ONE_PARAM(i);
	convert_to_long(i);
	glEvalPoint1((int)Z_LVAL_P(i));
}
// }}}

// {{{ void glevalpoint2(long i, long j)
PHP_FUNCTION(glevalpoint2)
{
	zval *i, *j;
	TWO_PARAM(i, j);
	convert_to_long(i);
	convert_to_long(j);
	glEvalPoint2((int)Z_LVAL_P(i),(int)Z_LVAL_P(j));
}
// }}}

// {{{ void glfeedbackbuffer(long size, long type, array buffer)
PHP_FUNCTION(glfeedbackbuffer)
{
	zval *size, *type, *buffer;
	GLfloat *v_buffer;
	THREE_PARAM(size, type, buffer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_array(buffer);
	v_buffer = (GLfloat *)emalloc(sizeof(GLfloat) * Z_LVAL_P(size));
	glFeedbackBuffer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),v_buffer);
	float_array_to_php_array(v_buffer,Z_LVAL_P(size),buffer);
	efree(v_buffer);
}
// }}}

// {{{ void glfinish()
PHP_FUNCTION(glfinish)
{
	
	ZERO_PARAM();
	glFinish();
}
// }}}

// {{{ void glflush()
PHP_FUNCTION(glflush)
{
	
	ZERO_PARAM();
	glFlush();
}
// }}}

// {{{ void glfogf(long pname, double param)
PHP_FUNCTION(glfogf)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_double(param);
	glFogf((int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void glfogfv(long pname, array params)
PHP_FUNCTION(glfogfv)
{
	zval *pname, *params;
	GLfloat *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glFogfv((int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glfogi(long pname, long param)
PHP_FUNCTION(glfogi)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_long(param);
	glFogi((int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void glfogiv(long pname, array params)
PHP_FUNCTION(glfogiv)
{
	zval *pname, *params;
	GLint *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glFogiv((int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glfrontface(long mode)
PHP_FUNCTION(glfrontface)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glFrontFace((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glfrustum(double left, double right, double bottom, double top, double zNear, double zFar)
PHP_FUNCTION(glfrustum)
{
	zval *left, *right, *bottom, *top, *zNear, *zFar;
	SIX_PARAM(left, right, bottom, top, zNear, zFar);
	convert_to_double(left);
	convert_to_double(right);
	convert_to_double(bottom);
	convert_to_double(top);
	convert_to_double(zNear);
	convert_to_double(zFar);
	glFrustum(Z_DVAL_P(left),Z_DVAL_P(right),Z_DVAL_P(bottom),Z_DVAL_P(top),Z_DVAL_P(zNear),Z_DVAL_P(zFar));
}
// }}}

// {{{ long glgenlists(long range)
PHP_FUNCTION(glgenlists)
{
	zval *range;
	unsigned int return_unsigned_int;
	ONE_PARAM(range);
	convert_to_long(range);
	return_unsigned_int = glGenLists((int)Z_LVAL_P(range));
	RETURN_LONG(return_unsigned_int);
}
// }}}

// {{{ void glgentextures(long n, array textures)
PHP_FUNCTION(glgentextures)
{
	zval *n, *textures;
	GLuint *v_textures;
	TWO_PARAM(n, textures);
	convert_to_long(n);
	convert_to_array(textures);
	v_textures = (GLuint *)emalloc(sizeof(GLuint) * Z_LVAL_P(n));
	glGenTextures((int)Z_LVAL_P(n),v_textures);
	uint_array_to_php_array(v_textures,Z_LVAL_P(n),textures);
	efree(v_textures);
}
// }}}

// {{{ void glgetbooleanv(long pname, mixed params)
PHP_FUNCTION(glgetbooleanv)
{
	zval *pname, *params;
	char num = 0;
	GLboolean *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	GET_STATE_NUM_PARAMS(Z_LVAL_P(pname), num);
	v_params = (GLboolean *)emalloc(num * sizeof(GLboolean));
	glGetBooleanv((unsigned int)Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_boolean(params);
		ZVAL_BOOL(params, *v_params);
	} else {
		convert_to_array(params);
		boolean_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ void glgetclipplane(long plane, array equation)
PHP_FUNCTION(glgetclipplane)
{
	zval *plane, *equation;
	GLdouble v_equation[4];
	TWO_PARAM(plane, equation);
	convert_to_long(plane);
	convert_to_array(equation);
	glGetClipPlane((int)Z_LVAL_P(plane),v_equation);
	double_array_to_php_array(v_equation,4,equation);
}
// }}}

// {{{ void glgetdoublev(long pname, mixed params)
PHP_FUNCTION(glgetdoublev)
{
	zval *pname, *params;
	char num = 0;
	GLdouble *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	GET_STATE_NUM_PARAMS(Z_LVAL_P(pname), num);
	v_params = (GLdouble *)emalloc(num * sizeof(GLdouble));
	glGetDoublev((int)Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_double(params);
		ZVAL_DOUBLE(params, *v_params);
	} else {
		convert_to_array(params);
		double_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ long glgeterror()
PHP_FUNCTION(glgeterror)
{
	int return_int;
	ZERO_PARAM();
	return_int = glGetError();
	RETURN_LONG(return_int);
}
// }}}

// {{{ void glgetfloatv(long pname, mixed params)
PHP_FUNCTION(glgetfloatv)
{
	zval *pname, *params;
	char num = 0;
	GLfloat *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	GET_STATE_NUM_PARAMS(Z_LVAL_P(pname), num);
	v_params = (GLfloat *)emalloc(num * sizeof(GLint));
	glGetFloatv((unsigned int)Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_double(params);
		ZVAL_DOUBLE(params, *v_params);
	} else {
		convert_to_array(params);
		float_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ void glgetintegerv(long pname, mixed params)
PHP_FUNCTION(glgetintegerv)
{
	zval *pname, *params;
	char num = 0;
	GLint *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	GET_STATE_NUM_PARAMS(Z_LVAL_P(pname), num);
	v_params = (GLint *)emalloc(num * sizeof(GLint));
	glGetIntegerv((unsigned int)Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_long(params);
		ZVAL_LONG(params, *v_params);
	} else {
		convert_to_array(params);
		int_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ void glgetlightfv(long light, long pname, mixed params)
PHP_FUNCTION(glgetlightfv)
{
	zval *light, *pname, *params;
	int num;
	GLfloat *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(pname);
	GET_LIGHT_NUM(Z_LVAL_P(pname), num);
	convert_to_long(light);
	v_params = (GLfloat *)emalloc(num * sizeof(GLfloat));
	glGetLightfv(Z_LVAL_P(light),Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_double(params);
		ZVAL_DOUBLE(params, *v_params);
	} else {
		convert_to_array(params);
		float_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ void glgetlightiv(long light, long pname, mixed params)
PHP_FUNCTION(glgetlightiv)
{
	zval *light, *pname, *params;
	int num;
	GLint *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(pname);
	GET_LIGHT_NUM(Z_LVAL_P(pname), num);
	convert_to_long(light);
	v_params = (GLint *)emalloc(num * sizeof(GLint));
	glGetLightiv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
	if (num == 1) {
		convert_to_long(params);
		ZVAL_LONG(params, *v_params);
	} else {
		convert_to_array(params);
		int_array_to_php_array(v_params,num,params);
	}
	efree(v_params);
}
// }}}

// {{{ void glgetmapdv(long target, long query, array v, long num)
PHP_FUNCTION(glgetmapdv)
{
	zval *target, *query, *v, *num;
	GLdouble *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLdouble *)emalloc(Z_LVAL_P(num) * sizeof(GLdouble));
	glGetMapdv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	double_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
// }}}

// {{{ void glgetmapfv(long target, long query, array v, long num)
PHP_FUNCTION(glgetmapfv)
{
	zval *target, *query, *v, *num;
	GLfloat *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLfloat *)emalloc(Z_LVAL_P(num) * sizeof(GLfloat));
	glGetMapfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	float_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
// }}}

// {{{ void glgetmapiv(long target, long query, array v, long num)
PHP_FUNCTION(glgetmapiv)
{
	zval *target, *query, *v, *num;
	GLint *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLint *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetMapiv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	int_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
// }}}

// {{{ void glgetmaterialfv(long face, long pname, array params)
PHP_FUNCTION(glgetmaterialfv)
{
	zval *face, *pname, *params;
	int num;
	GLfloat *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(pname);
	GET_MATERIAL_NUM(Z_LVAL_P(pname), num);
	convert_to_long(face);
	convert_to_array(params);
	v_params = (GLfloat *)emalloc(num * sizeof(GLfloat));
	glGetMaterialfv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgetmaterialiv(long face, long pname, array params)
PHP_FUNCTION(glgetmaterialiv)
{
	zval *face, *pname, *params;
	int num;
	GLint *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(pname);
	GET_MATERIAL_NUM(Z_LVAL_P(pname), num);
	convert_to_long(face);
	convert_to_array(params);
	v_params = (GLint *)emalloc(num * sizeof(GLint));
	glGetMaterialiv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgetpixelmapfv(long map, array values)
PHP_FUNCTION(glgetpixelmapfv)
{
	zval *map, *values;
	GLfloat *v_values;
	int size;

	TWO_PARAM(map, values);
	convert_to_long(map);
	convert_to_array(values);
	glGetIntegerv(Z_LVAL_P(map)+0x040,&size);
	v_values = (GLfloat *)emalloc(size * sizeof(GLfloat));
	glGetPixelMapfv(Z_LVAL_P(map),v_values);
	float_array_to_php_array(v_values,size,values);
	efree(v_values);
}
// }}}


// {{{ void glgetpixelmapuiv(long map, array values)
PHP_FUNCTION(glgetpixelmapuiv)
{
	zval *map, *values;
	GLuint *v_values;
	int size;

	TWO_PARAM(map, values);
	convert_to_long(map);
	convert_to_array(values);
	glGetIntegerv(Z_LVAL_P(map)+0x040,&size);
	v_values = (GLuint *)emalloc(size * sizeof(GLuint));
	glGetPixelMapuiv(Z_LVAL_P(map),v_values);
	uint_array_to_php_array(v_values,size,values);
	efree(v_values);
}
// }}}

// {{{ void glgetpixelmapusv(long map, array values)
PHP_FUNCTION(glgetpixelmapusv)
{
	zval *map, *values;
	GLushort *v_values;
	int size;

	TWO_PARAM(map, values);
	convert_to_long(map);
	convert_to_array(values);
	glGetIntegerv(Z_LVAL_P(map)+0x040,&size);
	v_values = (GLushort *)emalloc(size * sizeof(GLushort));
	glGetPixelMapusv(Z_LVAL_P(map),v_values);
	ushort_array_to_php_array(v_values,size,values);
	efree(v_values);
}
// }}}

// {{{ void glgetpointerv(long pname, array params)
PHP_FUNCTION(glgetpointerv)
{
	php_error(E_WARNING, "%s() is not implemented", get_active_function_name(TSRMLS_C));
	//TODO: do this function
	/*	zval *pname, *params;
	GLvoid* *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_long_array(params);
	glGetPointerv((int)Z_LVAL_P(pname),v_params);*/
}
// }}}

// {{{ void glgetpolygonstipple(string mask)
PHP_FUNCTION(glgetpolygonstipple)
{
	zval *mask;
	GLubyte *v_mask;
	v_mask = (GLubyte *)emalloc(32 * 32);
	ONE_PARAM(mask);
	convert_to_string(mask);
	glGetPolygonStipple(v_mask);
	ZVAL_STRING(mask,v_mask,0);
}
// }}}

// {{{ string glgetstring(long name)
PHP_FUNCTION(glgetstring)
{
	zval *name;
	GLubyte *ret;
	ONE_PARAM(name);
	convert_to_long(name);
	ret = (GLubyte*)glGetString(Z_LVAL_P(name));
	RETURN_STRING((char*)ret,1);
}
// }}}

// {{{ void glgettexenvfv(long target, long pname, array params)
PHP_FUNCTION(glgettexenvfv)
{
	zval *target, *pname, *params;
	int num;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(pname);
	GET_TEXENV_NUM(Z_LVAL_P(pname), num)
	convert_to_long(target);
	convert_to_array(params);
	v_params = (GLfloat*)emalloc(sizeof(GLfloat) * num);
	glGetTexEnvfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgettexenviv(long target, long pname, array params)
PHP_FUNCTION(glgettexenviv)
{
	zval *target, *pname, *params;
	int num;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(pname);
	GET_TEXENV_NUM(Z_LVAL_P(pname), num)
	convert_to_long(target);
	convert_to_array(params);
	v_params = (GLint*)emalloc(sizeof(GLint) * num);
	glGetTexEnviv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgettexgendv(long coord, long pname, array params)
PHP_FUNCTION(glgettexgendv)
{
	zval *coord, *pname, *params;
	int num;
	GLdouble *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(pname);
	GET_TEXGEN_NUM(Z_LVAL_P(pname), num);
	convert_to_long(coord);
	convert_to_array(params);
	v_params = (GLdouble*)emalloc(sizeof(GLdouble) * num);
	glGetTexGendv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgettexgenfv(long coord, long pname, array params)
PHP_FUNCTION(glgettexgenfv)
{
	zval *coord, *pname, *params;
	int num;
	GLfloat *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(pname);
	GET_TEXGEN_NUM(Z_LVAL_P(pname), num);
	convert_to_long(coord);
	convert_to_array(params);
	v_params = (GLfloat*)emalloc(sizeof(GLfloat) * num);
	glGetTexGenfv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgettexgeniv(long coord, long pname, array params)
PHP_FUNCTION(glgettexgeniv)
{
	zval *coord, *pname, *params;
	int num;
	GLint *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(pname);
	GET_TEXGEN_NUM(Z_LVAL_P(pname), num);
	convert_to_long(coord);
	convert_to_array(params);
	v_params = (GLint*)emalloc(sizeof(GLint) * num);
	glGetTexGeniv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgetteximage(long target, long level, long format, long type, array pixels)
PHP_FUNCTION(glgetteximage)
{
	zval *target, *level, *format, *type, *pixels;
	GLvoid *v_pixels;
	FIVE_PARAM(target, level, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels); // XXX
	glGetTexImage((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	// XXX long_array_to_php_array(v_pixels, v_pixels_len, pixels);        
}
// }}}

// {{{ void glgettexlevelparameterfv(long target, long level, long pname, array params)
PHP_FUNCTION(glgettexlevelparameterfv)
{
	zval *target, *level, *pname, *params;
	GLfloat *v_params;
	FOUR_PARAM(target, level, pname, params);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glGetTexLevelParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glgettexlevelparameteriv(long target, long level, long pname, array params)
PHP_FUNCTION(glgettexlevelparameteriv)
{
	zval *target, *level, *pname, *params;
	GLint *v_params;
	FOUR_PARAM(target, level, pname, params);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glGetTexLevelParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(pname),v_params);
	ZVAL_LONG(params, *v_params);
	efree(v_params);
}
// }}}

// {{{ void glgettexparameterfv(long target, long pname, array params)
PHP_FUNCTION(glgettexparameterfv)
{
	zval *target, *pname, *params;
	int num;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(pname);
	GET_TEXPARAMETER_NUM(Z_LVAL_P(pname), num);
	convert_to_long(target);
	convert_to_array(params);
	v_params = (GLfloat*)emalloc(sizeof(GLfloat) * num);
	glGetTexParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glgettexparameteriv(long target, long pname, array params)
PHP_FUNCTION(glgettexparameteriv)
{
	zval *target, *pname, *params;
	int num;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(pname);
	GET_TEXPARAMETER_NUM(Z_LVAL_P(pname), num);
	convert_to_long(target);
	convert_to_array(params);
	v_params = (GLint*)emalloc(sizeof(GLint) * num);
	glGetTexParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,num,params);
	efree(v_params);
}
// }}}

// {{{ void glhint(long target, long mode)
PHP_FUNCTION(glhint)
{
	zval *target, *mode;
	TWO_PARAM(target, mode);
	convert_to_long(target);
	convert_to_long(mode);
	glHint((int)Z_LVAL_P(target),(int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glindexmask(long mask)
PHP_FUNCTION(glindexmask)
{
	zval *mask;
	ONE_PARAM(mask);
	convert_to_long(mask);
	glIndexMask((unsigned int)Z_LVAL_P(mask));
}
// }}}

// {{{ void glindexpointer(long type, long stride, array pointer)
PHP_FUNCTION(glindexpointer)
{
	zval *type, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(type, stride, pointer);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glIndexPointer((int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer);
}
// }}}

// {{{ void glindexd(double c)
PHP_FUNCTION(glindexd)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_double(c);
	glIndexd(Z_DVAL_P(c));
}
// }}}

// {{{ void glindexdv(array c)
PHP_FUNCTION(glindexdv)
{
	zval *c;
	GLdouble *v_c;
	ONE_PARAM(c);
	convert_to_array(c);
	v_c = php_array_to_double_array(c);
	glIndexdv(v_c);
	efree(v_c);
}
// }}}

// {{{ void glindexf(double c)
PHP_FUNCTION(glindexf)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_double(c);
	glIndexf((float)Z_DVAL_P(c));
}
// }}}

// {{{ void glindexfv(array c)
PHP_FUNCTION(glindexfv)
{
	zval *c;
	GLfloat *v_c;
	ONE_PARAM(c);
	convert_to_array(c);
	v_c = php_array_to_float_array(c);
	glIndexfv(v_c);
	efree(v_c);
}
// }}}

// {{{ void glindexi(long c)
PHP_FUNCTION(glindexi)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_long(c);
	glIndexi((int)Z_LVAL_P(c));
}
// }}}

// {{{ void glindexiv(array c)
PHP_FUNCTION(glindexiv)
{
	zval *c;
	GLint *v_c;
	ONE_PARAM(c);
	convert_to_array(c);
	v_c = php_array_to_int_array(c);
	glIndexiv(v_c);
	efree(v_c);
}
// }}}

// {{{ void glindexs(long c)
PHP_FUNCTION(glindexs)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_long(c);
	glIndexs((short)Z_LVAL_P(c));
}
// }}}

// {{{ void glindexsv(array c)
PHP_FUNCTION(glindexsv)
{
	zval *c;
	GLshort *v_c;
	ONE_PARAM(c);
	convert_to_array(c);
	v_c = php_array_to_short_array(c);
	glIndexsv(v_c);
	efree(v_c);
}
// }}}

// {{{ void glindexub(long c)
PHP_FUNCTION(glindexub)
{
	zval *c;
	ONE_PARAM(c);
	convert_to_long(c);
	glIndexub((unsigned char)Z_LVAL_P(c));
}
// }}}

// {{{ void glindexubv(array c)
PHP_FUNCTION(glindexubv)
{
	zval *c;
	GLubyte *v_c;
	ONE_PARAM(c);
	convert_to_array(c);
	v_c = php_array_to_uchar_array(c);
	glIndexubv(v_c);
	efree(v_c);
}
// }}}

// {{{ void glinitnames()
PHP_FUNCTION(glinitnames)
{
	ZERO_PARAM();
	glInitNames();
}
// }}}

// {{{ void glinterleavedarrays(long format, long stride, array pointer)
PHP_FUNCTION(glinterleavedarrays)
{
	//TODO: Do this function
	/*zval *format, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(format, stride, pointer);
	convert_to_long(format);
	convert_to_long(stride);
	convert_to_array(pointer);
	v_pointer = php_array_to_long_array(pointer);
	glInterleavedArrays((int)Z_LVAL_P(format),(int)Z_LVAL_P(stride),v_pointer);*/
}
// }}}

// {{{ bool glisenabled(long cap)
PHP_FUNCTION(glisenabled)
{
	zval *cap;
	int return_int;
	ONE_PARAM(cap);
	convert_to_long(cap);
	return_int = glIsEnabled((int)Z_LVAL_P(cap));
	RETURN_BOOL(return_int);
}
// }}}

// {{{ bool glislist(long list)
PHP_FUNCTION(glislist)
{
	zval *list;
	int return_int;
	ONE_PARAM(list);
	convert_to_long(list);
	return_int = glIsList((unsigned int)Z_LVAL_P(list));
	RETURN_BOOL(return_int);
}
// }}}

// {{{ bool glistexture(long texture)
PHP_FUNCTION(glistexture)
{
	zval *texture;
	int return_int;
	ONE_PARAM(texture);
	convert_to_long(texture);
	return_int = glIsTexture((unsigned int)Z_LVAL_P(texture));
	RETURN_BOOL(return_int);
}
// }}}

// {{{ void gllightmodelf(long pname, double param)
PHP_FUNCTION(gllightmodelf)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_double(param);
	glLightModelf((int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void gllightmodelfv(long pname, array params)
PHP_FUNCTION(gllightmodelfv)
{
	zval *pname, *params;
	GLfloat *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glLightModelfv((int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gllightmodeli(long pname, long param)
PHP_FUNCTION(gllightmodeli)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_long(param);
	glLightModeli((int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void gllightmodeliv(long pname, array params)
PHP_FUNCTION(gllightmodeliv)
{
	zval *pname, *params;
	GLint *v_params;
	TWO_PARAM(pname, params);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glLightModeliv((int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gllightf(long light, long pname, double param)
PHP_FUNCTION(gllightf)
{
	zval *light, *pname, *param;
	THREE_PARAM(light, pname, param);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_double(param);
	glLightf((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void gllightfv(long light, long pname, array params)
PHP_FUNCTION(gllightfv)
{
	zval *light, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glLightfv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gllighti(long light, long pname, long param)
PHP_FUNCTION(gllighti)
{
	zval *light, *pname, *param;
	THREE_PARAM(light, pname, param);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_long(param);
	glLighti((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void gllightiv(long light, long pname, array params)
PHP_FUNCTION(gllightiv)
{
	zval *light, *pname, *params;
	GLint *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glLightiv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gllinestipple(long factor, long pattern)
PHP_FUNCTION(gllinestipple)
{
	zval *factor, *pattern;
	TWO_PARAM(factor, pattern);
	convert_to_long(factor);
	convert_to_long(pattern);
	glLineStipple((int)Z_LVAL_P(factor),(unsigned short)Z_LVAL_P(pattern));
}
// }}}

// {{{ void gllinewidth(double width)
PHP_FUNCTION(gllinewidth)
{
	zval *width;
	ONE_PARAM(width);
	convert_to_double(width);
	glLineWidth((float)Z_DVAL_P(width));
}
// }}}

// {{{ void gllistbase(long base)
PHP_FUNCTION(gllistbase)
{
	zval *base;
	ONE_PARAM(base);
	convert_to_long(base);
	glListBase((unsigned int)Z_LVAL_P(base));
}
// }}}

// {{{ void glloadidentity()
PHP_FUNCTION(glloadidentity)
{
	
	ZERO_PARAM();
	glLoadIdentity();
}
// }}}

// {{{ void glloadmatrixd(array m)
PHP_FUNCTION(glloadmatrixd)
{
	zval *m;
	GLdouble *v_m;
	ONE_PARAM(m);
	convert_to_array(m);
	v_m = php_array_to_double_array(m);
	glLoadMatrixd(v_m);
	efree(v_m);
}
// }}}

// {{{ void glloadmatrixf(array m)
PHP_FUNCTION(glloadmatrixf)
{
	zval *m;
	GLfloat *v_m;
	ONE_PARAM(m);
	convert_to_array(m);
	v_m = php_array_to_float_array(m);
	glLoadMatrixf(v_m);
	efree(v_m);
}
// }}}

// {{{ void glloadname(long name)
PHP_FUNCTION(glloadname)
{
	zval *name;
	ONE_PARAM(name);
	convert_to_long(name);
	glLoadName((unsigned int)Z_LVAL_P(name));
}
// }}}

// {{{ void gllogicop(long opcode)
PHP_FUNCTION(gllogicop)
{
	zval *opcode;
	ONE_PARAM(opcode);
	convert_to_long(opcode);
	glLogicOp((int)Z_LVAL_P(opcode));
}
// }}}

// {{{ void glmap1d(long target, double u1, double u2, long stride, long order, array points)
PHP_FUNCTION(glmap1d)
{
	zval *target, *u1, *u2, *stride, *order, *points;
	GLdouble *v_points;
	SIX_PARAM(target, u1, u2, stride, order, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(stride);
	convert_to_long(order);
	convert_to_array(points);
	v_points = php_array_to_double_array(points);
	glMap1d((int)Z_LVAL_P(target),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(stride),(int)Z_LVAL_P(order),v_points);
	efree(v_points);
}
// }}}

// {{{ void glmap1f(long target, double u1, double u2, long stride, long order, array points)
PHP_FUNCTION(glmap1f)
{
	zval *target, *u1, *u2, *stride, *order, *points;
	GLfloat *v_points;
	SIX_PARAM(target, u1, u2, stride, order, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(stride);
	convert_to_long(order);
	convert_to_array(points);
	v_points = php_array_to_float_array(points);
	glMap1f((int)Z_LVAL_P(target),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(stride),(int)Z_LVAL_P(order),v_points);
	efree(v_points);
}
// }}}

// {{{ void glmap2d(long target, double u1, double u2, long ustride, long uorder, double v1, double v2, long vstride, long vorder, array points)
PHP_FUNCTION(glmap2d)
{
	zval *target, *u1, *u2, *ustride, *uorder, *v1, *v2, *vstride, *vorder, *points;
	GLdouble *v_points;
	TEN_PARAM(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(ustride);
	convert_to_long(uorder);
	convert_to_double(v1);
	convert_to_double(v2);
	convert_to_long(vstride);
	convert_to_long(vorder);
	convert_to_array(points);
	v_points = php_array_to_double_array(points);
	glMap2d((int)Z_LVAL_P(target),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(ustride),(int)Z_LVAL_P(uorder),Z_DVAL_P(v1),Z_DVAL_P(v2),(int)Z_LVAL_P(vstride),(int)Z_LVAL_P(vorder),v_points);
	efree(v_points);
}
// }}}

// {{{ void glmap2f(long target, double u1, double u2, long ustride, long uorder, double v1, double v2, long vstride, long vorder, array points)
PHP_FUNCTION(glmap2f)
{
	zval *target, *u1, *u2, *ustride, *uorder, *v1, *v2, *vstride, *vorder, *points;
	GLfloat *v_points;
	TEN_PARAM(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(ustride);
	convert_to_long(uorder);
	convert_to_double(v1);
	convert_to_double(v2);
	convert_to_long(vstride);
	convert_to_long(vorder);
	convert_to_array(points);
	v_points = php_array_to_float_array(points);
	glMap2f((int)Z_LVAL_P(target),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(ustride),(int)Z_LVAL_P(uorder),(float)Z_DVAL_P(v1),(float)Z_DVAL_P(v2),(int)Z_LVAL_P(vstride),(int)Z_LVAL_P(vorder),v_points);
	efree(v_points);
}
// }}}

// {{{ void glmapgrid1d(long un, double u1, double u2)
PHP_FUNCTION(glmapgrid1d)
{
	zval *un, *u1, *u2;
	THREE_PARAM(un, u1, u2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	glMapGrid1d((int)Z_LVAL_P(un),Z_DVAL_P(u1),Z_DVAL_P(u2));
}
// }}}

// {{{ void glmapgrid1f(long un, double u1, double u2)
PHP_FUNCTION(glmapgrid1f)
{
	zval *un, *u1, *u2;
	THREE_PARAM(un, u1, u2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	glMapGrid1f((int)Z_LVAL_P(un),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2));
}
// }}}

// {{{ void glmapgrid2d(long un, double u1, double u2, long vn, double v1, double v2)
PHP_FUNCTION(glmapgrid2d)
{
	zval *un, *u1, *u2, *vn, *v1, *v2;
	SIX_PARAM(un, u1, u2, vn, v1, v2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(vn);
	convert_to_double(v1);
	convert_to_double(v2);
	glMapGrid2d((int)Z_LVAL_P(un),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(vn),Z_DVAL_P(v1),Z_DVAL_P(v2));
}
// }}}

// {{{ void glmapgrid2f(long un, double u1, double u2, long vn, double v1, double v2)
PHP_FUNCTION(glmapgrid2f)
{
	zval *un, *u1, *u2, *vn, *v1, *v2;
	SIX_PARAM(un, u1, u2, vn, v1, v2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(vn);
	convert_to_double(v1);
	convert_to_double(v2);
	glMapGrid2f((int)Z_LVAL_P(un),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(vn),(float)Z_DVAL_P(v1),(float)Z_DVAL_P(v2));
}
// }}}

// {{{ void glmaterialf(long face, long pname, double param)
PHP_FUNCTION(glmaterialf)
{
	zval *face, *pname, *param;
	THREE_PARAM(face, pname, param);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_double(param);
	glMaterialf((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void glmaterialfv(long face, long pname, array params)
PHP_FUNCTION(glmaterialfv)
{
	zval *face, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glMaterialfv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glmateriali(long face, long pname, long param)
PHP_FUNCTION(glmateriali)
{
	zval *face, *pname, *param;
	THREE_PARAM(face, pname, param);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_long(param);
	glMateriali((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void glmaterialiv(long face, long pname, array params)
PHP_FUNCTION(glmaterialiv)
{
	zval *face, *pname, *params;
	GLint *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glMaterialiv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glmatrixmode(long mode)
PHP_FUNCTION(glmatrixmode)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glMatrixMode((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glmultmatrixd(array m)
PHP_FUNCTION(glmultmatrixd)
{
	zval *m;
	GLdouble *v_m;
	ONE_PARAM(m);
	convert_to_array(m);
	v_m = php_array_to_double_array(m);
	glMultMatrixd(v_m);
	efree(v_m);
}
// }}}

// {{{ void glmultmatrixf(array m)
PHP_FUNCTION(glmultmatrixf)
{
	zval *m;
	GLfloat *v_m;
	ONE_PARAM(m);
	convert_to_array(m);
	v_m = php_array_to_float_array(m);
	glMultMatrixf(v_m);
	efree(v_m);
}
// }}}

// {{{ void glnewlist(long list, long mode)
PHP_FUNCTION(glnewlist)
{
	zval *list, *mode;
	TWO_PARAM(list, mode);
	convert_to_long(list);
	convert_to_long(mode);
	glNewList((unsigned int)Z_LVAL_P(list),(int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glnormal3b(long nx, long ny, long nz)
PHP_FUNCTION(glnormal3b)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3b((signed char)Z_LVAL_P(nx),(signed char)Z_LVAL_P(ny),(signed char)Z_LVAL_P(nz));
}
// }}}

// {{{ void glnormal3bv(array v)
PHP_FUNCTION(glnormal3bv)
{
	zval *v;
	GLbyte *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_schar_array(v);
	glNormal3bv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glnormal3d(double nx, double ny, double nz)
PHP_FUNCTION(glnormal3d)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_double(nx);
	convert_to_double(ny);
	convert_to_double(nz);
	glNormal3d(Z_DVAL_P(nx),Z_DVAL_P(ny),Z_DVAL_P(nz));
}
// }}}

// {{{ void glnormal3dv(array v)
PHP_FUNCTION(glnormal3dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glNormal3dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glnormal3f(double nx, double ny, double nz)
PHP_FUNCTION(glnormal3f)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_double(nx);
	convert_to_double(ny);
	convert_to_double(nz);
	glNormal3f((float)Z_DVAL_P(nx),(float)Z_DVAL_P(ny),(float)Z_DVAL_P(nz));
}
// }}}

// {{{ void glnormal3fv(array v)
PHP_FUNCTION(glnormal3fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glNormal3fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glnormal3i(long nx, long ny, long nz)
PHP_FUNCTION(glnormal3i)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3i((int)Z_LVAL_P(nx),(int)Z_LVAL_P(ny),(int)Z_LVAL_P(nz));
}
// }}}

// {{{ void glnormal3iv(array v)
PHP_FUNCTION(glnormal3iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glNormal3iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glnormal3s(long nx, long ny, long nz)
PHP_FUNCTION(glnormal3s)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3s((short)Z_LVAL_P(nx),(short)Z_LVAL_P(ny),(short)Z_LVAL_P(nz));
}
// }}}

// {{{ void glnormal3sv(array v)
PHP_FUNCTION(glnormal3sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glNormal3sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glnormalpointer(long type, long stride, array pointer)
PHP_FUNCTION(glnormalpointer)
{
	zval *type, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(type, stride, pointer);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_BYTE:
		v_pointer = php_array_to_byte_array(pointer);
		break;
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glNormalPointer((int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer);
}
// }}}

// {{{ void glortho(double left, double right, double bottom, double top, double zNear, double zFar)
PHP_FUNCTION(glortho)
{
	zval *left, *right, *bottom, *top, *zNear, *zFar;
	SIX_PARAM(left, right, bottom, top, zNear, zFar);
	convert_to_double(left);
	convert_to_double(right);
	convert_to_double(bottom);
	convert_to_double(top);
	convert_to_double(zNear);
	convert_to_double(zFar);
	glOrtho(Z_DVAL_P(left),Z_DVAL_P(right),Z_DVAL_P(bottom),Z_DVAL_P(top),Z_DVAL_P(zNear),Z_DVAL_P(zFar));
}
// }}}

// {{{ void glpassthrough(double token)
PHP_FUNCTION(glpassthrough)
{
	zval *token;
	ONE_PARAM(token);
	convert_to_double(token);
	glPassThrough((float)Z_DVAL_P(token));
}
// }}}

// {{{ void glpixelmapfv(long map, long mapsize, array values)
PHP_FUNCTION(glpixelmapfv)
{
	zval *map, *mapsize, *values;
	GLfloat *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_float_array(values);
	glPixelMapfv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
	efree(v_values);
}
// }}}

// {{{ void glpixelmapuiv(long map, long mapsize, array values)
PHP_FUNCTION(glpixelmapuiv)
{
	zval *map, *mapsize, *values;
	GLuint *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_uint_array(values);
	glPixelMapuiv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
	efree(v_values);
}
// }}}

// {{{ void glpixelmapusv(long map, long mapsize, array values)
PHP_FUNCTION(glpixelmapusv)
{
	zval *map, *mapsize, *values;
	GLushort *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_ushort_array(values);
	glPixelMapusv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
	efree(v_values);
}
// }}}

// {{{ void glpixelstoref(long pname, double param)
PHP_FUNCTION(glpixelstoref)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_double(param);
	glPixelStoref((int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void glpixelstorei(long pname, long param)
PHP_FUNCTION(glpixelstorei)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_long(param);
	glPixelStorei((int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void glpixeltransferf(long pname, double param)
PHP_FUNCTION(glpixeltransferf)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_double(param);
	glPixelTransferf((int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void glpixeltransferi(long pname, long param)
PHP_FUNCTION(glpixeltransferi)
{
	zval *pname, *param;
	TWO_PARAM(pname, param);
	convert_to_long(pname);
	convert_to_long(param);
	glPixelTransferi((int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void glpixelzoom(double xfactor, double yfactor)
PHP_FUNCTION(glpixelzoom)
{
	zval *xfactor, *yfactor;
	TWO_PARAM(xfactor, yfactor);
	convert_to_double(xfactor);
	convert_to_double(yfactor);
	glPixelZoom((float)Z_DVAL_P(xfactor),(float)Z_DVAL_P(yfactor));
}
// }}}

// {{{ void glpointsize(double size)
PHP_FUNCTION(glpointsize)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_double(size);
	glPointSize((float)Z_DVAL_P(size));
}
// }}}

// {{{ void glPointParameterfv(long pname, double *params) by yoya
PHP_FUNCTION(glPointParameterfv)
{
	zval *pname, *params;
	GLfloat *v_params;
	TWO_PARAM(pname, params)
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glPointParameterfv((int)Z_LVAL_P(pname), v_params);
	efree(v_params);
}
// }}}

// {{{ void glpolygonmode(long face, long mode)
PHP_FUNCTION(glpolygonmode)
{
	zval *face, *mode;
	TWO_PARAM(face, mode);
	convert_to_long(face);
	convert_to_long(mode);
	glPolygonMode((int)Z_LVAL_P(face),(int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glpolygonoffset(double factor, double units)
PHP_FUNCTION(glpolygonoffset)
{
	zval *factor, *units;
	TWO_PARAM(factor, units);
	convert_to_double(factor);
	convert_to_double(units);
	glPolygonOffset((float)Z_DVAL_P(factor),(float)Z_DVAL_P(units));
}
// }}}

// {{{ void glpolygonstipple(array mask)
PHP_FUNCTION(glpolygonstipple)
{
	zval *mask;
	GLubyte *v_mask;
	ONE_PARAM(mask);
	convert_to_array(mask);
	v_mask = php_array_to_uchar_array(mask);
	glPolygonStipple(v_mask);
	efree(v_mask);
}
// }}}

// {{{ void glpopattrib()
PHP_FUNCTION(glpopattrib)
{
	ZERO_PARAM();
	glPopAttrib();
}
// }}}

// {{{ void glpopclientattrib()
PHP_FUNCTION(glpopclientattrib)
{
	ZERO_PARAM();
	glPopClientAttrib();
}
// }}}

// {{{ void glpopmatrix()
PHP_FUNCTION(glpopmatrix)
{
	ZERO_PARAM();
	glPopMatrix();
}
// }}}

// {{{ void glpopname()
PHP_FUNCTION(glpopname)
{
	ZERO_PARAM();
	glPopName();
}
// }}}

// {{{ void glprioritizetextures(long n, array textures, array priorities)
PHP_FUNCTION(glprioritizetextures)
{
	zval *n, *textures, *priorities;
	GLuint *v_textures;
	GLclampf *v_priorities;
	THREE_PARAM(n, textures, priorities);
	convert_to_long(n);
	convert_to_array(textures);
	convert_to_array(priorities);
	v_textures = php_array_to_uint_array(textures);
	v_priorities = php_array_to_float_array(priorities);
	glPrioritizeTextures((int)Z_LVAL_P(n),v_textures,v_priorities);
	efree(v_textures);
	efree(v_priorities);
}
// }}}

// {{{ void glpushattrib(long mask)
PHP_FUNCTION(glpushattrib)
{
	zval *mask;
	ONE_PARAM(mask);
	convert_to_long(mask);
	glPushAttrib(Z_LVAL_P(mask));
}
// }}}

// {{{ void glpushclientattrib(long mask)
PHP_FUNCTION(glpushclientattrib)
{
	zval *mask;
	ONE_PARAM(mask);
	convert_to_long(mask);
	glPushClientAttrib(Z_LVAL_P(mask));
}
// }}}

// {{{ void glpushmatrix()
PHP_FUNCTION(glpushmatrix)
{
	
	ZERO_PARAM();
	glPushMatrix();
}
// }}}

// {{{ void glpushname(long name)
PHP_FUNCTION(glpushname)
{
	zval *name;
	ONE_PARAM(name);
	convert_to_long(name);
	glPushName((unsigned int)Z_LVAL_P(name));
}
// }}}

// {{{ void glrasterpos2d(double x, double y)
PHP_FUNCTION(glrasterpos2d)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_double(x);
	convert_to_double(y);
	glRasterPos2d(Z_DVAL_P(x),Z_DVAL_P(y));
}
// }}}

// {{{ void glrasterpos2dv(array v)
PHP_FUNCTION(glrasterpos2dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glRasterPos2dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos2f(double x, double y)
PHP_FUNCTION(glrasterpos2f)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_double(x);
	convert_to_double(y);
	glRasterPos2f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y));
}
// }}}

// {{{ void glrasterpos2fv(array v)
PHP_FUNCTION(glrasterpos2fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glRasterPos2fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos2i(long x, long y)
PHP_FUNCTION(glrasterpos2i)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_long(x);
	convert_to_long(y);
	glRasterPos2i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y));
}
// }}}

// {{{ void glrasterpos2iv(array v)
PHP_FUNCTION(glrasterpos2iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glRasterPos2iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos2s(long x, long y)
PHP_FUNCTION(glrasterpos2s)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_long(x);
	convert_to_long(y);
	glRasterPos2s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y));
}
// }}}

// {{{ void glrasterpos2sv(array v)
PHP_FUNCTION(glrasterpos2sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glRasterPos2sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos3d(double x, double y, double z)
PHP_FUNCTION(glrasterpos3d)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRasterPos3d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
// }}}

// {{{ void glrasterpos3dv(array v)
PHP_FUNCTION(glrasterpos3dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glRasterPos3dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos3f(double x, double y, double z)
PHP_FUNCTION(glrasterpos3f)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRasterPos3f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
// }}}

// {{{ void glrasterpos3fv(array v)
PHP_FUNCTION(glrasterpos3fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glRasterPos3fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos3i(long x, long y, long z)
PHP_FUNCTION(glrasterpos3i)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glRasterPos3i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z));
}
// }}}

// {{{ void glrasterpos3iv(array v)
PHP_FUNCTION(glrasterpos3iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glRasterPos3iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos3s(long x, long y, long z)
PHP_FUNCTION(glrasterpos3s)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glRasterPos3s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z));
}
// }}}

// {{{ void glrasterpos3sv(array v)
PHP_FUNCTION(glrasterpos3sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glRasterPos3sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos4d(double x, double y, double z, double w)
PHP_FUNCTION(glrasterpos4d)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glRasterPos4d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z),Z_DVAL_P(w));
}
// }}}

// {{{ void glrasterpos4dv(array v)
PHP_FUNCTION(glrasterpos4dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glRasterPos4dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos4f(double x, double y, double z, double w)
PHP_FUNCTION(glrasterpos4f)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glRasterPos4f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z),(float)Z_DVAL_P(w));
}
// }}}

// {{{ void glrasterpos4fv(array v)
PHP_FUNCTION(glrasterpos4fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glRasterPos4fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos4i(long x, long y, long z, long w)
PHP_FUNCTION(glrasterpos4i)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glRasterPos4i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z),(int)Z_LVAL_P(w));
}
// }}}

// {{{ void glrasterpos4iv(array v)
PHP_FUNCTION(glrasterpos4iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glRasterPos4iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glrasterpos4s(long x, long y, long z, long w)
PHP_FUNCTION(glrasterpos4s)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glRasterPos4s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z),(short)Z_LVAL_P(w));
}
// }}}

// {{{ void glrasterpos4sv(array v)
PHP_FUNCTION(glrasterpos4sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glRasterPos4sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glreadbuffer(long mode)
PHP_FUNCTION(glreadbuffer)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glReadBuffer((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glreadpixels(long x, long y, long width, long height, long format, long type, array pixels)
PHP_FUNCTION(glreadpixels)
{
	zval *x, *y, *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	int v_pixels_len;
	SEVEN_PARAM(x, y, width, height, format, type, pixels);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels_len = (int)Z_LVAL_P(width) * Z_LVAL_P(height)* sizeof(long);
	v_pixels = emalloc(v_pixels_len);
	glReadPixels((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	long_array_to_php_array(v_pixels, v_pixels_len, pixels);
	efree(v_pixels);
}
// }}}

// {{{ void glreadpixels_yoya(long x, long y, long width, long height, long format)
PHP_FUNCTION(glreadpixels_yoya)
{
	zval *x, *y, *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	int v_pixels_length;
	FIVE_PARAM(x, y, width, height, format);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	v_pixels_length = 4 * (int)Z_LVAL_P(width) * (int)Z_LVAL_P(height);
	v_pixels = (GLvoid *) emalloc(v_pixels_length);
	glReadPixels((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),GL_UNSIGNED_BYTE,v_pixels);
	RETURN_STRINGL(v_pixels, v_pixels_length, 0);
}
// }}}

// {{{ void glrectd(double x1, double y1, double x2, double y2)
PHP_FUNCTION(glrectd)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_double(x1);
	convert_to_double(y1);
	convert_to_double(x2);
	convert_to_double(y2);
	glRectd(Z_DVAL_P(x1),Z_DVAL_P(y1),Z_DVAL_P(x2),Z_DVAL_P(y2));
}
// }}}

// {{{ void glrectdv(array v1, array v2)
PHP_FUNCTION(glrectdv)
{
	zval *v1, *v2;
	GLdouble *v_v1;
	GLdouble *v_v2;
	TWO_PARAM(v1, v2);
	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_double_array(v1);
	v_v2 = php_array_to_double_array(v2);
	glRectdv(v_v1,v_v2);
	efree(v_v1);
	efree(v_v2);
}
// }}}

// {{{ void glrectf(double x1, double y1, double x2, double y2)
PHP_FUNCTION(glrectf)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_double(x1);
	convert_to_double(y1);
	convert_to_double(x2);
	convert_to_double(y2);
	glRectf((float)Z_DVAL_P(x1),(float)Z_DVAL_P(y1),(float)Z_DVAL_P(x2),(float)Z_DVAL_P(y2));
}
// }}}

// {{{ void glrectfv(array v1, array v2)
PHP_FUNCTION(glrectfv)
{
	zval *v1, *v2;
	GLfloat *v_v1;
	GLfloat *v_v2;
	TWO_PARAM(v1, v2);
	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_float_array(v1);
	v_v2 = php_array_to_float_array(v2);
	glRectfv(v_v1,v_v2);
	efree(v_v1);
	efree(v_v2);
}
// }}}

// {{{ void glrecti(long x1, long y1, long x2, long y2)
PHP_FUNCTION(glrecti)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_long(x1);
	convert_to_long(y1);
	convert_to_long(x2);
	convert_to_long(y2);
	glRecti((int)Z_LVAL_P(x1),(int)Z_LVAL_P(y1),(int)Z_LVAL_P(x2),(int)Z_LVAL_P(y2));
}
// }}}

// {{{ void glrectiv(array v1, array v2)
PHP_FUNCTION(glrectiv)
{
	zval *v1, *v2;
	GLint *v_v1;
	GLint *v_v2;
	TWO_PARAM(v1, v2);
	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_int_array(v1);
	v_v2 = php_array_to_int_array(v2);
	glRectiv(v_v1,v_v2);
	efree(v_v1);
	efree(v_v2);
}
// }}}

// {{{ void glrects(long x1, long y1, long x2, long y2)
PHP_FUNCTION(glrects)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_long(x1);
	convert_to_long(y1);
	convert_to_long(x2);
	convert_to_long(y2);
	glRects((short)Z_LVAL_P(x1),(short)Z_LVAL_P(y1),(short)Z_LVAL_P(x2),(short)Z_LVAL_P(y2));
}
// }}}

// {{{ void glrectsv(array v1, array v2)
PHP_FUNCTION(glrectsv)
{
	zval *v1, *v2;
	GLshort *v_v1;
	GLshort *v_v2;
	TWO_PARAM(v1, v2);
	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_short_array(v1);
	v_v2 = php_array_to_short_array(v2);
	glRectsv(v_v1,v_v2);
	efree(v_v1);
	efree(v_v2);
}
// }}}

// {{{ long glrendermode(long mode)
PHP_FUNCTION(glrendermode)
{
	zval *mode;
	int return_int;
	ONE_PARAM(mode);
	convert_to_long(mode);
	return_int = glRenderMode((int)Z_LVAL_P(mode));
	RETURN_LONG(return_int);
}
// }}}

// {{{ void glrotated(double angle, double x, double y, double z)
PHP_FUNCTION(glrotated)
{
	zval *angle, *x, *y, *z;
	FOUR_PARAM(angle, x, y, z);
	convert_to_double(angle);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRotated(Z_DVAL_P(angle),Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
// }}}

// {{{ void glrotatef(double angle, double x, double y, double z)
PHP_FUNCTION(glrotatef)
{
	zval *angle, *x, *y, *z;
	FOUR_PARAM(angle, x, y, z);
	convert_to_double(angle);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRotatef((float)Z_DVAL_P(angle),(float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
// }}}

// {{{ void glscaled(double x, double y, double z)
PHP_FUNCTION(glscaled)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glScaled(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
// }}}

// {{{ void glscalef(double x, double y, double z)
PHP_FUNCTION(glscalef)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glScalef((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
// }}}

// {{{ void glscissor(long x, long y, long width, long height)
PHP_FUNCTION(glscissor)
{
	zval *x, *y, *width, *height;
	FOUR_PARAM(x, y, width, height);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glScissor((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height));
}
// }}}

// {{{ void glselectbuffer(long size)
PHP_FUNCTION(glselectbuffer)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_long(size);
	if (select_buffer != NULL)
		efree(select_buffer);
	select_buffer_size = Z_LVAL_P(size);
	select_buffer = (GLuint*)emalloc(select_buffer_size * sizeof(GLuint));
	memset(select_buffer, 0, select_buffer_size);
	glSelectBuffer((int)select_buffer_size, select_buffer);
}
// }}}

// {{{ glgetselectbuffer()
PHP_FUNCTION(glgetselectbuffer)
{
	array_init(return_value);
	int_array_to_php_array(select_buffer, select_buffer_size, return_value);
}

// {{{ void glshademodel(long mode)
PHP_FUNCTION(glshademodel)
{
	zval *mode;
	ONE_PARAM(mode);
	convert_to_long(mode);
	glShadeModel((int)Z_LVAL_P(mode));
}
// }}}

// {{{ void glstencilfunc(long func, long ref, long mask)
PHP_FUNCTION(glstencilfunc)
{
	zval *func, *ref, *mask;
	THREE_PARAM(func, ref, mask);
	convert_to_long(func);
	convert_to_long(ref);
	convert_to_long(mask);
	glStencilFunc((int)Z_LVAL_P(func),(int)Z_LVAL_P(ref),(unsigned int)Z_LVAL_P(mask));
}
// }}}

// {{{ void glstencilmask(long mask)
PHP_FUNCTION(glstencilmask)
{
	zval *mask;
	ONE_PARAM(mask);
	convert_to_long(mask);
	glStencilMask((unsigned int)Z_LVAL_P(mask));
}
// }}}

// {{{ void glstencilop(long fail, long zfail, long zpass)
PHP_FUNCTION(glstencilop)
{
	zval *fail, *zfail, *zpass;
	THREE_PARAM(fail, zfail, zpass);
	convert_to_long(fail);
	convert_to_long(zfail);
	convert_to_long(zpass);
	glStencilOp((int)Z_LVAL_P(fail),(int)Z_LVAL_P(zfail),(int)Z_LVAL_P(zpass));
}
// }}}

// {{{ void gltexcoord1d(double s)
PHP_FUNCTION(gltexcoord1d)
{
	zval *s;
	ONE_PARAM(s);
	convert_to_double(s);
	glTexCoord1d(Z_DVAL_P(s));
}
// }}}

// {{{ void gltexcoord1dv(array v)
PHP_FUNCTION(gltexcoord1dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glTexCoord1dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord1f(double s)
PHP_FUNCTION(gltexcoord1f)
{
	zval *s;
	ONE_PARAM(s);
	convert_to_double(s);
	glTexCoord1f((float)Z_DVAL_P(s));
}
// }}}

// {{{ void gltexcoord1fv(array v)
PHP_FUNCTION(gltexcoord1fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glTexCoord1fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord1i(long s)
PHP_FUNCTION(gltexcoord1i)
{
	zval *s;
	ONE_PARAM(s);
	convert_to_long(s);
	glTexCoord1i((int)Z_LVAL_P(s));
}
// }}}

// {{{ void gltexcoord1iv(array v)
PHP_FUNCTION(gltexcoord1iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glTexCoord1iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord1s(long s)
PHP_FUNCTION(gltexcoord1s)
{
	zval *s;
	ONE_PARAM(s);
	convert_to_long(s);
	glTexCoord1s((short)Z_LVAL_P(s));
}
// }}}

// {{{ void gltexcoord1sv(array v)
PHP_FUNCTION(gltexcoord1sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glTexCoord1sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord2d(double s, double t)
PHP_FUNCTION(gltexcoord2d)
{
	zval *s, *t;
	TWO_PARAM(s, t);
	convert_to_double(s);
	convert_to_double(t);
	glTexCoord2d(Z_DVAL_P(s),Z_DVAL_P(t));
}
// }}}

// {{{ void gltexcoord2dv(array v)
PHP_FUNCTION(gltexcoord2dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glTexCoord2dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord2f(double s, double t)
PHP_FUNCTION(gltexcoord2f)
{
	zval *s, *t;
	TWO_PARAM(s, t);
	convert_to_double(s);
	convert_to_double(t);
	glTexCoord2f((float)Z_DVAL_P(s),(float)Z_DVAL_P(t));
}
// }}}

// {{{ void gltexcoord2fv(array v)
PHP_FUNCTION(gltexcoord2fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glTexCoord2fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord2i(long s, long t)
PHP_FUNCTION(gltexcoord2i)
{
	zval *s, *t;
	TWO_PARAM(s, t);
	convert_to_long(s);
	convert_to_long(t);
	glTexCoord2i((int)Z_LVAL_P(s),(int)Z_LVAL_P(t));
}
// }}}

// {{{ void gltexcoord2iv(array v)
PHP_FUNCTION(gltexcoord2iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glTexCoord2iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord2s(long s, long t)
PHP_FUNCTION(gltexcoord2s)
{
	zval *s, *t;
	TWO_PARAM(s, t);
	convert_to_long(s);
	convert_to_long(t);
	glTexCoord2s((short)Z_LVAL_P(s),(short)Z_LVAL_P(t));
}
// }}}

// {{{ void gltexcoord2sv(array v)
PHP_FUNCTION(gltexcoord2sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glTexCoord2sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord3d(double s, double t, double r)
PHP_FUNCTION(gltexcoord3d)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	glTexCoord3d(Z_DVAL_P(s),Z_DVAL_P(t),Z_DVAL_P(r));
}
// }}}

// {{{ void gltexcoord3dv(array v)
PHP_FUNCTION(gltexcoord3dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glTexCoord3dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord3f(double s, double t, double r)
PHP_FUNCTION(gltexcoord3f)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	glTexCoord3f((float)Z_DVAL_P(s),(float)Z_DVAL_P(t),(float)Z_DVAL_P(r));
}
// }}}

// {{{ void gltexcoord3fv(array v)
PHP_FUNCTION(gltexcoord3fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glTexCoord3fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord3i(long s, long t, long r)
PHP_FUNCTION(gltexcoord3i)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	glTexCoord3i((int)Z_LVAL_P(s),(int)Z_LVAL_P(t),(int)Z_LVAL_P(r));
}
// }}}

// {{{ void gltexcoord3iv(array v)
PHP_FUNCTION(gltexcoord3iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glTexCoord3iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord3s(long s, long t, long r)
PHP_FUNCTION(gltexcoord3s)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	glTexCoord3s((short)Z_LVAL_P(s),(short)Z_LVAL_P(t),(short)Z_LVAL_P(r));
}
// }}}

// {{{ void gltexcoord3sv(array v)
PHP_FUNCTION(gltexcoord3sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glTexCoord3sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord4d(double s, double t, double r, double q)
PHP_FUNCTION(gltexcoord4d)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	convert_to_double(q);
	glTexCoord4d(Z_DVAL_P(s),Z_DVAL_P(t),Z_DVAL_P(r),Z_DVAL_P(q));
}
// }}}

// {{{ void gltexcoord4dv(array v)
PHP_FUNCTION(gltexcoord4dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glTexCoord4dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord4f(double s, double t, double r, double q)
PHP_FUNCTION(gltexcoord4f)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	convert_to_double(q);
	glTexCoord4f((float)Z_DVAL_P(s),(float)Z_DVAL_P(t),(float)Z_DVAL_P(r),(float)Z_DVAL_P(q));
}
// }}}

// {{{ void gltexcoord4fv(array v)
PHP_FUNCTION(gltexcoord4fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glTexCoord4fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord4i(long s, long t, long r, long q)
PHP_FUNCTION(gltexcoord4i)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	convert_to_long(q);
	glTexCoord4i((int)Z_LVAL_P(s),(int)Z_LVAL_P(t),(int)Z_LVAL_P(r),(int)Z_LVAL_P(q));
}
// }}}

// {{{ void gltexcoord4iv(array v)
PHP_FUNCTION(gltexcoord4iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glTexCoord4iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoord4s(long s, long t, long r, long q)
PHP_FUNCTION(gltexcoord4s)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	convert_to_long(q);
	glTexCoord4s((short)Z_LVAL_P(s),(short)Z_LVAL_P(t),(short)Z_LVAL_P(r),(short)Z_LVAL_P(q));
}
// }}}

// {{{ void gltexcoord4sv(array v)
PHP_FUNCTION(gltexcoord4sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glTexCoord4sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void gltexcoordpointer(long size, long type, long stride, array pointer)
PHP_FUNCTION(gltexcoordpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glTexCoordPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer);
}
// }}}

// {{{ void gltexenvf(long target, long pname, double param)
PHP_FUNCTION(gltexenvf)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_double(param);
	glTexEnvf((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void gltexenvfv(long target, long pname, array params)
PHP_FUNCTION(gltexenvfv)
{
	zval *target, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexEnvfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexenvi(long target, long pname, long param)
PHP_FUNCTION(gltexenvi)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(param);
	glTexEnvi((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void gltexenviv(long target, long pname, array params)
PHP_FUNCTION(gltexenviv)
{
	zval *target, *pname, *params;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexEnviv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexgend(long coord, long pname, double param)
PHP_FUNCTION(gltexgend)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_double(param);
	glTexGend((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),Z_DVAL_P(param));
}
// }}}

// {{{ void gltexgendv(long coord, long pname, array params)
PHP_FUNCTION(gltexgendv)
{
	zval *coord, *pname, *params;
	GLdouble *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_double_array(params);
	glTexGendv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexgenf(long coord, long pname, double param)
PHP_FUNCTION(gltexgenf)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_double(param);
	glTexGenf((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void gltexgenfv(long coord, long pname, array params)
PHP_FUNCTION(gltexgenfv)
{
	zval *coord, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexGenfv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexgeni(long coord, long pname, long param)
PHP_FUNCTION(gltexgeni)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_long(param);
	glTexGeni((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void gltexgeniv(long coord, long pname, array params)
PHP_FUNCTION(gltexgeniv)
{
	zval *coord, *pname, *params;
	GLint *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexGeniv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void glteximage1d(long target, long level, long internalformat, long width, long border, long format, long type, array pixels)
PHP_FUNCTION(glteximage1d)
{
	zval *target, *level, *internalformat, *width, *border, *format, *type, *pixels;
	GLvoid *v_pixels;
	EIGHT_PARAM(target, level, internalformat, width, border, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalformat);
	convert_to_long(width);
	convert_to_long(border);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalformat),(int)Z_LVAL_P(width),(int)Z_LVAL_P(border),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	efree(v_pixels);
}
// }}}

// {{{ void glteximage2d(long target, long level, long internalformat, long width, long height, long border, long format, long type, array pixels)
PHP_FUNCTION(glteximage2d)
{
	zval *target, *level, *internalformat, *width, *height, *border, *format, *type, *pixels;
	GLvoid *v_pixels;
	NINE_PARAM(target, level, internalformat, width, height, border, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalformat);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(border);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	switch(Z_LVAL_P(type)) {
	case GL_UNSIGNED_BYTE:
		v_pixels = php_array_to_uchar_array(pixels);
		break;
	default:
		zend_error(E_ERROR,"Wrong data type");
		RETURN_NULL();
		break;
	}
	glTexImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalformat),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(border),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	efree(v_pixels);
}
// }}}

// {{{ void gltexparameterf(long target, long pname, double param)
PHP_FUNCTION(gltexparameterf)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_double(param);
	glTexParameterf((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
// }}}

// {{{ void gltexparameterfv(long target, long pname, array params)
PHP_FUNCTION(gltexparameterfv)
{
	zval *target, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexparameteri(long target, long pname, long param)
PHP_FUNCTION(gltexparameteri)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(param);
	glTexParameteri((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
// }}}

// {{{ void gltexparameteriv(long target, long pname, array params)
PHP_FUNCTION(gltexparameteriv)
{
	zval *target, *pname, *params;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	efree(v_params);
}
// }}}

// {{{ void gltexsubimage1d(long target, long level, long xoffset, long width, long format, long type, array pixels)
PHP_FUNCTION(gltexsubimage1d)
{
	zval *target, *level, *xoffset, *width, *format, *type, *pixels;
	GLvoid *v_pixels;
	SEVEN_PARAM(target, level, xoffset, width, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(width);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexSubImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(width),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	efree(v_pixels);
}
// }}}

// {{{ void gltexsubimage2d(long target, long level, long xoffset, long yoffset, long width, long height, long format, long type, array pixels)
PHP_FUNCTION(gltexsubimage2d)
{
	zval *target, *level, *xoffset, *yoffset, *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	NINE_PARAM(target, level, xoffset, yoffset, width, height, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(yoffset);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexSubImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(yoffset),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
	efree(v_pixels);
}
// }}}

// {{{ void gltranslated(double x, double y, double z)
PHP_FUNCTION(gltranslated)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glTranslated(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
// }}}

// {{{ void gltranslatef(double x, double y, double z)
PHP_FUNCTION(gltranslatef)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glTranslatef((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
// }}}

// {{{ void glvertex2d(double x, double y)
PHP_FUNCTION(glvertex2d)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_double(x);
	convert_to_double(y);
	glVertex2d(Z_DVAL_P(x),Z_DVAL_P(y));
}
// }}}

// {{{ void glvertex2dv(array v)
PHP_FUNCTION(glvertex2dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glVertex2dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex2f(double x, double y)
PHP_FUNCTION(glvertex2f)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_double(x);
	convert_to_double(y);
	glVertex2f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y));
}
// }}}

// {{{ void glvertex2fv(array v)
PHP_FUNCTION(glvertex2fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glVertex2fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex2i(long x, long y)
PHP_FUNCTION(glvertex2i)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_long(x);
	convert_to_long(y);
	glVertex2i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y));
}
// }}}

// {{{ void glvertex2iv(array v)
PHP_FUNCTION(glvertex2iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glVertex2iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex2s(long x, long y)
PHP_FUNCTION(glvertex2s)
{
	zval *x, *y;
	TWO_PARAM(x, y);
	convert_to_long(x);
	convert_to_long(y);
	glVertex2s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y));
}
// }}}

// {{{ void glvertex2sv(array v)
PHP_FUNCTION(glvertex2sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glVertex2sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex3d(double x, double y, double z)
PHP_FUNCTION(glvertex3d)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glVertex3d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
// }}}

// {{{ void glvertex3dv(array v)
PHP_FUNCTION(glvertex3dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glVertex3dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex3f(double x, double y, double z)
PHP_FUNCTION(glvertex3f)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glVertex3f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
// }}}

// {{{ void glvertex3fv(array v)
PHP_FUNCTION(glvertex3fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glVertex3fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex3i(long x, long y, long z)
PHP_FUNCTION(glvertex3i)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glVertex3i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z));
}
// }}}

// {{{ void glvertex3iv(array v)
PHP_FUNCTION(glvertex3iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glVertex3iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex3s(long x, long y, long z)
PHP_FUNCTION(glvertex3s)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glVertex3s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z));
}
// }}}

// {{{ void glvertex3sv(array v)
PHP_FUNCTION(glvertex3sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glVertex3sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex4d(double x, double y, double z, double w)
PHP_FUNCTION(glvertex4d)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glVertex4d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z),Z_DVAL_P(w));
}
// }}}

// {{{ void glvertex4dv(array v)
PHP_FUNCTION(glvertex4dv)
{
	zval *v;
	GLdouble *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glVertex4dv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex4f(double x, double y, double z, double w)
PHP_FUNCTION(glvertex4f)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glVertex4f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z),(float)Z_DVAL_P(w));
}
// }}}

// {{{ void glvertex4fv(array v)
PHP_FUNCTION(glvertex4fv)
{
	zval *v;
	GLfloat *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glVertex4fv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex4i(long x, long y, long z, long w)
PHP_FUNCTION(glvertex4i)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glVertex4i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z),(int)Z_LVAL_P(w));
}
// }}}

// {{{ void glvertex4iv(array v)
PHP_FUNCTION(glvertex4iv)
{
	zval *v;
	GLint *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_int_array(v);
	glVertex4iv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertex4s(long x, long y, long z, long w)
PHP_FUNCTION(glvertex4s)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glVertex4s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z),(short)Z_LVAL_P(w));
}
// }}}

// {{{ void glvertex4sv(array v)
PHP_FUNCTION(glvertex4sv)
{
	zval *v;
	GLshort *v_v;
	ONE_PARAM(v);
	convert_to_array(v);
	v_v = php_array_to_short_array(v);
	glVertex4sv(v_v);
	efree(v_v);
}
// }}}

// {{{ void glvertexpointer(long size, long type, long stride, array pointer)
PHP_FUNCTION(glvertexpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glVertexPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
	efree(v_pointer);
}
// }}}

// {{{ void glviewport(long x, long y, long width, long height)
PHP_FUNCTION(glviewport)
{
	zval *x, *y, *width, *height;
	FOUR_PARAM(x, y, width, height);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glViewport((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height));
}
// }}}


PHP_MINIT_FUNCTION(opengl)
{
	// Number of parameters map mask for glGet*() friends
	unsigned int glget_const[] = {
		GL_ACCUM_ALPHA_BITS , GL_ACCUM_BLUE_BITS , GL_ACCUM_CLEAR_VALUE ,
		GL_ACCUM_GREEN_BITS , GL_ACCUM_RED_BITS , GL_ALPHA_BIAS ,
		GL_ALPHA_BITS , GL_ALPHA_SCALE , GL_ALPHA_TEST , GL_ALPHA_TEST_FUNC ,
		GL_ALPHA_TEST_REF , GL_ATTRIB_STACK_DEPTH , GL_AUTO_NORMAL ,
		GL_AUX_BUFFERS , GL_BLEND , GL_BLEND_DST , GL_BLEND_SRC , GL_BLUE_BIAS
		, GL_BLUE_BITS , GL_BLUE_SCALE , GL_CLIP_PLANE0 , GL_CLIP_PLANE1 ,
		GL_CLIP_PLANE2 , GL_CLIP_PLANE3 , GL_CLIP_PLANE4 , GL_CLIP_PLANE5 ,
		GL_COLOR_ARRAY_EXT , GL_COLOR_ARRAY_COUNT_EXT ,
		GL_COLOR_ARRAY_SIZE_EXT , GL_COLOR_ARRAY_STRIDE_EXT ,
		GL_COLOR_ARRAY_TYPE_EXT , GL_COLOR_CLEAR_VALUE , GL_COLOR_MATERIAL ,
		GL_COLOR_MATERIAL_FACE , GL_COLOR_MATERIAL_PARAMETER ,
		GL_COLOR_WRITEMASK , GL_CULL_FACE , GL_CULL_FACE_MODE ,
		GL_CURRENT_COLOR , GL_CURRENT_INDEX , GL_CURRENT_NORMAL ,
		GL_CURRENT_RASTER_COLOR , GL_CURRENT_RASTER_DISTANCE ,
		GL_CURRENT_RASTER_INDEX , GL_CURRENT_RASTER_POSITION ,
		GL_CURRENT_RASTER_TEXTURE_COORDS , GL_CURRENT_RASTER_POSITION_VALID ,
		GL_CURRENT_TEXTURE_COORDS , GL_DEPTH_BIAS , GL_DEPTH_BITS ,
		GL_DEPTH_CLEAR_VALUE , GL_DEPTH_FUNC , GL_DEPTH_RANGE , GL_DEPTH_SCALE
		, GL_DEPTH_TEST , GL_DEPTH_WRITEMASK , GL_DITHER , GL_EDGE_FLAG ,
		GL_EDGE_FLAG_ARRAY_EXT , GL_EDGE_FLAG_ARRAY_COUNT_EXT ,
		GL_EDGE_FLAG_ARRAY_STRIDE_EXT , GL_FOG , GL_FOG_COLOR , GL_FOG_DENSITY
		, GL_FOG_END , GL_FOG_HINT , GL_FOG_INDEX , GL_FOG_MODE ,
		GL_FOG_START , GL_FRONT_FACE , GL_GREEN_BIAS , GL_GREEN_BITS ,
		GL_GREEN_SCALE , GL_INDEX_ARRAY_EXT , GL_INDEX_ARRAY_COUNT_EXT ,
		GL_INDEX_ARRAY_STRIDE_EXT , GL_INDEX_ARRAY_TYPE_EXT , GL_INDEX_BITS ,
		GL_INDEX_CLEAR_VALUE , GL_INDEX_MODE , GL_INDEX_OFFSET ,
		GL_INDEX_SHIFT , GL_INDEX_WRITEMASK , GL_LIGHT0 , GL_LIGHT1 ,
		GL_LIGHT2 , GL_LIGHT3 , GL_LIGHT4 , GL_LIGHT5 , GL_LIGHT6 , GL_LIGHT7
		, GL_LIGHTING , GL_LIGHT_MODEL_AMBIENT ,
		GL_LIGHT_MODEL_LOCAL_VIEWER , GL_LIGHT_MODEL_TWO_SIDE , GL_LINE_SMOOTH
		, GL_LINE_SMOOTH_HINT , GL_LINE_STIPPLE , GL_LINE_STIPPLE_PATTERN
		, GL_LINE_STIPPLE_REPEAT , GL_LINE_WIDTH ,
		GL_LINE_WIDTH_GRANULARITY , GL_LINE_WIDTH_RANGE , GL_LIST_BASE ,
		GL_LIST_INDEX , GL_LIST_MODE , GL_LOGIC_OP , GL_LOGIC_OP_MODE ,
		GL_MAP1_COLOR_4 , GL_MAP1_GRID_DOMAIN , GL_MAP1_GRID_SEGMENTS ,
		GL_MAP1_INDEX , GL_MAP1_NORMAL , GL_MAP1_TEXTURE_COORD_1 ,
		GL_MAP1_TEXTURE_COORD_2 , GL_MAP1_TEXTURE_COORD_3 , GL_MAP2_VERTEX_4 ,
		GL_MAP_COLOR , GL_MAP_STENCIL , GL_MATRIX_MODE ,
		GL_MAX_ATTRIB_STACK_DEPTH , GL_MAX_CLIP_PLANES , GL_MAX_EVAL_ORDER ,
		GL_MAX_LIGHTS , GL_MAX_LIST_NESTING , GL_MAX_MODELVIEW_STACK_DEPTH ,
		GL_MAX_NAME_STACK_DEPTH , GL_MAX_PIXEL_MAP_TABLE ,
		GL_MAX_PROJECTION_STACK_DEPTH , GL_MAX_TEXTURE_SIZE ,
		GL_MAX_TEXTURE_STACK_DEPTH , GL_MAX_VIEWPORT_DIMS ,
		GL_MODELVIEW_MATRIX , GL_MODELVIEW_STACK_DEPTH , GL_NAME_STACK_DEPTH ,
		GL_NORMAL_ARRAY_EXT , GL_NORMAL_ARRAY_COUNT_EXT ,
		GL_NORMAL_ARRAY_STRIDE_EXT , GL_NORMAL_ARRAY_TYPE_EXT , GL_NORMALIZE ,
		GL_PACK_ALIGNMENT , GL_PACK_LSB_FIRST , GL_PACK_ROW_LENGTH ,
		GL_PACK_SKIP_PIXELS , GL_PACK_SKIP_ROWS , GL_PACK_SWAP_BYTES ,
		GL_PERSPECTIVE_CORRECTION_HINT , GL_PIXEL_MAP_A_TO_A_SIZE ,
		GL_PIXEL_MAP_B_TO_B_SIZE , GL_PIXEL_MAP_G_TO_G_SIZE ,
		GL_PIXEL_MAP_I_TO_A_SIZE , GL_PIXEL_MAP_I_TO_B_SIZE ,
		GL_PIXEL_MAP_I_TO_G_SIZE , GL_PIXEL_MAP_I_TO_I_SIZE ,
		GL_PIXEL_MAP_I_TO_R_SIZE , GL_PIXEL_MAP_R_TO_R_SIZE ,
		GL_PIXEL_MAP_S_TO_S_SIZE , GL_POINT_SIZE , GL_POINT_SIZE_GRANULARITY ,
		GL_POINT_SIZE_RANGE , GL_POINT_SMOOTH , GL_POINT_SMOOTH_HINT ,
		GL_POLYGON_MODE , GL_POLYGON_SMOOTH , GL_POLYGON_SMOOTH_HINT ,
		GL_POLYGON_STIPPLE , GL_PROJECTION_MATRIX , GL_PROJECTION_STACK_DEPTH
		, GL_READ_BUFFER , GL_RED_BIAS , GL_RED_BITS , GL_RED_SCALE ,
		GL_RENDER_MODE , GL_RGBA_MODE , GL_SCISSOR_BOX , GL_SCISSOR_TEST ,
		GL_SHADE_MODEL , GL_STENCIL_BITS , GL_STENCIL_CLEAR_VALUE ,
		GL_STENCIL_FAIL , GL_STENCIL_FUNC , GL_STENCIL_PASS_DEPTH_FAIL ,
		GL_STENCIL_PASS_DEPTH_PASS , GL_STENCIL_REF , GL_STENCIL_TEST ,
		GL_STENCIL_VALUE_MASK , GL_STENCIL_WRITEMASK , GL_STEREO ,
		GL_SUBPIXEL_BITS , GL_TEXTURE_1D , GL_TEXTURE_2D ,
		GL_TEXTURE_COORD_ARRAY_EXT , GL_TEXTURE_COORD_ARRAY_COUNT_EXT ,
		GL_TEXTURE_COORD_ARRAY_SIZE_EXT , GL_TEXTURE_COORD_ARRAY_STRIDE_EXT ,
		GL_TEXTURE_COORD_ARRAY_TYPE_EXT , GL_TEXTURE_GEN_S , GL_TEXTURE_GEN_T
		, GL_TEXTURE_GEN_R , GL_TEXTURE_GEN_Q , GL_TEXTURE_MATRIX ,
		GL_TEXTURE_STACK_DEPTH , GL_UNPACK_ALIGNMENT , GL_UNPACK_LSB_FIRST ,
		GL_UNPACK_ROW_LENGTH , GL_UNPACK_SKIP_PIXELS , GL_UNPACK_SKIP_ROWS ,
		GL_UNPACK_SWAP_BYTES , GL_VERTEX_ARRAY_EXT , GL_VERTEX_ARRAY_COUNT_EXT
		, GL_VERTEX_ARRAY_SIZE_EXT , GL_VERTEX_ARRAY_STRIDE_EXT ,
		GL_VERTEX_ARRAY_TYPE_EXT , GL_VIEWPORT , GL_ZOOM_X , GL_ZOOM_Y ,
		0
	};
	unsigned int i;

	for (i = 0; glget_const[i] != 0; i++)
		if (glget_const[i] > glget_max)
				glget_max = glget_const[i];

	glget_mask = (char*)pemalloc((glget_max+1) * sizeof(char), 1);

	for (i = 0; i < glget_max; i++)
		glget_mask[i] = -1;

	glget_mask[GL_ACCUM_ALPHA_BITS] = 1;
	glget_mask[GL_ACCUM_BLUE_BITS] = 1;
	glget_mask[GL_ACCUM_CLEAR_VALUE] = 4;
	glget_mask[GL_ACCUM_GREEN_BITS] = 1;
	glget_mask[GL_ACCUM_RED_BITS] = 1;
	glget_mask[GL_ALPHA_BIAS] = 1;
	glget_mask[GL_ALPHA_BITS] = 1;
	glget_mask[GL_ALPHA_SCALE] = 1;
	glget_mask[GL_ALPHA_TEST] = 1;
	glget_mask[GL_ALPHA_TEST_FUNC] = 1;
	glget_mask[GL_ALPHA_TEST_REF] = 1;
	glget_mask[GL_ATTRIB_STACK_DEPTH] = 1;
	glget_mask[GL_AUTO_NORMAL] = 1;
	glget_mask[GL_AUX_BUFFERS] = 1;
	glget_mask[GL_BLEND] = 1;
	glget_mask[GL_BLEND_DST] = 1;
	glget_mask[GL_BLEND_SRC] = 1;
	glget_mask[GL_BLUE_BIAS] = 1;
	glget_mask[GL_BLUE_BITS] = 1;
	glget_mask[GL_BLUE_SCALE] = 1;
	glget_mask[GL_CLIP_PLANE0] = 1;
	glget_mask[GL_CLIP_PLANE1] = 1;
	glget_mask[GL_CLIP_PLANE2] = 1;
	glget_mask[GL_CLIP_PLANE3] = 1;
	glget_mask[GL_CLIP_PLANE4] = 1;
	glget_mask[GL_CLIP_PLANE5] = 1;
	glget_mask[GL_COLOR_ARRAY_EXT] = 1;
	glget_mask[GL_COLOR_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_COLOR_ARRAY_SIZE_EXT] = 1;
	glget_mask[GL_COLOR_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_COLOR_ARRAY_TYPE_EXT] = 1;
	glget_mask[GL_COLOR_CLEAR_VALUE] = 4;
	glget_mask[GL_COLOR_MATERIAL] = 1;
	glget_mask[GL_COLOR_MATERIAL_FACE] = 1;
	glget_mask[GL_COLOR_MATERIAL_PARAMETER] = 1;
	glget_mask[GL_COLOR_WRITEMASK] = 4;
	glget_mask[GL_CULL_FACE] = 1;
	glget_mask[GL_CULL_FACE_MODE] = 1;
	glget_mask[GL_CURRENT_COLOR] = 4;
	glget_mask[GL_CURRENT_INDEX] = 1;
	glget_mask[GL_CURRENT_NORMAL] = 3;
	glget_mask[GL_CURRENT_RASTER_COLOR] = 4;
	glget_mask[GL_CURRENT_RASTER_DISTANCE] = 1;
	glget_mask[GL_CURRENT_RASTER_INDEX] = 1;
	glget_mask[GL_CURRENT_RASTER_POSITION] = 4;
	glget_mask[GL_CURRENT_RASTER_TEXTURE_COORDS] = 4;
	glget_mask[GL_CURRENT_RASTER_POSITION_VALID] = 1;
	glget_mask[GL_CURRENT_TEXTURE_COORDS] = 4;
	glget_mask[GL_DEPTH_BIAS] = 1;
	glget_mask[GL_DEPTH_BITS] = 1;
	glget_mask[GL_DEPTH_CLEAR_VALUE] = 1;
	glget_mask[GL_DEPTH_FUNC] = 1;
	glget_mask[GL_DEPTH_RANGE] = 2;
	glget_mask[GL_DEPTH_SCALE] = 1;
	glget_mask[GL_DEPTH_TEST] = 1;
	glget_mask[GL_DEPTH_WRITEMASK] = 1;
	glget_mask[GL_DITHER] = 1;
	glget_mask[GL_EDGE_FLAG] = 1;
	glget_mask[GL_EDGE_FLAG_ARRAY_EXT] = 1;
	glget_mask[GL_EDGE_FLAG_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_EDGE_FLAG_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_FOG] = 1;
	glget_mask[GL_FOG_COLOR] = 4;
	glget_mask[GL_FOG_DENSITY] = 1;
	glget_mask[GL_FOG_END] = 1;
	glget_mask[GL_FOG_HINT] = 1;
	glget_mask[GL_FOG_INDEX] = 1;
	glget_mask[GL_FOG_MODE] = 1;
	glget_mask[GL_FOG_START] = 1;
	glget_mask[GL_FRONT_FACE] = 1;
	glget_mask[GL_GREEN_BIAS] = 1;
	glget_mask[GL_GREEN_BITS] = 1;
	glget_mask[GL_GREEN_SCALE] = 1;
	glget_mask[GL_INDEX_ARRAY_EXT] = 1;
	glget_mask[GL_INDEX_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_INDEX_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_INDEX_ARRAY_TYPE_EXT] = 1;
	glget_mask[GL_INDEX_BITS] = 1;
	glget_mask[GL_INDEX_CLEAR_VALUE] = 1;
	glget_mask[GL_INDEX_MODE] = 1;
	glget_mask[GL_INDEX_OFFSET] = 1;
	glget_mask[GL_INDEX_SHIFT] = 1;
	glget_mask[GL_INDEX_WRITEMASK] = 1;
	glget_mask[GL_LIGHT0] = 1;
	glget_mask[GL_LIGHT1] = 1;
	glget_mask[GL_LIGHT2] = 1;
	glget_mask[GL_LIGHT3] = 1;
	glget_mask[GL_LIGHT4] = 1;
	glget_mask[GL_LIGHT5] = 1;
	glget_mask[GL_LIGHT6] = 1;
	glget_mask[GL_LIGHT7] = 1;
	glget_mask[GL_LIGHTING] = 1;
	glget_mask[GL_LIGHT_MODEL_AMBIENT] = 4;
	glget_mask[GL_LIGHT_MODEL_LOCAL_VIEWER] = 1;
	glget_mask[GL_LIGHT_MODEL_TWO_SIDE] = 1;
	glget_mask[GL_LINE_SMOOTH] = 1;
	glget_mask[GL_LINE_SMOOTH_HINT] = 1;
	glget_mask[GL_LINE_STIPPLE] = 1;
	glget_mask[GL_LINE_STIPPLE_PATTERN] = 1;
	glget_mask[GL_LINE_STIPPLE_REPEAT] = 1;
	glget_mask[GL_LINE_WIDTH] = 1;
	glget_mask[GL_LINE_WIDTH_GRANULARITY] = 1;
	glget_mask[GL_LINE_WIDTH_RANGE] = 2;
	glget_mask[GL_LIST_BASE] = 1;
	glget_mask[GL_LIST_INDEX] = 1;
	glget_mask[GL_LIST_MODE] = 1;
	glget_mask[GL_LOGIC_OP] = 1;
	glget_mask[GL_LOGIC_OP_MODE] = 1;
	glget_mask[GL_MAP1_COLOR_4] = 1;
	glget_mask[GL_MAP1_GRID_DOMAIN] = 2;
	glget_mask[GL_MAP1_GRID_SEGMENTS] = 1;
	glget_mask[GL_MAP1_INDEX] = 1;
	glget_mask[GL_MAP1_NORMAL] = 1;
	glget_mask[GL_MAP1_TEXTURE_COORD_1] = 1;
	glget_mask[GL_MAP1_TEXTURE_COORD_2] = 1;
	glget_mask[GL_MAP1_TEXTURE_COORD_3] = 1;
	glget_mask[GL_MAP2_VERTEX_4] = 1;
	glget_mask[GL_MAP_COLOR] = 1;
	glget_mask[GL_MAP_STENCIL] = 1;
	glget_mask[GL_MATRIX_MODE] = 1;
	glget_mask[GL_MAX_ATTRIB_STACK_DEPTH] = 1;
	glget_mask[GL_MAX_CLIP_PLANES] = 1;
	glget_mask[GL_MAX_EVAL_ORDER] = 1;
	glget_mask[GL_MAX_LIGHTS] = 1;
	glget_mask[GL_MAX_LIST_NESTING] = 1;
	glget_mask[GL_MAX_MODELVIEW_STACK_DEPTH] = 1;
	glget_mask[GL_MAX_NAME_STACK_DEPTH] = 1;
	glget_mask[GL_MAX_PIXEL_MAP_TABLE] = 1;
	glget_mask[GL_MAX_PROJECTION_STACK_DEPTH] = 1;
	glget_mask[GL_MAX_TEXTURE_SIZE] = 1;
	glget_mask[GL_MAX_TEXTURE_STACK_DEPTH] = 1;
	glget_mask[GL_MAX_VIEWPORT_DIMS] = 2;
	glget_mask[GL_MODELVIEW_MATRIX] = 16;
	glget_mask[GL_MODELVIEW_STACK_DEPTH] = 1;
	glget_mask[GL_NAME_STACK_DEPTH] = 1;
	glget_mask[GL_NORMAL_ARRAY_EXT] = 1;
	glget_mask[GL_NORMAL_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_NORMAL_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_NORMAL_ARRAY_TYPE_EXT] = 1;
	glget_mask[GL_NORMALIZE] = 1;
	glget_mask[GL_PACK_ALIGNMENT] = 1;
	glget_mask[GL_PACK_LSB_FIRST] = 1;
	glget_mask[GL_PACK_ROW_LENGTH] = 1;
	glget_mask[GL_PACK_SKIP_PIXELS] = 1;
	glget_mask[GL_PACK_SKIP_ROWS] = 1;
	glget_mask[GL_PACK_SWAP_BYTES] = 1;
	glget_mask[GL_PERSPECTIVE_CORRECTION_HINT] = 1;
	glget_mask[GL_PIXEL_MAP_A_TO_A_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_B_TO_B_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_G_TO_G_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_I_TO_A_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_I_TO_B_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_I_TO_G_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_I_TO_I_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_I_TO_R_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_R_TO_R_SIZE] = 1;
	glget_mask[GL_PIXEL_MAP_S_TO_S_SIZE] = 1;
	glget_mask[GL_POINT_SIZE] = 1;
	glget_mask[GL_POINT_SIZE_GRANULARITY] = 1;
	glget_mask[GL_POINT_SIZE_RANGE] = 2;
	glget_mask[GL_POINT_SMOOTH] = 1;
	glget_mask[GL_POINT_SMOOTH_HINT] = 1;
	glget_mask[GL_POLYGON_MODE] = 2;
	glget_mask[GL_POLYGON_SMOOTH] = 1;
	glget_mask[GL_POLYGON_SMOOTH_HINT] = 1;
	glget_mask[GL_POLYGON_STIPPLE] = 1;
	glget_mask[GL_PROJECTION_MATRIX] = 16;
	glget_mask[GL_PROJECTION_STACK_DEPTH] = 1;
	glget_mask[GL_READ_BUFFER] = 1;
	glget_mask[GL_RED_BIAS] = 1;
	glget_mask[GL_RED_BITS] = 1;
	glget_mask[GL_RED_SCALE] = 1;
	glget_mask[GL_RENDER_MODE] = 1;
	glget_mask[GL_RGBA_MODE] = 1;
	glget_mask[GL_SCISSOR_BOX] = 4;
	glget_mask[GL_SCISSOR_TEST] = 1;
	glget_mask[GL_SHADE_MODEL] = 1;
	glget_mask[GL_STENCIL_BITS] = 1;
	glget_mask[GL_STENCIL_CLEAR_VALUE] = 1;
	glget_mask[GL_STENCIL_FAIL] = 1;
	glget_mask[GL_STENCIL_FUNC] = 1;
	glget_mask[GL_STENCIL_PASS_DEPTH_FAIL] = 1;
	glget_mask[GL_STENCIL_PASS_DEPTH_PASS] = 1;
	glget_mask[GL_STENCIL_REF] = 1;
	glget_mask[GL_STENCIL_TEST] = 1;
	glget_mask[GL_STENCIL_VALUE_MASK] = 1;
	glget_mask[GL_STENCIL_WRITEMASK] = 1;
	glget_mask[GL_STEREO] = 1;
	glget_mask[GL_SUBPIXEL_BITS] = 1;
	glget_mask[GL_TEXTURE_1D] = 1;
	glget_mask[GL_TEXTURE_2D] = 1;
	glget_mask[GL_TEXTURE_COORD_ARRAY_EXT] = 1;
	glget_mask[GL_TEXTURE_COORD_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_TEXTURE_COORD_ARRAY_SIZE_EXT] = 1;
	glget_mask[GL_TEXTURE_COORD_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_TEXTURE_COORD_ARRAY_TYPE_EXT] = 1;
	glget_mask[GL_TEXTURE_GEN_S] = 1;
	glget_mask[GL_TEXTURE_GEN_T] = 1;
	glget_mask[GL_TEXTURE_GEN_R] = 1;
	glget_mask[GL_TEXTURE_GEN_Q] = 1;
	glget_mask[GL_TEXTURE_MATRIX] = 16;
	glget_mask[GL_TEXTURE_STACK_DEPTH] = 1;
	glget_mask[GL_UNPACK_ALIGNMENT] = 1;
	glget_mask[GL_UNPACK_LSB_FIRST] = 1;
	glget_mask[GL_UNPACK_ROW_LENGTH] = 1;
	glget_mask[GL_UNPACK_SKIP_PIXELS] = 1;
	glget_mask[GL_UNPACK_SKIP_ROWS] = 1;
	glget_mask[GL_UNPACK_SWAP_BYTES] = 1;
	glget_mask[GL_VERTEX_ARRAY_EXT] = 1;
	glget_mask[GL_VERTEX_ARRAY_COUNT_EXT] = 1;
	glget_mask[GL_VERTEX_ARRAY_SIZE_EXT] = 1;
	glget_mask[GL_VERTEX_ARRAY_STRIDE_EXT] = 1;
	glget_mask[GL_VERTEX_ARRAY_TYPE_EXT] = 1;
	glget_mask[GL_VIEWPORT] = 4;
	glget_mask[GL_ZOOM_X] = 1;
	glget_mask[GL_ZOOM_Y] = 1;
	
#ifdef PHP_DEBUG
	for (i = 0; glget_const[i] != 0; i++)
		if (glget_mask[glget_const[i]] == -1)
			fprintf(stderr, "%d is missing\n", glget_const[i]);
#endif

	REGISTER_LONG_CONSTANT("GL_ACCUM", GL_ACCUM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LOAD", GL_LOAD , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RETURN", GL_RETURN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MULT", GL_MULT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ADD", GL_ADD , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEVER", GL_NEVER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LESS", GL_LESS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EQUAL", GL_EQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LEQUAL", GL_LEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREATER", GL_GREATER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOTEQUAL", GL_NOTEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GEQUAL", GL_GEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALWAYS", GL_ALWAYS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_BIT", GL_CURRENT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_BIT", GL_POINT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_BIT", GL_LINE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_BIT", GL_POLYGON_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_STIPPLE_BIT", GL_POLYGON_STIPPLE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MODE_BIT", GL_PIXEL_MODE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHTING_BIT", GL_LIGHTING_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_BIT", GL_FOG_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_BUFFER_BIT", GL_ACCUM_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_BUFFER_BIT", GL_STENCIL_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VIEWPORT_BIT", GL_VIEWPORT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRANSFORM_BIT", GL_TRANSFORM_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ENABLE_BIT", GL_ENABLE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_HINT_BIT", GL_HINT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EVAL_BIT", GL_EVAL_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_BIT", GL_LIST_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BIT", GL_TEXTURE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_BIT", GL_SCISSOR_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALL_ATTRIB_BITS", GL_ALL_ATTRIB_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINTS", GL_POINTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINES", GL_LINES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_LOOP", GL_LINE_LOOP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STRIP", GL_LINE_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLES", GL_TRIANGLES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLE_FAN", GL_TRIANGLE_FAN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUADS", GL_QUADS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUAD_STRIP", GL_QUAD_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON", GL_POLYGON , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZERO", GL_ZERO , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE", GL_ONE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_COLOR", GL_SRC_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_ALPHA", GL_SRC_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DST_ALPHA", GL_DST_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_DST_ALPHA", GL_ONE_MINUS_DST_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DST_COLOR", GL_DST_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRUE", GL_TRUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FALSE", GL_FALSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE0", GL_CLIP_PLANE0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE1", GL_CLIP_PLANE1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE2", GL_CLIP_PLANE2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE3", GL_CLIP_PLANE3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE4", GL_CLIP_PLANE4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE5", GL_CLIP_PLANE5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BYTE", GL_BYTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHORT", GL_SHORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_SHORT", GL_UNSIGNED_SHORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INT", GL_INT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_INT", GL_UNSIGNED_INT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FLOAT", GL_FLOAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_2_BYTES", GL_2_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3_BYTES", GL_3_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_4_BYTES", GL_4_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOUBLE", GL_DOUBLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NONE", GL_NONE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_LEFT", GL_FRONT_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_RIGHT", GL_FRONT_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK_LEFT", GL_BACK_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK_RIGHT", GL_BACK_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT", GL_FRONT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK", GL_BACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LEFT", GL_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RIGHT", GL_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_AND_BACK", GL_FRONT_AND_BACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX0", GL_AUX0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX1", GL_AUX1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX2", GL_AUX2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX3", GL_AUX3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NO_ERROR", GL_NO_ERROR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_ENUM", GL_INVALID_ENUM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_VALUE", GL_INVALID_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_OPERATION", GL_INVALID_OPERATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STACK_OVERFLOW", GL_STACK_OVERFLOW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STACK_UNDERFLOW", GL_STACK_UNDERFLOW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OUT_OF_MEMORY", GL_OUT_OF_MEMORY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_2D", GL_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D", GL_3D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D_COLOR", GL_3D_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D_COLOR_TEXTURE", GL_3D_COLOR_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_4D_COLOR_TEXTURE", GL_4D_COLOR_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PASS_THROUGH_TOKEN", GL_PASS_THROUGH_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_TOKEN", GL_POINT_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_TOKEN", GL_LINE_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_TOKEN", GL_POLYGON_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BITMAP_TOKEN", GL_BITMAP_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DRAW_PIXEL_TOKEN", GL_DRAW_PIXEL_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY_PIXEL_TOKEN", GL_COPY_PIXEL_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_RESET_TOKEN", GL_LINE_RESET_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXP", GL_EXP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXP2", GL_EXP2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CW", GL_CW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CCW", GL_CCW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COEFF", GL_COEFF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ORDER", GL_ORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOMAIN", GL_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_COLOR", GL_CURRENT_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_INDEX", GL_CURRENT_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_NORMAL", GL_CURRENT_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_TEXTURE_COORDS", GL_CURRENT_TEXTURE_COORDS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_COLOR", GL_CURRENT_RASTER_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_INDEX", GL_CURRENT_RASTER_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_TEXTURE_COORDS", GL_CURRENT_RASTER_TEXTURE_COORDS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_POSITION", GL_CURRENT_RASTER_POSITION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_POSITION_VALID", GL_CURRENT_RASTER_POSITION_VALID , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_DISTANCE", GL_CURRENT_RASTER_DISTANCE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SMOOTH", GL_POINT_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE", GL_POINT_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE_RANGE", GL_POINT_SIZE_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE_GRANULARITY", GL_POINT_SIZE_GRANULARITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_SMOOTH", GL_LINE_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH", GL_LINE_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH_RANGE", GL_LINE_WIDTH_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH_GRANULARITY", GL_LINE_WIDTH_GRANULARITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE", GL_LINE_STIPPLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE_PATTERN", GL_LINE_STIPPLE_PATTERN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE_REPEAT", GL_LINE_STIPPLE_REPEAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_MODE", GL_LIST_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_LIST_NESTING", GL_MAX_LIST_NESTING , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_BASE", GL_LIST_BASE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_INDEX", GL_LIST_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_MODE", GL_POLYGON_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_SMOOTH", GL_POLYGON_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_STIPPLE", GL_POLYGON_STIPPLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG", GL_EDGE_FLAG , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CULL_FACE", GL_CULL_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CULL_FACE_MODE", GL_CULL_FACE_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_FACE", GL_FRONT_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHTING", GL_LIGHTING , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_LOCAL_VIEWER", GL_LIGHT_MODEL_LOCAL_VIEWER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_TWO_SIDE", GL_LIGHT_MODEL_TWO_SIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_AMBIENT", GL_LIGHT_MODEL_AMBIENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHADE_MODEL", GL_SHADE_MODEL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL_FACE", GL_COLOR_MATERIAL_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL_PARAMETER", GL_COLOR_MATERIAL_PARAMETER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL", GL_COLOR_MATERIAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG", GL_FOG , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_INDEX", GL_FOG_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_DENSITY", GL_FOG_DENSITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_START", GL_FOG_START , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_END", GL_FOG_END , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_MODE", GL_FOG_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_COLOR", GL_FOG_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_RANGE", GL_DEPTH_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_TEST", GL_DEPTH_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_WRITEMASK", GL_DEPTH_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_CLEAR_VALUE", GL_DEPTH_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_FUNC", GL_DEPTH_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_CLEAR_VALUE", GL_ACCUM_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_TEST", GL_STENCIL_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_CLEAR_VALUE", GL_STENCIL_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_FUNC", GL_STENCIL_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_VALUE_MASK", GL_STENCIL_VALUE_MASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_FAIL", GL_STENCIL_FAIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_PASS_DEPTH_FAIL", GL_STENCIL_PASS_DEPTH_FAIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_PASS_DEPTH_PASS", GL_STENCIL_PASS_DEPTH_PASS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_REF", GL_STENCIL_REF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_WRITEMASK", GL_STENCIL_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MATRIX_MODE", GL_MATRIX_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMALIZE", GL_NORMALIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VIEWPORT", GL_VIEWPORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW_STACK_DEPTH", GL_MODELVIEW_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION_STACK_DEPTH", GL_PROJECTION_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_STACK_DEPTH", GL_TEXTURE_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW_MATRIX", GL_MODELVIEW_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION_MATRIX", GL_PROJECTION_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MATRIX", GL_TEXTURE_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ATTRIB_STACK_DEPTH", GL_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_ATTRIB_STACK_DEPTH", GL_CLIENT_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST", GL_ALPHA_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST_FUNC", GL_ALPHA_TEST_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST_REF", GL_ALPHA_TEST_REF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DITHER", GL_DITHER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND_DST", GL_BLEND_DST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND_SRC", GL_BLEND_SRC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND", GL_BLEND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LOGIC_OP_MODE", GL_LOGIC_OP_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_LOGIC_OP", GL_INDEX_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_LOGIC_OP", GL_COLOR_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX_BUFFERS", GL_AUX_BUFFERS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DRAW_BUFFER", GL_DRAW_BUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_READ_BUFFER", GL_READ_BUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_BOX", GL_SCISSOR_BOX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_TEST", GL_SCISSOR_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_CLEAR_VALUE", GL_INDEX_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_WRITEMASK", GL_INDEX_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_CLEAR_VALUE", GL_COLOR_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_WRITEMASK", GL_COLOR_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_MODE", GL_INDEX_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA_MODE", GL_RGBA_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOUBLEBUFFER", GL_DOUBLEBUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STEREO", GL_STEREO , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDER_MODE", GL_RENDER_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PERSPECTIVE_CORRECTION_HINT", GL_PERSPECTIVE_CORRECTION_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SMOOTH_HINT", GL_POINT_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_SMOOTH_HINT", GL_LINE_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_SMOOTH_HINT", GL_POLYGON_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_HINT", GL_FOG_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_S", GL_TEXTURE_GEN_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_T", GL_TEXTURE_GEN_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_R", GL_TEXTURE_GEN_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_Q", GL_TEXTURE_GEN_Q , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_I", GL_PIXEL_MAP_I_TO_I , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_S_TO_S", GL_PIXEL_MAP_S_TO_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_R", GL_PIXEL_MAP_I_TO_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_G", GL_PIXEL_MAP_I_TO_G , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_B", GL_PIXEL_MAP_I_TO_B , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_A", GL_PIXEL_MAP_I_TO_A , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_R_TO_R", GL_PIXEL_MAP_R_TO_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_G_TO_G", GL_PIXEL_MAP_G_TO_G , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_B_TO_B", GL_PIXEL_MAP_B_TO_B , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_A_TO_A", GL_PIXEL_MAP_A_TO_A , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_I_SIZE", GL_PIXEL_MAP_I_TO_I_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_S_TO_S_SIZE", GL_PIXEL_MAP_S_TO_S_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_R_SIZE", GL_PIXEL_MAP_I_TO_R_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_G_SIZE", GL_PIXEL_MAP_I_TO_G_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_B_SIZE", GL_PIXEL_MAP_I_TO_B_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_A_SIZE", GL_PIXEL_MAP_I_TO_A_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_R_TO_R_SIZE", GL_PIXEL_MAP_R_TO_R_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_G_TO_G_SIZE", GL_PIXEL_MAP_G_TO_G_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_B_TO_B_SIZE", GL_PIXEL_MAP_B_TO_B_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_A_TO_A_SIZE", GL_PIXEL_MAP_A_TO_A_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SWAP_BYTES", GL_UNPACK_SWAP_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_LSB_FIRST", GL_UNPACK_LSB_FIRST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_ROW_LENGTH", GL_UNPACK_ROW_LENGTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SKIP_ROWS", GL_UNPACK_SKIP_ROWS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SKIP_PIXELS", GL_UNPACK_SKIP_PIXELS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_ALIGNMENT", GL_UNPACK_ALIGNMENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SWAP_BYTES", GL_PACK_SWAP_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_LSB_FIRST", GL_PACK_LSB_FIRST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_ROW_LENGTH", GL_PACK_ROW_LENGTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SKIP_ROWS", GL_PACK_SKIP_ROWS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SKIP_PIXELS", GL_PACK_SKIP_PIXELS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_ALIGNMENT", GL_PACK_ALIGNMENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP_COLOR", GL_MAP_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP_STENCIL", GL_MAP_STENCIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_SHIFT", GL_INDEX_SHIFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_OFFSET", GL_INDEX_OFFSET , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_SCALE", GL_RED_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_BIAS", GL_RED_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZOOM_X", GL_ZOOM_X , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZOOM_Y", GL_ZOOM_Y , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_SCALE", GL_GREEN_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_BIAS", GL_GREEN_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_SCALE", GL_BLUE_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_BIAS", GL_BLUE_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_SCALE", GL_ALPHA_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_BIAS", GL_ALPHA_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_SCALE", GL_DEPTH_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BIAS", GL_DEPTH_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_EVAL_ORDER", GL_MAX_EVAL_ORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_LIGHTS", GL_MAX_LIGHTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_CLIP_PLANES", GL_MAX_CLIP_PLANES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_TEXTURE_SIZE", GL_MAX_TEXTURE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_PIXEL_MAP_TABLE", GL_MAX_PIXEL_MAP_TABLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ATTRIB_STACK_DEPTH", GL_MAX_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_MODELVIEW_STACK_DEPTH", GL_MAX_MODELVIEW_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_NAME_STACK_DEPTH", GL_MAX_NAME_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_PROJECTION_STACK_DEPTH", GL_MAX_PROJECTION_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_TEXTURE_STACK_DEPTH", GL_MAX_TEXTURE_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_VIEWPORT_DIMS", GL_MAX_VIEWPORT_DIMS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_CLIENT_ATTRIB_STACK_DEPTH", GL_MAX_CLIENT_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SUBPIXEL_BITS", GL_SUBPIXEL_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_BITS", GL_INDEX_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_BITS", GL_RED_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_BITS", GL_GREEN_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_BITS", GL_BLUE_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_BITS", GL_ALPHA_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BITS", GL_DEPTH_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_BITS", GL_STENCIL_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_RED_BITS", GL_ACCUM_RED_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_GREEN_BITS", GL_ACCUM_GREEN_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_BLUE_BITS", GL_ACCUM_BLUE_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_ALPHA_BITS", GL_ACCUM_ALPHA_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NAME_STACK_DEPTH", GL_NAME_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUTO_NORMAL", GL_AUTO_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_COLOR_4", GL_MAP1_COLOR_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_INDEX", GL_MAP1_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_NORMAL", GL_MAP1_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_1", GL_MAP1_TEXTURE_COORD_1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_2", GL_MAP1_TEXTURE_COORD_2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_3", GL_MAP1_TEXTURE_COORD_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_4", GL_MAP1_TEXTURE_COORD_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_VERTEX_3", GL_MAP1_VERTEX_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_VERTEX_4", GL_MAP1_VERTEX_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_COLOR_4", GL_MAP2_COLOR_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_INDEX", GL_MAP2_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_NORMAL", GL_MAP2_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_1", GL_MAP2_TEXTURE_COORD_1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_2", GL_MAP2_TEXTURE_COORD_2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_3", GL_MAP2_TEXTURE_COORD_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_4", GL_MAP2_TEXTURE_COORD_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_VERTEX_3", GL_MAP2_VERTEX_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_VERTEX_4", GL_MAP2_VERTEX_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_GRID_DOMAIN", GL_MAP1_GRID_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_GRID_SEGMENTS", GL_MAP1_GRID_SEGMENTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_GRID_DOMAIN", GL_MAP2_GRID_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_GRID_SEGMENTS", GL_MAP2_GRID_SEGMENTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_1D", GL_TEXTURE_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_2D", GL_TEXTURE_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_POINTER", GL_FEEDBACK_BUFFER_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_SIZE", GL_FEEDBACK_BUFFER_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_TYPE", GL_FEEDBACK_BUFFER_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECTION_BUFFER_POINTER", GL_SELECTION_BUFFER_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECTION_BUFFER_SIZE", GL_SELECTION_BUFFER_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WIDTH", GL_TEXTURE_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_HEIGHT", GL_TEXTURE_HEIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_INTERNAL_FORMAT", GL_TEXTURE_INTERNAL_FORMAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BORDER_COLOR", GL_TEXTURE_BORDER_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BORDER", GL_TEXTURE_BORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DONT_CARE", GL_DONT_CARE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FASTEST", GL_FASTEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NICEST", GL_NICEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT0", GL_LIGHT0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT1", GL_LIGHT1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT2", GL_LIGHT2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT3", GL_LIGHT3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT4", GL_LIGHT4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT5", GL_LIGHT5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT6", GL_LIGHT6 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT7", GL_LIGHT7 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AMBIENT", GL_AMBIENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DIFFUSE", GL_DIFFUSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPECULAR", GL_SPECULAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POSITION", GL_POSITION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_DIRECTION", GL_SPOT_DIRECTION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_EXPONENT", GL_SPOT_EXPONENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_CUTOFF", GL_SPOT_CUTOFF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CONSTANT_ATTENUATION", GL_CONSTANT_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_ATTENUATION", GL_LINEAR_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUADRATIC_ATTENUATION", GL_QUADRATIC_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COMPILE", GL_COMPILE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COMPILE_AND_EXECUTE", GL_COMPILE_AND_EXECUTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLEAR", GL_CLEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND", GL_AND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND_REVERSE", GL_AND_REVERSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY", GL_COPY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND_INVERTED", GL_AND_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOOP", GL_NOOP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_XOR", GL_XOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR", GL_OR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOR", GL_NOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EQUIV", GL_EQUIV , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVERT", GL_INVERT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR_REVERSE", GL_OR_REVERSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY_INVERTED", GL_COPY_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR_INVERTED", GL_OR_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NAND", GL_NAND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SET", GL_SET , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EMISSION", GL_EMISSION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHININESS", GL_SHININESS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AMBIENT_AND_DIFFUSE", GL_AMBIENT_AND_DIFFUSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEXES", GL_COLOR_INDEXES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW", GL_MODELVIEW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION", GL_PROJECTION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE", GL_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR", GL_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH", GL_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL", GL_STENCIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX", GL_COLOR_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_INDEX", GL_STENCIL_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_COMPONENT", GL_DEPTH_COMPONENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED", GL_RED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN", GL_GREEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE", GL_BLUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA", GL_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB", GL_RGB , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA", GL_RGBA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE", GL_LUMINANCE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE_ALPHA", GL_LUMINANCE_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BITMAP", GL_BITMAP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT", GL_POINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE", GL_LINE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FILL", GL_FILL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDER", GL_RENDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK", GL_FEEDBACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECT", GL_SELECT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FLAT", GL_FLAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SMOOTH", GL_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_KEEP", GL_KEEP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_REPLACE", GL_REPLACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INCR", GL_INCR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DECR", GL_DECR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VENDOR", GL_VENDOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDERER", GL_RENDERER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERSION", GL_VERSION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXTENSIONS", GL_EXTENSIONS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_S", GL_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T", GL_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_R", GL_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_Q", GL_Q , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODULATE", GL_MODULATE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DECAL", GL_DECAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV_MODE", GL_TEXTURE_ENV_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV_COLOR", GL_TEXTURE_ENV_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV", GL_TEXTURE_ENV , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EYE_LINEAR", GL_EYE_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OBJECT_LINEAR", GL_OBJECT_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPHERE_MAP", GL_SPHERE_MAP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_MODE", GL_TEXTURE_GEN_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OBJECT_PLANE", GL_OBJECT_PLANE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EYE_PLANE", GL_EYE_PLANE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST", GL_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR", GL_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MAG_FILTER", GL_TEXTURE_MAG_FILTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MIN_FILTER", GL_TEXTURE_MIN_FILTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WRAP_S", GL_TEXTURE_WRAP_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WRAP_T", GL_TEXTURE_WRAP_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLAMP", GL_CLAMP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_REPEAT", GL_REPEAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_PIXEL_STORE_BIT", GL_CLIENT_PIXEL_STORE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_VERTEX_ARRAY_BIT", GL_CLIENT_VERTEX_ARRAY_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_FACTOR", GL_POLYGON_OFFSET_FACTOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_UNITS", GL_POLYGON_OFFSET_UNITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_POINT", GL_POLYGON_OFFSET_POINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_LINE", GL_POLYGON_OFFSET_LINE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_FILL", GL_POLYGON_OFFSET_FILL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA4", GL_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA8", GL_ALPHA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA12", GL_ALPHA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA16", GL_ALPHA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE4", GL_LUMINANCE4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE8", GL_LUMINANCE8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12", GL_LUMINANCE12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE16", GL_LUMINANCE16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE4_ALPHA4", GL_LUMINANCE4_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE6_ALPHA2", GL_LUMINANCE6_ALPHA2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE8_ALPHA8", GL_LUMINANCE8_ALPHA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12_ALPHA4", GL_LUMINANCE12_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12_ALPHA12", GL_LUMINANCE12_ALPHA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE16_ALPHA16", GL_LUMINANCE16_ALPHA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY", GL_INTENSITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY4", GL_INTENSITY4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY8", GL_INTENSITY8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY12", GL_INTENSITY12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY16", GL_INTENSITY16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_R3_G3_B2", GL_R3_G3_B2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB4", GL_RGB4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB5", GL_RGB5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB8", GL_RGB8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB10", GL_RGB10 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB12", GL_RGB12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB16", GL_RGB16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA2", GL_RGBA2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA4", GL_RGBA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB5_A1", GL_RGB5_A1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA8", GL_RGBA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB10_A2", GL_RGB10_A2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA12", GL_RGBA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA16", GL_RGBA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_RED_SIZE", GL_TEXTURE_RED_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GREEN_SIZE", GL_TEXTURE_GREEN_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BLUE_SIZE", GL_TEXTURE_BLUE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ALPHA_SIZE", GL_TEXTURE_ALPHA_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_LUMINANCE_SIZE", GL_TEXTURE_LUMINANCE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_INTENSITY_SIZE", GL_TEXTURE_INTENSITY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROXY_TEXTURE_1D", GL_PROXY_TEXTURE_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROXY_TEXTURE_2D", GL_PROXY_TEXTURE_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_PRIORITY", GL_TEXTURE_PRIORITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_RESIDENT", GL_TEXTURE_RESIDENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BINDING_1D", GL_TEXTURE_BINDING_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BINDING_2D", GL_TEXTURE_BINDING_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY", GL_VERTEX_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY", GL_NORMAL_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY", GL_COLOR_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY", GL_INDEX_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY", GL_TEXTURE_COORD_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY", GL_EDGE_FLAG_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_SIZE", GL_VERTEX_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_TYPE", GL_VERTEX_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_STRIDE", GL_VERTEX_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_TYPE", GL_NORMAL_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_STRIDE", GL_NORMAL_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_SIZE", GL_COLOR_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_TYPE", GL_COLOR_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_STRIDE", GL_COLOR_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_TYPE", GL_INDEX_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_STRIDE", GL_INDEX_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_SIZE", GL_TEXTURE_COORD_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_TYPE", GL_TEXTURE_COORD_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_STRIDE", GL_TEXTURE_COORD_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_STRIDE", GL_EDGE_FLAG_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_POINTER", GL_VERTEX_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_POINTER", GL_NORMAL_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_POINTER", GL_COLOR_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_POINTER", GL_INDEX_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_POINTER", GL_TEXTURE_COORD_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_POINTER", GL_EDGE_FLAG_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_V2F", GL_V2F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_V3F", GL_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4UB_V2F", GL_C4UB_V2F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4UB_V3F", GL_C4UB_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C3F_V3F", GL_C3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_N3F_V3F", GL_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4F_N3F_V3F", GL_C4F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_V3F", GL_T2F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T4F_V4F", GL_T4F_V4F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C4UB_V3F", GL_T2F_C4UB_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C3F_V3F", GL_T2F_C3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_N3F_V3F", GL_T2F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C4F_N3F_V3F", GL_T2F_C4F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T4F_C4F_N3F_V4F", GL_T4F_C4F_N3F_V4F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXT_vertex_array", GL_EXT_vertex_array , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXT_bgra", GL_EXT_bgra , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXT_paletted_texture", GL_EXT_paletted_texture , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_EXT", GL_VERTEX_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_EXT", GL_NORMAL_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_EXT", GL_COLOR_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_EXT", GL_INDEX_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_EXT", GL_TEXTURE_COORD_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_EXT", GL_EDGE_FLAG_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_SIZE_EXT", GL_VERTEX_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_TYPE_EXT", GL_VERTEX_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_STRIDE_EXT", GL_VERTEX_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_COUNT_EXT", GL_VERTEX_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_TYPE_EXT", GL_NORMAL_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_STRIDE_EXT", GL_NORMAL_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_COUNT_EXT", GL_NORMAL_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_SIZE_EXT", GL_COLOR_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_TYPE_EXT", GL_COLOR_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_STRIDE_EXT", GL_COLOR_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_COUNT_EXT", GL_COLOR_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_TYPE_EXT", GL_INDEX_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_STRIDE_EXT", GL_INDEX_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_COUNT_EXT", GL_INDEX_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_SIZE_EXT", GL_TEXTURE_COORD_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_TYPE_EXT", GL_TEXTURE_COORD_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_STRIDE_EXT", GL_TEXTURE_COORD_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_COUNT_EXT", GL_TEXTURE_COORD_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_STRIDE_EXT", GL_EDGE_FLAG_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_COUNT_EXT", GL_EDGE_FLAG_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_POINTER_EXT", GL_VERTEX_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_POINTER_EXT", GL_NORMAL_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_POINTER_EXT", GL_COLOR_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_POINTER_EXT", GL_INDEX_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_POINTER_EXT", GL_TEXTURE_COORD_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_POINTER_EXT", GL_EDGE_FLAG_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BGR_EXT", GL_BGR_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BGRA_EXT", GL_BGRA_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_FORMAT_EXT", GL_COLOR_TABLE_FORMAT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_WIDTH_EXT", GL_COLOR_TABLE_WIDTH_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_RED_SIZE_EXT", GL_COLOR_TABLE_RED_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_GREEN_SIZE_EXT", GL_COLOR_TABLE_GREEN_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_BLUE_SIZE_EXT", GL_COLOR_TABLE_BLUE_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_ALPHA_SIZE_EXT", GL_COLOR_TABLE_ALPHA_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_LUMINANCE_SIZE_EXT", GL_COLOR_TABLE_LUMINANCE_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_INTENSITY_SIZE_EXT", GL_COLOR_TABLE_INTENSITY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX1_EXT", GL_COLOR_INDEX1_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX2_EXT", GL_COLOR_INDEX2_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX4_EXT", GL_COLOR_INDEX4_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX8_EXT", GL_COLOR_INDEX8_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX12_EXT", GL_COLOR_INDEX12_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX16_EXT", GL_COLOR_INDEX16_EXT , CONST_CS | CONST_PERSISTENT);
#ifdef PHP_WIN32
	REGISTER_LONG_CONSTANT("GL_DOUBLE_EXT", GL_DOUBLE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_ALL_ATTRIB_BITS", GL_CLIENT_ALL_ATTRIB_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_WIN_swap_hint", GL_WIN_swap_hint , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_WIN_draw_range_elements", GL_WIN_draw_range_elements , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ELEMENTS_VERTICES_WIN", GL_MAX_ELEMENTS_VERTICES_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ELEMENTS_INDICES_WIN", GL_MAX_ELEMENTS_INDICES_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PHONG_WIN", GL_PHONG_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PHONG_HINT_WIN", GL_PHONG_HINT_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_SPECULAR_TEXTURE_WIN", GL_FOG_SPECULAR_TEXTURE_WIN , CONST_CS | CONST_PERSISTENT);
#endif
	REGISTER_LONG_CONSTANT("GL_LOGIC_OP", GL_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COMPONENTS", GL_TEXTURE_COMPONENTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SPRITE", GL_POINT_SPRITE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_DISTANCE_ATTENUATION", GL_POINT_DISTANCE_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GENERATE_MIPMAP", GL_GENERATE_MIPMAP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COORD_REPLACE", GL_COORD_REPLACE , CONST_CS | CONST_PERSISTENT);

	glu_init(module_number);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(opengl)
{
	// initialize other modules
	glu_request_init(module_number);

#ifdef PHP_OPENGL_OSMESA
	osmesa_init(module_number);
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(opengl)
{
	if (select_buffer != NULL) {
		efree(select_buffer);
		select_buffer = NULL;
	}

	// shutdown other modules
	glu_request_shutdown();

#ifdef PHP_OPENGL_OSMESA
	osmesa_request_shutdown();
#endif

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(opengl)
{
	if (glget_mask != NULL)
		pefree(glget_mask, 1);

	return SUCCESS;
}
