#include "commontypes.h"
#include "collisions.h"
#include <cfloat>

// search the aabb binary tree for any nodes that intersect with the given aabb rect
void get_intersecting_nodes( struct Node * test_node, uint32 current_shape_idx, QRectF aabb, std::vector<uint32> *output_shape_idx )
{
   // make sure test node is not null
   if ( ( test_node == NULL ) || ( test_node->shape_idx == current_shape_idx ) )
   {
      return;
   }

   // only need to continue if test node intersects with given aabb
   if ( test_node->aabb.intersects( aabb ) )
   {
      // check if leaf node
      if ( test_node->left == NULL && test_node->right == NULL )
      {
         output_shape_idx->push_back( test_node->shape_idx );
      }
      else
      {
         // query each child
         get_intersecting_nodes( test_node->left, current_shape_idx, aabb, output_shape_idx );
         get_intersecting_nodes( test_node->right, current_shape_idx, aabb, output_shape_idx );
      }
   }

   return;
}

// recalculates the tree after inserting a new node
void recalculate_tree_up( struct Node * node )
{
   if ( node->parent != NULL )
   {
      struct Node * sibling = NULL;
      if ( node->parent->left == node )
      {
         sibling = node->parent->right;
      }
      else if ( node->parent->right == node )
      {
         sibling = node->parent->left;
      }

      if ( sibling == NULL )
      {
         // just set parent AABB to this node's AABB
         node->parent->aabb = QRectF( node->aabb );
      }
      else
      {
         // set parent's AABB to the combination of this node's
         // and its sibling's AABB
         node->parent->aabb = sibling->aabb.united( node->aabb );
      }

      // step up recursively
      recalculate_tree_up( node->parent );
   }
   return;
}

void remove_node( struct Node ** root, struct Node * node )
{
   if ( node->parent == NULL )
   {
      qDebug( ) << "is root";
      free( node );
      *root = NULL;
      return;
   }

   // replace this node's parent with its sibling
   struct Node * sibling = NULL;
   if ( node->parent->left == node )
   {
      sibling = node->parent->right;
   }
   else
   {
      sibling = node->parent->left;
   }

   // check for a grandparent
   if ( node->parent->parent != NULL )
   {
      struct Node * grandpa = node->parent->parent;

      if ( grandpa->left == node->parent )
      {
         grandpa->left = sibling;
      }
      else
      {
         grandpa->right = sibling;
      }

      // set sibling's parent to grandparent
      sibling->parent = grandpa;

      free( node->parent );
      free( node );
   }
   else
   {
      // parent is root.  Replace root with sibling
      *root = sibling;
      sibling->parent = NULL;
      free( node );
   }

   recalculate_tree_up( sibling );
   return;
}

// inserts a new leaf node to the aabb tree
struct Node * insert_aabb_node( struct Node ** root, struct Node * new_node )
{
   new_node->left = NULL;
   new_node->right = NULL;
   new_node->parent = *root;
   new_node->height = 0;

   if ( *root == NULL )
   {
      // for uninitialized root
      *root = new_node;
   }
   else if ( ( *root )->left == NULL && ( *root )->right == NULL )
   {
      // if root is leaf

      // create a new parent node
      struct Node * new_parent = ( struct Node * ) malloc( sizeof( struct Node ) );
      new_parent->shape_idx = 0xFFFF;

      // tie the new parent's parent to the root's parent
      new_parent->parent = ( *root )->parent;
      if ( new_parent->parent != NULL )
      {
         if ( new_parent->parent->left == *root )
         {
            new_parent->parent->left = new_parent;
         }
         else
         {
            new_parent->parent->right = new_parent;
         }
      }

      // set the new parent's left child to the old root
      new_parent->left = *root;
      ( *root )->parent = new_parent;

      // set the new parent's right child to the node to insert
      new_parent->right = new_node;

      // change node to insert's parent to new parent
      new_node->parent = new_parent;

      // change root to the new parent
      *root = new_parent;
   }
   else
   {
      // if root is node w/ children
      Node * new_root = *root;
      QRectF combined_aabb;
      float cost1;
      float cost2;

      // while new_root is not a leaf
      while ( ( new_root->left != NULL ) || ( new_root->right != NULL ) )
      {
         cost1 = FLT_MAX;
         cost2 = FLT_MAX;
         // calculate cost of each side
         if ( new_root->left != NULL )
         {
            combined_aabb = new_node->aabb.united( new_root->left->aabb );
            cost1 = combined_aabb.width( ) * 2 + combined_aabb.height( ) * 2;
         }
         if ( new_root->right != NULL )
         {
            combined_aabb = new_node->aabb.united( new_root->right->aabb );
            cost2 = combined_aabb.width( ) * 2 + combined_aabb.height( ) * 2;
         }

         if ( cost1 < cost2 )
         {
            new_root = new_root->left;
         }
         else
         {
            new_root = new_root->right;
         }
      }
      return insert_aabb_node( &new_root, new_node );
   }

   recalculate_tree_up( new_node );
   return new_node;
}

void draw_aabb_from_tree( struct Node * node, QPainter& painter, uint8 color )
{

   Qt::GlobalColor colors[ 11 ] = { Qt::red,
                                   Qt::blue,
                                   Qt::cyan,
                                   Qt::magenta,
                                   Qt::yellow,
                                   Qt::darkRed,
                                   Qt::darkGreen,
                                   Qt::darkBlue,
                                   Qt::darkCyan,
                                   Qt::darkMagenta,
                                   Qt::darkYellow };

   color %= 11;

   if ( node != NULL )
   {
      if ( node->left == NULL && node->right == NULL )
      {
         // leaf
         painter.setPen( QPen( Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
      }
      else
      {
         painter.setPen( QPen( colors[ color ], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
      }
      painter.drawRect( node->aabb );
      //qDebug( ) << "node" << node << "shapeidx" << node->shape_idx << "parent" << node->parent << "left" << node->left << "right" << node->right;
      if ( node->left != NULL )
      {
         draw_aabb_from_tree( node->left, painter, color + 1 );
      }
      if ( node->right != NULL )
      {
         draw_aabb_from_tree( node->right, painter, color + 1 );
      }
   }
   return;
}

Node * get_node_for_shape( Node * node, uint32 shape_idx )
{
   Node * test;
   if ( node->left != NULL )
   {
      test = get_node_for_shape( node->left, shape_idx );
      if ( test != NULL )
      {
         return test;
      }
   }

   if ( node->right != NULL )
   {
      test = get_node_for_shape( node->right, shape_idx );
      if ( test != NULL )
      {
         return test;
      }
   }

   if ( node->shape_idx == shape_idx )
   {
      return node;
   }
   else
   {
      return NULL;
   }
}
