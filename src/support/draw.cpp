#include "rubyk/support/draw.h"
#include "opencv/cv.h"
#include "rubyk/opengl.h"

#include <iostream>

void rk::draw_matrix(const cv::Mat &mat, float start_x, float start_y, float end_x, float end_y,
                     float alpha, float z_mul, bool strips) {
  if (mat.cols && mat.rows) {
    if (mat.type() != CV_8UC3) {
      std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
      return;
    }

    size_t rows = mat.rows;
    size_t cols = mat.cols;
    size_t row_step = mat.step1();

    float x1, y1;
    float pix_width  = (end_x - start_x) / cols;
    float pix_height = (end_y - start_y) / rows;
    float z;
    unsigned char *color;
    unsigned char *row_start;
    unsigned char *data = (unsigned char *)mat.data;


    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    // glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (z_mul) {
      if (strips) {
        for (size_t j = 0; j < rows; ++j) {
          row_start = data + j * row_step;
          color = row_start;
          x1 = start_x;
          y1 = start_y + j * pix_height;
          glBegin(GL_TRIANGLE_STRIP);
            /*
             [2]       [4]       [6]


             [1]       [3]       [5]
            */
            z = (color[0] + color[1] + color[2] - 382.5) * z_mul / 765.0;
            glVertex3f(x1            , y1             , z); // [1]
            glVertex3f(x1            , y1 + pix_height, z); // [2]
            for (size_t i = 0; i < cols; ++i) {
              color = row_start + 3 * i;
              x1    = start_x + pix_width * i;
              z     = (color[0] + color[1] + color[2] - 382.5) * z_mul / 765.0;
              glColor4f(
                color[0] / 255.0,
                color[1] / 255.0,
                color[2] / 255.0,
                alpha
              );

              glVertex3f(x1 + pix_width, y1             , z); // [3] ... [5] ...
              glVertex3f(x1 + pix_width, y1 + pix_height, z); // [4] ... [6] ...
            }
          glEnd();
        }
      } else {
        for (size_t j = 0; j < rows; ++j) {
          row_start = data + j * row_step;
          color = row_start;
          y1 = start_y + j * pix_height;
          for (size_t i = 0; i < cols; ++i) {
            color = row_start + 3 * i;
            x1    = start_x + pix_width * i;
            z     = (color[0] + color[1] + color[2] - 382.5) * z_mul / 765.0;
            glColor4f(
              color[0] / 255.0,
              color[1] / 255.0,
              color[2] / 255.0,
              alpha
            );

            glBegin(GL_TRIANGLE_STRIP);
              /*
               [2]       [4]


               [1]       [3]
              */
              // top
              glVertex3f(x1            , y1             , z); // [1]
              glVertex3f(x1            , y1 + pix_height, z); // [2]
              glVertex3f(x1 + pix_width, y1             , z); // [3]
              glVertex3f(x1 + pix_width, y1 + pix_height, z); // [4]

              // borders
              glVertex3f(x1 + pix_width, y1 + pix_height, 0); // [5]
              glVertex3f(x1            , y1 + pix_height, z); // [6] = [2]
              glVertex3f(x1            , y1 + pix_height, 0); // [7]
              glVertex3f(x1            , y1             , z); // [8] = [1]
              glVertex3f(x1            , y1             , 0); // [9]
              glVertex3f(x1 + pix_width, y1             , z); // [10] = [3]
              glVertex3f(x1 + pix_width, y1             , 0); // [11]
              glVertex3f(x1 + pix_width, y1 + pix_height, 0); // [12] = [5]
            glEnd();
          }
        }

      }
    } else {
      // this might be slower then the strip above
      for (size_t j = 0; j < rows; ++j) {
        row_start = data + j * row_step;
        y1 = start_y + j * pix_height;
        for (size_t i = 0; i < cols; ++i) {
          color = row_start + 3 * i;
          x1    = start_x + pix_width * i;
          glColor4f(
            color[0] / 255.0,
            color[1] / 255.0,
            color[2] / 255.0,
            alpha
          );
          glRectf(
            x1,
            y1,
            x1 + pix_width,
            y1 + pix_height
          );
        }
      }
    }
  }
}

void glu::Build2DMipmaps(const cv::Mat &mat) {
  if (mat.cols && mat.rows) {
    if (mat.type() != CV_8UC3) {
      std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
      return;
    }

    if (!mat.isContinuous()) {
      std::cerr << "Cannot build mipmap for matrix (data not continuous)\n";
      return;
    }

    gluBuild2DMipmaps(
      GL_TEXTURE_2D,
      3,                  // internalFormat
      mat.rows,           // width
      mat.cols,           // height
      GL_RGB,             // data ordering format
      GL_UNSIGNED_BYTE,   // data value format
      mat.data
    );
  }
}

void gl::DrawPixels(const cv::Mat &mat) {
  if (mat.cols && mat.rows) {
    if (mat.type() != CV_8UC3) {
      std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
      return;
    }

    if (!mat.isContinuous()) {
      std::cerr << "Cannot build mipmap for matrix (data not continuous)\n";
      return;
    }

    glDrawPixels(
      mat.rows,          // width
      mat.cols,          // height
      GL_RGB,            // data ordering format
      GL_UNSIGNED_BYTE, // data value format
      mat.data
    );
  }
}

void gl::TexImage(const cv::Mat &mat) {
  if (mat.rows && mat.cols) {
    if (mat.type() != CV_8UC3) {
      std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
      return;
    }

    if (!mat.isContinuous()) {
      std::cerr << "Cannot build mipmap for matrix (data not continuous)\n";
      return;
    }

    // ??
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
  }
}





// static int gl_draw_pixels(lua_State *L)
// {
//    GLenum e;
//    GLfloat *pixels;
//
//    /* test arguments type */
//    if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
//         lua_isstring(L, 3) && lua_istable (L, 4)) )
//       luaL_error(L, "incorrect argument to function 'gl.DrawPixels'");
//
//    /* get parameters */
//    e = get_gl_enum(L, 3);
//    get_arrayf(L, 4, &pixels);
//
//    /* test argument */
//    if(e == ENUM_ERROR)
//       luaL_error(L, "incorrect string argument to function 'gl.DrawPixels'");
//
//    /* call opengl function */
//    glDrawPixels((GLsizei)lua_tonumber(L, 1), (GLsizei)lua_tonumber(L, 2), e, GL_FLOAT, pixels);
//
//    free(pixels);
//
//    return 0;
// }

