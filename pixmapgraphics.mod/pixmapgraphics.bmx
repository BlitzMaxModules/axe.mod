Strict

Rem
bbdoc: PixmapGraphics
End Rem
Module axe.PixmapGraphics

ModuleInfo "Version: 0.02"
ModuleInfo "Author: Simon Armstrong"

ModuleInfo "History: 1.01 Release"
ModuleInfo "History: Added anti alias linedrawing by AGG"

Import brl.max2d

Import "agg.cpp"
Import "aggpixmap.cpp"

Extern "C"
	Function ras_open%(pixels:Byte Ptr,width,height)
	Function ras_close(rashandle)
	Function ras_clear(rashandle)
	Function ras_moveto(rashandle,x,y)
	Function ras_lineto(rashandle,x,y)
	Function ras_render(rashandle,r,g,b,a)
End Extern

Type TPixmapFrame Extends TImageFrame
	
	Field _owner:TPixmapDriver
	Field _pix:TPixmap
	Field _flags:Int
		
	Method Draw( x0#,y0#,x1#,y1#,tx#,ty#,sx#,sy#,sw#,sh# )
'	Method Draw( x0#,y0#,x1#,y1#,tx#,ty# )
		Local dest:TPixmap		
		_owner.DrawPixRect(x0,y0,x1-x0,y1-y0,tx,ty,_pix)
	End Method

	Method init:TPixmapFrame(owner:TPixmapDriver,pixmap:TPixmap,flags)
		_owner=owner
		_pix=pixmap
		_flags=flags
		Return Self
	End Method

End Type

Type TPixmapDriver Extends TMax2DDriver	'Extends TGraphicsDriver

	Field _pixmap:TPixmap
	Field _width
	Field _height
	Field _span
	Field _pix:Int Ptr
'drawstate
	Field _blend
	Field _alpha
	Field _color
	Field _clscolor
	Field _clipx,_clipy,_clipw,_cliph
	Field _hastrans
	Field _trans[4]

	Field _ras:Int
	Field _linewidth#
	
	Method CreateFrameFromPixmap:TImageFrame( pixmap:TPixmap,flags ) 
		Return New TPixmapFrame.init(Self,pixmap,flags)
	End Method
		
	Method SetBlend( blend ) 
		_blend=blend
	End Method

	Method SetAlpha( alpha# ) 
		_alpha=alpha*255
	End Method

	Method SetColor( red,green,blue ) 
		_color=$ff000000|(red Shl 16)|(green Shl 8)|blue
	End Method

	Method SetClsColor( red,green,blue ) 
		_clscolor=$ff000000|(red Shl 16)|(green Shl 8)|blue
	End Method

	Method SetViewport( x,y,width,height ) 
		_clipx=x
		_clipy=y
		_clipw=width
		_cliph=height	
	End Method
	
	Method SetTransform( xx#,xy#,yx#,yy# ) 
		If xx=1.0 And xy=0.0 And yx=0.0 And yy=1.0
			_hastrans=False
		Else
			_trans[0]=xx
			_trans[1]=xy
			_trans[2]=yx
			_trans[3]=yy		
			_hastrans=True
		EndIf
	End Method

	Method SetLineWidth( width# ) 
		_linewidth=256*width
	End Method
	
	Method Cls() 
		Local p:Int Ptr
		Local x,y
		p=_pix		
		For y=0 Until _height
			For x=0 Until _width
				p[x]=_clscolor
			Next
			p:+_span
		Next
	End Method

	Method Plot( x#,y# ) 
		Local ix,iy
		ix=Int(x+0.5)
		iy=Int(y+0.5)
		If ix>=_clipx And ix<_clipx+_clipw And iy>=_clipy And iy<_clipy+_cliph
			_pix[iy*_span+ix]=_color
		EndIf
	End Method

	Method DrawLine( lx0#,ly0#,lx1#,ly1#,tx#,ty# ) 
		Local x1#,y1#,x2#,y2#
				
		x1=(tx+lx0)*256
		y1=(ty+ly0)*256
		x2=(tx+lx1)*256
		y2=(ty+ly1)*256	

		Local dx# = x2 - x1
		Local dy# = y2 - y1
		Local d# = Sqr(dx*dx + dy*dy)		

		dx = _linewidth * (y2 - y1) / d
		dy = _linewidth * (x2 - x1) / d	
		
		ras_moveto(_ras,x1 - dx,  y1 + dy)
		ras_lineto(_ras,x2 - dx,  y2 + dy)
		ras_lineto(_ras,x2 + dx,  y2 - dy)
		ras_lineto(_ras,x1 + dx,  y1 - dy)
		ras_render(_ras,255,255,255,255)
	End Method

	Method DrawPoly( xy#[],handlex#,handley#,originx#,originy# ) 
		Local n,i,ii
		Local x0#,y0#,x,y
		
		n=xy.length Shr 1
		For i=0 To n
			ii=i	
			If ii=n Then ii=0	
			x=xy[ii*2+0]*256
			y=xy[ii*2+1]*256			
			If i=0
				ras_moveto(_ras,x,y)
			Else
				ras_lineto(_ras,x,y)
			EndIf
		Next			
		ras_render(_ras,255,255,255,255)
		
	End Method


	Method DrawLine2( lx0#,ly0#,lx1#,ly1#,tx#,ty# ) 
		Local x0,y0,x1,y1
		Local cx0,cy0,cx1,cy1
		Local dx,dy
		Local clip0,clip1
		Local sx,sy,ax,ay
		Local ddf,sadj,padj

		x0=lx0
		y0=ly0
		x1=lx1
		y1=ly1

		cx0=_clipx
		cy0=_clipy
		cx1=_clipx+_clipw-1
		cy1=_clipy+_cliph-1
	
		While True
			clip0=0
			clip1=0
			
			If y0>cy1 clip0=clip0 Or 1 Else If y0<cy0 clip0=clip0 Or 2 
			If x0>cx1 clip0=clip0 Or 4 Else If x0<cx0 clip0=clip0 Or 8 
			If y1>cy1 clip1=clip1 Or 1 Else If y1<cy0 clip1=clip1 Or 2
			If x1>cx1 clip1=clip1 Or 4 Else If x1<cx0 clip1=clip1 Or 8 
	
			If (clip0 Or clip1)=0 Exit		'draw Line
			If (clip0 And clip1) Return		'outside
	
			If (clip0 And 1)=1 x0=x0+((x1-x0)*(cy1-y0))/(y1-y0);y0=cy1;Continue 
			If (clip0 And 2)=2 x0=x0+((x1-x0)*(cy0-y0))/(y1-y0);y0=cy0;Continue 
			If (clip0 And 4)=4 y0=y0+((y1-y0)*(cx1-x0))/(x1-x0);x0=cx1;Continue 
			If (clip0 And 8)=8 y0=y0+((y1-y0)*(cx0-x0))/(x1-x0);x0=cx0;Continue 
	
			If (clip1 And 1)=1 x1=x0+((x1-x0)*(cy1-y0))/(y1-y0);y1=cy1;Continue 
			If (clip1 And 2)=2 x1=x0+((x1-x0)*(cy0-y0))/(y1-y0);y1=cy0;Continue 
			If (clip1 And 4)=4 y1=y0+((y1-y0)*(cx1-x0))/(x1-x0);x1=cx1;Continue 
			If (clip1 And 8)=8 y1=y0+((y1-y0)*(cx0-x0))/(x1-x0);x1=cx0;Continue 
		Wend	
			
		dx=x1-x0
		dy=y1-y0
		
		If dx=0 And dy=0 
			_pix[y0*_span+x0]=_color
			Return
		EndIf
		
		If (dx>=0) sx=1;ax=dx Else sx=-1;ax=-dx 
		
		If (dy>=0) sy=1;ay=dy Else sy=-1;ay=-dy 

		If (ax>ay)
			ddf=ay+ay-ax
			sadj=ax+ax
			padj=ay+ay 
			While (ax>0)
				_pix[y0*_span+x0]=_color
				x0=x0+sx
				ddf=ddf+padj
				If (ddf>0) y0=y0+sy;ddf=ddf-sadj
				ax=ax-1
			Wend
		Else
			ddf=ax+ax-ay
			sadj=ay+ay
			padj=ax+ax 
			While (ay>0)
				_pix[y0*_span+x0]=_color
				y0=y0+sy
				ddf=ddf+padj
				If (ddf>0) x0=x0+sx;ddf=ddf-sadj
				ay=ay-1
			Wend
		EndIf
	End Method

	Method DrawRect( lx0#,ly0#,lx1#,ly1#,tx#,ty# ) 
		Local p:Int Ptr
		Local x,y
		Local x0,y0,x1,y1
		
		x0=tx+lx0
		y0=ty+ly0
		x1=tx+lx1
		y1=ty+ly1		
		x0=Max(_clipx,x0)
		y0=Max(_clipy,y0)
		x1=Min(_clipx+_clipw-1,x1)
		y1=Min(_clipy+_cliph-1,y1)		
		p=_pix+y0*_span
		For y=y0 Until y1
			For x=x0 Until x1
				p[x]=_color
			Next
			p:+_span
		Next
	End Method

	Method DrawPixRect( lx0#,ly0#,lx1#,ly1#,tx#,ty#,srcpix:TPixmap ) 
		Local src:Int Ptr
		Local srcspan:Int
		Local dest:Int Ptr
		Local x,y
		Local x0,y0,x1,y1
		Local c,c1,a,a1
		Local r,g,b
				
		x0=tx+lx0
		y0=ty+ly0
		x1=tx+lx1
		y1=ty+ly1		
		x0=Max(_clipx,x0)
		y0=Max(_clipy,y0)
		x1=Min(_clipx+_clipw-1,x1)
		y1=Min(_clipy+_cliph-1,y1)		

		src=Int Ptr srcpix.pixels
		srcspan=srcpix.width
		
		src=src+(y0-ty)*srcspan+(x0-tx-x0)
		dest=_pix+y0*_span

		For y=y0 Until y1
			For x=x0 Until x1
				c=src[x]
				a=(c Shr 24)&255
				If a				
					If a<>255
						c1=dest[x]
						a1=255-a
						r=( ((c Shr 16)&255)*a+((c1 Shr 16)&255)*a1 ) Shr 8
						g=( ((c Shr 8)&255)*a+((c1 Shr 8)&255)*a1 ) Shr 8
						b=( (c&255)*a+(c1&255)*a1 ) Shr 8						
						c=$ff000000 | (r Shl 16) | (g Shl 8) | b
					EndIf
					dest[x]=c
				EndIf
			Next
			src:+srcspan
			dest:+_span
		Next
	End Method
	
	Function Blend32(s32,c24,a8)
		Local r,g,b,a
		Local s8=(s32 Shr 24) & 255
		Local aa8=255-a8		
		r=((s32 & 255)*aa8 + (c24 & 255)*a8) Shr 8
		g=(((s32 Shr 8) & 255)*aa8 + ((c24 Shr 8)& 255)*a8) Shr 8
		b=(((s32 Shr 16) & 255)*aa8 + ((c24 Shr 16)& 255)*a8) Shr 8		
		Return (s8 Shl 24) | (b Shl 16) | (g Shl 8) | (r)
	End Function

	Method DrawOval( lx0#,ly0#,lx1#,ly1#,tx#,ty# ) 
		DebugLog "oval"+lx0+","+ly0+","+lx1+","+ly1+","+tx+","+ty
		Local p:Int Ptr
		Local w=lx1
		Local h=ly1
		Local cx#=tx+w*0.5
		Local cy#=ty+h*0.5
		Local xyxy#=w*w*1.0/(h*h)		
		Local d#=w/2
		Local dd#=d*d
		Local ddd#=(d+1)*(d+1)
		Local m=255.0/(ddd-dd)		
		For Local y=ty To ty+h
			p=_pix+y*_span
			For Local x=tx To tx+w
				Local rr#=(x-cx)*(x-cx)+(y-cy)*(y-cy)*xyxy
				If rr<ddd		
					Local c=_color	
					If rr>dd					
						c=blend32(p[x],c,255-m*(rr-dd))					
					EndIf
					p[x]=c
				EndIf
			Next
		Next
	End Method
		
	Method DrawPixmap( pixmap:TPixmap,x,y ) 
	End Method

	Method GrabPixmap:TPixmap( x,y,width,height ) 
	End Method

' graphics interface

	Method GraphicsModes:TGraphicsMode[]()
	End Method 
	
	Method SetResolution(width#,height#)
	End Method

	Method AttachGraphics:TGraphics( widget,flags )
	End Method 
	
	Method CreateGraphics:TGraphics( width,height,depth,hertz,flags )
	End Method 
	
	Method SetGraphics( g:TGraphics )
		Local pgfx:TPixmapGraphics
		pgfx=TPixmapGraphics(g)
		If pgfx 
			_pixmap=pgfx._pix
			_ras=pgfx._rashandle
			_width=_pixmap.width
			_height=_pixmap.height
			_clipx=0
			_clipy=0
			_clipw=_width
			_cliph=_height	
			_pix=Int Ptr _pixmap.pixels
			_span=_pixmap.pitch Shr 2
			pgfx._max2d.MakeCurrent
		EndIf
	End Method 
	
	Method Flip( sync )
	End Method 

End Type

Type TPixmapGraphics Extends TGraphics
	Field _pix:TPixmap
	Field _max2d:TMax2DGraphics
	Field _rashandle:Int

	Method CreateFromPixmap:TPixmapGraphics(pix:TPixmap)
		_pix=pix
		_rashandle=ras_open(pix.pixelptr(0,0),pix.width,pix.height)
		_max2d=TMax2DGraphics.Create( Self,pixmapdriver )
		Return Self
	End Method	

	Method Driver:TMax2DDriver()
		Return pixmapdriver
	End Method

	Method GetSettings( width Var,height Var,depth Var,hertz Var,flags Var )
		width=_pix.width
		height=_pix.height
	End Method

	Method Close()
	End Method
	
End Type

Global pixmapdriver:TPixmapDriver=New TPixmapDriver

Rem
bbdoc: Returns a new Max2DGraphics drawing context
EndRem
Function PixmapGraphics:TPixmapGraphics(pix:TPixmap)
	Return New TPixmapGraphics.CreateFromPixmap(pix)
End Function
