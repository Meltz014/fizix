#include "animator.h"
#include <QDebug>

Animator::Animator( std::vector<ShapeObj> *all_shapes )
{
   this->all_shapes = all_shapes;
   time.start( );
}

bool Animator::checkBoundsCollisionX( QRect& bounds, QPainterPath& shape_path, float * dist_to_move )
{
   QPainterPath left = QPainterPath( bounds.topLeft( ) );
   QPainterPath right = QPainterPath( bounds.topRight( ) );
   QPainterPath bounds_path;
   bool check = false;

   left.lineTo( bounds.bottomLeft( ) );
   right.lineTo( bounds.bottomRight( ) );
   if ( shape_path.intersects( left ) )
   {
      bounds_path.addRect( bounds );
      (*dist_to_move) = shape_path.subtracted( bounds_path ).boundingRect( ).width( );
      check = true;
   }
   else if ( shape_path.intersects( right ) )
   {
      bounds_path.addRect( bounds );
      (*dist_to_move) = -shape_path.subtracted( bounds_path ).boundingRect( ).width( );
      check = true;
   }
   return check;
}

bool Animator::checkBoundsCollisionY( QRect& bounds, QPainterPath& shape_path, float * dist_to_move )
{
   QPainterPath top = QPainterPath( bounds.topLeft( ) );
   QPainterPath bot = QPainterPath( bounds.bottomLeft( ) );
   QPainterPath bounds_path;
   bool check = false;

   top.lineTo( bounds.topRight( ) );
   bot.lineTo( bounds.bottomRight( ) );
   if ( shape_path.intersects( top ) )
   {
      bounds_path.addRect( bounds );
      (*dist_to_move) = shape_path.subtracted( bounds_path ).boundingRect( ).height( );
      check = true;
   }
   else if( shape_path.intersects( bot ) )
   {
      bounds_path.addRect( bounds );
      (*dist_to_move) = -shape_path.subtracted( bounds_path ).boundingRect( ).height( );
      check = true;
   }
   return check;
}

float Animator::getFPS( )
{
   return this->fps;
}

void Animator::animateNextFrame( struct Node ** aabb_tree_root, QRect bounds )
{
   for ( uint32 shape_idx = 0; shape_idx < ( uint32 ) all_shapes->size( ); shape_idx++ )
   {
      // only animate if shape is not being dragged
      if ( !(*all_shapes)[ shape_idx ].drag_state )
      {

         // TODO:  apply forces

         // Broad phase
         broad_collisions.clear( );
         get_intersecting_nodes( *aabb_tree_root,
                                 shape_idx,
                                 (*all_shapes)[ shape_idx ].getAABB( ),
                                 &broad_collisions );
         // TODO: narrow-phase collision detection
         // TODO:  resolve collisions
         calcNewPos( shape_idx, bounds, aabb_tree_root );
      }
   }
   // calculate frames per second
   fps = 1000.0F / time.elapsed( );
   time.restart( );
   return;
}

void Animator::calcNewPos( uint32 shape_idx, QRect& bounds, struct Node ** aabb_tree_root )
{
   QPainterPath shape_path;
   QPointF vel( 0.0, 0.0 );
   QPointF travel_dist( 0.0, 0.0 );
   QPointF new_center( 0.0, 0.0 );
   float dist_to_move = 0.0F;
   int elapsed = 0; // msec elapsed since last frame
   bool aabb_updated = false;

   shape_path = ( *all_shapes )[ shape_idx ].getPath( );
   elapsed = time.elapsed( );
   vel = ( *all_shapes )[ shape_idx ].getVelocity( );

   // check collision with boundary
   if ( !bounds.contains( shape_path.boundingRect( ).toRect( ) ) )
   {
      new_center = (*all_shapes)[ shape_idx ].getCenter( );
      if ( checkBoundsCollisionX( bounds, shape_path, &dist_to_move ) )
      {
         vel.setX( -vel.x( ) );
         new_center.setX( new_center.x( ) + dist_to_move );
      }
      if ( checkBoundsCollisionY( bounds, shape_path, &dist_to_move ) )
      {
         vel.setY( -vel.y( ) );
         new_center.setY( new_center.y( ) + dist_to_move );
      }
      (*all_shapes)[ shape_idx ].setCenter( new_center );
      (*all_shapes)[ shape_idx ].setVelocity( vel );
   }
   // v = d/t
   // d = v*t
   travel_dist = vel * ( elapsed / 1000.0F );
   //qDebug( ) << "dist" << travel_dist << "vel" << vel << "elapsed" << elapsed;
   aabb_updated = ( *all_shapes )[ shape_idx ].setCenter( travel_dist
                                           + ( *all_shapes )[ shape_idx ].getCenter( ) );

   if ( aabb_updated )
   {
      // If aabb is updated, delete the old node, create a new one, and
      // re-insert it from the root.  This ensures balance in the tree
      struct Node * new_node = ( struct Node * ) malloc( sizeof( struct Node ) );
      new_node->aabb = ( *all_shapes )[ shape_idx ].getAABB( );
      new_node->shape_idx = shape_idx;

      // remove old node
      remove_node( aabb_tree_root, ( *all_shapes )[ shape_idx ].node );

      // insert the new one
      insert_aabb_node( aabb_tree_root, new_node );

      // set shape's node to new node
      ( *all_shapes )[ shape_idx ].node = new_node;
   }

   return;
}
