/* This file is part of the fiducials Project.
 * https://github.com/stevenlovegrove/fiducials
 *
 * Copyright (c) 2011 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

 #include "drawing.h"

using namespace std;
using namespace TooN;
using namespace CVD;
using namespace pangolin;

void DrawRectangle( const IRectangle& r )
{
  glBegin(GL_LINE_STRIP);
    glVertex2f(r.x1,r.y1);
    glVertex2f(r.x2,r.y1);
    glVertex2f(r.x2,r.y2);
    glVertex2f(r.x1,r.y2);
    glVertex2f(r.x1,r.y1);
  glEnd();
}

void DrawCross( float x, float y, int r )
{
  glBegin(GL_LINES);
    glVertex2f(x,y-r);
    glVertex2f(x,y+r);
    glVertex2f(x-r,y);
    glVertex2f(x+r,y);
  glEnd();
}

void DrawCross( float x, float y, float z, int r )
{
  glBegin(GL_LINES);
    glVertex3f(x,y-r,z);
    glVertex3f(x,y+r,z);
    glVertex3f(x-r,y,z);
    glVertex3f(x+r,y,z);
    glVertex3f(x,y,z-r);
    glVertex3f(x,y,z+r);
  glEnd();
}

void DrawCross( const Vector<2>& p, int r )
{
  DrawCross(p[0],p[1],r);
}

void DrawCross( const Vector<3>& p, int r )
{
  DrawCross(p[0],p[1],p[2],r);
}

void DrawCircle( const Vector<2>& p, double radius )
{
  glBegin(GL_POLYGON);
  for( double a=0; a< 2*M_PI; a += M_PI/50.0 )
  {
    glVertex2d(
      p[0] + radius * cos(a),
      p[1] + radius * sin(a)
    );
  }
  glEnd();
}

void DrawTarget( const Target& t, const Vector<2>& offset, double scale, double sat, double val )
{
  const double r = t.Radius() * scale;

  for( unsigned int i=0; i<t.circles().size(); ++i )
  {
    const Vector<2> p = t.circles()[i] * scale + offset;
    glColorBin(i,t.circles().size(),sat,val);
    DrawCircle(p,r);
  }
}

void DrawTarget( const vector<int>& map, const Target& target, const Vector<2>& offset, double scale, double sat, double val )
{
  const double r = target.Radius() * scale;

  for( unsigned int i=0; i<map.size(); ++i )
  {
    const int t = map[i];
    if( t >= 0 )
    {
      const Vector<2> p = target.circles()[t] * scale + offset;
      glColorBin(t,target.circles().size(),sat,val);
      DrawCircle(p,r);
    }
  }
}

void glDrawAxis(float s)
{
  glBegin(GL_LINES);
  glColor3f(1,0,0);
  glVertex3f(0,0,0);
  glVertex3f(s,0,0);
  glColor3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(0,s,0);
  glColor3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(0,0,s);
  glEnd();
}

void glDrawFrustrum( const Matrix<3,3>& Kinv, int w, int h, float scale )
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0,0,0);
  CVD::glVertex( scale * Kinv * TooN::makeVector(0,0,1) );
  CVD::glVertex( scale * Kinv * TooN::makeVector(w,0,1) );
  CVD::glVertex( scale * Kinv * TooN::makeVector(w,h,1) );
  CVD::glVertex( scale * Kinv * TooN::makeVector(0,h,1) );
  CVD::glVertex( scale * Kinv * TooN::makeVector(0,0,1) );
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void glDrawFrustrum( const Matrix<3,3>& Kinv, int w, int h, const TooN::SE3<>& T_wf, float scale )
{
  glSetFrameOfReferenceF(T_wf);
  glDrawFrustrum(Kinv,w,h,scale);
  glUnsetFrameOfReference();
}

void glDrawGrid(float num_lines, float line_delta)
{
    glBegin(GL_LINES);

    for(int i = -num_lines; i < num_lines; i++){
        glVertex3f( line_delta*num_lines, i*line_delta, 0.0);
        glVertex3f(-line_delta*num_lines, i*line_delta, 0.0);

        glVertex3f(i*line_delta,  line_delta*num_lines, 0.0);
        glVertex3f(i*line_delta, -line_delta*num_lines, 0.0);
    }

//    glColor4ub(255, 0, 0, 128);
//    glVertex3f( line_delta*num_lines , 0.0, 0.0);
//    glVertex3f(-line_delta*num_lines , 0.0, 0.0);

//    glColor4ub(0, 255, 0, 128);
//    glVertex3f( 0.0,  line_delta*num_lines, 0.0);
//    glVertex3f( 0.0, -line_delta*num_lines, 0.0);

    glEnd();
}
