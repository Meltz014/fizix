#include "animator.h"
#include <QDebug>

Animator::Animator( std::vector<ShapeObj> *all_shapes )
{
   this->all_shapes = all_shapes;
   time.start( );
}

bool Animator::checkBoundsCollisionX( QRect& bounds, QPainterPath& shape_path )
{
   QPainterPath left = QPainterPath( bounds.topLeft( ) );
   QPainterPath right = QPainterPath( bounds.topRight( ) );
   left.lineTo( bounds.bottomLeft( ) );
   right.lineTo( bounds.bottomRight( ) );
   if ( shape_path.intersects( left ) ||
        shape_path.intersects( right ) )
   {
      return true;
   }
   return false;
}

bool Animator::checkBoundsCollisionY( QRect& bounds, QPainterPath& shape_path )
{
   QPainterPath top = QPainterPath( bounds.topLeft( ) );
   QPainterPath bot = QPainterPath( bounds.bottomLeft( ) );
   top.lineTo( bounds.topRight( ) );
   bot.lineTo( bounds.bottomRight( ) );
   if ( shape_path.intersects( top ) ||
        shape_path.intersects( bot ) )
   {
      return true;
   }
   return false;
}

float Animator::getFPS( )
{
   return this->fps;
}

void Animator::animateNextFrame( QRect bounds )
{
   QPointF vel( 0.0, 0.0 );
   QPointF travel_dist( 0.0, 0.0 );
   QPainterPath shape_path;
   int elapsed = 0; // msec elapsed since last frame
   for ( int shape_idx = 0; shape_idx < ( int ) all_shapes->size( ); shape_idx++ )
   {
      // only animate if shape is not being dragged
      if ( !(*all_shapes)[ shape_idx ].drag_state )
      {
         shape_path = (*all_shapes)[ shape_idx ].getPath( );
         elapsed = time.elapsed( );
         vel = ( *all_shapes )[ shape_idx ].getVelocity( );
         // check collision with boundary
         if ( !bounds.contains( shape_path.boundingRect( ).toRect( ) ) )
         {
            if ( checkBoundsCollisionX( bounds, shape_path ) )
            {
               vel.setX( -vel.x( ) );
            }
            if ( checkBoundsCollisionY( bounds, shape_path ) )
            {
               vel.setY( -vel.y( ) );
            }
            (*all_shapes)[ shape_idx ].setVelocity( vel );
         }
         // v = d/t
         // d = v*t
         travel_dist = vel * ( elapsed / 1000.0F );
         //qDebug( ) << "dist" << travel_dist << "vel" << vel << "elapsed" << elapsed;
         ( *all_shapes )[ shape_idx ].setCenter( travel_dist
                                                 + ( *all_shapes )[ shape_idx ].getCenter( ) );
      }
   }
   // calculate frames per second
   fps = 1000.0F / time.elapsed( );
   time.restart( );
   return;
}
