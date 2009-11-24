Strict

Rem
bbdoc: PixmapGraphics
End Rem
Module axe.PixmapGraphics

ModuleInfo "Version: 0.01"
ModuleInfo "Author: Simon Armstrong"
ModuleInfo "Modserver: BRL"

Import brl.max2d

Type TPixmapFrame Extends TImageFrame
	
	Field _owner:TPixmapDriver
	Field _pix:TPixmap
	Field _flags:Int
		
	Method Draw( x0#,y0#,x1#,y1#,tx#,ty# )
		Local dest:TPixmap		
		_owner.DrawRect(x0,y0,x1-x0,y1-y0,tx,ty)
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

	Method DrawOval( lx0#,ly0#,lx1#,ly1#,tx#,ty# ) 
	End Method

	Method DrawPoly( xy#[],handlex#,handley#,originx#,originy# ) 
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

	Method CreateFromPixmap:TPixmapGraphics(pix:TPixmap)
		_pix=pix
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
