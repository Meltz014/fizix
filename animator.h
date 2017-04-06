#include "shapeobj.h"
#include <QTime>
#include <vector>


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
   bool checkBoundsCollisionX( QRect& bounds, QPainterPath& shape_path );
   bool checkBoundsCollisionY( QRect& bounds, QPainterPath& shape_path );
};
