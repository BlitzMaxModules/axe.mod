ModuleInfo "Version: 0.01"
ModuleInfo "Author: Simon Armstrong"
ModuleInfo "Modserver: BRL"
import brl.blitz
import brl.max2d
TPixmapDriver^brl.max2d.TMax2DDriver{
._pixmap:brl.pixmap.TPixmap&
._width%&
._height%&
._span%&
._pix%*&
._blend%&
._alpha%&
._color%&
._clscolor%&
._clipx%&
._clipy%&
._clipw%&
._cliph%&
-New%()="_axe_pixmapgraphics_TPixmapDriver_New"
-Delete%()="_axe_pixmapgraphics_TPixmapDriver_Delete"
-CreateFrameFromPixmap:brl.max2d.TImageFrame(pixmap:brl.pixmap.TPixmap,flags%)="_axe_pixmapgraphics_TPixmapDriver_CreateFrameFromPixmap"
-SetBlend%(blend%)="_axe_pixmapgraphics_TPixmapDriver_SetBlend"
-SetAlpha%(alpha#)="_axe_pixmapgraphics_TPixmapDriver_SetAlpha"
-SetColor%(red%,green%,blue%)="_axe_pixmapgraphics_TPixmapDriver_SetColor"
-SetClsColor%(red%,green%,blue%)="_axe_pixmapgraphics_TPixmapDriver_SetClsColor"
-SetViewport%(x%,y%,width%,height%)="_axe_pixmapgraphics_TPixmapDriver_SetViewport"
-SetTransform%(xx#,xy#,yx#,yy#)="_axe_pixmapgraphics_TPixmapDriver_SetTransform"
-SetLineWidth%(width#)="_axe_pixmapgraphics_TPixmapDriver_SetLineWidth"
-Cls%()="_axe_pixmapgraphics_TPixmapDriver_Cls"
-Plot%(x#,y#)="_axe_pixmapgraphics_TPixmapDriver_Plot"
-DrawLine%(lx0#,ly0#,lx1#,ly1#,tx#,ty#)="_axe_pixmapgraphics_TPixmapDriver_DrawLine"
-DrawRect%(lx0#,ly0#,lx1#,ly1#,tx#,ty#)="_axe_pixmapgraphics_TPixmapDriver_DrawRect"
-DrawOval%(lx0#,ly0#,lx1#,ly1#,tx#,ty#)="_axe_pixmapgraphics_TPixmapDriver_DrawOval"
-DrawPoly%(xy#&[],handlex#,handley#,originx#,originy#)="_axe_pixmapgraphics_TPixmapDriver_DrawPoly"
-DrawPixmap%(pixmap:brl.pixmap.TPixmap,x%,y%)="_axe_pixmapgraphics_TPixmapDriver_DrawPixmap"
-GrabPixmap:brl.pixmap.TPixmap(x%,y%,width%,height%)="_axe_pixmapgraphics_TPixmapDriver_GrabPixmap"
-GraphicsModes:brl.graphics.TGraphicsMode&[]()="_axe_pixmapgraphics_TPixmapDriver_GraphicsModes"
-AttachGraphics:brl.graphics.TGraphics(widget%,flags%)="_axe_pixmapgraphics_TPixmapDriver_AttachGraphics"
-CreateGraphics:brl.graphics.TGraphics(width%,height%,depth%,hertz%,flags%)="_axe_pixmapgraphics_TPixmapDriver_CreateGraphics"
-SetGraphics%(g:brl.graphics.TGraphics)="_axe_pixmapgraphics_TPixmapDriver_SetGraphics"
-Flip%(sync%)="_axe_pixmapgraphics_TPixmapDriver_Flip"
}="axe_pixmapgraphics_TPixmapDriver"
TPixmapGraphics^brl.graphics.TGraphics{
._pix:brl.pixmap.TPixmap&
._max2d:brl.max2d.TMax2DGraphics&
-New%()="_axe_pixmapgraphics_TPixmapGraphics_New"
-Delete%()="_axe_pixmapgraphics_TPixmapGraphics_Delete"
-CreateFromPixmap:TPixmapGraphics(pix:brl.pixmap.TPixmap)="_axe_pixmapgraphics_TPixmapGraphics_CreateFromPixmap"
-Driver:brl.max2d.TMax2DDriver()="_axe_pixmapgraphics_TPixmapGraphics_Driver"
-GetSettings%(width% Var,height% Var,depth% Var,hertz% Var,flags% Var)="_axe_pixmapgraphics_TPixmapGraphics_GetSettings"
-Close%()="_axe_pixmapgraphics_TPixmapGraphics_Close"
}="axe_pixmapgraphics_TPixmapGraphics"
PixmapGraphics:TPixmapGraphics(pix:brl.pixmap.TPixmap)="axe_pixmapgraphics_PixmapGraphics"
pixmapdriver:TPixmapDriver&=mem:p("axe_pixmapgraphics_pixmapdriver")
