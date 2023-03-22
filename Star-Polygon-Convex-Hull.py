#!/usr/bin/env python
# coding: utf-8

# In[1]:


def enter_points():
  '''
  Take input of the points in counter-clockwise order and returns it in a list
  '''
  # stores the points
  points = list()

  # input number of points
  n = int(input('Enter number of points: '))

  # input points
  print('Points should be in counter-clockwise order.')
  for i in range(n):
    point = tuple([float(x) for x in input(f'Enter point {i+1}: ').split(' ')])
    points.append(point)
  
  return points


# In[2]:


def turn(p0, p1, p2):
  '''
  Returns if p0-p1-p2 is a left turn or right turn or collinear
  args:
    - p0, p1, p2: tuple of the form (x,y)
  '''
  
  det = (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p2[0] - p0[0]) * (p1[1] - p0[1])
  
  if det > 0:
    return 'left'
  elif det == 0:
    return 'collinear'
  else:
    return 'right'


# In[19]:


def reorder_points(points):
  '''
  Reorders points so that the points start from the lowest y value points (leftmost if y value clash)
  '''
  start_point_idx = 0
  for i in range(len(points)):
    if points[i][1] < points[start_point_idx][1]:
      start_point_idx = i
    elif points[i][1] == points[start_point_idx][1]:
      if points[i][0] < points[start_point_idx][0]:
        start_point_idx = i
    else:
      continue
  
  reordered = points[start_point_idx:] if isinstance(points[start_point_idx:], list) else [points[start_point_idx:]]
  reordered += points[:start_point_idx] if isinstance(points[:start_point_idx], list) else [points[:start_point_idx]]
  reordered += [points[start_point_idx]]
  
  return reordered


# In[20]:


def find_convex_hull(points):
  '''
  Finds and returns the convex hull using Graham Scan in a list of counter-clockwise points
  '''
  
  # if len(points) < 3: # since, we already assume that we will get star 'polygon'
  #   return 'No convex hull exists for less than 3 points.'
  
  # initialization of the convex hull (ch) stack
  ch = list()
  
  # reorders points
  points = reorder_points(points)
  
  # Add two initial points in the stack
  ch.extend(points[:2])
  
  # computes the convex hull
  for i in range(2, len(points)):
    
    print('stack:', ch)
    print('incoming point:', points[i])
    
    while len(ch) >= 2 and turn(ch[-2], ch[-1], points[i]) != 'left':
      ch.pop(-1)
    ch.append(points[i])
    
  return ch[:-1]


# In[21]:


# input points
points = enter_points()

# get convex hull
convex_hull = find_convex_hull(points)

print('Convex Hull:')
print(convex_hull)

