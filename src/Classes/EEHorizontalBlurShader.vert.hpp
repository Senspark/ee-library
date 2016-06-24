//
//  EEHorizontalBlurShader.vert.hpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

constexpr const char* ee_horizontal_blur_shader_vert = "\
attribute vec4 a_position;                                                   \n\
attribute vec2 a_texCoord;                                                   \n\
attribute vec4 a_color;                                                      \n\
                                                                             \n\
#ifdef GL_ES                                                                 \n\
varying lowp vec4 v_fragmentColor;                                           \n\
varying mediump vec2 v_texCoord;                                             \n\
varying mediump vec2 v_blurTexCoords[4];                                     \n\
uniform mediump float u_dimension;                                           \n\
#else                                                                        \n\
varying vec4 v_fragmentColor;                                                \n\
varying vec2 v_texCoord;                                                     \n\
varying vec2 v_blurTexCoords[4];                                             \n\
uniform float u_dimension;                                                   \n\
#endif                                                                       \n\
                                                                             \n\
void main() {                                                                \n\
    gl_Position = CC_PMatrix * a_position;                                   \n\
    v_fragmentColor = a_color;                                               \n\
    v_texCoord = a_texCoord;                                                 \n\
                                                                             \n\
    float o1 = 1.3846153846 / u_dimension;                                   \n\
    float o2 = 3.2307692308 / u_dimension;                                   \n\
                                                                             \n\
    v_blurTexCoords[0] = v_texCoord + vec2(-o2, 0.0);                        \n\
    v_blurTexCoords[1] = v_texCoord + vec2(-o1, 0.0);                        \n\
    v_blurTexCoords[2] = v_texCoord + vec2( o1, 0.0);                        \n\
    v_blurTexCoords[3] = v_texCoord + vec2( o2, 0.0);                        \n\
}                                                                            \n\
";