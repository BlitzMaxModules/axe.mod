Strict

Import axe.PixmapGraphics

Global DisplayGraphics:TGraphics


Function PixmapGraphics:TGraphics(pix:TPixmap)
	Return New TPixmapGraphics.CreateFromPixmap(pix)
End Function

Function drawtopixmap(pix:TPixmap)
	Local gfx:TGraphics=PixmapGraphics(pix)	
	
	SetGraphics gfx
	SetClsColor 255,0,255
	Cls
	SetColor 0,255,255
	Plot 50,50
	Plot 100,100
	SetColor 0,25,25
	DrawRect 20,20,200,200
	SetColor 255,255,255
	DrawLine 0,0,100,100
	Flip
	CloseGraphics gfx
	SetGraphics DisplayGraphics

End Function

Local image:TImage

DisplayGraphics= Graphics(640,480)

Local pix:TPixmap=CreatePixmap(256,256,PF_COLORALPHA)

drawtopixmap(pix)

image=LoadImage(pix)
DrawImage image,0,0
Flip


WaitKey

End

