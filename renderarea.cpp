#include "renderarea.h"
#include "window.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QFlags>
#include <stdlib.h>
#include <time.h>

RenderArea::RenderArea( QWidget * parent )
   : QWidget( parent ), animator( &all_shapes )
{
   drag_shape = NULL;
   aabb_tree_root = NULL;
   setBackgroundRole( QPalette::Base );
   setAutoFillBackground( true );

   srand( time( NULL ) );
   drag_timer.start( );

   // start timer
   startTimer( 5 );
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(800, 500);
}

void RenderArea::addShape( ShapeObj new_shape )
{
   // allocate new node for aabb tree
   struct Node * new_node = ( struct Node * ) malloc( sizeof( struct Node ) );

   // store reference to node in shape obj
   new_shape.node = new_node;

   // add shape obj to vector of shapes
   this->all_shapes.push_back( new_shape );

   // populate and insert new node
   new_node->shape_idx = this->all_shapes.size( ) - 1;
   new_node->aabb = new_shape.getAABB( );
   insert_aabb_node( &aabb_tree_root, new_node );
   return;
}

void RenderArea::removeShape( uint32 shape_idx )
{
   // remove shape's node from aabb tree
   remove_node( &aabb_tree_root, all_shapes[ shape_idx ].node );

   // swap current shape with last shape
   std::swap( all_shapes[ shape_idx ], all_shapes.back( ) );

   // remove last shape in vector
   all_shapes.pop_back( );

   // since shape was swapped with the end shape, update
   // the swapped shape's node's shape_idx
   if ( ( uint32 ) all_shapes.size( ) > shape_idx )
   {
      all_shapes[ shape_idx ].node->shape_idx = ( uint32 ) shape_idx;
   }
   return;
}

void RenderArea::timerEvent( QTimerEvent * /* event */ )
{
   this->animator.animateNextFrame( &aabb_tree_root, rect( ) );
   update( );
   return;
}

void RenderArea::setNewShape( ShapeType new_shape )
{
   this->new_shape_type = new_shape;
   return;
}

bool RenderArea::shapeOutOfBounds( uint32 shape_idx )
{
   // check if canvas fully contains the shape's (tight) bounding rect
   return !QRectF( rect( ) ).intersects( all_shapes[ shape_idx ].getPath( ).boundingRect( ) );
}

void RenderArea::paintEvent( QPaintEvent * /* event */ )
{
   QPainter painter( this );

   // remove any shapes that are outside of the bounds
   for ( uint32 shape_idx = 0; shape_idx < ( uint32 )all_shapes.size( ); shape_idx++ )
   {
      if ( !all_shapes[ shape_idx ].drag_state ) // don't check shapes being dragged
      {
         if ( shapeOutOfBounds( shape_idx ) )
         {
            removeShape( shape_idx );
            shape_idx--;
         }
      }
   }

   for ( uint32 shape_idx = 0; shape_idx < ( uint32 )all_shapes.size( ); shape_idx++ )
   {
      painter.save( );
      all_shapes[ shape_idx ].draw( painter );

   #if DRAW_INTERSECTIONS
      // fill any intersections
      for ( uint32 other_idx = 0; other_idx < ( uint32 )all_shapes.size( ); other_idx++ )
      {
         if ( other_idx == shape_idx )
         {
            continue;
         }
         if ( all_shapes[ shape_idx ].intersects( all_shapes[ other_idx ] ) )
         {
            QPainterPath intersection = all_shapes[ shape_idx ].getPath( ) & all_shapes[ other_idx ].getPath( );
            painter.fillPath( intersection, Qt::blue );

            painter.save( );
            painter.setPen( QPen( Qt::cyan, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.0 ), 5, 5 );
            painter.setPen( QPen( Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.25 ), 5, 5 );
            painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.50 ), 5, 5 );
            painter.setPen( QPen( Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.75 ), 5, 5 );

            painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawLine( intersection.pointAtPercent( 0.0 ), intersection.pointAtPercent( 0.50 ) );
            painter.drawLine( intersection.pointAtPercent( 0.25 ), intersection.pointAtPercent( 0.75 ) );
            painter.restore( );
         }
      }
   #endif

      painter.restore( );
   }


#if DRAW_AABB
   //// draw aabb boxes 
   /// 
   if ( aabb_tree_root != NULL )
   {
      painter.save( );
      draw_aabb_from_tree( aabb_tree_root, painter, 0 );
      painter.restore( );
   }
#endif

   // Draw border around render area
   painter.setRenderHint(QPainter::Antialiasing, false);
   painter.setPen(palette().dark().color());
   painter.setBrush(Qt::NoBrush);
   painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

   // update FPS label
   ( ( Window * ) parentWidget( ) )->setFPS( animator.getFPS( ) );
}

void RenderArea::mousePressEvent( QMouseEvent *event )
{
   Qt::MouseButton button = event->button(  );
   int x_coord = event->x( );
   int y_coord = event->y( );

   if ( button & Qt::LeftButton )
   {
      for ( int i = 0; i < ( int )all_shapes.size( ); i++ )
      {
         if ( all_shapes[ i ].getPath( ).contains( event->pos( ) ) )
         {
            // restart drag timer
            drag_timer.restart( );
            drag_shape = &all_shapes[ i ];
            drag_offset = drag_shape->getCenter( ) - event->pos( );
            break;
         }
         else
         {
            drag_shape = NULL;
         }
      }
   }
   else if ( button & Qt::RightButton )
   {
      ShapeObj new_shape = ShapeObj( this->new_shape_type );
      new_shape.setPen( QPen( Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
      new_shape.setBrush( QBrush( ) );
      new_shape.setCenter( QPoint( x_coord, y_coord ) );
      new_shape.setSize( QSize( 80, 80 ) );
      addShape( new_shape );
      drag_offset = QPoint( 0, 0 );
      drag_shape = NULL;
      update( );
   }
}

void RenderArea::mouseMoveEvent( QMouseEvent *event )
{
   QPointF new_pos;
   QPointF new_vel;
   if ( drag_shape )
   {
      drag_shape->drag_state = true;
      // calculate new position
      new_pos = event->pos( ) + drag_offset;
      // calculate velocity
      // v = d/t
      new_vel = ( new_pos - drag_shape->getCenter( ) ) / ( drag_timer.elapsed( ) / 1000.0F );

      // set properties
      drag_shape->setVelocity( new_vel );
      if ( drag_shape->setCenter( new_pos ) )
      {
         // If aabb is updated, delete the old node, create a new one, and
         // re-insert it from the root.  This ensures balance in the tree
         struct Node * new_node = ( struct Node * ) malloc( sizeof( struct Node ) );
         new_node->aabb = drag_shape->getAABB( );
         new_node->shape_idx = drag_shape->node->shape_idx;

         // remove old node
         remove_node( &aabb_tree_root, drag_shape->node );

         // insert the new one
         insert_aabb_node( &aabb_tree_root, new_node );

         // set shape's node to new node
         drag_shape->node = new_node;
      }

      // restart drag timer
      drag_timer.restart( );
      update( );
   }
}

void RenderArea::mouseReleaseEvent( QMouseEvent * /* event */ )
{
   if ( drag_shape )
   {
      drag_shape->drag_state = false;
      drag_shape = NULL;
   }
   return;
}
