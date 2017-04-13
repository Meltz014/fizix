#include "commontypes.h"
#include "shapeobj.h"
#include "collisions.h"

class Animator
{
 public:
   Animator( std::vector<ShapeObj> *all_shapes );
   void animateNextFrame( struct Node ** aabb_tree_root, QRect bounds );
   float getFPS( );

 private:
   std::vector<ShapeObj> *all_shapes;
   std::vector<uint32> broad_collisions;
   QTime time = QTime( );
   float fps = 0.0F;
   bool checkBoundsCollisionX( QRect& bounds, QPainterPath& shape_path, float * dist_to_move );
   bool checkBoundsCollisionY( QRect& bounds, QPainterPath& shape_path, float * dist_to_move );
   void calcNewPos( uint32 shape_idx, QRect& bounds, struct Node ** aabb_tree_root );
};
