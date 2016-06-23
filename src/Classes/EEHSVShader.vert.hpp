//
//  EEHSVShader.frag
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

constexpr const char* ee_hsv_shader_vert = "\
attribute vec4 a_position;                                                   \n\
attribute vec2 a_texCoord;                                                   \n\
attribute vec4 a_color;                                                      \n\
                                                                             \n\
#ifdef GL_ES                                                                 \n\
varying lowp vec4 v_fragmentColor;                                           \n\
varying mediump vec2 v_texCoord;                                             \n\
#else                                                                        \n\
varying vec4 v_fragmentColor;                                                \n\
varying vec2 v_texCoord;                                                     \n\
#endif                                                                       \n\
                                                                             \n\
void main() {                                                                \n\
    gl_Position = CC_PMatrix * a_position;                                   \n\
    v_fragmentColor = a_color;                                               \n\
    v_texCoord = a_texCoord;                                                 \n\
}                                                                            \n\
";