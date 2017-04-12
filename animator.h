#include "commontypes.h"
#include "shapeobj.h"
#include "collisions.h"

class Animator
{
 public:
   Animator( std::vector<ShapeObj> *all_shapes );
   void animateNextFrame( QRect bounds );
   float getFPS( );

 private:
   std::vector<ShapeObj> *all_shapes;
   QTime time = QTime( );
   float fps = 0.0F;
   bool checkBoundsCollisionX( QRect& bounds, QPainterPath& shape_path, float * dist_to_move );
   bool checkBoundsCollisionY( QRect& bounds, QPainterPath& shape_path, float * dist_to_move );
   void calcNewPos( uint32 shape_idx, QRect& bounds );
};
