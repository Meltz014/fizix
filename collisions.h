#include "commontypes.h"

struct Node {
   struct Node * parent;
   struct Node * left;
   struct Node * right;

   // bounding box
   QRectF aabb;

   // shape index if leaf node
   uint32 shape_idx;

   // height in the tree for maintaining balance
   int height;
};

void get_intersecting_nodes( struct Node * test_node, uint32 current_shape_idx, QRectF aabb, std::vector<uint32> *output_shape_idx );
void recalculate_tree_up( struct Node * node );
void remove_node( struct Node ** root, struct Node * node );
struct Node * insert_aabb_node( struct Node ** root, struct Node * new_node );
void draw_aabb_from_tree( struct Node * node, QPainter& painter, uint8 color );
Node * get_node_for_shape( Node * node, uint32 shape_idx );
