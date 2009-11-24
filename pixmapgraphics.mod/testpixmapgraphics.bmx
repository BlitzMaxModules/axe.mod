Strict

Import axe.PixmapGraphics

Global DisplayGraphics:TGraphics

Function testpixmapdrawing(pix:TPixmap)
	Local gfx:TGraphics
	
	gfx=PixmapGraphics(pix)	
	
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


Local pix:TPixmap
Local image:TImage

DisplayGraphics=Graphics(640,480)

pix=CreatePixmap(256,256,PF_COLORALPHA)

testpixmapdrawing(pix)

image=LoadImage(pix)

DrawImage image,0,0

Flip


WaitKey

End

