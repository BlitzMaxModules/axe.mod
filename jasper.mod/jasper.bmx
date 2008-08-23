
Strict

Rem
bbdoc: Jasper image loader
about:
The Jasper loader module provides the ability to load JPEG2000 (.jp2) format #brl.pixmap.pixmaps.
End Rem
Module axe.jasper

ModuleInfo "Version: 1.02"
ModuleInfo "Author:  D. Richard Hipp"
ModuleInfo "License: Public Domain"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 1.02 Release"
ModuleInfo "History: jasper_config.h fix for gcc4 compatability"
ModuleInfo "History: 1.01 Release"
ModuleInfo "History: Initial release"

Import BRL.Pixmap

Import "libjasper\bmp\bmp_cod.c"
Import "libjasper\bmp\bmp_dec.c"
Import "libjasper\bmp\bmp_enc.c"
Import "libjasper\base\jas_cm.c"
Import "libjasper\base\jas_debug.c"
Import "libjasper\base\jas_getopt.c"
Import "libjasper\base\jas_icc.c"
Import "libjasper\base\jas_iccdata.c"
Import "libjasper\base\jas_image.c"
Import "libjasper\base\jas_init.c"
Import "libjasper\base\jas_malloc.c"
Import "libjasper\base\jas_seq.c"
Import "libjasper\base\jas_stream.c"
Import "libjasper\base\jas_string.c"
Import "libjasper\base\jas_tvp.c"
Import "libjasper\base\jas_version.c"
Import "libjasper\jp2\jp2_cod.c"
Import "libjasper\jp2\jp2_dec.c"
Import "libjasper\jp2\jp2_enc.c"
Import "libjasper\jpc\jpc_bs.c"
Import "libjasper\jpc\jpc_cs.c"
Import "libjasper\jpc\jpc_dec.c"
Import "libjasper\jpc\jpc_enc.c"
Import "libjasper\jpc\jpc_math.c"
Import "libjasper\jpc\jpc_mct.c"
Import "libjasper\jpc\jpc_mqcod.c"
Import "libjasper\jpc\jpc_mqdec.c"
Import "libjasper\jpc\jpc_mqenc.c"
Import "libjasper\jpc\jpc_qmfb.c"
Import "libjasper\jpc\jpc_t1cod.c"
Import "libjasper\jpc\jpc_t1dec.c"
Import "libjasper\jpc\jpc_t1enc.c"
Import "libjasper\jpc\jpc_t2cod.c"
Import "libjasper\jpc\jpc_t2dec.c"
Import "libjasper\jpc\jpc_t2enc.c"
Import "libjasper\jpc\jpc_tagtree.c"
Import "libjasper\jpc\jpc_tsfb.c"
Import "libjasper\jpc\jpc_util.c"
Import "libjasper\jpg\jpg_dummy.c"
Import "libjasper\jpg\jpg_val.c"
Import "libjasper\mif\mif_cod.c"
Import "libjasper\pgx\pgx_cod.c"
Import "libjasper\pgx\pgx_dec.c"
Import "libjasper\pgx\pgx_enc.c"
Import "libjasper\pnm\pnm_cod.c"
Import "libjasper\pnm\pnm_dec.c"
Import "libjasper\pnm\pnm_enc.c"
Import "libjasper\ras\ras_cod.c"
Import "libjasper\ras\ras_dec.c"
Import "libjasper\ras\ras_enc.c"

'Import "loadjpg2.c"

Const JAS_IMAGE_CT_UNKNOWN=$10000
Const JAS_IMAGE_CT_OPACITY=$8000

Const JAS_IMAGE_CT_RGB_R=0
Const JAS_IMAGE_CT_RGB_G=1
Const JAS_IMAGE_CT_RGB_B=2

Const JAS_IMAGE_CT_YCBCR_Y=0
Const JAS_IMAGE_CT_YCBCR_CB=1
Const JAS_IMAGE_CT_YCBCR_CR=2
Const JAS_IMAGE_CT_GRAY_Y=0

Const JAS_STREAM_READ=1
Const JAS_STREAM_WRITE=2
Const JAS_STREAM_APPEND=4
Const JAS_STREAM_BINARY=8
Const JAS_STREAM_CREATE=16


Const JAS_STREAM_UNBUF=0
Const JAS_STREAM_LINEBUF=1
Const JAS_STREAM_FULLBUF=2
Const JAS_STREAM_BUFMODEMASK=15

Const JAS_STREAM_FREEBUF=8
Const JAS_STREAM_RDBUF=16
Const JAS_STREAM_WRBUF=32

Const JAS_STREAM_EOF=1
Const JAS_STREAM_ERR=2
Const JAS_STREAM_RWLIMIT=4
Const JAS_STREAM_ERRMASK=JAS_STREAM_EOF|JAS_STREAM_ERR|JAS_STREAM_RWLIMIT

Const JAS_STREAM_BUFSIZE=8192
Const JAS_STREAM_PERMS=666

Const JAS_STREAM_MAXPUTBACK=16

Type jas_stream_ops_t
	Field read_(stream:TStream,buf:Byte Ptr,cnt) 'Int (*read_)(jas_stream_obj_t *obj, char *buf, Int cnt)
	Field write_(stream:TStream,buf:Byte Ptr,cnt) 'Int (*write_)(jas_stream_obj_t *obj, char *buf, Int cnt)
	Field seek_(stream:TStream,offset,origin) ' Long (*seek_)(jas_stream_obj_t *obj, Long offset, Int origin)
	Field close_(stream:TStream) 'Int (*close_)(jas_stream_obj_t *obj)
End Type

Type jas_stream_t
	Field openmode_
	Field bufmode_
	Field flags_
	Field bufbase_:Byte Ptr
	Field bufstart_:Byte Ptr
	Field bufsize_
	Field ptr_:Byte Ptr
	Field cnt_
	Field tinybuf_0
	Field tinybuf_1
	Field tinybuf_2
	Field tinybuf_3
	Field tinybuf_4					'uchar tinybuf_[JAS_STREAM_MAXPUTBACK + 1]
	Field ops_:Byte Ptr'jas_stream_ops_t
	Field obj_:TStream				'jas_stream_obj_t
	Field rwcnt_
	Field rwlimit_
End Type

Extern

Type jas_matrix_t
'	Field flags_	arrrrrghhh!
	Field xstart_
	Field ystart_
	Field xend_
	Field yend_
	Field numrows_
	Field numcols_
	Field rows_:Int Ptr Ptr
	Field maxrows_
	Field data:Byte Ptr
	Field datasize_
End Type

Type jas_image_cmpt_t
'	Field tlx_,			arrrgggh!
	Field tly_,hstep_,vstep_,width_,height_
	Field prec_,sgnd_
	Field stream_:Byte Ptr
	Field cps_
	Field type_
End Type

Type jas_image_t
'	Field tlx_,			//arrgh mark come here
	Field tly_,brx_,bry_
	Field numcmpts_
	Field maxcmpts_
	Field cmpts:jas_image_cmpt_t Ptr
	Field clrspc_			
	Field cmpref_:Byte Ptr
	Field inmem_
End Type

Function jas_init()
Function jas_setdbglevel(level)
Function jas_getdbglevel()

Function jas_stream_fopen(file$z,opts$z)
Function jas_stream_flush(handle)
Function jas_stream_close(stream:Byte Ptr)

Function jas_image_decode:jas_image_t(in:Byte Ptr,fmt,opts)
Function jas_image_encode(image:jas_image_t,out,fmt,opts)

Function jas_image_destroy(image:jas_image_t)
Function jas_image_clearfmts()

Function jas_image_strtofmt(fmt$z)

Function jas_matrix_create:jas_matrix_t(numrows,numcols)
Function jas_image_readcmpt(image:Byte Ptr,cmptno,x,y,width,height,matrix:Byte Ptr)
Function jas_matrix_getv(matrix,pos)

End Extern

Function ReadJStream(stream:TStream,buf:Byte Ptr,cnt)
	Return stream.Read(buf,cnt)
End Function

Function WriteJStream(stream:TStream,buf:Byte Ptr,cnt)
	Return stream.Write(buf,cnt)
End Function

Function SeekJStream(stream:TStream,offset,origin)
	Select origin
		Case 0 'SEEK_SET
			stream.Seek(offset)			
		Case 1 'SEEK_CUR
			stream.Seek(stream.Pos()+offset)
		Case 2 'SEEK_END
			stream.Seek(stream.Size()+offset)	
	End Select
	Return stream.Pos()
End Function

Function CloseJStream(stream:TStream)
	stream.Close()
End Function

Function JasperStream:jas_stream_t(stream:TStream)
	Local ops:jas_stream_ops_t
	Local jstream:jas_stream_t
	ops=New jas_stream_ops_t
	ops.read_=ReadJStream
	ops.write_=WriteJStream
	ops.seek_=SeekJStream
	ops.close_=CloseJStream
	jstream=New jas_stream_t
	jstream.rwlimit_=-1
	jstream.ops_=Byte Ptr(ops)
	jstream.obj_=stream
	jstream.bufbase_=Varptr jstream.tinybuf_0
	jstream.bufsize_=1
	jstream.bufstart_=jstream.bufbase_+JAS_STREAM_MAXPUTBACK
	jstream.ptr_=jstream.bufstart_
	jstream.cnt_=0
	jstream.bufmode_=JAS_STREAM_UNBUF
	Return jstream
End Function

Function ReadJasperStream:jas_stream_t(url:Object)
	Local stream:TStream
	Local jstream:jas_stream_t
	stream=ReadStream(url)
	If Not stream Return
	jstream=JasperStream(stream)
	jstream.openmode_=JAS_STREAM_READ|JAS_STREAM_BINARY
	Return jstream
End Function

Function LoadJpeg2000:TPixmap(url:Object)
	Local	pixmap:TPixmap
	Local	streamin:jas_stream_t
	Local	in,out,image:jas_image_t	

	jas_init()
	jas_setdbglevel(0)

	streamin=ReadJasperStream(url)
	If Not streamin RuntimeError "ReadJasperStream failed"
	
	image:jas_image_t=jas_image_decode(Byte Ptr(streamin),-1,0)
	jas_stream_close(streamin)

	If Not image RuntimeError "jas_image_decode failed"

	Local width,height,depth,pformat
	Local matrix:jas_matrix_t
	Local src:Int Ptr
	Local res,x,y,d,c
	Local line[]
	Local lshfts[],rshfts[],lshft,rshft
	
	depth=image.numcmpts_
	width=image.brx_
	height=image.bry_
	matrix=jas_matrix_create(1,width)
	src=matrix.rows_[0]
		
	line=New Int[width]
	pformat=PF_BGR888
	Select depth
		Case 1
			pformat=PF_I8
		Case 2,3
			pformat=PF_BGR888
		Case 4
			pformat=PF_BGRA8888
	End Select
	pixmap=TPixmap.Create( width,height,pformat )
	lshfts=New Int[depth]
	rshfts=New Int[depth]
	For d=0 Until depth
		Local cmpt:jas_image_cmpt_t
		cmpt=image.cmpts[d]
'		Print cmpt.prec_+","+cmpt.sgnd_+","+cmpt.cps_+","+cmpt.type_				
		Local type_=cmpt.type_
		If type_=32768 type_=3
?bigendian
		type_=3-type_		
?		
		lshft=type_*8
		rshft=cmpt.prec_-8
		If rshft<0 lshft:-rshft;rshft=0		
		lshfts[d]=lshft
		rshfts[d]=rshft
	Next	
	For y=0 Until height
		memset_ line,0,width*4
		For d=0 Until depth
			res=jas_image_readcmpt(image,d,0,y,width,1,matrix)
			lshft=lshfts[d]
			rshft=rshfts[d]
			For x=0 Until width
				c=src[x]
				line[x]:|((c Shr rshft) Shl lshft)
			Next
		Next
		ConvertPixels(line,PF_RGBA8888,pixmap.pixelptr(0,y),pixmap.format,width)
	Next
	jas_image_destroy(image)
	jas_image_clearfmts()
	Return pixmap
End Function

Type TJasperPixmapLoader Extends TPixmapLoader
	Method LoadPixmap:TPixmap( stream:TStream )
		Local pix:TPixmap
		Try
			pix=LoadJPEG2000(stream)
		EndTry
		Return pix
	End Method
End Type

New TJasperPixmapLoader
