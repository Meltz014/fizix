#include "shapeobj.h"

ShapeObj::ShapeObj( ShapeType shape_type )
{
   this->shape_type = shape_type;
   this->node = NULL;
}

bool ShapeObj::setCenter( QPointF center )
{
   this->center = center;
   return updatePath();
}

void ShapeObj::setSize( QSizeF size )
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

void ShapeObj::setVelocity( QPointF vel )
{
   this->velocity = vel;
}

QPointF ShapeObj::getCenter( )
{
   return this->center;
}

QSizeF ShapeObj::getSize( )
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

QPointF ShapeObj::getVelocity( )
{
   return this->velocity;
}

ShapeType ShapeObj::getShapeType( )
{
   return this->shape_type;
}

bool ShapeObj::updatePath( )
{
   QPointF top_left = center - QPointF( size.width( ) / 2.0,
                                        size.height( ) / 2.0 );
   path = QPainterPath( );
   path.moveTo( top_left );
   switch ( shape_type )
   {
      case FLine:
         path.lineTo( top_left + QPointF( size.width( ), size.height( ) ) );
         break;
      case FRect:
         path.addRect( QRectF( top_left, size ) );
         break;
      case FCirc:
         path.addEllipse( center, size.width( ) / 2.0, size.height( ) / 2.0 );
         break;
      default:
         break;
   }

   return updateAABB( );
}

bool ShapeObj::updateAABB( )
{
   // returns true if aabb was updated

   // update aabb
   // only update aabb if it's empty
   // OR current path is not already fully contained
   if ( aabb.isEmpty( ) || !aabb.contains( path.controlPointRect( ) ) )
   {
      aabb = path.controlPointRect( );
      aabb.adjust( -FAT_FACTOR, -FAT_FACTOR, FAT_FACTOR, FAT_FACTOR );
      return true;
   }
   return false;
}

void ShapeObj::draw( QPainter& painter )
{
   // draw the shape
   painter.setPen( this->pen );
   painter.setBrush( this->brush );
   painter.setRenderHint( QPainter::Antialiasing, true );
   painter.drawPath( this->path );
#if DRAW_AABB
   painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
   painter.drawRect( this->aabb );
#endif
}

bool ShapeObj::intersects( ShapeObj& other )
{
   return this->path.intersects( other.getPath( ) );
}

QRectF ShapeObj::getAABB( )
{
   return this->aabb;
}
