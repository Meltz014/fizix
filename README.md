## Synopsis

This project is for my own personal experimentation with Qt gui framework and for learning how physics engines work by implementing one from scratch.





## Example

Creating shapes

![](https://i.gyazo.com/ac94556e83fa5fb80cd55e019e8d267d.gif)

Visualizing collisions

![](https://i.gyazo.com/a1543d2be080b43ed65f93759e682362.gif)

The objects are kept track of in a binary tree organized by AABB's (axis-aligned bounding boxes) to enable an efficient collision detection.
The project may be compiled with the DRAW_AABB debug flag enabled to visualize this tree.

![](https://i.imgur.com/RJkMJ8l.png)

The corresponding binary tree for this configuration would look like this:

![](https://i.imgur.com/uIYIPO6.png)

The leaf nodes represent shapes with an AABB.  Each non-leaf node represents an AABB containing its two children.  You can see that the individual shape AABBs (green) are padded.  This is a performance gain so the tree does not need to be rebalanced every frame.  For instance, if a shape is moving, its AABB only gets update once the shape moves outside of the current AABB.  A shape will only be re-inserted in the tree once its AABB gets updated.