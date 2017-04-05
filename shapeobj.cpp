#include "shapeobj.h"

ShapeObj::ShapeObj( ShapeType shape_type )
{
   this->shape_type = shape_type;
}

void ShapeObj::setCenter( QPoint center )
{
   this->center = center;
   updatePath();
}

void ShapeObj::setSize( QSize size )
{
   this->size = size;
   updatePath();
}

void ShapeObj::setPen( QPen pen )
{
   this->pen = pen;
   updatePath();
}

void ShapeObj::setBrush( QBrush brush )
{
   this->brush = brush;
   updatePath();
}

QPoint ShapeObj::getCenter( )
{
   return this->center;
}

QSize ShapeObj::getSize( )
{
   return this->size;
}

QPen ShapeObj::getPen( )
{
   return this->pen;
}

QBrush ShapeObj::getBrush( )
{
   return this->brush;
}

QPainterPath ShapeObj::getPath( )
{
   return this->path;
}

void ShapeObj::updatePath( )
{
   QPoint top_left = center - QPoint( size.width( ) / 2,
                                      size.height( ) / 2 );
   path = QPainterPath( );
   path.moveTo( top_left );
   switch ( shape_type )
   {
      case FLine:
         path.lineTo( top_left + QPoint( size.width( ), size.height( ) ) );
         break;
      case FRect:
         path.addRect( QRect( top_left, size ) );
         break;
      case FCirc:
         path.addEllipse( center, size.width( ) / 2.0, size.height( ) / 2.0 );
         break;
      default:
         break;
   }
}
