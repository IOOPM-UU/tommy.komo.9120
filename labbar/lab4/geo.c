#include <stdio.h>
#include <stdbool.h>

struct point 
{
  int x;
  int y;
};
typedef struct point point_t;

struct rectangle
{
  point_t topleft;
  point_t botright; 
};
typedef struct rectangle rectangle_t;

void print_point(point_t *point){
  printf("point(%d,%d)\n",point->x,point->y);
}

void print_rect(rectangle_t *rect){
  printf("rectangle(upper_left)=");
  print_point(&(rect->topleft));
  printf(", lower right=");
  print_point(&(rect->botright));
  printf("\n");
}


point_t make_point(int x, int y){
  point_t point = { .x = x, .y = y};
  return point;
}

rectangle_t make_rect(int x1, int y1, int x2, int y2){
  point_t point1 = make_point(x1,y1);
  point_t point2 = make_point(x2,y2);
  rectangle_t rectangle = { .topleft = point1, .botright = point2};
  return rectangle;
}


void translate(point_t *p1, point_t *p2)
{
  p1->x += p2->x;
  p1->y += p2->y;
}

int area_rect(rectangle_t *rect){
  int heightY = (rect->botright.y)-(rect->topleft.y);
  int widthX = (rect->botright.x)-(rect->topleft.x);
  
  if (heightY < 0) {heightY = -heightY;}
  if (widthX < 0) {widthX = -widthX;}

  return (heightY*widthX);
}

bool intersects_rect(rectangle_t *rect1, rectangle_t *rect2){
   if(rect1->botright.x < rect2->topleft.x || rect2->botright.x < rect1->topleft.x){
    return false;}
  
  if(rect1->botright.y < rect2->topleft.y || rect2->botright.y < rect1->topleft.y){
    return false;}
  
  return true;
}

rectangle_t intersection_rect(rectangle_t *rect1, rectangle_t *rect2) {
  int intersectionX1 = (rect1->topleft.x > rect2->topleft.x) ? rect1->topleft.x :rect2->topleft.x;
  int intersectionY1 = (rect1->topleft.y > rect2->topleft.y) ? rect1->topleft.y : rect2->topleft.y;

    
  int intersectionX2 = (rect1->botright.x < rect2->botright.x) ? rect1->botright.x : rect2->botright.x;
  int intersectionY2 = (rect1->botright.y < rect2->botright.y) ? rect1->botright.y : rect2->botright.y;

   
    rectangle_t intersectionRect = {
        .topleft = {intersectionX1, intersectionY1},
        .botright = {intersectionX2, intersectionY2}
    };

    return intersectionRect;

}

int main(){
    point_t p1 = {.x = 3, .y = 4};
    point_t p2 = {.x = 2, .y = 8};
    
    rectangle_t rect1 = make_rect(2,2,4,4);
    int area = area_rect(&rect1);
    print_rect(&rect1);
    printf("%d\n",area);
    return 0;
}
